#pragma once

//////////////////////////////////////////////////////////////////////////
namespace lslib
{
    namespace os
    {
#ifndef MAX_PATH
#define MAX_PATH    260
#endif

        // ��ȡ��ǰϵͳ·���ָ���
        LSLIB_API const _lchar get_slash();

        // �ж��ַ��Ƿ�Ϊ·���ָ���
        LSLIB_API const bool is_slash(_lchar c);

        // ��·���л�ȡĿ¼
        LSLIB_API const lstring path_get_dir(_lpcstr path);

        // ��·���л�ȡ���һ������
        LSLIB_API const lstring path_get_name(_lpcstr path);

        // ��·���л�ȡ�ļ����ƣ���������׺��
        LSLIB_API const lstring path_get_filename(_lpcstr path);

        // ��·���л�ȡ�ļ���׺��������'.'��
        LSLIB_API const lstring path_get_ext(_lpcstr path);

        // ����·��(תΪСд����ȥ������ָ���)
        LSLIB_API const lstring path_pretty(_lpcstr path);

        // ���·��
        LSLIB_API const lstring path_combine(_lpcstr path, _lpcstr join);

        // ��ȡ����·��
        LSLIB_API const lstring path_make_absolute(_lpcstr path);


        // ����ļ���Ŀ¼�Ƿ����
        LSLIB_API const bool is_exist(_lpcstr path);

        // ���·���Ƿ��ļ�
        LSLIB_API const bool is_file(_lpcstr path);

        // ���·���Ƿ�Ŀ¼
        LSLIB_API const bool is_dir(_lpcstr path);

        // �����ļ���Ŀ¼
        LSLIB_API const int copy(_lpcstr path, _lpcstr target);

        // �ƶ��ļ���Ŀ¼
        LSLIB_API const int move(_lpcstr path, _lpcstr target);

        // �������ļ���Ŀ¼
        LSLIB_API const bool rename(_lpcstr path, _lpcstr target);

        // ����Ŀ¼���ɵݹ飩
        LSLIB_API const int mkdir(_lpcstr path);

        // ɾ���ļ���Ŀ¼
        LSLIB_API const int rm(_lpcstr path);

        // ��ȡ��������·��
        LSLIB_API const lstring get_module_file_path();

        // ��ȡ����·��
        LSLIB_API const lstring get_module_path();

        // ��ȡ��������
        LSLIB_API const lstring get_module_name();

#ifdef _MSC_VER

        // ��ȡ����·����csidl�ο���https://msdn.microsoft.com/en-us/library/bb762494(VS.85).aspx
        LSLIB_API const lstring get_special_folder_path(int csidl, bool bcreate = false);

        // ��ȡ%appdata%·��
        LSLIB_API const lstring get_app_data_path();

        // ��ȡ%appdata%�µ�ǰ����·��
        LSLIB_API const lstring get_module_app_data_path(bool bcreate /*= false*/);

        // ��ȡ����·��
        LSLIB_API const lstring get_desktop_path();

        // ��ȡ����������·��
        LSLIB_API const lstring get_quick_launch_path();

        // ��ȡϵͳ����װ·��
        LSLIB_API const lstring get_program_files_path();

        // ����Դ�������д�һ��·����ѡ��һ���ļ�
        LSLIB_API void open_dir(_lpcstr path, _lpcstr file);

        // ���ļ�ѡ��Ի��� [out] arr_files: ѡ����ļ��б� title: �Ի�����⣻ filter: �ļ����ͣ� multi: �Ƿ���Զ�ѡ�� owner: ������
        /* ����lpstrFilter��ʽ���£�
                _T(" ����ͼƬ��ʽ\0*.jpg;*.jpeg;*.png;*.bmp\0"
                " JPGͼƬ(*.jpg)\0*.jpg;*.jpeg\0"
                " PNGͼƬ(*.png)\0*.png\0"
                " BMPͼƬ(*.bmp)\0*.bmp\0"
                " �����ļ�(*.*)\0*.*\0");
        */
        LSLIB_API bool open_file_select_dialog(__out__ lstring_array& arr_files, _lpcstr title, _lpcstr filter, bool multi, HWND owner);

        // ��Ŀ¼ѡ��Ի��� [out] target��ѡ���·���� title:�Ի�����⣻ owner: ������
        LSLIB_API bool open_folder_select_dialog(__out__ lstring& target, _lpcstr title, HWND owner);

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

        // ��ȡϵͳ����
        LSLIB_API os_type get_os_type();

        // ��ȡ�ļ��汾��
        LSLIB_API lstring get_product_version(_lpcstr path);

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
        declare_stl_obj(enum_file);
        // ������ǰĿ¼�������ļ���
        // ������
        //      array_files [out]:  �ļ��б�
        //      path��              ��Ҫ������Ŀ¼���ļ�ͨ�����
        //      extention:          �ļ����ͣ�'\0'�ָ����������"\0\0"������
        //      filter:             �����ַ�����'\0'�ָ����������"\0\0"������
        //      recurse:            �Ƿ�ݹ���Ŀ¼�����lpstrPathΪĿ¼�������lpstrPathΪ�ļ�ͨ���ʱ��Ŀ¼����ͨ�����Ч����
        // �磺
        // �ݹ�����û�ͼƬĿ¼������.jpg��.png�ļ���
        //      EnumerateFiles(array_files, "C:\\Users\\ituser\\Pictures", "*.jpg\0*.png\0", NULL, true);
        // �����û�ͼƬĿ¼������.jpg�ļ�����ʱ��5������bRecurse��Ч����
        //      EnumerateFiles(array_files, "C:\\Users\\ituser\\Pictures\\*.jpg", NULL, NULL, true);
        LSLIB_API bool enumerate_files(__out__ enum_file_array& array_files, _lpcstr path, _lpcstr extention, _lpcstr filter, bool recurse/* = false*/);

        // ��ȡ�ļ��Ĵ�С
        LSLIB_API _ldword get_file_size(_lpcstr file);

        // ��ȡ�ļ����ݣ����ؽ����new�������ڴ棬���ú���Ҫrelease_file_buffer�ͷţ�outsize�����ȡ���ֽ���
        LSLIB_API _lpbyte get_file_buffer(_lpcstr file, __out__ _lpdword outsize);

        // �ͷ�get_file_buffer������ȡ���ڴ�
        LSLIB_API bool release_file_buffer(_lpbyte data);

        // д���ļ����ļ��������򴴽�������д���С��flag: 0-���� 1-׷��
        LSLIB_API _ldword save_buffer_to_file(_lpbyte data, _ldword size, _lpcstr file, int flag);
    } // os
} // lslib
