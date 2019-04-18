#pragma once

//////////////////////////////////////////////////////////////////////////
namespace lslib
{
	namespace os
	{
#ifndef MAX_PATH
#define MAX_PATH	260
#endif

		// 获取当前系统路径分隔符
		LSLIB_API const _lchar get_slash();

		// 判断字符是否为路径分隔符
		LSLIB_API const bool is_slash(_lchar c);

		// 从路径中获取目录
		LSLIB_API const lstring path_get_dir(_lpcstr path);

		// 从路径中获取最后一段名称
		LSLIB_API const lstring path_get_name(_lpcstr path);

		// 从路径中获取文件名称（不包含后缀）
		LSLIB_API const lstring path_get_filename(_lpcstr path);

		// 从路径中获取文件后缀（不包含'.'）
		LSLIB_API const lstring path_get_ext(_lpcstr path);

		// 美化路径(转为小写，并去除多余分隔符)
		LSLIB_API const lstring path_pretty(_lpcstr path);

		// 获取绝对路径
		LSLIB_API const lstring path_make_absolute(_lpcstr path);


		// 检查文件或目录是否存在
		LSLIB_API const bool is_exist(_lpcstr path);

		// 检查路径是否文件
		LSLIB_API const bool is_file(_lpcstr path);

		// 检查路径是否目录
		LSLIB_API const bool is_dir(_lpcstr path);

		// 重命名
		LSLIB_API const bool rename(_lpcstr path, _lpcstr target);

		// 创建目录（可递归）
		LSLIB_API const int mkdir(_lpcstr path);

		// 删除文件或目录
		LSLIB_API const int rm(_lpcstr path);

#ifdef _MSC_VER

		// 获取程序路径
		LSLIB_API const lstring get_module_path();

		// 获取程序名称
		LSLIB_API const lstring get_module_name();

		// 获取特殊路径，csidl参考：https://msdn.microsoft.com/en-us/library/bb762494(VS.85).aspx
		LSLIB_API const lstring get_special_folder_path(int csidl, bool bcreate = false);

		// 获取%appdata%路径
		LSLIB_API const lstring get_app_data_path();

		// 获取%appdata%下当前程序路径
		LSLIB_API const lstring get_module_app_data_path(bool bcreate /*= false*/);

		// 获取桌面路径
		LSLIB_API const lstring get_desktop_path();

		// 获取快速启动栏路径
		LSLIB_API const lstring get_quick_launch_path();

		// 获取系统程序安装路径
		LSLIB_API const lstring get_program_files_path();

		// 在资源管理器中打开一个路径并选中一个文件
		LSLIB_API void opendir(_lpcstr path, _lpcstr file);

		// 打开文件选择对话框。 [out] vctFiles: 选择的文件列表； lpstrTitle: 对话框标题； lpstrFilter: 文件类型； hwndOwner: 父窗口； bMulti: 是否可以多选
		/* 其中lpstrFilter格式如下：
				_T(" 常见图片格式\0*.jpg;*.jpeg;*.png;*.bmp\0"
				" JPG图片(*.jpg)\0*.jpg;*.jpeg\0"
				" PNG图片(*.png)\0*.png\0"
				" BMP图片(*.bmp)\0*.bmp\0"
				" 所有文件(*.*)\0*.*\0");
		*/		
		LSLIB_API bool open_file_select_dialog(__out lstring_array& arr_files, _lpcstr title, _lpcstr filter, bool multi, HWND owner);

		// 打开目录选择对话框。 [out] lpstrDest：选择的路径； lpstrTitle:对话框标题； hwndOwner: 父窗口
		LSLIB_API bool open_folder_select_dialog(__out lstring& target, _lpcstr title, HWND owner);
#endif


		struct enum_file
		{
			lstring fullPath;
			lstring filePath;
			lstring fileName;
			lstring name;
			lstring extName;
			lstring lastWriteTime;
			int     size;
		};
		stl_register_obj(enum_file);
		// 遍历当前目录下所有文件。
		// 参数：
		//		array_files [out]:	文件列表；
		//		path：				需要遍历的目录或文件通配符；
		//		extention:			文件类型（'\0'分隔多个），以"\0\0"结束；
		//		filter:				过滤字符串（'\0'分隔多个），以"\0\0"结束；
		//		recurse:			是否递归子目录（针对lpstrPath为目录的情况，lpstrPath为文件通配符时子目录满足通配才有效）；
		// 如：
		// 递归遍历用户图片目录下所有.jpg和.png文件： 
		//		EnumerateFiles(array_files, "C:\\Users\\ituser\\Pictures", "*.jpg\0*.png\0", NULL, true);
		// 遍历用户图片目录下所有.jpg文件（此时第5个参数bRecurse无效）：
		//		EnumerateFiles(array_files, "C:\\Users\\ituser\\Pictures\\*.jpg", NULL, NULL, true);
		LSLIB_API bool enumerate_files(__out enum_file_array &array_files, _lpcstr path, _lpcstr extention, _lpcstr filter, bool recurse/* = false*/);

		// 获取文件的大小
		LSLIB_API _ldword get_file_size(_lpcstr file);

		// 获取文件内容，返回结果会new出所需内存，调用后需要release_file_buffer释放，outsize保存读取的字节数
		LSLIB_API _lpbyte get_file_buffer(_lpcstr file, __out _lpdword outsize);

		// 释放get_file_buffer函数获取的内存
		LSLIB_API bool release_file_buffer(_lpbyte data);

		// 写入文件，文件不存在则创建，返回写入大小。flag: 0-覆盖 1-追加
		LSLIB_API _ldword save_buffer_to_file(_lpbyte data, _ldword size, _lpcstr file, int flag);
	} // os
} // lslib