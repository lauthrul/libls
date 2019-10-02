#pragma once

namespace lslib
{
    /// ѹ��/��ѹ���
    namespace zip
    {
        /// @brief ѹ��/��ѹ�ص�����
        /// @param clientp  �û�����
        /// @param dltotal  ���ļ�����
        /// @param dlnow    ��ǰ�����ڼ���
        /// @param error    ���������ǰ�ļ�ʱ�������󣬽�ͨ���˲������ش�����
        /// @return ����0�������������жϴ���
        typedef int fn_zip_callback(void* clientp, int dltotal, int dlnow, int error);

        // under linux, miniz lib is used by default, which means password is not supported. so @pwd param is ignored.

        /// @brief ѹ�������ļ�
        /// @param dst_zip  Ŀ���ļ�·��
        /// @param src_file ��ѹ�����ļ�
        /// @param pwd      �������
        /// @param name_in_zip  ��ѹ�����е��ļ���
        /// @return �ɹ�����0��ʧ�ܷ��ش�����
        /// @note linux���������pwd��������
        LSLIB_API int zip_file(lpcstr dst_zip, lpcstr src_file, lpstr pwd = NULL, lpcstr name_in_zip = NULL);

        /// @brief ѹ���ļ���
        /// @param dst_zip  Ŀ���ļ�·��
        /// @param src_dir  ��ѹ�����ļ���
        /// @param pwd      �������
        /// @param cb       �ص�����
        /// @param clientp  �û���������͸����fn_zip_callback
        /// @return �ɹ�����0��ʧ�ܷ��ش�����
        /// @note linux���������pwd��������
        LSLIB_API int zip_folder(lpcstr dst_zip, lpcstr src_dir, lpstr pwd = NULL, fn_zip_callback cb = NULL, void* clientp = NULL);

        /// @brief ��ѹѹ����
        /// @param src_zip  ����ѹ��ѹ����
        /// @param dst_dir  ��ѹ����Ŀ¼·��
        /// @param pwd      ��ѹ����
        /// @param cb       �ص�����
        /// @param clientp  �û���������͸����fn_zip_callback
        /// @return �ɹ�����0��ʧ�ܷ��ش�����
        /// @note linux���������pwd��������
        LSLIB_API int unzip(lpcstr src_zip, lpcstr dst_dir, lpstr pwd = NULL, fn_zip_callback cb = NULL, void* clientp = NULL);
    };
}