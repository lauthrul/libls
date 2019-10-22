#include "stdafx.h"
#include "cfghandler.h"
#include <cerrno>

namespace lslib
{
    using namespace logger;

    const int FLAG_MODULE_BEGIN = 0x01010101;
    const int FLAG_ITEM_BEGIN   = 0x02020202;
    const int FLAG_ITEM_END     = 0x03030303;
    const int FLAG_MODULE_END   = 0x04040404;
    const int FLAG_LENGTH       = 4;

    CMutexLock m_mtxLock;

    enum_str_begin(ECfgValueType)
    enum_str_item_ex(CVT_INT, "int")
    enum_str_item_ex(CVT_FLOAT, "float")
    enum_str_item_ex(CVT_STRING, "string")
    enum_str_item_ex(CVT_BLOB, "blob")
    enum_str_end(ECfgValueType)

    struct tagPointer
    {
        lpustr pModuleStart;
        lpustr pKeyStart;
        lpustr pKeyEnd;
        lpustr pModuleEnd;

        tagPointer() { memset(this, 0, sizeof(tagPointer)); }
    };

    static int GetHandler(lpcstr pFile, FILE*& hFile, lpustr* pOutBuf, lpint dwBufLen /*= NULL*/)
    {
        if (strtool::is_empty(pFile)) return INVALID_PARAM;

        // check if file exist
        if (!os::is_exist(pFile)) return FILE_NOT_EXIST;

        // open file with read && write permission
        hFile = fopen(pFile, "rb+");
        if (hFile == NULL)
        {
            ERROR_LOG(g_logger, "open file fail, error[%d]", errno);
            return OPEN_FILE_FAIL;
        }

        // get file size
        fseek(hFile, 0, SEEK_END);
        lint dwFileSize = ftell(hFile);
        fseek(hFile, 0, SEEK_SET);

        // new buffer
        lpustr pBuffer = (lpustr)malloc(dwFileSize);
        memset(pBuffer, 0, dwFileSize);

        lint dwReadSize = fread(pBuffer, 1, dwFileSize, hFile);
        if (dwReadSize < 0)
        {
            ERROR_LOG(g_logger, "read file fail, error: %d", errno);
            return READ_FILE_FAIL;
        }
        if (dwFileSize != dwReadSize)
        {
            WARN_LOG(g_logger, "read size not equal file size. dwFileSize[%d], dwReadSize[%d]", dwFileSize, dwReadSize);
        }

        lint dwCryptSize = 0;
        string strCryptBuf = crypto::des_decrypt_cbc(pBuffer, dwReadSize, DEFAULT_CRYPT_KEY, crypto::crypto_pkcs7padding, DEFAULT_CRYPT_IV, (int*)&dwCryptSize);
        if (dwCryptSize > 0)
        {
            free(pBuffer);
            pBuffer = (lpustr)malloc(dwCryptSize + 1);
            memcpy(pBuffer, strCryptBuf.data(), dwCryptSize);
            pBuffer[dwCryptSize] = 0;
            dwReadSize = dwCryptSize;
        }
        else
        {
            WARN_LOG(g_logger, "decrypte string fail. err[%d]. try raw data...", errno);
        }

        if (pOutBuf) *pOutBuf = pBuffer;
        if (dwBufLen) *dwBufLen = dwReadSize;

        return 0;
    }

    static bool CloseHandler(FILE* hFile, lpustr pBuffer)
    {
        if (pBuffer != NULL)
            free(pBuffer);

        if (hFile != NULL)
            return fclose(hFile);

        return true;
    }

    static int GetPointer(lpcstr pModule, lpcstr pKey, lpustr pBuffer, lint dwBufLen, tagPointer& pt)
    {
        if (pModule == NULL || pKey == NULL /*|| pBuffer == NULL || dwBufLen < 0*/) return INVALID_PARAM;

        lpustr pModuleStart = NULL, pModuleEnd = NULL;
        lpustr pStart = NULL, pEnd = NULL;
        lint dwModuleLen = strlen(pModule);
        lint dwKeyLen = strlen(pKey);

        // module start
        lpustr pCh = pBuffer;
        for (; pCh < pBuffer + dwBufLen; pCh++)
        {
            if (memcmp(pCh, &FLAG_MODULE_BEGIN, FLAG_LENGTH) == 0 &&
                    memcmp(pCh + FLAG_LENGTH, &dwModuleLen, sizeof(lint)) == 0 &&
                    memcmp(pCh + FLAG_LENGTH + sizeof(lint), pModule, dwModuleLen) == 0)
            {
                pModuleStart = pCh + FLAG_LENGTH;
                break;
            }
        }
        pt.pModuleStart = pModuleStart;
        if (pModuleStart == NULL) return MODULE_NOT_EXIST;
        pCh += FLAG_LENGTH + sizeof(lint) + dwModuleLen;

        // module end
        for (; pCh < pBuffer + dwBufLen; pCh++)
        {
            if (memcmp(pCh, &FLAG_ITEM_END, FLAG_LENGTH) == 0 &&
                    memcmp(pCh + FLAG_LENGTH, &FLAG_MODULE_END, FLAG_LENGTH) == 0)
            {
                pModuleEnd = pCh + FLAG_LENGTH;
                break;
            }
        }
        pt.pModuleEnd = pModuleEnd;
        if (pModuleEnd == NULL) return INVALID_MODULE;

        // key start
        pCh = pModuleStart + sizeof(lint) + dwModuleLen;
        for (; pCh < pModuleEnd; pCh++)
        {
            if (memcmp(pCh, &FLAG_ITEM_BEGIN, FLAG_LENGTH) == 0 &&
                    memcmp(pCh + FLAG_LENGTH, &dwKeyLen, sizeof(lint)) == 0 &&
                    memcmp(pCh + FLAG_LENGTH + sizeof(lint), pKey, dwKeyLen) == 0)
            {
                pStart = pCh + FLAG_LENGTH;
                break;
            }
        }
        pt.pKeyStart = pStart;
        if (pStart == NULL) return KEY_NOT_EXIST;
        pCh += FLAG_LENGTH + sizeof(lint) + dwKeyLen;

        // key end
        for (; pCh < pModuleEnd; pCh++)
        {
            if (memcmp(pCh, &FLAG_ITEM_END, FLAG_LENGTH) == 0)
            {
                pEnd = pCh;
                break;
            }
        }
        pt.pKeyEnd = pEnd;

        if (pEnd == NULL) return INVALID_KEY;

        return 0;
    }

    static int GetCfg(__out__ lpvoid& pOut, lpcstr pModule, lpcstr pKey, lpcstr pFile) // remember to free pOut for caller
    {
        m_mtxLock.Lock();

        FILE* hFile = NULL;
        lpustr pBuffer = NULL;
        lint dwBufLen = 0;
        tagPointer pt;

        GetHandler(pFile, hFile, &pBuffer, &dwBufLen);
        if (GetPointer(pModule, pKey, pBuffer, dwBufLen, pt) != 0)
        {
            CloseHandler(hFile, pBuffer);
            m_mtxLock.Unlock();
            return -1;
        }

        lint dwKeyLen = strlen(pKey);
        lint nValueLen = 0;
        memcpy(&nValueLen, pt.pKeyStart + sizeof(lint) + dwKeyLen + 1, sizeof(lint));
        lpvoid pbuf = malloc(nValueLen);
        memset(pbuf, 0, nValueLen);
        memcpy(pbuf, pt.pKeyStart + sizeof(lint) + dwKeyLen + 1 + sizeof(lint), nValueLen);
        pOut = pbuf;

        CloseHandler(hFile, pBuffer);

        m_mtxLock.Unlock();
        return nValueLen;
    }

    static int SetCfg(lpcstr pModule, lpcstr pKey, int nValueType, lpvoid pValue, lint dwValueLen, lpcstr pFile)
    {
        m_mtxLock.Lock();

        FILE* hFile = NULL;
        lpustr pBuffer = NULL;
        lint dwBufLen = 0;
        tagPointer pt;

        int nRet = GetHandler(pFile, hFile, &pBuffer, &dwBufLen);
        if (nRet == FILE_NOT_EXIST)
        {
            hFile = fopen(pFile, "wb+");
            nRet = (hFile ? 0 : OPEN_FILE_FAIL);
        }
        if (nRet != 0 && nRet != FILE_NOT_EXIST)
        {
            m_mtxLock.Unlock();
            return nRet;
        }
        else if (nRet == 0) nRet = GetPointer(pModule, pKey, pBuffer, dwBufLen, pt);

        bool bRet = true;
        lpustr pCh = NULL;
        lint dwModuleLen = strlen(pModule);
        lint dwKeyLen = strlen(pKey);

        // Data format:
        //  01 01 01 01 | xx xx xx xx | xx xx ... xx | 02 02 02 02 | xx xx xx xx | xx xx ... xx | 0x         | xx xx xx xx | xx xx ... xx | 03 03 03 03 | 04 04 04 04
        // Module Begin | Module Len  | Module Name  | Item Begin  | Key Len     | Key Name     | Value type | Value Len   | Value        | Item end    | Module end
        lint dwNewBufLen = dwBufLen //old buffer length
                            + FLAG_LENGTH + sizeof(lint) + dwModuleLen //additional module length
                            + FLAG_LENGTH + sizeof(lint) + dwKeyLen //additional key length
                            + 1 + sizeof(lint) + dwValueLen //additional value length
                            + FLAG_LENGTH  //additional item end flag length
                            + FLAG_LENGTH;  //additional module end flag length
        lpustr pNewBuffer = (lpustr)malloc(dwNewBufLen);
        memset(pNewBuffer, 0, dwNewBufLen);
        lint dwNewBufWriten = 0;

        switch (nRet)
        {
            case 0:
                {
                    pCh = pBuffer;
                    while (pCh < pBuffer + dwBufLen)
                    {
                        if (pCh < pt.pKeyStart + sizeof(lint) + dwKeyLen || pCh >= pt.pKeyEnd)
                        {
                            memcpy(pNewBuffer + dwNewBufWriten, pCh, 1);
                            dwNewBufWriten++;
                            pCh++;
                        }
                        else
                        {
                            memcpy(pNewBuffer + dwNewBufWriten, &nValueType, 1); //value type
                            dwNewBufWriten += 1;
                            memcpy(pNewBuffer + dwNewBufWriten, &dwValueLen, sizeof(lint)); //value length
                            dwNewBufWriten += sizeof(lint);
                            memcpy(pNewBuffer + dwNewBufWriten, pValue, dwValueLen); // value content
                            dwNewBufWriten += dwValueLen;
                            pCh = pt.pKeyEnd;
                        }
                    }
                }
                break;
            case MODULE_NOT_EXIST:
                memcpy(pNewBuffer + dwNewBufWriten, pBuffer, dwBufLen);
                dwNewBufWriten += dwBufLen;
            case FILE_NOT_EXIST:
                {
                    memcpy(pNewBuffer + dwNewBufWriten, &FLAG_MODULE_BEGIN, FLAG_LENGTH); // one module item begin flag
                    dwNewBufWriten += FLAG_LENGTH;

                    memcpy(pNewBuffer + dwNewBufWriten, &dwModuleLen, sizeof(lint)); // module length
                    dwNewBufWriten += sizeof(lint);
                    memcpy(pNewBuffer + dwNewBufWriten, pModule, dwModuleLen); // module content
                    dwNewBufWriten += dwModuleLen;

                    memcpy(pNewBuffer + dwNewBufWriten, &FLAG_ITEM_BEGIN, FLAG_LENGTH); // one config item begin flag
                    dwNewBufWriten += FLAG_LENGTH;

                    memcpy(pNewBuffer + dwNewBufWriten, &dwKeyLen, sizeof(lint)); // key length
                    dwNewBufWriten += sizeof(lint);
                    memcpy(pNewBuffer + dwNewBufWriten, pKey, dwKeyLen); //key content
                    dwNewBufWriten += dwKeyLen;

                    memcpy(pNewBuffer + dwNewBufWriten, &nValueType, 1); //value type
                    dwNewBufWriten += 1;
                    memcpy(pNewBuffer + dwNewBufWriten, &dwValueLen, sizeof(lint)); //value length
                    dwNewBufWriten += sizeof(lint);
                    memcpy(pNewBuffer + dwNewBufWriten, pValue, dwValueLen); // value content
                    dwNewBufWriten += dwValueLen;

                    memcpy(pNewBuffer + dwNewBufWriten, &FLAG_ITEM_END, FLAG_LENGTH); // one config item end flag
                    dwNewBufWriten += FLAG_LENGTH;

                    memcpy(pNewBuffer + dwNewBufWriten, &FLAG_MODULE_END, FLAG_LENGTH); // one module item end flag
                    dwNewBufWriten += FLAG_LENGTH;
                }
                break;
            case KEY_NOT_EXIST:
                {
                    lpustr pCh = pBuffer;
                    while (pCh <= pBuffer + dwBufLen)
                    {
                        if (pCh == pt.pModuleEnd)
                        {
                            memcpy(pNewBuffer + dwNewBufWriten, &FLAG_ITEM_BEGIN, FLAG_LENGTH); // one config item begin flag
                            dwNewBufWriten += FLAG_LENGTH;

                            memcpy(pNewBuffer + dwNewBufWriten, &dwKeyLen, sizeof(lint)); // key length
                            dwNewBufWriten += sizeof(lint);
                            memcpy(pNewBuffer + dwNewBufWriten, pKey, dwKeyLen); //key content
                            dwNewBufWriten += dwKeyLen;

                            memcpy(pNewBuffer + dwNewBufWriten, &nValueType, 1); //value type
                            dwNewBufWriten += 1;
                            memcpy(pNewBuffer + dwNewBufWriten, &dwValueLen, sizeof(lint)); //value length
                            dwNewBufWriten += sizeof(lint);
                            memcpy(pNewBuffer + dwNewBufWriten, pValue, dwValueLen); // value content
                            dwNewBufWriten += dwValueLen;

                            memcpy(pNewBuffer + dwNewBufWriten, &FLAG_ITEM_END, FLAG_LENGTH); // one config item end flag
                            dwNewBufWriten += FLAG_LENGTH;
                        }
                        else if (pCh < pt.pModuleEnd)
                        {
                            memcpy(pNewBuffer + dwNewBufWriten, pCh, 1);
                            dwNewBufWriten++;
                        }
                        else
                        {
                            memcpy(pNewBuffer + dwNewBufWriten, pCh - 1, 1);
                            dwNewBufWriten++;
                        }
                        pCh++;
                    }
                }
                break;
            default:
                bRet = false;
                break;
        }

        int nCryptSize = 0;
        string strCryptBuf = crypto::des_encrypt_cbc(pNewBuffer, dwNewBufWriten, DEFAULT_CRYPT_KEY, crypto::crypto_pkcs7padding, DEFAULT_CRYPT_IV, &nCryptSize);
        if (nCryptSize > 0)
        {
            free(pNewBuffer);
            pNewBuffer = (lpustr)strCryptBuf.data();
            dwNewBufWriten = nCryptSize;
        }
        else
        {
            WARN_LOG(g_logger, "encrypte string fail. err[%d]. try raw data...", nCryptSize);
        }

        fseek(hFile, 0, SEEK_SET);
        lint dwWrite = fwrite(pNewBuffer, 1, dwNewBufWriten, hFile);
        CloseHandler(hFile, pBuffer);

        m_mtxLock.Unlock();

        if (dwWrite <= 0)
        {
            WARN_LOG(g_logger, "write cfg file fail. err[%d]. try raw data...", errno);
            return WRITE_FILE_FAIL;
        }

        return bRet ? 0 : CCFGHANDLER_ERROR;
    }

    //////////////////////////////////////////////////////////////////////////

    string CCfgHandler::GetCfgText(lpcstr pModule, lpcstr pKey, lpcstr pDefault, lpcstr pFile)
    {
        lpvoid pbuf = NULL;
        int len = GetCfg(pbuf, pModule, pKey, pFile);
        if (len < 0) return pDefault;
        string str; str.assign((lpcstr)pbuf, len);
        free(pbuf);
        return str;
    }

    int CCfgHandler::GetCfgInt(lpcstr pModule, lpcstr pKey, int nDefault, lpcstr pFile)
    {
        lpvoid pbuf = NULL;
        int len = GetCfg(pbuf, pModule, pKey, pFile);
        if (len < 0) return nDefault;
        int ret = *(int*)pbuf;
        free(pbuf);
        return ret;
    }

    float CCfgHandler::GetCfgFloat(lpcstr pModule, lpcstr pKey, float fDefault, lpcstr pFile)
    {
        lpvoid pbuf = NULL;
        int len = GetCfg(pbuf, pModule, pKey, pFile);
        if (len < 0) return fDefault;
        float ret = *(float*)pbuf;
        free(pbuf);
        return ret;
    }

    int CCfgHandler::GetCfgBlob(__out__ lpvoid& pOut, lpcstr pModule, lpcstr pKey, lpcstr pFile)
    {
        return GetCfg(pOut, pModule, pKey, pFile);
    }

    int CCfgHandler::SetCfg(lpcstr pModule, lpcstr pKey, lpcstr value, lpcstr pFile)
    {
        return lslib::SetCfg(pModule, pKey, CVT_STRING, (lpvoid)value, strlen(value), pFile);
    }

    int CCfgHandler::SetCfg(lpcstr pModule, lpcstr pKey, int value, lpcstr pFile)
    {
        return lslib::SetCfg(pModule, pKey, CVT_INT, (lpvoid)&value, sizeof(int), pFile);
    }

    int CCfgHandler::SetCfg(lpcstr pModule, lpcstr pKey, float value, lpcstr pFile)
    {
        return lslib::SetCfg(pModule, pKey, CVT_FLOAT, (lpvoid)&value, sizeof(float), pFile);
    }

    int CCfgHandler::SetCfg(lpcstr pModule, lpcstr pKey, lpvoid pValue, lint dwValueLen, lpcstr pFile)
    {
        return lslib::SetCfg(pModule, pKey, CVT_BLOB, pValue, dwValueLen, pFile);
    }

    int CCfgHandler::LoadCfg(__out__ SCfgData_list& lst, lpcstr pFile)
    {
        m_mtxLock.Lock();

        FILE* hFile = NULL;
        lpustr pBuffer = NULL;
        lint dwBufLen = 0;
        tagPointer pt;

        int ret = GetHandler(pFile, hFile, &pBuffer, &dwBufLen);
        if (ret != 0)
        {
            m_mtxLock.Unlock();
            return ret;
        }

        lpustr pCh = pBuffer;
        while (pCh != NULL)
        {
            lpustr pModuleStart = NULL, pModuleEnd = NULL;
            lpustr tcModule = NULL;
            lint dwModuleLen = 0;

            // module start
            for (; pCh < pBuffer + dwBufLen; pCh++)
            {
                if (memcmp(pCh, &FLAG_MODULE_BEGIN, FLAG_LENGTH) == 0)
                {
                    pModuleStart = pCh + FLAG_LENGTH;
                    break;
                }
            }
            if (pModuleStart == NULL) break;
            else
            {
                memcpy(&dwModuleLen, pModuleStart, sizeof(lint));
                tcModule = (lpustr)malloc(dwModuleLen + 1);
                memcpy(tcModule, pModuleStart + sizeof(lint), dwModuleLen);
                tcModule[dwModuleLen] = 0;
                pCh += FLAG_LENGTH + sizeof(lint) + dwModuleLen;
            }

            // module end
            for (; pCh < pBuffer + dwBufLen; pCh++)
            {
                if (memcmp(pCh, &FLAG_MODULE_END, FLAG_LENGTH) == 0)
                {
                    pModuleEnd = pCh;
                    break;
                }
            }
            if (pModuleEnd == NULL)
            {
                if (tcModule != NULL) free(tcModule);
                break;
            }
            else pCh = pModuleEnd /*+ FLAG_LENGTH*/;

            lpustr pCh2 = pModuleStart;
            for (; pCh2 < pModuleEnd; pCh2++)
            {
                lpustr pKey = NULL, pValue = NULL, pEnd = NULL;
                lpustr tcKey = NULL;
                int nValueType = 0;
                lpustr tcValue = NULL;
                lint dwKeyLen = 0, dwValueLen = 0;

                // key start
                for (; pCh2 < pModuleEnd; pCh2++)
                {
                    if (memcmp(pCh2, &FLAG_ITEM_BEGIN, FLAG_LENGTH) == 0)
                    {
                        pKey = pCh2 + FLAG_LENGTH;
                        break;
                    }
                }
                if (pKey == NULL) break;
                else
                {
                    memcpy(&dwKeyLen, pKey, sizeof(lint));
                    tcKey = (lpustr)malloc(dwKeyLen + 1);
                    memcpy(tcKey, pKey + sizeof(lint), dwKeyLen);
                    tcKey[dwKeyLen] = 0;
                    pCh2 += FLAG_LENGTH + sizeof(lint) + dwKeyLen;
                    memcpy(&nValueType, pCh2, 1);
                    pValue = pCh2 + 1;
                }

                // key end
                for (; pCh2 < pModuleEnd; pCh2++)
                {
                    if (memcmp(pCh2, &FLAG_ITEM_END, FLAG_LENGTH) == 0)
                    {
                        pEnd = pCh2;
                        break;
                    }
                }
                if (pEnd == NULL)
                {
                    if (tcKey != NULL) free(tcKey);
                    break;
                }
                else pCh2 = pEnd /*+ FLAG_LENGTH*/;

                if (pValue < pEnd)
                {
                    memcpy(&dwValueLen, pValue, sizeof(lint));
                    tcValue = (lpustr)malloc(dwValueLen + 1);
                    memcpy(tcValue, pValue + sizeof(lint), dwValueLen);
                    tcValue[dwValueLen] = 0;
                }

                SCfgData cfg;
                cfg.strModule = (char*)tcModule;
                cfg.strKey = (char*)tcKey;
                cfg.eValueType = (ECfgValueType)nValueType;
                if (cfg.eValueType == CVT_STRING)       cfg.v_str.assign((const char*)tcValue, dwValueLen);
                else if (cfg.eValueType == CVT_INT)     memcpy(&cfg.v_int, tcValue, sizeof(lint));
                else if (cfg.eValueType == CVT_FLOAT)   memcpy(&cfg.v_float, tcValue, sizeof(lint));
                else if (cfg.eValueType == CVT_BLOB)
                {
                    cfg.v_blob.assign((const char*)tcValue, dwValueLen);
                    cfg.v_bloblen = dwValueLen;
                }

                lst.push_back(cfg);

                if (tcKey != NULL) free(tcKey);
                if (tcValue != NULL) free(tcValue);
            }

            if (tcModule != NULL) free(tcModule);
        }

        CloseHandler(hFile, pBuffer);
        m_mtxLock.Unlock();

        return 0;
    }
}
