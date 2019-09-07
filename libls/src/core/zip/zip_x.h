#pragma once

namespace lslib
{
    /// ѹ�����API
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
        /// @param cb       ��ѹ�����е��ļ���
        /// @return �ɹ�����0��ʧ�ܷ��ش�����
        /// @note linux���������pwd��������
        LSLIB_API int zip_file(_lpcstr dst_zip, _lpcstr src_file, _lpstr pwd = NULL, _lpcstr name_in_zip = NULL);

        /// @brief ѹ���ļ���
        /// @param dst_zip  Ŀ���ļ�·��
        /// @param src_dir  ��ѹ�����ļ���
        /// @param pwd      �������
        /// @param cb       �ص�����
        /// @param clientp  �û���������͸����fn_zip_callback
        /// @return �ɹ�����0��ʧ�ܷ��ش�����
        /// @note linux���������pwd��������
        LSLIB_API int zip_folder(_lpcstr dst_zip, _lpcstr src_dir, _lpstr pwd = NULL, fn_zip_callback cb = NULL, void* clientp = NULL);

        /// @brief ��ѹѹ����
        /// @param src_zip  ����ѹ��ѹ����
        /// @param dst_dir  ��ѹ����Ŀ¼·��
        /// @param pwd      ��ѹ����
        /// @param cb       �ص�����
        /// @param clientp  �û���������͸����fn_zip_callback
        /// @return �ɹ�����0��ʧ�ܷ��ش�����
        /// @note linux���������pwd��������
        LSLIB_API int unzip(_lpcstr src_zip, _lpcstr dst_dir, _lpstr pwd = NULL, fn_zip_callback cb = NULL, void* clientp = NULL);
    };
}