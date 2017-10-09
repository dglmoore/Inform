// Copyright 2016-2017 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <inform/dist.h>
#include <inform/information_flow.h>
#include <inform/transfer_entropy.h>
#include <inform/utilities/black_boxing.h>
#include <math.h>

// static void accumulate_observations(int const *src, int const *dst,
//     int const *back, size_t l_src, size_t l_dst, size_t l_back, size_t n,
//     size_t m, int b, inform_dist *states, inform_dist *histories,
//     inform_dist *sources, inform_dist *predicates)
// {
//     for (size_t i = 0; i < n; ++i, src += m, dst += m)
//     {
//         int src_state, future, state, source, predicate, back_state;
//         int history = 0, q = 1;
//         for (size_t j = 0; j < k; ++j)
//         {
//             q *= b;
//             history *= b;
//             history += dst[j];
//         }
//         for (size_t j = k; j < m; ++j)
//         {
//             back_state = 0;
//             for (size_t u = 0; u < l; ++u)
//             {
//                 back_state = b * back_state + back[j+n*(i+m*u)-1];
//             }
//             history += back_state * q;
//
//             src_state = src[j-1];
//             future    = dst[j];
//             source    = history * b + src_state;
//             predicate = history * b + future;
//             state     = predicate * b + src_state;
//
//             states->histogram[state]++;
//             histories->histogram[history]++;
//             sources->histogram[source]++;
//             predicates->histogram[predicate]++;
//
//             history = predicate - (dst[j - k] + back_state * b) * q;
//         }
//     }
// }

static bool check_arguments(int const *src, int const *dst, int const *back,
    size_t l_src, size_t l_dst, size_t l_back, size_t n, size_t m, int b,
    inform_error *err)
{
    if (src == NULL || l_src == 0)
    {
        INFORM_ERROR_RETURN(err, INFORM_ETIMESERIES, true);
    }
    else if (dst == NULL || l_dst == 0)
    {
        INFORM_ERROR_RETURN(err, INFORM_ETIMESERIES, true);
    }
    else if (back == NULL && l_back != 0)
    {
        INFORM_ERROR_RETURN(err, INFORM_ETIMESERIES, true);
    }
    else if (n < 1)
    {
        INFORM_ERROR_RETURN(err, INFORM_ENOINITS, true);
    }
    else if (m < 1)
    {
        INFORM_ERROR_RETURN(err, INFORM_ESHORTSERIES, true);
    }
    else if (b < 2)
    {
        INFORM_ERROR_RETURN(err, INFORM_EBASE, true);
    }
    for (size_t i = 0; i < l_src * n * m; ++i)
    {
        if (b <= src[i])
        {
            INFORM_ERROR_RETURN(err, INFORM_EBADSTATE, true);
        }
        else if (src[i] < 0)
        {
            INFORM_ERROR_RETURN(err, INFORM_ENEGSTATE, true);
        }
    }
    for (size_t i = 0; i < l_dst * n * m; ++i)
    {
        if (b <= dst[i])
        {
            INFORM_ERROR_RETURN(err, INFORM_EBADSTATE, true);
        }
        else if (dst[i] < 0)
        {
            INFORM_ERROR_RETURN(err, INFORM_ENEGSTATE, true);
        }
    }
    if (back != NULL)
    {
        for (size_t i = 0; i < l_back * n * m; ++i)
        {
            if (b <= back[i])
            {
                INFORM_ERROR_RETURN(err, INFORM_EBADSTATE, true);
            }
            else if (back[i] < 0)
            {
                INFORM_ERROR_RETURN(err, INFORM_ENEGSTATE, true);
            }
        }
    }
    return false;
}

double inform_information_flow(int const *src, int const *dst, int const *back,
    size_t l_src, size_t l_dst, size_t l_back, size_t n, size_t m, int b,
    inform_error *err)
{
    if (check_arguments(src, dst, back, l_src, l_dst, l_back, n, m, b, err))
    {
        return NAN;
    }

    return 0.0;
}
