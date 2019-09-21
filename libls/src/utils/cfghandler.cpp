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
        _lpustr pModuleStart;
        _lpustr pKeyStart;
        _lpustr pKeyEnd;
        _lpustr pModuleEnd;

        tagPointer() { memset(this, 0, sizeof(tagPointer)); }
    };

    static int GetHandler(_lpcstr pFile, FILE*& hFile, _lpustr* pOutBuf, _lpint dwBufLen /*= NULL*/)
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
        _lint dwFileSize = ftell(hFile);
        fseek(hFile, 0, SEEK_SET);

        // new buffer
        _lpustr pBuffer = (_lpustr)malloc(dwFileSize);
        memset(pBuffer, 0, dwFileSize);

        _lint dwReadSize = fread(pBuffer, 1, dwFileSize, hFile);
        if (dwReadSize < 0)
        {
            ERROR_LOG(g_logger, "read file fail, error: %d", errno);
            return READ_FILE_FAIL;
        }
        if (dwFileSize != dwReadSize)
        {
            WARN_LOG(g_logger, "read size not equal file size. dwFileSize[%d], dwReadSize[%d]", dwFileSize, dwReadSize);
        }

        _lint dwCryptSize = 0;
        string strCryptBuf = crypto::des_decrypt_cbc((_lpcstr)pBuffer, dwReadSize, DEFAULT_CRYPT_KEY, crypto::crypto_pkcs7padding, DEFAULT_CRYPT_IV, (int*)&dwCryptSize);
        if (dwCryptSize > 0)
        {
            free(pBuffer);
            pBuffer = (_lpustr)malloc(dwCryptSize + 1);
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

    static bool CloseHandler(FILE* hFile, _lpustr pBuffer)
    {
        if (pBuffer != NULL)
            free(pBuffer);

        if (hFile != NULL)
            return fclose(hFile);

        return true;
    }

    static int GetPointer(_lpcstr pModule, _lpcstr pKey, _lpustr pBuffer, _lint dwBufLen, tagPointer& pt)
    {
        if (pModule == NULL || pKey == NULL /*|| pBuffer == NULL || dwBufLen < 0*/) return INVALID_PARAM;

        _lpustr pModuleStart = NULL, pModuleEnd = NULL;
        _lpustr pStart = NULL, pEnd = NULL;
        _lint dwModuleLen = strlen(pModule);
        _lint dwKeyLen = strlen(pKey);

        // module start
        _lpustr pCh = pBuffer;
        for (; pCh < pBuffer + dwBufLen; pCh++)
        {
            if (memcmp(pCh, &FLAG_MODULE_BEGIN, FLAG_LENGTH) == 0 &&
                    memcmp(pCh + FLAG_LENGTH, &dwModuleLen, sizeof(_lint)) == 0 &&
                    memcmp(pCh + FLAG_LENGTH + sizeof(_lint), pModule, dwModuleLen) == 0)
            {
                pModuleStart = pCh + FLAG_LENGTH;
                break;
            }
        }
        pt.pModuleStart = pModuleStart;
        if (pModuleStart == NULL) return MODULE_NOT_EXIST;
        pCh += FLAG_LENGTH + sizeof(_lint) + dwModuleLen;

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
        pCh = pModuleStart + sizeof(_lint) + dwModuleLen;
        for (; pCh < pModuleEnd; pCh++)
        {
            if (memcmp(pCh, &FLAG_ITEM_BEGIN, FLAG_LENGTH) == 0 &&
                    memcmp(pCh + FLAG_LENGTH, &dwKeyLen, sizeof(_lint)) == 0 &&
                    memcmp(pCh + FLAG_LENGTH + sizeof(_lint), pKey, dwKeyLen) == 0)
            {
                pStart = pCh + FLAG_LENGTH;
                break;
            }
        }
        pt.pKeyStart = pStart;
        if (pStart == NULL) return KEY_NOT_EXIST;
        pCh += FLAG_LENGTH + sizeof(_lint) + dwKeyLen;

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

    static int GetCfg(__out__ _lpvoid& pOut, _lpcstr pModule, _lpcstr pKey, _lpcstr pFile) // remember to free pOut for caller
    {
        m_mtxLock.Lock();

        FILE* hFile = NULL;
        _lpustr pBuffer = NULL;
        _lint dwBufLen = 0;
        tagPointer pt;

        GetHandler(pFile, hFile, &pBuffer, &dwBufLen);
        if (GetPointer(pModule, pKey, pBuffer, dwBufLen, pt) != 0)
        {
            CloseHandler(hFile, pBuffer);
            m_mtxLock.Unlock();
            return -1;
        }

        _lint dwKeyLen = strlen(pKey);
        _lint nValueLen = 0;
        memcpy(&nValueLen, pt.pKeyStart + sizeof(_lint) + dwKeyLen + 1, sizeof(_lint));
        _lpvoid pbuf = malloc(nValueLen);
        memset(pbuf, 0, nValueLen);
        memcpy(pbuf, pt.pKeyStart + sizeof(_lint) + dwKeyLen + 1 + sizeof(_lint), nValueLen);
        pOut = pbuf;

        CloseHandler(hFile, pBuffer);

        m_mtxLock.Unlock();
        return nValueLen;
    }

    static int SetCfg(_lpcstr pModule, _lpcstr pKey, int nValueType, _lpvoid pValue, _lint dwValueLen, _lpcstr pFile)
    {
        m_mtxLock.Lock();

        FILE* hFile = NULL;
        _lpustr pBuffer = NULL;
        _lint dwBufLen = 0;
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
        _lpustr pCh = NULL;
        _lint dwModuleLen = strlen(pModule);
        _lint dwKeyLen = strlen(pKey);

        // Data format:
        //  01 01 01 01 | xx xx xx xx | xx xx ... xx | 02 02 02 02 | xx xx xx xx | xx xx ... xx | 0x         | xx xx xx xx | xx xx ... xx | 03 03 03 03 | 04 04 04 04
        // Module Begin | Module Len  | Module Name  | Item Begin  | Key Len     | Key Name     | Value type | Value Len   | Value        | Item end    | Module end
        _lint dwNewBufLen = dwBufLen //old buffer length
                            + FLAG_LENGTH + sizeof(_lint) + dwModuleLen //additional module length
                            + FLAG_LENGTH + sizeof(_lint) + dwKeyLen //additional key length
                            + 1 + sizeof(_lint) + dwValueLen //additional value length
                            + FLAG_LENGTH  //additional item end flag length
                            + FLAG_LENGTH;  //additional module end flag length
        _lpustr pNewBuffer = (_lpustr)malloc(dwNewBufLen);
        memset(pNewBuffer, 0, dwNewBufLen);
        _lint dwNewBufWriten = 0;

        switch (nRet)
        {
            case 0:
                {
                    pCh = pBuffer;
                    while (pCh < pBuffer + dwBufLen)
                    {
                        if (pCh < pt.pKeyStart + sizeof(_lint) + dwKeyLen || pCh >= pt.pKeyEnd)
                        {
                            memcpy(pNewBuffer + dwNewBufWriten, pCh, 1);
                            dwNewBufWriten++;
                            pCh++;
                        }
                        else
                        {
                            memcpy(pNewBuffer + dwNewBufWriten, &nValueType, 1); //value type
                            dwNewBufWriten += 1;
                            memcpy(pNewBuffer + dwNewBufWriten, &dwValueLen, sizeof(_lint)); //value length
                            dwNewBufWriten += sizeof(_lint);
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

                    memcpy(pNewBuffer + dwNewBufWriten, &dwModuleLen, sizeof(_lint)); // module length
                    dwNewBufWriten += sizeof(_lint);
                    memcpy(pNewBuffer + dwNewBufWriten, pModule, dwModuleLen); // module content
                    dwNewBufWriten += dwModuleLen;

                    memcpy(pNewBuffer + dwNewBufWriten, &FLAG_ITEM_BEGIN, FLAG_LENGTH); // one config item begin flag
                    dwNewBufWriten += FLAG_LENGTH;

                    memcpy(pNewBuffer + dwNewBufWriten, &dwKeyLen, sizeof(_lint)); // key length
                    dwNewBufWriten += sizeof(_lint);
                    memcpy(pNewBuffer + dwNewBufWriten, pKey, dwKeyLen); //key content
                    dwNewBufWriten += dwKeyLen;

                    memcpy(pNewBuffer + dwNewBufWriten, &nValueType, 1); //value type
                    dwNewBufWriten += 1;
                    memcpy(pNewBuffer + dwNewBufWriten, &dwValueLen, sizeof(_lint)); //value length
                    dwNewBufWriten += sizeof(_lint);
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
                    _lpustr pCh = pBuffer;
                    while (pCh <= pBuffer + dwBufLen)
                    {
                        if (pCh == pt.pModuleEnd)
                        {
                            memcpy(pNewBuffer + dwNewBufWriten, &FLAG_ITEM_BEGIN, FLAG_LENGTH); // one config item begin flag
                            dwNewBufWriten += FLAG_LENGTH;

                            memcpy(pNewBuffer + dwNewBufWriten, &dwKeyLen, sizeof(_lint)); // key length
                            dwNewBufWriten += sizeof(_lint);
                            memcpy(pNewBuffer + dwNewBufWriten, pKey, dwKeyLen); //key content
                            dwNewBufWriten += dwKeyLen;

                            memcpy(pNewBuffer + dwNewBufWriten, &nValueType, 1); //value type
                            dwNewBufWriten += 1;
                            memcpy(pNewBuffer + dwNewBufWriten, &dwValueLen, sizeof(_lint)); //value length
                            dwNewBufWriten += sizeof(_lint);
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
        string strCryptBuf = crypto::des_encrypt_cbc((_lpcstr)pNewBuffer, dwNewBufWriten, DEFAULT_CRYPT_KEY, crypto::crypto_pkcs7padding, DEFAULT_CRYPT_IV, &nCryptSize);
        if (nCryptSize > 0)
        {
            free(pNewBuffer);
            pNewBuffer = (_lpustr)strCryptBuf.data();
            dwNewBufWriten = nCryptSize;
        }
        else
        {
            WARN_LOG(g_logger, "encrypte string fail. err[%d]. try raw data...", nCryptSize);
        }

        fseek(hFile, 0, SEEK_SET);
        _lint dwWrite = fwrite(pNewBuffer, 1, dwNewBufWriten, hFile);
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

    string CCfgHandler::GetCfgText(_lpcstr pModule, _lpcstr pKey, _lpcstr pDefault, _lpcstr pFile)
    {
        _lpvoid pbuf = NULL;
        int len = GetCfg(pbuf, pModule, pKey, pFile);
        if (len < 0) return pDefault;
        string str; str.assign((_lpcstr)pbuf, len);
        free(pbuf);
        return str;
    }

    int CCfgHandler::GetCfgInt(_lpcstr pModule, _lpcstr pKey, int nDefault, _lpcstr pFile)
    {
        _lpvoid pbuf = NULL;
        int len = GetCfg(pbuf, pModule, pKey, pFile);
        if (len < 0) return nDefault;
        int ret = *(int*)pbuf;
        free(pbuf);
        return ret;
    }

    float CCfgHandler::GetCfgFloat(_lpcstr pModule, _lpcstr pKey, float fDefault, _lpcstr pFile)
    {
        _lpvoid pbuf = NULL;
        int len = GetCfg(pbuf, pModule, pKey, pFile);
        if (len < 0) return fDefault;
        float ret = *(float*)pbuf;
        free(pbuf);
        return ret;
    }

    int CCfgHandler::GetCfgBlob(__out__ _lpvoid& pOut, _lpcstr pModule, _lpcstr pKey, _lpcstr pFile)
    {
        return GetCfg(pOut, pModule, pKey, pFile);
    }

    int CCfgHandler::SetCfg(_lpcstr pModule, _lpcstr pKey, _lpcstr value, _lpcstr pFile)
    {
        return lslib::SetCfg(pModule, pKey, CVT_STRING, (_lpvoid)value, strlen(value), pFile);
    }

    int CCfgHandler::SetCfg(_lpcstr pModule, _lpcstr pKey, int value, _lpcstr pFile)
    {
        return lslib::SetCfg(pModule, pKey, CVT_INT, (_lpvoid)&value, sizeof(int), pFile);
    }

    int CCfgHandler::SetCfg(_lpcstr pModule, _lpcstr pKey, float value, _lpcstr pFile)
    {
        return lslib::SetCfg(pModule, pKey, CVT_FLOAT, (_lpvoid)&value, sizeof(float), pFile);
    }

    int CCfgHandler::SetCfg(_lpcstr pModule, _lpcstr pKey, _lpvoid pValue, _lint dwValueLen, _lpcstr pFile)
    {
        return lslib::SetCfg(pModule, pKey, CVT_BLOB, pValue, dwValueLen, pFile);
    }

    int CCfgHandler::LoadCfg(__out__ SCfgData_list& lst, _lpcstr pFile)
    {
        m_mtxLock.Lock();

        FILE* hFile = NULL;
        _lpustr pBuffer = NULL;
        _lint dwBufLen = 0;
        tagPointer pt;

        int ret = GetHandler(pFile, hFile, &pBuffer, &dwBufLen);
        if (ret != 0)
        {
            m_mtxLock.Unlock();
            return ret;
        }

        _lpustr pCh = pBuffer;
        while (pCh != NULL)
        {
            _lpustr pModuleStart = NULL, pModuleEnd = NULL;
            _lpustr tcModule = NULL;
            _lint dwModuleLen = 0;

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
                memcpy(&dwModuleLen, pModuleStart, sizeof(_lint));
                tcModule = (_lpustr)malloc(dwModuleLen + 1);
                memcpy(tcModule, pModuleStart + sizeof(_lint), dwModuleLen);
                tcModule[dwModuleLen] = 0;
                pCh += FLAG_LENGTH + sizeof(_lint) + dwModuleLen;
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

            _lpustr pCh2 = pModuleStart;
            for (; pCh2 < pModuleEnd; pCh2++)
            {
                _lpustr pKey = NULL, pValue = NULL, pEnd = NULL;
                _lpustr tcKey = NULL;
                int nValueType = 0;
                _lpustr tcValue = NULL;
                _lint dwKeyLen = 0, dwValueLen = 0;

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
                    memcpy(&dwKeyLen, pKey, sizeof(_lint));
                    tcKey = (_lpustr)malloc(dwKeyLen + 1);
                    memcpy(tcKey, pKey + sizeof(_lint), dwKeyLen);
                    tcKey[dwKeyLen] = 0;
                    pCh2 += FLAG_LENGTH + sizeof(_lint) + dwKeyLen;
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
                    memcpy(&dwValueLen, pValue, sizeof(_lint));
                    tcValue = (_lpustr)malloc(dwValueLen + 1);
                    memcpy(tcValue, pValue + sizeof(_lint), dwValueLen);
                    tcValue[dwValueLen] = 0;
                }

                SCfgData cfg;
                cfg.strModule = (char*)tcModule;
                cfg.strKey = (char*)tcKey;
                cfg.eValueType = (ECfgValueType)nValueType;
                if (cfg.eValueType == CVT_STRING)       cfg.v_str.assign((const char*)tcValue, dwValueLen);
                else if (cfg.eValueType == CVT_INT)     memcpy(&cfg.v_int, tcValue, sizeof(_lint));
                else if (cfg.eValueType == CVT_FLOAT)   memcpy(&cfg.v_float, tcValue, sizeof(_lint));
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
