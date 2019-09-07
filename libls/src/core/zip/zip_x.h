#pragma once

namespace lslib
{
    /// 压缩相关API
    namespace zip
    {
        /// @brief 压缩/解压回调函数
        /// @param clientp  用户参数
        /// @param dltotal  总文件个数
        /// @param dlnow    当前处理到第几个
        /// @param error    如果处理到当前文件时发生错误，将通过此参数返回错误码
        /// @return 返回0继续处理，否则将中断处理
        typedef int fn_zip_callback(void* clientp, int dltotal, int dlnow, int error);

        // under linux, miniz lib is used by default, which means password is not supported. so @pwd param is ignored.

        /// @brief 压缩单个文件
        /// @param dst_zip  目标文件路径
        /// @param src_file 待压缩的文件
        /// @param pwd      添加密码
        /// @param cb       在压缩包中的文件名
        /// @return 成功返回0，失败返回错误码
        /// @note linux下密码参数pwd将被忽略
        LSLIB_API int zip_file(_lpcstr dst_zip, _lpcstr src_file, _lpstr pwd = NULL, _lpcstr name_in_zip = NULL);

        /// @brief 压缩文件夹
        /// @param dst_zip  目标文件路径
        /// @param src_dir  待压缩的文件夹
        /// @param pwd      添加密码
        /// @param cb       回调函数
        /// @param clientp  用户参数，将透传给fn_zip_callback
        /// @return 成功返回0，失败返回错误码
        /// @note linux下密码参数pwd将被忽略
        LSLIB_API int zip_folder(_lpcstr dst_zip, _lpcstr src_dir, _lpstr pwd = NULL, fn_zip_callback cb = NULL, void* clientp = NULL);

        /// @brief 解压压缩包
        /// @param src_zip  待解压的压缩包
        /// @param dst_dir  解压到的目录路径
        /// @param pwd      解压密码
        /// @param cb       回调函数
        /// @param clientp  用户参数，将透传给fn_zip_callback
        /// @return 成功返回0，失败返回错误码
        /// @note linux下密码参数pwd将被忽略
        LSLIB_API int unzip(_lpcstr src_zip, _lpcstr dst_dir, _lpstr pwd = NULL, fn_zip_callback cb = NULL, void* clientp = NULL);
    };
}