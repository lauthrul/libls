#pragma once

#ifndef MAX_PATH
#define MAX_PATH    260
#endif

//////////////////////////////////////////////////////////////////////////
namespace lslib
{
    /// ϵͳ���
    namespace os
    {
        /// ��ȡ��ǰϵͳ·���ָ���
        LSLIB_API const lchar get_slash();

        /// �ж��ַ��Ƿ�Ϊ·���ָ���
        LSLIB_API const bool is_slash(lchar c);

        /// ��·���л�ȡĿ¼
        LSLIB_API string path_get_dir(lpcstr path);

        /// ��·���л�ȡ���һ������
        LSLIB_API string path_get_name(lpcstr path);

        /// ��·���л�ȡ�ļ����ƣ���������׺��
        LSLIB_API string path_get_filename(lpcstr path);

        /// ��·���л�ȡ�ļ���׺��������'.'��
        LSLIB_API string path_get_ext(lpcstr path);

        /// ����·��(תΪСд����ȥ������ָ���)
        LSLIB_API string path_pretty(lpcstr path);

        /// ���·��
        LSLIB_API string path_combine(lpcstr path, lpcstr join);

        /// ��ȡ����·��
        LSLIB_API string path_make_absolute(lpcstr path);


        /// ����ļ���Ŀ¼�Ƿ����
        LSLIB_API const bool is_exist(lpcstr path);

        /// ���·���Ƿ��ļ�
        LSLIB_API const bool is_file(lpcstr path);

        /// ���·���Ƿ�Ŀ¼
        LSLIB_API const bool is_dir(lpcstr path);

        /// ���·�����Ǿ���·��
        LSLIB_API const bool is_absolute(lpcstr path);

        /// �����ļ���Ŀ¼
        LSLIB_API const int copy(lpcstr path, lpcstr target);

        /// �ƶ��ļ���Ŀ¼
        LSLIB_API const int move(lpcstr path, lpcstr target);

        /// �������ļ���Ŀ¼
        LSLIB_API const int rename(lpcstr path, lpcstr target);

        /// ����Ŀ¼���ɵݹ飩
        LSLIB_API const int mkdir(lpcstr path);

        /// ɾ���ļ���Ŀ¼
        LSLIB_API const int rm(lpcstr path);

        /// ��ȡ��������·��
        LSLIB_API string get_module_file_path();

        /// ��ȡ����·��
        LSLIB_API string get_module_path();

        /// ��ȡ��������
        LSLIB_API string get_module_name();

#ifdef _MSC_VER

        /// @brief ��ȡ����·��
        /// @param csidl    �ο� https://msdn.microsoft.com/en-us/library/bb762494(VS.85).aspx
        /// @param bcreate  ���������Ƿ񴴽�
        LSLIB_API string get_special_folder_path(int csidl, bool bcreate = false);

        /// ��ȡ%appdata%·��
        LSLIB_API string get_app_data_path();

        /// ��ȡ%appdata%�µ�ǰ����·��
        LSLIB_API string get_module_app_data_path(bool bcreate /*= false*/);

        /// ��ȡ����·��
        LSLIB_API string get_desktop_path();

        /// ��ȡ����������·��
        LSLIB_API string get_quick_launch_path();

        /// ��ȡϵͳ����װ·��
        LSLIB_API string get_program_files_path();

        /// ����Դ�������д�һ��·����ѡ��һ���ļ�
        LSLIB_API void open_dir(lpcstr path, lpcstr file);

        /// @brief ���ļ�ѡ��Ի���
        /// @param[out] arr_files   ѡ����ļ��б�
        /// @param[in] title        �Ի������
        /// @param[in] filter       �ļ�����
        /// @param[in] multi        �Ƿ���Զ�ѡ
        /// @param[in] owner        ������
        /// @details 
        /// ����lpstrFilter��ʽ���£�
        ///      _T(" ����ͼƬ��ʽ\0*.jpg;*.jpeg;*.png;*.bmp\0"
        ///      " JPGͼƬ(*.jpg)\0*.jpg;*.jpeg\0"
        ///      " PNGͼƬ(*.png)\0*.png\0"
        ///      " BMPͼƬ(*.bmp)\0*.bmp\0"
        ///      " �����ļ�(*.*)\0*.*\0");
        LSLIB_API bool open_file_select_dialog(__out__ string_array& arr_files, lpcstr title, lpcstr filter, bool multi, HWND owner);

        /// @brief ��Ŀ¼ѡ��Ի���
        /// @param[out] target  ѡ���·��
        /// @param[in] title    �Ի������
        /// @param[in] owner    ������
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

        /// ��ȡϵͳ����
        LSLIB_API os_type get_os_type();

        /// ��ȡ�ļ��汾��
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

        /// @brief ������ǰĿ¼�������ļ���
        /// @param[out] array_files ����ļ��б�
        /// @param[in] path         ��Ҫ������Ŀ¼���ļ�ͨ���
        /// @param[in] extention    �ļ����ͣ������';'�ָ���
        /// @param[in] filter       �����ַ����������';'�ָ���
        /// @param[in] recurse      �Ƿ�ݹ���Ŀ¼
        /// @details
        /// �磺�ݹ�����û�ͼƬĿ¼�����а���test��picture�ؼ��ֵ�.jpg��.png�ļ���
        ///      EnumerateFiles(array_files, "C:\\Users\\ituser\\Pictures", "*.jpg;*.png", "test;picture", true);
        LSLIB_API void enumerate_files(__out__ enum_file_array& array_files, lpcstr path, lpcstr extention, lpcstr filter, bool recurse/* = false*/);

        /// ��ȡ�ļ��Ĵ�С
        LSLIB_API ldword get_file_size(lpcstr file);

        /// ��ȡ�ļ�����
        LSLIB_API file_attr get_file_attr(lpcstr file);

        /// @brief ��ȡ�ļ�����
        /// @param[in] file �ļ�·��
        /// @param[out] outsize �����ȡ���ֽ���
        /// @return �����ļ�����
        /// @post ���ؽ����Ҫ���� ::release_file_buffer �ͷ�
        LSLIB_API lpbyte get_file_buffer(lpcstr file, __out__ lpdword outsize);

        /// @brief �ͷ��ļ�����
        /// @param data ͨ�� ::get_file_buffer ������ȡ���Ļ���
        LSLIB_API bool release_file_buffer(lpbyte data);

        /// @brief д���ļ����ļ��������򴴽�
        /// @param data ��Ҫд�������
        /// @param size ���ݳ���
        /// @param file Ŀ���ļ�·��
        /// @param flag д�뷽ʽ��- 0 ���ǣ� - 1 ׷��
        /// @return ����д���С
        LSLIB_API ldword save_buffer_to_file(lpbyte data, ldword size, lpcstr file, int flag);
    } // os
} // lslib
