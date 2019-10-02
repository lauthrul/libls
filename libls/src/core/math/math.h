#pragma once

namespace lslib
{
    /// 数学计算相关
    namespace math
    {
        //////////////////////////////////////////////////////////////////////////
        // basic math functions

        /// @brief 浮点数四舍五入
        /// @details 如 Corner(0.1251, 2) = 0.13, Corner(0.1251, 3) = 0.125
        /// @param v 操作数
        /// @param b 保留位数
        LSLIB_API double Round(double v, short b);

        /// @brief 浮点数向下取整
        /// @details 如 Floor(0.1251, 2) = 0.12, Floor(0.1251, 3) = 0.125
        /// @param v 操作数
        /// @param b 保留位数
        LSLIB_API double Floor(double v, short b);

        /// @brief 浮点数向上取整
        /// @details 如 Ceil(0.1251, 2) = 0.13, Ceil(0.1251, 3) = 0.126
        /// @param v 操作数
        /// @param b 保留位数
        LSLIB_API double Ceil(double v, short b);

        /// 排列数A(m, n)
        LSLIB_API int Permut(unsigned int m, unsigned int n);

        /// 组合数C(m, n)
        LSLIB_API int Combin(unsigned int m, unsigned int n);

        //////////////////////////////////////////////////////////////////////////
        /// @brief 排列
        /// @param[in] v 操作数
        /// @param[in]  count   排列数
        /// @param[out] result  保存排列结果
        template <typename T>
        void permutation(vector<T> v, size_t count, __out__ vector<vector<T> >& result)
        {
            assert(count <= v.size());
            do
            {
                vector<T> sv;
                for (size_t i = 0; i != count; ++i)
                {
                    sv.push_back(v[i]);
                }
                result.push_back(sv);
                reverse(v.begin() + count, v.end());
            }
            while (next_permutation(v.begin(), v.end()));
        }

        /// @brief 组合
        /// @param[in] v 操作数
        /// @param[in]  count   组合数
        /// @param[out] result  保存组合结果
        template <typename T>
        void combination(vector<T> v, size_t count, __out__ vector<vector<T> >& result)
        {
            assert(count <= v.size());
            vector<bool> bitset(v.size() - count, 0);
            bitset.resize(v.size(), 1);

            do
            {
                vector<T> sv;
                for (size_t i = 0; i != v.size(); ++i)
                {
                    if (bitset[i]) sv.push_back(v[i]);
                }
                result.push_back(sv);
            }
            while (next_permutation(bitset.begin(), bitset.end()));
        }

    } // math

} // lslib
