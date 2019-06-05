#pragma once

namespace lslib
{
    namespace math
    {
        //////////////////////////////////////////////////////////////////////////
        // basic math functions
        LSLIB_API double Round(double v, short b); // ex.: Corner(0.1251, 2) = 0.13, Corner(0.1251, 3) = 0.125
        LSLIB_API double Floor(double v, short b); // ex.: Floor(0.1251, 2) = 0.12, Floor(0.1251, 3) = 0.125
        LSLIB_API double Ceil(double v, short b); // ex.: Ceil(0.1251, 2) = 0.13, Ceil(0.1251, 3) = 0.126
        LSLIB_API int Permut(unsigned int m, unsigned int n); // A(m, n)
        LSLIB_API int Combin(unsigned int m, unsigned int n); // C(m, n)

        //////////////////////////////////////////////////////////////////////////
        template <typename T>
        void permutation(vector<T>& v, size_t count, vector<vector<T>>& result)
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

        template <typename T>
        void combination(const vector<T>& v, size_t count, vector<vector<T>>& result)
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
