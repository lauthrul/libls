#include "StdAfx.h"
#include "Math.h"
#include <math.h>



namespace lslib
{
    namespace math
    {
        //////////////////////////////////////////////////////////////////////////
#define PRECISION               0.0000001

        //////////////////////////////////////////////////////////////////////////
        double Round(double v, short b) // ex.: Corner(0.1251, 2) = 0.13, Corner(0.1251, 3) = 0.125
        {
            double precision = ((v < 0) ? -PRECISION : PRECISION);
            double result = v;
            result += (5.0 / pow(10.0, b + 1));
            result *= pow(10.0, b);
            result = floor(result + precision);
            result /= pow(10.0, b);
            return result;
        }

        double Floor(double v, short b) // ex.: Floor(0.1251, 2) = 0.12, Floor(0.1251, 3) = 0.125
        {
            double precision = ((v < 0) ? -PRECISION : PRECISION);
            double result = v;
            result *= pow(10.0, b);
            result = floor(result + precision);
            result /= pow(10.0, b);
            return result;
        }

        double Ceil(double v, short b) // ex.: Ceil(0.1251, 2) = 0.13, Ceil(0.1251, 3) = 0.126
        {
            double precision = ((v < 0) ? -PRECISION : PRECISION);
            double result = v;
            result += (9.0 / pow(10.0, b + 1));
            result *= pow(10.0, b);
            result = floor(result + precision);
            result /= pow(10.0, b);
            return result;
        }

        int Permut(unsigned int m, unsigned int n) // A(m, n) = m!/(m-n)!
        {
            if (m < n) return 0;
            int result = 1;
            for (unsigned int i = 1; i <= m; i++)
                result *= i;
            for (unsigned int i = 1; i <= m - n; i++)
                result /= i;
            return result;
        }

        int Combin(unsigned int m, unsigned int n) // C(m, n) = m!/(n!(m-n)!)
        {
            if (m < n) return 0;
            int result = 1;
            for (unsigned int i = n + 1; i <= m; ++i)
                result *= i;
            for (unsigned int i = 1; i <= m - n; ++i)
                result /= i;
            return result;
        }

    } // math

} // lslib
