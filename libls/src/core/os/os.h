#pragma once

#ifndef MAX_PATH
#define MAX_PATH    260
#endif

//////////////////////////////////////////////////////////////////////////
namespace lslib
{
    /// 系统相关
    namespace os
    {
        /// 获取当前系统路径分隔符
        LSLIB_API const lchar get_slash();

        /// 判断字符是否为路径分隔符
        LSLIB_API const bool is_slash(lchar c);

        /// 从路径中获取目录
        LSLIB_API string path_get_dir(lpcstr path);

        /// 从路径中获取最后一段名称
        LSLIB_API string path_get_name(lpcstr path);

        /// 从路径中获取文件名称（不包含后缀）
        LSLIB_API string path_get_filename(lpcstr path);

        /// 从路径中获取文件后缀（不包含'.'）
        LSLIB_API string path_get_ext(lpcstr path);

        /// 美化路径(转为小写，并去除多余分隔符)
        LSLIB_API string path_pretty(lpcstr path);

        /// 组合路径
        LSLIB_API string path_combine(lpcstr path, lpcstr join);

        /// 获取绝对路径
        LSLIB_API string path_make_absolute(lpcstr path);


        /// 检查文件或目录是否存在
        LSLIB_API const bool is_exist(lpcstr path);

        /// 检查路径是否文件
        LSLIB_API const bool is_file(lpcstr path);

        /// 检查路径是否目录
        LSLIB_API const bool is_dir(lpcstr path);

        /// 相对路径还是绝对路径
        LSLIB_API const bool is_absolute(lpcstr path);

        /// 复制文件或目录
        LSLIB_API const int copy(lpcstr path, lpcstr target);

        /// 移动文件或目录
        LSLIB_API const int move(lpcstr path, lpcstr target);

        /// 重命名文件或目录
        LSLIB_API const int rename(lpcstr path, lpcstr target);

        /// 创建目录（可递归）
        LSLIB_API const int mkdir(lpcstr path);

        /// 删除文件或目录
        LSLIB_API const int rm(lpcstr path);

        /// 获取程序完整路径
        LSLIB_API string get_module_file_path();

        /// 获取程序路径
        LSLIB_API string get_module_path();

        /// 获取程序名称
        LSLIB_API string get_module_name();

#ifdef _MSC_VER

        /// @brief 获取特殊路径
        /// @param csidl    参考 https://msdn.microsoft.com/en-us/library/bb762494(VS.85).aspx
        /// @param bcreate  若不存在是否创建
        LSLIB_API string get_special_folder_path(int csidl, bool bcreate = false);

        /// 获取%appdata%路径
        LSLIB_API string get_app_data_path();

        /// 获取%appdata%下当前程序路径
        LSLIB_API string get_module_app_data_path(bool bcreate /*= false*/);

        /// 获取桌面路径
        LSLIB_API string get_desktop_path();

        /// 获取快速启动栏路径
        LSLIB_API string get_quick_launch_path();

        /// 获取系统程序安装路径
        LSLIB_API string get_program_files_path();

        /// 在资源管理器中打开一个路径并选中一个文件
        LSLIB_API void open_dir(lpcstr path, lpcstr file);

        /// @brief 打开文件选择对话框
        /// @param[out] arr_files   选择的文件列表
        /// @param[in] title        对话框标题
        /// @param[in] filter       文件类型
        /// @param[in] multi        是否可以多选
        /// @param[in] owner        父窗口
        /// @details 
        /// 其中lpstrFilter格式如下：
        ///      _T(" 常见图片格式\0*.jpg;*.jpeg;*.png;*.bmp\0"
        ///      " JPG图片(*.jpg)\0*.jpg;*.jpeg\0"
        ///      " PNG图片(*.png)\0*.png\0"
        ///      " BMP图片(*.bmp)\0*.bmp\0"
        ///      " 所有文件(*.*)\0*.*\0");
        LSLIB_API bool open_file_select_dialog(__out__ string_array& arr_files, lpcstr title, lpcstr filter, bool multi, HWND owner);

        /// @brief 打开目录选择对话框
        /// @param[out] target  选择的路径
        /// @param[in] title    对话框标题
        /// @param[in] owner    父窗口
        LSLIB_API bool open_folder_select_dialog(__out__ string& target, lpcstr title, HWND owner);

        enum os_type
        {
            os_unknown,
            os_win_nt_4_0,
            os_win_95,
            os_win_98,
            os_win_me,
            os_win_2000,
            os_win_xp,
            os_win_xp_64,
            os_win_server_2003,
            os_win_home_server,
            os_win_server_2003_r2,
            os_win_vista,
            os_win_server_2008,
            os_win_server_2008_r2,
            os_win_7,
            os_win_server_2012,
            os_win_8,
            os_win_server_2012_r2,
            os_win_8_1,
            os_win_server_2016,
            os_win_10,
        };
        declare_enum_str(os_type, LSLIB_API);

        /// 获取系统类型
        LSLIB_API os_type get_os_type();

        /// 获取文件版本号
        LSLIB_API string get_product_version(lpcstr path);

#endif

        struct file_attr
        {
            string createTime;
            string writeTime;
            string accessTime;
        };

        struct enum_file
        {
            string fullPath;
            string filePath;
            string fileName;
            string name;
            string extName;
            file_attr attr;
            int     size;
        };
        quick_define_stl(enum_file);

        /// @brief 遍历当前目录下所有文件。
        /// @param[out] array_files 输出文件列表
        /// @param[in] path         需要遍历的目录或文件通配符
        /// @param[in] extention    文件类型（多个以';'分隔）
        /// @param[in] filter       过滤字符串（多个以';'分隔）
        /// @param[in] recurse      是否递归子目录
        /// @details
        /// 如：递归遍历用户图片目录下所有包含test和picture关键字的.jpg和.png文件：
        ///      EnumerateFiles(array_files, "C:\\Users\\ituser\\Pictures", "*.jpg;*.png", "test;picture", true);
        LSLIB_API void enumerate_files(__out__ enum_file_array& array_files, lpcstr path, lpcstr extention, lpcstr filter, bool recurse/* = false*/);

        /// 获取文件的大小
        LSLIB_API ldword get_file_size(lpcstr file);

        /// 获取文件属性
        LSLIB_API file_attr get_file_attr(lpcstr file);

        /// @brief 获取文件内容
        /// @param[in] file 文件路径
        /// @param[out] outsize 保存读取的字节数
        /// @return 返回文件内容
        /// @post 返回结果需要调用 ::release_file_buffer 释放
        LSLIB_API lpbyte get_file_buffer(lpcstr file, __out__ lpdword outsize);

        /// @brief 释放文件内容
        /// @param data 通过 ::get_file_buffer 函数获取到的缓存
        LSLIB_API bool release_file_buffer(lpbyte data);

        /// @brief 写入文件，文件不存在则创建
        /// @param data 需要写入的数据
        /// @param size 数据长度
        /// @param file 目标文件路径
        /// @param flag 写入方式，- 0 覆盖， - 1 追加
        /// @return 返回写入大小
        LSLIB_API ldword save_buffer_to_file(lpbyte data, ldword size, lpcstr file, int flag);
    } // os
} // lslib
