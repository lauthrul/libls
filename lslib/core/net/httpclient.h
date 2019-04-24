#pragma once
#include "curl/curl.h"

namespace lslib
{
	namespace net
	{
		//////////////////////////////////////////////////////////////////////////
#define TRYCOUNT_HTTPGET       			3
#define TRYCOUNT_HTTPPOST      			3
#define TRYCOUNT_DOWNLOAD      			3
#define TRYCOUNT_UPLOAD        			3

#define TIMEOUT_CONNECT       			5		//sec
#define TIMEOUT_HTTPGET       			15		//sec
#define TIMEOUT_HTTPPOST      			60		//sec
#define TIMEOUT_HTTPDOWNLOAD      		60		//sec
#define TIMEOUT_HTTPUPLOAD     			60		//sec

#define GZIP_THRESHOLD_SIZE				(100*1024)

		//////////////////////////////////////////////////////////////////////////
		enum EHttpMethod
		{
			HTTP_GET,
			HTTP_POST,
			HTTP_DOWNLOAD,
			HTTP_UPLOAD,
		};
		inline lstring GetHttpMethodStr(EHttpMethod eMethod)
		{
			switch (eMethod)
			{
			case HTTP_GET: return "HTTP_GET";				break;
			case HTTP_POST: return "HTTP_POST";				break;
			case HTTP_DOWNLOAD: return "HTTP_DOWNLOAD";		break;
			case HTTP_UPLOAD: return "HTTP_UPLOAD";			break;
			}
			return "";
		}

		//////////////////////////////////////////////////////////////////////////
		struct SHttpParam
		{
			EHttpMethod				eMethod;
			lstring					strUrl;
			map<lstring, lstring>	mapHeaders;
			int						nConnetTimeout;
			int						nPerformTimeout;
			int						nTryCount;

			SHttpParam()
			{
				Reset();
			}
			SHttpParam(_lpcstr lpstrUrl)
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
			}
		};

		struct SHttpGetParam : public SHttpParam
		{
			SHttpGetParam()
			{
				Reset();
			}
			SHttpGetParam(_lpcstr lpstrUrl)
			{
				Reset();
				strUrl = lpstrUrl;
			}
			void Reset()
			{
				__super::Reset();
				eMethod = HTTP_GET;
				nPerformTimeout = TIMEOUT_HTTPGET;
				nTryCount = TRYCOUNT_HTTPGET;
			}
		};

		struct SHttpPostParam : public SHttpParam
		{
			lstring					strPost;
			bool					bgZip;
			int						nThresholdSize;		// above this value, data will be gzip

			SHttpPostParam()
			{
				Reset();
			}
			SHttpPostParam(_lpcstr lpstrUrl, _lpcstr lpstrPost)
			{
				Reset();
				strUrl = lpstrUrl;
				strPost = lpstrPost;
			}
			void Reset()
			{
				__super::Reset();
				eMethod = HTTP_POST;
				nPerformTimeout = TIMEOUT_HTTPPOST;
				nTryCount = TRYCOUNT_HTTPPOST;
				strPost.clear();
				bgZip = false;
				nThresholdSize = GZIP_THRESHOLD_SIZE;
			}
		};

		typedef int (*DownloadFileCallback)(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow);

		struct SHttpDowloadParam : public SHttpParam
		{
			lstring					strFile;
			lstring					strToken;
			bool					bBreakPointSupport;
			DownloadFileCallback	cb;
			void*					clientp;

			SHttpDowloadParam()
			{
				Reset();
			}
			SHttpDowloadParam(_lpcstr lpstrUrl, _lpcstr lpstrFile)
			{
				Reset();
				strUrl = lpstrUrl;
				strFile = lpstrFile;
			}
			void Reset()
			{
				__super::Reset();
				eMethod = HTTP_DOWNLOAD;
				nPerformTimeout = TIMEOUT_HTTPDOWNLOAD;
				nTryCount = TRYCOUNT_DOWNLOAD;
				strFile.clear();
				strToken.clear();
				bBreakPointSupport = false;
				cb = NULL;
				clientp = NULL;
			}
		};

		struct SHttpUploadParam : public SHttpParam
		{
			lstring					strFile;
			lstring					strRemoteFile;

			SHttpUploadParam()
			{
				Reset();
			}
			SHttpUploadParam(_lpcstr lpstrUrl, _lpcstr lpstrFile)
			{
				Reset();
				strUrl = lpstrUrl;
				strFile = lpstrFile;
			}
			void Reset()
			{
				__super::Reset();
				eMethod = HTTP_UPLOAD;
				nPerformTimeout = TIMEOUT_HTTPUPLOAD;
				nTryCount = TRYCOUNT_UPLOAD;
				strFile.clear();
				strRemoteFile.clear();
			}
		};

		struct SHttpResult
		{
			int						nCode;
			map<lstring,lstring>	mapHeaders;
			lstring					strData;
			int						nDataLen;
			int						nTimeSpend; // second

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

		struct SHttpUrl
		{
			lstring strScheme;
			lstring strHostName;
			int nPort;
			lstring strPath;
			map<lstring, lstring> mapQuerys;
			lstring strTag;
		};
		LSLIB_API SHttpUrl			CrackUrl(_lpcstr lpstrUrl);
		LSLIB_API lstring			ReverseUrl(const SHttpUrl& vUrl);

		//////////////////////////////////////////////////////////////////////////
		class LSLIB_API CHttpClient
		{
		public:
			static void				Init(_lpcstr lpstrDefaultCookieFile = NULL, _lpcstr lpstrDefaultAgent = NULL);
			static bool				IsInit();
			static void				SetDefaultCookieFile(_lpcstr lpstrDefaultCookieFile);
			static void				SetDefaultAgent(_lpcstr lpstrDefaultAgent);

			static SHttpResult		HttpGet(const SHttpGetParam& vParam);
			static SHttpResult		HttpPost(const SHttpPostParam& vParam);
			static SHttpResult		DownloadFile(const SHttpDowloadParam& vParam);
			static SHttpResult		UploadFile(const SHttpUploadParam& vParam);

			// for debug print
		protected:
			static int				Perform(CURL* pCurl, const SHttpParam& vParam, __inout SHttpResult& vResult);
			static lstring			DumpParamText(SHttpParam* pParam);
			static lstring			DumpResultText(SHttpResult* pResult);

		private:
			static lstring			m_strDefaultAgent;
			static lstring			m_strDefaultCookieFile;
		};

	} // namespace net

} // namespace lslib
