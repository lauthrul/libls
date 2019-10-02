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
    /// 网络相关
    namespace net
    {
        //////////////////////////////////////////////////////////////////////////
        /// HTTP请求类型
        enum EHttpMethod
        {
            HTTP_GET,       ///< GET请求
            HTTP_POST,      ///< POST请求
            HTTP_DOWNLOAD,  ///< 下载请求
            HTTP_UPLOAD,    ///< 上传请求
        };
        declare_enum_str(EHttpMethod, LSLIB_API);

        /// @brief HTTP回调函数
        /// @param clientp  用户参数
        /// @param dltotal  总下载内容大小
        /// @param dlnow    当前已下载内容大小
        /// @param ultotal  总上传内容大小
        /// @param ulnow    当前已上传内容大小
        typedef int(*HttpCallback)(void* clientp, double dltotal, double dlnow, double ultotal, double ulnow);

        //////////////////////////////////////////////////////////////////////////
        /// @brief HTTP请求参数基类
        struct SHttpParam
        {
            EHttpMethod             eMethod;            ///< 请求类型， 参考@ref EHttpMethod
            string                  strUrl;             ///< 请求URL
            map<string, string>     mapHeaders;         ///< 请求头
            int                     nConnetTimeout;     ///< 连接超时时间，默认3s
            int                     nPerformTimeout;    ///< 总执行超时时间（包括连接时间），小于0由程序决定
            int                     nTryCount;          ///< 失败重试次数，小于0由程序决定
            HttpCallback            cb;                 ///< 回调函数，参考@ref HttpCallback
            void*                   clientp;            ///< 回调函数用户参数

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

        /// @brief HTTP GET请求参数
        /// @note 默认执行超时时间为5s，重试次数为2次
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

        /// @brief HTTP POST请求参数
        /// @note 默认执行超时时间为15s，重试次数为2次
        struct SHttpPostParam : public SHttpParam
        {
            string                  strPost;        ///< POST内容
            bool                    bgZip;          ///< 是否进行压缩，默认不压缩
            int                     nThresholdSize; ///< 压缩大小阀值，POST内容长度超过该值时，将进行压缩，默认为1KB

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

        /// @brief 下载请求参数
        /// @note 默认执行超时时间为30s，重试次数为2次
        struct SHttpDowloadParam : public SHttpParam
        {
            string                  strFile;            ///< 下载保存文件路径
            string                  strToken;           ///< token值，未使用
            bool                    bBreakPointSupport; ///< 是否支持断点续传，默认为否

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

        /// @brief 上传请求参数
        /// @note 默认执行超时时间为15s，重试次数为2次
        struct SHttpUploadParam : public SHttpParam
        {
            string                 strFile;         ///< 本地文件路径
            string                 strRemoteFile;   ///< 远程文件名

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

        /// HTTP响应结果
        struct SHttpResult
        {
            int                     nCode;      ///< 错误码，大于等于100表示HTTP响应码，否则表示执行出错，错误码参考libcurl
            map<string, string>     mapHeaders; ///< 响应头
            string                  strData;    ///< 响应数据
            int                     nDataLen;   ///< 响应数据字节长度
            int                     nTimeSpend; ///< 执行耗时，毫秒

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

        /// HTTP URL结构体
        struct SHttpUrl
        {
            string strScheme;   ///< http或https
            string strHostName; ///< 主域名
            int nPort;          ///< 端口
            string strPath;     ///< 请求路径
            map<string, string> mapQuerys;  ///< 请求参数(?)
            string strTag;      ///< 请求标签(#)
        };

        /// 解析URL结构
        LSLIB_API SHttpUrl          CrackUrl(lpcstr lpstrUrl);

        /// 还原URL结构为字符串
        LSLIB_API string            ReverseUrl(const SHttpUrl& vUrl);

        //////////////////////////////////////////////////////////////////////////
        /// HTTP客户端
        class LSLIB_API CHttpClient
        {
        public:
            /// @brief 初始化HTTP客户端
            static void             Init(lpcstr lpstrDefaultCookieFile = NULL, lpcstr lpstrDefaultAgent = NULL);

            /// @brief 检查HTTP客户端是否已初始化
            static bool             IsInit();

            /// @brief 设置默认cookie保存文件
            static void             SetDefaultCookieFile(lpcstr lpstrDefaultCookieFile);

            /// @brief 设置默认UA
            static void             SetDefaultAgent(lpcstr lpstrDefaultAgent);

            /// @brief 执行HTTP GET请求
            /// @param vParam 参考@ref SHttpGetParam
            /// @return 返回执行结果 参考@ref SHttpResult
            static SHttpResult      HttpGet(const SHttpGetParam& vParam);

            /// @brief 执行HTTP POS请求
            /// @param vParam 参考@ref SHttpPostParam
            /// @return 返回执行结果 参考@ref SHttpResult
            static SHttpResult      HttpPost(const SHttpPostParam& vParam);

            /// @brief 执行HTTP 下载请求
            /// @param vParam 参考@ref SHttpDowloadParam
            /// @return 返回执行结果 参考@ref SHttpResult
            static SHttpResult      DownloadFile(const SHttpDowloadParam& vParam);

            /// @brief 执行HTTP 上传请求
            /// @param vParam 参考@ref SHttpUploadParam
            /// @return 返回执行结果 参考@ref SHttpResult
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

        /// 多线程调用CHttpClient API时，为保证线程安全，需要在全局初始化调用
        extern LSLIB_API int httpclient_thread_setup(void);

        /// 多线程调用CHttpClient API时，为保证线程安全，需要在全局初始化调用
        extern LSLIB_API int httpclient_thread_cleanup(void);

    } // namespace net

} // namespace lslib

#endif // endof USE_LIBCURL
