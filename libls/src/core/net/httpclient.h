#pragma once

#ifdef USE_LIBCURL

#include "curl/curl.h"

//////////////////////////////////////////////////////////////////////////

#define TRYCOUNT_HTTPGET                2
#define TRYCOUNT_HTTPPOST               2
#define TRYCOUNT_DOWNLOAD               2
#define TRYCOUNT_UPLOAD                 2

#define TIMEOUT_CONNECT                 3       //sec
#define TIMEOUT_HTTPGET                 5       //sec
#define TIMEOUT_HTTPPOST                15      //sec
#define TIMEOUT_HTTPDOWNLOAD            30      //sec
#define TIMEOUT_HTTPUPLOAD              15      //sec

#define GZIP_THRESHOLD_SIZE             (100*1024)

//////////////////////////////////////////////////////////////////////////
namespace lslib
{
    /// �������
    namespace net
    {
        //////////////////////////////////////////////////////////////////////////
        /// HTTP��������
        enum EHttpMethod
        {
            HTTP_GET,       ///< GET����
            HTTP_POST,      ///< POST����
            HTTP_DOWNLOAD,  ///< ��������
            HTTP_UPLOAD,    ///< �ϴ�����
        };
        declare_enum_str(EHttpMethod, LSLIB_API);

        /// @brief HTTP�ص�����
        /// @param clientp  �û�����
        /// @param dltotal  ���������ݴ�С
        /// @param dlnow    ��ǰ���������ݴ�С
        /// @param ultotal  ���ϴ����ݴ�С
        /// @param ulnow    ��ǰ���ϴ����ݴ�С
        typedef int(*HttpCallback)(void* clientp, double dltotal, double dlnow, double ultotal, double ulnow);

        //////////////////////////////////////////////////////////////////////////
        /// @brief HTTP�����������
        struct SHttpParam
        {
            EHttpMethod             eMethod;            ///< �������ͣ� �ο�@ref EHttpMethod
            string                  strUrl;             ///< ����URL
            map<string, string>     mapHeaders;         ///< ����ͷ
            int                     nConnetTimeout;     ///< ���ӳ�ʱʱ�䣬Ĭ��3s
            int                     nPerformTimeout;    ///< ��ִ�г�ʱʱ�䣨��������ʱ�䣩��С��0�ɳ������
            int                     nTryCount;          ///< ʧ�����Դ�����С��0�ɳ������
            HttpCallback            cb;                 ///< �ص��������ο�@ref HttpCallback
            void*                   clientp;            ///< �ص������û�����

            SHttpParam()
            {
                Reset();
            }
            SHttpParam(lpcstr lpstrUrl)
            {
                Reset();
                strUrl = lpstrUrl;
            }
            void Reset()
            {
                eMethod = HTTP_GET;
                strUrl.clear();
                mapHeaders.clear();
                nConnetTimeout = TIMEOUT_CONNECT;
                nPerformTimeout = -1; // evaluate by program
                nTryCount = -1; // evaluate by program
                cb = NULL;
                clientp = NULL;
            }
        };

        /// @brief HTTP GET�������
        /// @note Ĭ��ִ�г�ʱʱ��Ϊ5s�����Դ���Ϊ2��
        struct SHttpGetParam : public SHttpParam
        {
            SHttpGetParam()
            {
                Reset();
            }
            SHttpGetParam(lpcstr lpstrUrl)
            {
                Reset();
                strUrl = lpstrUrl;
            }
            void Reset()
            {
                SHttpParam::Reset();
                eMethod = HTTP_GET;
                nPerformTimeout = TIMEOUT_HTTPGET;
                nTryCount = TRYCOUNT_HTTPGET;
            }
        };

        /// @brief HTTP POST�������
        /// @note Ĭ��ִ�г�ʱʱ��Ϊ15s�����Դ���Ϊ2��
        struct SHttpPostParam : public SHttpParam
        {
            string                  strPost;        ///< POST����
            bool                    bgZip;          ///< �Ƿ����ѹ����Ĭ�ϲ�ѹ��
            int                     nThresholdSize; ///< ѹ����С��ֵ��POST���ݳ��ȳ�����ֵʱ��������ѹ����Ĭ��Ϊ1KB

            SHttpPostParam()
            {
                Reset();
            }
            SHttpPostParam(lpcstr lpstrUrl, lpcstr lpstrPost)
            {
                Reset();
                strUrl = lpstrUrl;
                strPost = lpstrPost;
            }
            void Reset()
            {
                SHttpParam::Reset();
                eMethod = HTTP_POST;
                nPerformTimeout = TIMEOUT_HTTPPOST;
                nTryCount = TRYCOUNT_HTTPPOST;
                strPost.clear();
                bgZip = false;
                nThresholdSize = GZIP_THRESHOLD_SIZE;
            }
        };

        /// @brief �����������
        /// @note Ĭ��ִ�г�ʱʱ��Ϊ30s�����Դ���Ϊ2��
        struct SHttpDowloadParam : public SHttpParam
        {
            string                  strFile;            ///< ���ر����ļ�·��
            string                  strToken;           ///< tokenֵ��δʹ��
            bool                    bBreakPointSupport; ///< �Ƿ�֧�ֶϵ�������Ĭ��Ϊ��

            SHttpDowloadParam()
            {
                Reset();
            }
            SHttpDowloadParam(lpcstr lpstrUrl, lpcstr lpstrFile)
            {
                Reset();
                strUrl = lpstrUrl;
                strFile = lpstrFile;
            }
            void Reset()
            {
                SHttpParam::Reset();
                eMethod = HTTP_DOWNLOAD;
                nPerformTimeout = TIMEOUT_HTTPDOWNLOAD;
                nTryCount = TRYCOUNT_DOWNLOAD;
                strFile.clear();
                strToken.clear();
                bBreakPointSupport = false;
            }
        };

        /// @brief �ϴ��������
        /// @note Ĭ��ִ�г�ʱʱ��Ϊ15s�����Դ���Ϊ2��
        struct SHttpUploadParam : public SHttpParam
        {
            string                 strFile;         ///< �����ļ�·��
            string                 strRemoteFile;   ///< Զ���ļ���

            SHttpUploadParam()
            {
                Reset();
            }
            SHttpUploadParam(lpcstr lpstrUrl, lpcstr lpstrFile)
            {
                Reset();
                strUrl = lpstrUrl;
                strFile = lpstrFile;
            }
            void Reset()
            {
                SHttpParam::Reset();
                eMethod = HTTP_UPLOAD;
                nPerformTimeout = TIMEOUT_HTTPUPLOAD;
                nTryCount = TRYCOUNT_UPLOAD;
                strFile.clear();
                strRemoteFile.clear();
            }
        };

        /// HTTP��Ӧ���
        struct SHttpResult
        {
            int                     nCode;      ///< �����룬���ڵ���100��ʾHTTP��Ӧ�룬�����ʾִ�г���������ο�libcurl
            map<string, string>     mapHeaders; ///< ��Ӧͷ
            string                  strData;    ///< ��Ӧ����
            int                     nDataLen;   ///< ��Ӧ�����ֽڳ���
            int                     nTimeSpend; ///< ִ�к�ʱ������

            SHttpResult()
            {
                Reset();
            }
            void Reset()
            {
                nCode = 0;
                mapHeaders.clear();
                strData.clear();
                nDataLen = 0;
            }
        };

        /// HTTP URL�ṹ��
        struct SHttpUrl
        {
            string strScheme;   ///< http��https
            string strHostName; ///< ������
            int nPort;          ///< �˿�
            string strPath;     ///< ����·��
            map<string, string> mapQuerys;  ///< �������(?)
            string strTag;      ///< �����ǩ(#)
        };

        /// ����URL�ṹ
        LSLIB_API SHttpUrl          CrackUrl(lpcstr lpstrUrl);

        /// ��ԭURL�ṹΪ�ַ���
        LSLIB_API string            ReverseUrl(const SHttpUrl& vUrl);

        //////////////////////////////////////////////////////////////////////////
        /// HTTP�ͻ���
        class LSLIB_API CHttpClient
        {
        public:
            /// @brief ��ʼ��HTTP�ͻ���
            static void             Init(lpcstr lpstrDefaultCookieFile = NULL, lpcstr lpstrDefaultAgent = NULL);

            /// @brief ���HTTP�ͻ����Ƿ��ѳ�ʼ��
            static bool             IsInit();

            /// @brief ����Ĭ��cookie�����ļ�
            static void             SetDefaultCookieFile(lpcstr lpstrDefaultCookieFile);

            /// @brief ����Ĭ��UA
            static void             SetDefaultAgent(lpcstr lpstrDefaultAgent);

            /// @brief ִ��HTTP GET����
            /// @param vParam �ο�@ref SHttpGetParam
            /// @return ����ִ�н�� �ο�@ref SHttpResult
            static SHttpResult      HttpGet(const SHttpGetParam& vParam);

            /// @brief ִ��HTTP POS����
            /// @param vParam �ο�@ref SHttpPostParam
            /// @return ����ִ�н�� �ο�@ref SHttpResult
            static SHttpResult      HttpPost(const SHttpPostParam& vParam);

            /// @brief ִ��HTTP ��������
            /// @param vParam �ο�@ref SHttpDowloadParam
            /// @return ����ִ�н�� �ο�@ref SHttpResult
            static SHttpResult      DownloadFile(const SHttpDowloadParam& vParam);

            /// @brief ִ��HTTP �ϴ�����
            /// @param vParam �ο�@ref SHttpUploadParam
            /// @return ����ִ�н�� �ο�@ref SHttpResult
            static SHttpResult      UploadFile(const SHttpUploadParam& vParam);

        protected:
            static int              Perform(CURL* pCurl, const SHttpParam& vParam, __inout__ SHttpResult& vResult);
            static string           DumpParamText(SHttpParam* pParam);
            static string           DumpResultText(SHttpResult* pResult);

        private:
            static string           m_strDefaultAgent;
            static string           m_strDefaultCookieFile;
        };

        //////////////////////////////////////////////////////////////////////////
        // for multi-thread calling CHttpClient, must call following two functions in main thread to avoid https multi-thread unsafety.

        /// ���̵߳���CHttpClient APIʱ��Ϊ��֤�̰߳�ȫ����Ҫ��ȫ�ֳ�ʼ������
        extern LSLIB_API int httpclient_thread_setup(void);

        /// ���̵߳���CHttpClient APIʱ��Ϊ��֤�̰߳�ȫ����Ҫ��ȫ�ֳ�ʼ������
        extern LSLIB_API int httpclient_thread_cleanup(void);

    } // namespace net

} // namespace lslib

#endif // endof USE_LIBCURL
