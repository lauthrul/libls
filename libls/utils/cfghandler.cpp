#include "stdafx.h"
#include "cfghandler.h"
#include <cerrno>

namespace lslib
{
    using namespace logger;

#define DEFAULT_CRYPT_KEY       "l53d70p3"
#define DEFAULT_CRYPT_IV        "12345678"

    const int FLAG_MODULE_BEGIN = 0x01010101;
    const int FLAG_ITEM_BEGIN = 0x02020202;
    const int FLAG_ITEM_END = 0x03030303;
    const int FLAG_MODULE_END = 0x04040404;
    const int FLAG_LENGTH = 4;

    CMutexLock CCfgHandler::m_mtxLock;
    //////////////////////////////////////////////////////////////////////////

    string CCfgHandler::GetCfgText(_lpcstr pModule, _lpcstr pKey, _lpcstr pDefault, _lpcstr pFile)
    {
        m_mtxLock.Lock();

        FILE* hFile = NULL;
        _lpustr pBuffer = NULL;
        _ldword dwBufLen = 0;
        tagPointer pt;

        GetHandler(pFile, hFile, &pBuffer, &dwBufLen);
        if (GetPointer(pModule, pKey, pBuffer, dwBufLen, pt) != 0)
        {
            CloseHandler(hFile, pBuffer);
            m_mtxLock.Unlock();
            return pDefault;
        }

        _ldword dwKeyLen = strlen(pKey);
        int nValueLen = 0;
        memcpy(&nValueLen, pt.pKeyStart + sizeof(_ldword) + dwKeyLen + 1, sizeof(_ldword));
        _lchar* tszValue = new _lchar[nValueLen + 1];
        memset(tszValue, 0, nValueLen + 1);
        memcpy(tszValue, pt.pKeyStart + sizeof(_ldword) + dwKeyLen + 1 + sizeof(_ldword), nValueLen);
        string strValue = tszValue;

        delete[] tszValue;
        CloseHandler(hFile, pBuffer);

        m_mtxLock.Unlock();
        return strValue.empty() ? pDefault : strValue;
    }

    int CCfgHandler::GetCfgInt(_lpcstr pModule, _lpcstr pKey, int nDefault, _lpcstr pFile)
    {
        m_mtxLock.Lock();

        FILE* hFile = NULL;
        _lpustr pBuffer = NULL;
        _ldword dwBufLen = 0;
        tagPointer pt;

        GetHandler(pFile, hFile, &pBuffer, &dwBufLen);
        if (GetPointer(pModule, pKey, pBuffer, dwBufLen, pt) != 0)
        {
            CloseHandler(hFile, pBuffer);
            m_mtxLock.Unlock();
            return nDefault;
        }

        _ldword dwKeyLen = strlen(pKey);
        int nValue = nDefault;
        int nValueLen = 0;
        memcpy(&nValueLen, pt.pKeyStart + sizeof(_ldword) + dwKeyLen + 1, sizeof(_ldword));
        memcpy(&nValue, pt.pKeyStart + sizeof(_ldword) + dwKeyLen + 1 + sizeof(_ldword), nValueLen);

        CloseHandler(hFile, pBuffer);

        m_mtxLock.Unlock();
        return nValue;
    }

    float CCfgHandler::GetCfgFloat(_lpcstr pModule, _lpcstr pKey, float fDefault, _lpcstr pFile)
    {
        m_mtxLock.Lock();

        FILE* hFile = NULL;
        _lpustr pBuffer = NULL;
        _ldword dwBufLen = 0;
        tagPointer pt;

        GetHandler(pFile, hFile, &pBuffer, &dwBufLen);
        if (GetPointer(pModule, pKey, pBuffer, dwBufLen, pt) != 0)
        {
            CloseHandler(hFile, pBuffer);
            m_mtxLock.Unlock();
            return fDefault;
        }

        _ldword dwKeyLen = strlen(pKey);
        float fValue = fDefault;
        int nValueLen = 0;
        memcpy(&nValueLen, pt.pKeyStart + sizeof(_ldword) + dwKeyLen + 1, sizeof(_ldword));
        memcpy(&fValue, pt.pKeyStart + sizeof(_ldword) + dwKeyLen + 1 + sizeof(_ldword), nValueLen);

        CloseHandler(hFile, pBuffer);

        m_mtxLock.Unlock();
        return fValue;
    }

    int CCfgHandler::SetCfg(_lpcstr pModule, _lpcstr pKey, int nValueType, _lpvoid pValue, _ldword dwValueLen, _lpcstr pFile)
    {
        m_mtxLock.Lock();

        FILE* hFile = NULL;
        _lpustr pBuffer = NULL;
        _ldword dwBufLen = 0;
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
        _ldword dwModuleLen = strlen(pModule);
        _ldword dwKeyLen = strlen(pKey);

        // Data format:
        //  01 01 01 01 | xx xx xx xx | xx xx ... xx | 02 02 02 02 | xx xx xx xx | xx xx ... xx | 0x         | xx xx xx xx | xx xx ... xx | 03 03 03 03 | 04 04 04 04
        // Module Begin | Module Len  | Module Name  | Item Begin  | Key Len     | Key Name     | Value type | Value Len   | Value        | Item end    | Module end
        _ldword dwNewBufLen = dwBufLen //old buffer length
                              + FLAG_LENGTH + sizeof(_ldword) + dwModuleLen //additional module length
                              + FLAG_LENGTH + sizeof(_ldword) + dwKeyLen //additional key length
                              + 1 + sizeof(_ldword) + dwValueLen //additional value length
                              + FLAG_LENGTH  //additional item end flag length
                              + FLAG_LENGTH;  //additional module end flag length
        _lpustr pNewBuffer = (_lpustr)malloc(dwNewBufLen);
        memset(pNewBuffer, 0, dwNewBufLen);
        _ldword dwNewBufWriten = 0;

        switch (nRet)
        {
            case 0:
                {
                    pCh = pBuffer;
                    while (pCh < pBuffer + dwBufLen)
                    {
                        if (pCh < pt.pKeyStart + sizeof(_ldword) + dwKeyLen || pCh >= pt.pKeyEnd)
                        {
                            memcpy(pNewBuffer + dwNewBufWriten, pCh, 1);
                            dwNewBufWriten++;
                            pCh++;
                        }
                        else
                        {
                            memcpy(pNewBuffer + dwNewBufWriten, &nValueType, 1); //value type
                            dwNewBufWriten += 1;
                            memcpy(pNewBuffer + dwNewBufWriten, &dwValueLen, sizeof(_ldword)); //value length
                            dwNewBufWriten += sizeof(_ldword);
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

                    memcpy(pNewBuffer + dwNewBufWriten, &dwModuleLen, sizeof(_ldword)); // module length
                    dwNewBufWriten += sizeof(_ldword);
                    memcpy(pNewBuffer + dwNewBufWriten, pModule, dwModuleLen); // module content
                    dwNewBufWriten += dwModuleLen;

                    memcpy(pNewBuffer + dwNewBufWriten, &FLAG_ITEM_BEGIN, FLAG_LENGTH); // one config item begin flag
                    dwNewBufWriten += FLAG_LENGTH;

                    memcpy(pNewBuffer + dwNewBufWriten, &dwKeyLen, sizeof(_ldword)); // key length
                    dwNewBufWriten += sizeof(_ldword);
                    memcpy(pNewBuffer + dwNewBufWriten, pKey, dwKeyLen); //key content
                    dwNewBufWriten += dwKeyLen;

                    memcpy(pNewBuffer + dwNewBufWriten, &nValueType, 1); //value type
                    dwNewBufWriten += 1;
                    memcpy(pNewBuffer + dwNewBufWriten, &dwValueLen, sizeof(_ldword)); //value length
                    dwNewBufWriten += sizeof(_ldword);
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

                            memcpy(pNewBuffer + dwNewBufWriten, &dwKeyLen, sizeof(_ldword)); // key length
                            dwNewBufWriten += sizeof(_ldword);
                            memcpy(pNewBuffer + dwNewBufWriten, pKey, dwKeyLen); //key content
                            dwNewBufWriten += dwKeyLen;

                            memcpy(pNewBuffer + dwNewBufWriten, &nValueType, 1); //value type
                            dwNewBufWriten += 1;
                            memcpy(pNewBuffer + dwNewBufWriten, &dwValueLen, sizeof(_ldword)); //value length
                            dwNewBufWriten += sizeof(_ldword);
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
        _ldword dwWrite = fwrite(pNewBuffer, 1, dwNewBufWriten, hFile);
        CloseHandler(hFile, pBuffer);

        m_mtxLock.Unlock();

        if (dwWrite <= 0)
        {
            WARN_LOG(g_logger, "write cfg file fail. err[%d]. try raw data...", errno);
            return WRITE_FILE_FAIL;
        }

        return bRet ? 0 : CCFGHANDLER_ERROR;
    }

    int CCfgHandler::SetCfg(_lpcstr pModule, _lpcstr pKey, _lpcstr value, _lpcstr pFile)
    {
        return SetCfg(pModule, pKey, VAT_STRING, (_lpvoid)value, strlen(value), pFile);
    }

    int CCfgHandler::SetCfg(_lpcstr pModule, _lpcstr pKey, int value, _lpcstr pFile)
    {
        return SetCfg(pModule, pKey, VAT_INT, (_lpvoid)&value, sizeof(int), pFile);
    }

    int CCfgHandler::SetCfg(_lpcstr pModule, _lpcstr pKey, float value, _lpcstr pFile)
    {
        return SetCfg(pModule, pKey, VAT_FLOAT, (_lpvoid)&value, sizeof(float), pFile);
    }

    int CCfgHandler::GetHandler(_lpcstr pFile, FILE*& hFile, _lpustr* pOutBuf, _lpdword dwBufLen /*= NULL*/)
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
        _ldword dwFileSize = ftell(hFile);
        fseek(hFile, 0, SEEK_SET);

        // new buffer
        _lpustr pBuffer = (_lpustr)malloc(dwFileSize);
        memset(pBuffer, 0, dwFileSize);

        _ldword dwReadSize = fread(pBuffer, 1, dwFileSize, hFile);
        if (dwReadSize < 0)
        {
            ERROR_LOG(g_logger, "read file fail, error: %d", errno);
            return READ_FILE_FAIL;
        }
        if (dwFileSize != dwReadSize)
        {
            WARN_LOG(g_logger, "read size not equal file size. dwFileSize[%d], dwReadSize[%d]", dwFileSize, dwReadSize);
        }

        _ldword dwCryptSize = 0;
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

    bool CCfgHandler::CloseHandler(FILE* hFile, _lpustr pBuffer)
    {
        if (pBuffer != NULL)
            free(pBuffer);

        if (hFile != NULL)
            return fclose(hFile);

        return true;
    }

    int CCfgHandler::GetPointer(_lpcstr pModule, _lpcstr pKey, _lpustr pBuffer, _ldword dwBufLen, tagPointer& pt)
    {
        if (pModule == NULL || pKey == NULL /*|| pBuffer == NULL || dwBufLen < 0*/) return INVALID_PARAM;

        _lpustr pModuleStart = NULL, pModuleEnd = NULL;
        _lpustr pStart = NULL, pEnd = NULL;
        _ldword dwModuleLen = strlen(pModule);
        _ldword dwKeyLen = strlen(pKey);

        // module start
        _lpustr pCh = pBuffer;
        for (; pCh < pBuffer + dwBufLen; pCh++)
        {
            if (memcmp(pCh, &FLAG_MODULE_BEGIN, FLAG_LENGTH) == 0 &&
                    memcmp(pCh + FLAG_LENGTH, &dwModuleLen, sizeof(_ldword)) == 0 &&
                    memcmp(pCh + FLAG_LENGTH + sizeof(_ldword), pModule, dwModuleLen) == 0)
            {
                pModuleStart = pCh + FLAG_LENGTH;
                break;
            }
        }
        pt.pModuleStart = pModuleStart;
        if (pModuleStart == NULL) return MODULE_NOT_EXIST;
        pCh += FLAG_LENGTH + sizeof(_ldword) + dwModuleLen;

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
        pCh = pModuleStart + sizeof(_ldword) + dwModuleLen;
        for (; pCh < pModuleEnd; pCh++)
        {
            if (memcmp(pCh, &FLAG_ITEM_BEGIN, FLAG_LENGTH) == 0 &&
                    memcmp(pCh + FLAG_LENGTH, &dwKeyLen, sizeof(_ldword)) == 0 &&
                    memcmp(pCh + FLAG_LENGTH + sizeof(_ldword), pKey, dwKeyLen) == 0)
            {
                pStart = pCh + FLAG_LENGTH;
                break;
            }
        }
        pt.pKeyStart = pStart;
        if (pStart == NULL) return KEY_NOT_EXIST;
        pCh += FLAG_LENGTH + sizeof(_ldword) + dwKeyLen;

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
}
