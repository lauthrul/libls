#include "stdafx.h"
#include "httpclient.h"

#ifdef USE_LIBCURL

#include "openssl/crypto.h"
using namespace lslib::logger;

#ifdef WIN32
#else
#define stricmp     strcasecmp
#endif

namespace lslib
{
    namespace net
    {
        enum_str_begin(EHttpMethod)
            enum_str_item(HTTP_GET)
            enum_str_item(HTTP_POST)
            enum_str_item(HTTP_DOWNLOAD)
            enum_str_item(HTTP_UPLOAD)
        enum_str_end(EHttpMethod)

        //////////////////////////////////////////////////////////////////////////
        static size_t OnWriteData(void* buffer, size_t size, size_t nmemb, void* lpVoid)
        {
            vector<char>* pOutBuf = dynamic_cast<vector<char> *>((vector<char>*)lpVoid);
            if ( NULL == pOutBuf || NULL == buffer )
                return -1;

            char* pData = (char*)buffer;
            for (size_t i = 0; i < size * nmemb; i++)
                pOutBuf->push_back( pData[i] );

            return size * nmemb;
        }

        static size_t OnWriteFileData(void* buffer, size_t size, size_t nmemb, void* lpVoid)
        {
            pair<FILE*, size_t*>* param = (pair<FILE*, size_t*>*)(lpVoid);
            if (param == NULL || param->first == NULL || param->second == NULL)
                return -1;

            FILE* file = param->first;
            size_t* wirte_size = param->second;

            size_t s = fwrite(buffer, 1, size * nmemb, file);
            *wirte_size += s;
            return  s;
        }

        static size_t OnGetHeaderInfo(void* ptr, size_t size, size_t nmemb, void* stream)
        {
            map<string, string>* pInfoHeader = static_cast< map<string, string>* >(stream);
            if (pInfoHeader)
            {
                string_array arr;
                strtool::split(arr, (char*)ptr, ":", false);
                if (arr.size() == 2)
                    (*pInfoHeader)[strtool::trim(arr[0])] = strtool::trim(arr[1]);
            }
            return size * nmemb;
        }

        LSLIB_API SHttpUrl CrackUrl(_lpcstr lpstrUrl)
        {
            SHttpUrl sUrl;
            string strUrl = lpstrUrl;
            size_t p = 0;
            size_t q = string::npos;

            // scheme
            q = strUrl.find("://", p);
            if (q != string::npos)
            {
                sUrl.strScheme = strUrl.substr(p, q - p);
                p = q + 3;
            }
            else sUrl.strScheme = "http";
            // host name & port
            char split[] = { '/', '?', '#' };
            int cnt = sizeof(split) / sizeof(char);
            int i = 0;
            for (; i < cnt; i++)
            {
                q = strUrl.find(split[i], p);
                if (q != string::npos)
                {
                    sUrl.strHostName = strUrl.substr(p, q - p);
                    p = q;
                    break;
                }
            }
            if (sUrl.strHostName.empty())
                sUrl.strHostName = strUrl.substr(p);
            size_t r = sUrl.strHostName.find_last_of(':');
            if (r != string::npos)
            {
                sUrl.nPort = strtool::to_int(sUrl.strHostName.substr(r + 1, sUrl.strHostName.length()));
                sUrl.strHostName = sUrl.strHostName.substr(0, r);
            }
            else
            {
                sUrl.nPort = 80;
            }
            // path
            if (strUrl[p] == '/')
            {
                for (i = 1; i < cnt; i++)
                {
                    q = strUrl.find(split[i], p);
                    if (q != string::npos)
                    {
                        sUrl.strPath = strUrl.substr(p, q - p);
                        p = q;
                        break;
                    }
                }
                if (sUrl.strPath.empty())
                {
                    sUrl.strPath = strUrl.substr(p);
                }
            }
            // query
            if (strUrl[p] == '?')
            {
                p += 1;
                string strQuery;
                for (i = 2; i < cnt; i++)
                {
                    q = strUrl.find(split[i], p);
                    if (q != string::npos)
                    {
                        strQuery = strUrl.substr(p, q - p);
                        p = q;
                        break;
                    }
                }
                if (strQuery.empty()) strQuery = strUrl.substr(p);
                string_array arr_querys;
                strtool::split(arr_querys, strQuery, "&");
                for (size_t j = 0; j < arr_querys.size(); j++)
                {
                    const string& query = arr_querys[j];
                    r = query.find("=");
                    sUrl.mapQuerys[query.substr(0, r)] = query.substr(r + 1);
                }
            }
            // tag
            if (strUrl[p] == '#')
            {
                sUrl.strTag = strUrl.substr(p + 1);
            }

            return sUrl;
        }

        LSLIB_API string ReverseUrl(const SHttpUrl& vUrl)
        {
            string strUrl;
            strUrl += strtool::format("%s://%s:%d", vUrl.strScheme.c_str(), vUrl.strHostName.c_str(), vUrl.nPort);
            if (!vUrl.strPath.empty())
                strUrl += vUrl.strPath;
            if (!vUrl.mapQuerys.empty())
            {
                strUrl += "?";
                for (map<string, string>::const_iterator it = vUrl.mapQuerys.begin();
                        it != vUrl.mapQuerys.end(); it++)
                {
                    strUrl += strtool::format("%s=%s&", it->first.c_str(), it->second.c_str());
                }
                strUrl = strtool::trim_right(strUrl, '&');
            }
            if (!vUrl.strTag.empty())
            {
                strUrl += "#" + vUrl.strTag;
            }
            return strUrl;
        }

        //////////////////////////////////////////////////////////////////////////

        // for https multi-thread safety
        // https://blog.csdn.net/qq_28234213/article/details/76588627
        // https://curl.haxx.se/libcurl/c/opensslthreadlock.html

        static CMutexLock* lockarray;
        static unsigned long id_function(void)
        {
            return __THREAD__;
        }
        static void locking_function(int mode, int n, const char* file, int line)
        {
            if (mode & CRYPTO_LOCK) lockarray[n].Lock();
            else                    lockarray[n].Unlock();
        }
        int httpclient_thread_setup(void)
        {
            int counts = CRYPTO_num_locks();
            lockarray = new CMutexLock[counts];
            CRYPTO_set_id_callback(id_function);
            CRYPTO_set_locking_callback(locking_function);
            return 0;
        }
        int httpclient_thread_cleanup(void)
        {
            if (lockarray == NULL) return -1;
            CRYPTO_set_id_callback(NULL);
            CRYPTO_set_locking_callback(NULL);
            delete[] lockarray;
            return 0;
        }

        //////////////////////////////////////////////////////////////////////////
        static CURLSH* s_shobject = NULL;
        string CHttpClient::m_strDefaultAgent;
        string CHttpClient::m_strDefaultCookieFile;
        void CHttpClient::Init(_lpcstr lpstrDefaultCookieFile /*= NULL*/, _lpcstr lpstrDefaultAgent /*= NULL*/)
        {
            if (s_shobject == NULL)
            {
                SetDefaultCookieFile(lpstrDefaultCookieFile);
                SetDefaultAgent(lpstrDefaultAgent);

                curl_global_init(CURL_GLOBAL_ALL);
                s_shobject = curl_share_init();
                curl_share_setopt(s_shobject, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
            }
        }

        bool CHttpClient::IsInit()
        {
            return (s_shobject != NULL);
        }

        void CHttpClient::SetDefaultCookieFile(_lpcstr lpstrDefaultCookieFile)
        {
            m_strDefaultCookieFile = lpstrDefaultCookieFile ? lpstrDefaultCookieFile : "";
        }

        void CHttpClient::SetDefaultAgent(_lpcstr lpstrDefaultAgent)
        {
            m_strDefaultAgent = lpstrDefaultAgent ? lpstrDefaultAgent : "";
        }

        SHttpResult CHttpClient::HttpGet(const SHttpGetParam& vParam)
        {
            if (!IsInit()) Init();

            string strText = DumpParamText((SHttpParam*)&vParam);
            DEBUG_LOG(g_netlogger, "begin to do http get, param:[%s]", strText.c_str());

            Time tmStart;
            SHttpResult vResult;

            //////////////////////////////////////////////////////////////////////////
            CURL* pCurl = curl_easy_init();
            if (NULL == pCurl)
            {
                vResult.nCode = CURLE_FAILED_INIT;
                goto label_exit;
            }
            else
            {
                // perform
                Perform(pCurl, vParam, vResult);

                curl_easy_cleanup(pCurl);
            }
            //////////////////////////////////////////////////////////////////////////

label_exit:
            vResult.nTimeSpend = Time::GetCurDateTime().BetweenAllMilliSec(tmStart);
            strText = DumpResultText(&vResult);
            DEBUG_LOG(g_netlogger, "finish http get[%s], result[%s]", vParam.strUrl.c_str(), strText.c_str());

            return vResult;
        }

        SHttpResult CHttpClient::HttpPost(const SHttpPostParam& vParam)
        {
            if (!IsInit()) Init();

            string strText = DumpParamText((SHttpParam*)&vParam);
            DEBUG_LOG(g_netlogger, "begin to do http post, param:[%s]", strText.c_str());

            Time tmStart;
            SHttpResult vResult;

            //////////////////////////////////////////////////////////////////////////
            CURL* pCurl = curl_easy_init();
            if (NULL == pCurl)
            {
                vResult.nCode = CURLE_FAILED_INIT;
                goto label_exit;
            }
            else
            {
                curl_easy_setopt(pCurl, CURLOPT_POST, 1);
                curl_easy_setopt(pCurl, CURLOPT_POSTFIELDS, vParam.strPost.c_str());
                curl_easy_setopt(pCurl, CURLOPT_POSTFIELDSIZE, vParam.strPost.length());

                // perform
                Perform(pCurl, vParam, vResult);

                curl_easy_cleanup(pCurl);
            }
            //////////////////////////////////////////////////////////////////////////

label_exit:
            vResult.nTimeSpend = Time::GetCurDateTime().BetweenAllMilliSec(tmStart);
            strText = DumpResultText(&vResult);
            DEBUG_LOG(g_netlogger, "finish http post[%s], result[%s]", vParam.strUrl.c_str(), strText.c_str());

            return vResult;
        }

        SHttpResult CHttpClient::DownloadFile(const SHttpDowloadParam& vParam)
        {
            if (!IsInit()) Init();

            string strText = DumpParamText((SHttpParam*)&vParam);
            DEBUG_LOG(g_netlogger, "begin to do http download, param:[%s]", strText.c_str());

            Time tmStart;
            SHttpResult vResult;
            string strRequest = vParam.strUrl;

            //////////////////////////////////////////////////////////////////////////
            CURL* pCurl = curl_easy_init();
            if (NULL == pCurl)
            {
                vResult.nCode = CURLE_FAILED_INIT;
                goto label_exit;
            }
            else
            {
                if (!vParam.strToken.empty())
                {
                    if (strRequest.find("?") != string::npos)
                        strRequest += "&token=" + vParam.strToken;
                    else
                        strRequest += "?token=" + vParam.strToken;
                }

                long infoHeader[2] = {0, 0}; // [0] = accept ranges or not, [1] = file size
                if (vParam.bBreakPointSupport)
                {
                    map<string, string> mapHeader;
                    curl_easy_setopt(pCurl, CURLOPT_URL, strRequest.c_str());
                    if (!m_strDefaultAgent.empty())
                        curl_easy_setopt(pCurl, CURLOPT_USERAGENT, m_strDefaultAgent.c_str());
                    curl_easy_setopt(pCurl, CURLOPT_HEADER, 1);
                    curl_easy_setopt(pCurl, CURLOPT_NOBODY, 1);
                    curl_easy_setopt(pCurl, CURLOPT_FOLLOWLOCATION, 1);
                    curl_easy_setopt(pCurl, CURLOPT_HEADERFUNCTION, OnGetHeaderInfo);
                    curl_easy_setopt(pCurl, CURLOPT_HEADERDATA, &mapHeader);
                    curl_easy_setopt(pCurl, CURLOPT_CONNECTTIMEOUT, vParam.nConnetTimeout);
                    curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, vParam.nPerformTimeout);
                    curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYPEER, 0);
                    curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYHOST, 0);

                    curl_easy_perform(pCurl);
                    curl_easy_cleanup(pCurl);

                    map<string, string>::iterator it = mapHeader.find("Accept-Ranges");
                    if (it == mapHeader.end()) it = mapHeader.find("accept-ranges");
                    if (it != mapHeader.end() && stricmp(it->second.c_str(), "bytes") == 0)
                        infoHeader[0] = 1;

                    it = mapHeader.find("Content-Length");
                    if (it == mapHeader.end()) it = mapHeader.find("content-length");
                    if (it != mapHeader.end())
                        infoHeader[1] = atol(it->second.c_str());
                }

                // download file from last broken position if server support range
                pCurl = curl_easy_init();
                if (NULL == pCurl)
                {
                    vResult.nCode = CURLE_FAILED_INIT;
                    goto label_exit;
                }

                struct curl_slist* headers = NULL;
                curl_easy_setopt(pCurl, CURLOPT_URL, strRequest.c_str());
                if (!m_strDefaultAgent.empty())
                    curl_easy_setopt(pCurl, CURLOPT_USERAGENT, m_strDefaultAgent.c_str());
                curl_easy_setopt(pCurl, CURLOPT_FOLLOWLOCATION, 1);
                curl_easy_setopt(pCurl, CURLOPT_NOSIGNAL, 1);
                curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYPEER, 0);
                curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYHOST, 0);
                curl_easy_setopt(pCurl, CURLOPT_ACCEPT_ENCODING, ""); // accept encodings supported built-in
                curl_easy_setopt(pCurl, CURLOPT_SHARE, s_shobject);
                if (!m_strDefaultCookieFile.empty())
                {
                    curl_easy_setopt(pCurl, CURLOPT_COOKIEFILE, m_strDefaultCookieFile.c_str()); //cookie read
                    curl_easy_setopt(pCurl, CURLOPT_COOKIEJAR, m_strDefaultCookieFile.c_str()); //cookie save
                }
                if (vParam.nConnetTimeout > 0)      curl_easy_setopt(pCurl, CURLOPT_CONNECTTIMEOUT, vParam.nConnetTimeout);
                if (vParam.nPerformTimeout > 0)     curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, vParam.nPerformTimeout);
                if (!vParam.mapHeaders.empty()) // request header
                {
                    string strHeader;
                    for (map<string, string>::const_iterator it = vParam.mapHeaders.begin();
                            it != vParam.mapHeaders.end(); it++)
                    {
                        strHeader = it->first + ": " + it->second;
                        headers = curl_slist_append(headers, strHeader.c_str());
                    }
                }
                curl_easy_setopt(pCurl, CURLOPT_HTTPHEADER, headers);
                // response header
                curl_easy_setopt(pCurl, CURLOPT_HEADERFUNCTION, OnGetHeaderInfo);
                curl_easy_setopt(pCurl, CURLOPT_HEADERDATA, &vResult.mapHeaders);

                string strFile = vParam.strFile;
                if (strFile.empty())
                    strFile = os::path_get_name(vParam.strUrl.c_str());
                FILE* pFile = NULL;
                if (infoHeader[0]) pFile = fopen(strFile.c_str(), "ab+" );
                else pFile = fopen(strFile.c_str(), "wb+" );
                if (pFile == NULL)
                {
                    vResult.nCode = CURLE_WRITE_ERROR;
                    goto label_exit;
                }

                pair<FILE*, size_t*> param;
                param.first = pFile;
                param.second = (size_t*)&vResult.nDataLen;
                curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &param);
                curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, OnWriteFileData);

                CURLcode ret = CURLE_OK;
                for (int i = 0; i < TRYCOUNT_DOWNLOAD; i++)
                {
                    if (infoHeader[0])
                    {
                        fseek(pFile, 0, SEEK_END);
                        int nSize = ftell(pFile);

                        if (nSize == infoHeader[1] && infoHeader[1] > 0) //already finished download
                        {
                            INFO_LOG(g_netlogger, "already finished download[%s], size: %d, file size: %d", strRequest.c_str(), nSize, infoHeader[1]);

                            vResult.nCode = CURLE_OK;
                            fclose(pFile);
                            goto label_exit;
                        }

                        char szRange[32] = {0}; sprintf(szRange, "%d-", nSize);
                        curl_easy_setopt(pCurl, CURLOPT_RANGE, szRange);
                    }

                    ret = curl_easy_perform(pCurl);
                    if (ret == CURLE_OK) break;
                }

                if (ret == CURLE_OK) curl_easy_getinfo(pCurl, CURLINFO_RESPONSE_CODE, &vResult.nCode);
                if (ret != CURLE_OK) vResult.nCode = ret;

                curl_slist_free_all(headers);
                curl_easy_cleanup(pCurl);
                fclose(pFile);

                if (ret != CURLE_OK)
                {
                    os::rm(vParam.strFile.c_str());
                }
                else
                {
                    vResult.strData = "[file://" + strFile + "]";
                }
            }
            //////////////////////////////////////////////////////////////////////////

label_exit:
            vResult.nTimeSpend = Time::GetCurDateTime().BetweenAllMilliSec(tmStart);
            strText = DumpResultText(&vResult);
            DEBUG_LOG(g_netlogger, "finish http download[%s], result[%s]", strRequest.c_str(), strText.c_str());

            return vResult;
        }

        SHttpResult CHttpClient::UploadFile(const SHttpUploadParam& vParam)
        {
            if (!IsInit()) Init();

            string strText = DumpParamText((SHttpParam*)&vParam);
            DEBUG_LOG(g_netlogger, "begin to do http upload, param:[%s]", strText.c_str());

            Time tmStart;
            SHttpResult vResult;

            //////////////////////////////////////////////////////////////////////////
            CURL* pCurl = curl_easy_init();
            if (NULL == pCurl)
            {
                vResult.nCode = CURLE_FAILED_INIT;
                goto label_exit;
            }
            else
            {
                CURLcode ret = CURLE_OK;

                FILE* pFile = fopen( vParam.strFile.c_str(), "rb");
                if (pFile == NULL)
                {
                    curl_easy_cleanup(pCurl);
                    vResult.nCode = CURLE_READ_ERROR;
                    goto label_exit;
                }

                fseek(pFile, 0, SEEK_END);
                int nFileSize = ftell(pFile);
                fseek(pFile, 0, SEEK_SET);
                char* szBuffer = new char[nFileSize + 1024];
                if (szBuffer == NULL)
                {
                    vResult.nCode = CURLE_OUT_OF_MEMORY;
                    goto label_exit;
                }
                memset(szBuffer, 0, nFileSize + 1024);
                char* pBuffer = szBuffer;
                int nContentLength = 0;

                //
                string strRemoteFileName;
                if (!vParam.strRemoteFile.empty())
                {
                    strRemoteFileName = vParam.strRemoteFile;
                }
                else
                {
                    int npos = vParam.strFile.find_last_of("/");
                    if (npos < 0) npos = vParam.strFile.find_last_of('\\');
                    if (npos >= 0) strRemoteFileName = vParam.strFile.substr(npos + 1);
                }

                string strBoundary = ""; //GenGUID();
                strText = "--" + strBoundary + "\r\n";
                strText += "Content-Disposition: form-data; name=\"file\"; filename=\"" + strRemoteFileName + "\"\r\n";
                strText += "Content-Type: application/octet-stream\r\n\r\n";
                memcpy(pBuffer, strText.c_str(), strText.length());
                pBuffer += strText.length();
                nContentLength += strText.length();

                //
                fread(pBuffer, nFileSize, 1, pFile);
                pBuffer += nFileSize;
                nContentLength += nFileSize;

                //
                strText = "\r\n--" + strBoundary + "--\r\n";
                memcpy(pBuffer, strText.c_str(), strText.length());
                pBuffer += strText.length();
                nContentLength += strText.length();

                //
                strText = "Content-Type: multipart/form-data; boundary=" + strBoundary;
                curl_slist* headers = NULL;
                headers = curl_slist_append(headers, strText.c_str());

                curl_easy_setopt(pCurl, CURLOPT_HTTPHEADER, headers);
                curl_easy_setopt(pCurl, CURLOPT_POST, 1);
                curl_easy_setopt(pCurl, CURLOPT_POSTFIELDS, szBuffer);
                curl_easy_setopt(pCurl, CURLOPT_POSTFIELDSIZE, nContentLength);

                // perform
                Perform(pCurl, vParam, vResult);

                fclose(pFile);
                curl_slist_free_all(headers);
                curl_easy_cleanup(pCurl);
                delete[] szBuffer;
            }
            //////////////////////////////////////////////////////////////////////////

label_exit:
            vResult.nTimeSpend = Time::GetCurDateTime().BetweenAllMilliSec(tmStart);
            strText = DumpResultText(&vResult);
            DEBUG_LOG(g_netlogger, "finish http upload[%s], result[%s]", vParam.strUrl.c_str(), strText.c_str());

            return vResult;
        }

        int CHttpClient::Perform(CURL* pCurl, const SHttpParam& vParam, __inout__ SHttpResult& vResult)
        {
            if (!IsInit()) Init();

            struct curl_slist* headers = NULL;
            vector<char> vctRespBuf;
            curl_easy_setopt(pCurl, CURLOPT_URL, vParam.strUrl.c_str());
            if (!m_strDefaultAgent.empty())
                curl_easy_setopt(pCurl, CURLOPT_USERAGENT, m_strDefaultAgent.c_str());
            curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, (void*)&vctRespBuf);
            curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, OnWriteData);
            curl_easy_setopt(pCurl, CURLOPT_FOLLOWLOCATION, 1);
            curl_easy_setopt(pCurl, CURLOPT_NOSIGNAL, 1);
            curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYPEER, 0);
            curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYHOST, 0);
            curl_easy_setopt(pCurl, CURLOPT_ACCEPT_ENCODING, ""); // accept encodings supported built-in
            curl_easy_setopt(pCurl, CURLOPT_SHARE, s_shobject);
            if (!m_strDefaultCookieFile.empty())
            {
                curl_easy_setopt(pCurl, CURLOPT_COOKIEFILE, m_strDefaultCookieFile.c_str()); //cookie read
                curl_easy_setopt(pCurl, CURLOPT_COOKIEJAR, m_strDefaultCookieFile.c_str()); //cookie save
            }
            if (vParam.nConnetTimeout > 0)      curl_easy_setopt(pCurl, CURLOPT_CONNECTTIMEOUT, vParam.nConnetTimeout);
            if (vParam.nPerformTimeout > 0)     curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, vParam.nPerformTimeout);
            if (!vParam.mapHeaders.empty()) // request header
            {
                string strHeader;
                for (map<string, string>::const_iterator it = vParam.mapHeaders.begin();
                        it != vParam.mapHeaders.end(); it++)
                {
                    strHeader = strtool::format("%s: %s", it->first.c_str(), it->second.c_str());
                    headers = curl_slist_append(headers, strHeader.c_str());
                }
                curl_easy_setopt(pCurl, CURLOPT_HTTPHEADER, headers);
            }
            // response header
            curl_easy_setopt(pCurl, CURLOPT_HEADERFUNCTION, OnGetHeaderInfo);
            curl_easy_setopt(pCurl, CURLOPT_HEADERDATA, &vResult.mapHeaders);
            // set download callback
            if (vParam.cb != NULL)
            {
                curl_easy_setopt(pCurl, CURLOPT_NOPROGRESS, 0);
                curl_easy_setopt(pCurl, CURLOPT_PROGRESSFUNCTION, *vParam.cb);
                curl_easy_setopt(pCurl, CURLOPT_PROGRESSDATA, vParam.clientp);
            }

            CURLcode ret = CURLE_OK;
            int nTry = (vParam.nTryCount > 0) ? vParam.nTryCount : 1;
            for (int i = 0; i < nTry; i++)
            {
                vctRespBuf.clear();
                ret = curl_easy_perform(pCurl);
                if (ret == CURLE_OK) break;

                ERROR_LOG(g_netlogger, "http perform fail[%d], try[%d], url[%s]", ret, i, vParam.strUrl.c_str());
            }

            if (ret == CURLE_OK) ret = curl_easy_getinfo(pCurl, CURLINFO_RESPONSE_CODE, &vResult.nCode);
            if (ret != CURLE_OK) vResult.nCode = ret;

            if (vResult.nCode >= 300 && vResult.nCode < 400) //redirect
            {
                char* pRedirect = NULL;
                CURLcode ret = curl_easy_getinfo(pCurl, CURLINFO_REDIRECT_URL, &pRedirect);
                if (ret == CURLE_OK)
                {
                    vctRespBuf.clear();
                    for (size_t i = 0; i < strlen(pRedirect); i++)
                        vctRespBuf.push_back(*pRedirect);
                }
            }

            if (headers != NULL) curl_slist_free_all(headers);

            vResult.strData.assign(vctRespBuf.begin(), vctRespBuf.end());
            vResult.nDataLen = vctRespBuf.size();

            return vResult.nCode;
        }

        string CHttpClient::DumpParamText(SHttpParam* pParam)
        {
            string strText;
            strText += strtool::format("method: %s, ", enum_str(EHttpMethod, pParam->eMethod).c_str());
            strText += strtool::format("url: %s, ", pParam->strUrl.c_str());
            strText += strtool::format("connettimeout: %d, ", pParam->nConnetTimeout);
            strText += strtool::format("performtimeout: %d, ", pParam->nPerformTimeout);
            strText += strtool::format("trycount: %d, ", pParam->nTryCount);
            if (!pParam->mapHeaders.empty())
            {
                strText += "header:{";
                for (map<string, string>::iterator it = pParam->mapHeaders.begin();
                        it != pParam->mapHeaders.end(); it++)
                {
                    strText += strtool::format("%s: %s, ", it->first.c_str(), it->second.c_str());
                }
                strText = strText.substr(0, strText.length() - 1);
                strText += "}, ";
            }

            switch (pParam->eMethod)
            {
                case HTTP_GET:
                    break;
                case HTTP_POST:
                    strText += strtool::format("post: %s ", ((SHttpPostParam*)pParam)->strPost.c_str());
                    strText += strtool::format("gzip: %d, ", ((SHttpPostParam*)pParam)->bgZip);
                    strText += strtool::format("thresholdsize: %d", ((SHttpPostParam*)pParam)->nThresholdSize);
                    break;
                case HTTP_DOWNLOAD:
                    strText += strtool::format("file: %s, ", ((SHttpDowloadParam*)pParam)->strFile.c_str());
                    strText += strtool::format("token: %s, ", ((SHttpDowloadParam*)pParam)->strToken.c_str());
                    strText += strtool::format("breakpointsupport: %d, ", ((SHttpDowloadParam*)pParam)->bBreakPointSupport);
                    strText += strtool::format("callback: 0x%p, ", ((SHttpDowloadParam*)pParam)->cb);
                    strText += strtool::format("clientp: 0x%p", ((SHttpDowloadParam*)pParam)->clientp);
                    break;
                case HTTP_UPLOAD:
                    strText += strtool::format("file: %s, ", ((SHttpDowloadParam*)pParam)->strFile.c_str());
                    strText += strtool::format("remotefile: %s", ((SHttpUploadParam*)pParam)->strRemoteFile.c_str());
                    break;
            }

            return strText;
        }

        string CHttpClient::DumpResultText(SHttpResult* pResult)
        {
            string strText;
            strText += strtool::format("code: %d, ", pResult->nCode);
            strText += strtool::format("data: %s, ", pResult->strData.c_str());
            strText += strtool::format("datalen: %d, ", pResult->nDataLen);
            strText += strtool::format("timespend: %d", pResult->nTimeSpend);
            return strText;
        }

    } // namespace net

} // namespace lslib

#endif // endof USE_LIBCURL
