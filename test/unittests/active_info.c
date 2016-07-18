// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <inform/active_info.h>
#include <math.h>
#include <stdio.h>
#include <unit.h>

#include "random.h"

UNIT(ActiveInfoSeriesTooShort)
{
    int const series[] = {1,1,0,0,1,0,0,1};
    inform_error err;
    inform_error *errptr = &err;

    *errptr = INFORM_SUCCESS;
    ASSERT_FALSE(inform_failed(errptr));
    ASSERT_TRUE(isnan(inform_active_info(series, 1, 0, 2, 2, errptr)));
    ASSERT_TRUE(inform_failed(errptr));

    *errptr = INFORM_SUCCESS;
    ASSERT_FALSE(inform_failed(errptr));
    ASSERT_TRUE(isnan(inform_active_info(series, 1, 1, 2, 2, errptr)));
    ASSERT_TRUE(inform_failed(errptr));
}

UNIT(ActiveInfoHistoryTooLong)
{
    inform_error err;
    inform_error *errptr = &err;
    int const series[] = {1,1,0,0,1,0,0,1};

    *errptr = INFORM_SUCCESS;
    ASSERT_FALSE(inform_failed(errptr));
    ASSERT_TRUE(isnan(inform_active_info(series, 1, 2, 2, 2, errptr)));
    ASSERT_TRUE(inform_failed(errptr));

    *errptr = INFORM_SUCCESS;
    ASSERT_FALSE(inform_failed(errptr));
    ASSERT_FALSE(isnan(inform_active_info(series, 1, 3, 2, 2, errptr)));
    ASSERT_FALSE(inform_failed(errptr));
}

UNIT(ActiveInfoEncodingError)
{
    inform_error err;
    inform_error *errptr = &err;
    int const series[] = {2,1,0,0,1,0,0,1};

    *errptr = INFORM_SUCCESS;
    ASSERT_FALSE(inform_failed(errptr));
    ASSERT_FALSE(isnan(inform_active_info(series, 1, 8, 3, 2, errptr)));
    ASSERT_FALSE(inform_failed(errptr));

    *errptr = INFORM_SUCCESS;
    ASSERT_FALSE(inform_failed(errptr));
    ASSERT_TRUE(isnan(inform_active_info(series, 1, 8, 2, 2, errptr)));
    ASSERT_TRUE(inform_failed(errptr));
}

UNIT(ActiveInfoSingleSeries_Base2)
{
    ASSERT_DBL_NEAR_TOL(0.918296,
            inform_active_info((int[]){1,1,0,0,1,0,0,1}, 1, 8, 2, 2, NULL),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.000000,
            inform_active_info((int[]){1,0,0,0,0,0,0,0,0}, 1, 9, 2, 2, NULL),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.305958,
            inform_active_info((int[]){0,0,1,1,1,1,0,0,0}, 1, 9, 2, 2, NULL),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.347458,
            inform_active_info((int[]){1,0,0,0,0,0,0,1,1}, 1, 9, 2, 2, NULL),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.347458,
            inform_active_info((int[]){1,0,0,0,0,0,0,1,1}, 1, 9, 2, 2, NULL),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.399533,
            inform_active_info((int[]){0,0,0,0,0,1,1,0,0}, 1, 9, 2, 2, NULL),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.399533,
            inform_active_info((int[]){0,0,0,0,1,1,0,0,0}, 1, 9, 2, 2, NULL),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.305958,
            inform_active_info((int[]){1,1,1,0,0,0,0,1,1}, 1, 9, 2, 2, NULL),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.305958,
            inform_active_info((int[]){0,0,0,1,1,1,1,0,0}, 1, 9, 2, 2, NULL),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.347458,
            inform_active_info((int[]){0,0,0,0,0,0,1,1,0}, 1, 9, 2, 2, NULL),
            1e-6);
}

UNIT(ActiveInfoSingleSeries_Base4)
{
    ASSERT_DBL_NEAR_TOL(0.635471,
            inform_active_info((int[]){3,3,3,2,1,0,0,0,1}, 1, 9, 4, 2, NULL),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.635471,
            inform_active_info((int[]){2,2,3,3,3,3,2,1,0}, 1, 9, 4, 2, NULL),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.234783,
            inform_active_info((int[]){2,2,2,2,2,2,1,1,1}, 1, 9, 4, 2, NULL),
            1e-6);
}

UNIT(ActiveInfoEnsemble)
{
    {
        int series[16] = {
            1,1,0,0,1,0,0,1,
            0,0,0,1,0,0,0,1,
        };
        ASSERT_DBL_NEAR_TOL(0.459148,
                inform_active_info(series, 2, 8, 2, 2, NULL),
                1e-6);
    }

    {
        int series[81] = {
            1,0,0,0,0,0,0,0,0,
            0,0,1,1,1,1,0,0,0,
            1,0,0,0,0,0,0,1,1,
            1,0,0,0,0,0,0,1,1,
            0,0,0,0,0,1,1,0,0,
            0,0,0,0,1,1,0,0,0,
            1,1,1,0,0,0,0,1,1,
            0,0,0,1,1,1,1,0,0,
            0,0,0,0,0,0,1,1,0,
        };
        ASSERT_DBL_NEAR_TOL(0.3080467,
                inform_active_info(series, 9, 9, 2, 2, NULL),
                1e-6);
    }
}

UNIT(ActiveInfoEnsemble_Base4)
{
    {
        int series[36] = {
            3, 3, 3, 2, 1, 0, 0, 0, 1,
            2, 2, 3, 3, 3, 3, 2, 1, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0,
            1, 1, 0, 0, 0, 1, 1, 2, 2,
        };
        ASSERT_DBL_NEAR_TOL(0.662146,
                inform_active_info(series, 4, 9, 4, 2, NULL),
                1e-6);
    }
}

#define AVERAGE(XS) average(XS, sizeof(XS) / sizeof(double))

static double average(double *xs, size_t n)
{
    double x = 0;
    for (size_t i = 0; i < n; ++i)
    {
        x += xs[i];
    }
    return x / n;
}

UNIT(LocalActiveInfoSeriesTooShort)
{
    double ai[8];
    int const series[] = {1,1,0,0,1,0,0,1};
    inform_error err;
    inform_error *errptr = &err;

    *errptr = INFORM_SUCCESS;
    ASSERT_FALSE(inform_failed(errptr));
    ASSERT_NULL(inform_local_active_info(series, 1, 0, 2, 2, ai, errptr));
    ASSERT_TRUE(inform_failed(errptr));

    *errptr = INFORM_SUCCESS;
    ASSERT_FALSE(inform_failed(errptr));
    ASSERT_NULL(inform_local_active_info(series, 1, 1, 2, 2, ai, errptr));
    ASSERT_TRUE(inform_failed(errptr));
}

UNIT(LocalActiveInfoHistoryTooLong)
{
    double ai[6];
    int const series[] = {1,1,0,0,1,0,0,1};
    inform_error err;
    inform_error *errptr = &err;

    *errptr = INFORM_SUCCESS;
    ASSERT_FALSE(inform_failed(errptr));
    ASSERT_NULL(inform_local_active_info(series, 1, 2, 2, 2, ai, errptr));
    ASSERT_TRUE(inform_failed(errptr));

    *errptr = INFORM_SUCCESS;
    ASSERT_FALSE(inform_failed(errptr));
    ASSERT_NOT_NULL(inform_local_active_info(series, 1, 3, 2, 2, ai, errptr));
    ASSERT_FALSE(inform_failed(errptr));
}

UNIT(LocalActiveInfoEncodingError)
{
    double ai[8];
    int const series[] = {2,1,0,0,1,0,0,1};
    inform_error err;
    inform_error *errptr = &err;

    *errptr = INFORM_SUCCESS;
    ASSERT_FALSE(inform_failed(errptr));
    ASSERT_NOT_NULL(inform_local_active_info(series, 1, 8, 3, 2, ai, errptr));
    ASSERT_FALSE(inform_failed(errptr));

    *errptr = INFORM_SUCCESS;
    ASSERT_FALSE(inform_failed(errptr));
    ASSERT_NULL(inform_local_active_info(series, 1, 8, 2, 2, ai, errptr));
    ASSERT_TRUE(inform_failed(errptr));
}

UNIT(LocalActiveInfoAllocatesOutput)
{
    inform_error err = INFORM_SUCCESS;
    double *ai = inform_local_active_info((int[]){0,0,1,1,1,1,0,0,0}, 1, 9, 2, 2, NULL, &err);
    ASSERT_NOT_NULL(ai);
    ASSERT_EQUAL(INFORM_SUCCESS, err);
    free(ai);
}

UNIT(LocalActiveInfoSingleSeries_Base2)
{
    double ai[7] = {0, 0, 0, 0, 0, 0, 0};

    ASSERT_NOT_NULL(inform_local_active_info((int[]){1,0,0,0,0,0,0,0,0}, 1, 9, 2, 2, ai, NULL));
    ASSERT_DBL_NEAR_TOL(0.000000, AVERAGE(ai), 1e-6);

    ASSERT_NOT_NULL(inform_local_active_info((int[]){0,0,1,1,1,1,0,0,0}, 1, 9, 2, 2, ai, NULL));
    ASSERT_DBL_NEAR_TOL(0.305958, AVERAGE(ai), 1e-6);

    ASSERT_NOT_NULL(inform_local_active_info((int[]){1,0,0,0,0,0,0,1,1}, 1, 9, 2, 2, ai, NULL));
    ASSERT_DBL_NEAR_TOL(0.347458, AVERAGE(ai), 1e-6);

    ASSERT_NOT_NULL(inform_local_active_info((int[]){1,0,0,0,0,0,0,1,1}, 1, 9, 2, 2, ai, NULL));
    ASSERT_DBL_NEAR_TOL(0.347458, AVERAGE(ai), 1e-6);

    ASSERT_NOT_NULL(inform_local_active_info((int[]){0,0,0,0,0,1,1,0,0}, 1, 9, 2, 2, ai, NULL));
    ASSERT_DBL_NEAR_TOL(0.399533, AVERAGE(ai), 1e-6);

    ASSERT_NOT_NULL(inform_local_active_info((int[]){0,0,0,0,1,1,0,0,0}, 1, 9, 2, 2, ai, NULL));
    ASSERT_DBL_NEAR_TOL(0.399533, AVERAGE(ai), 1e-6);

    ASSERT_NOT_NULL(inform_local_active_info((int[]){1,1,1,0,0,0,0,1,1}, 1, 9, 2, 2, ai, NULL));
    ASSERT_DBL_NEAR_TOL(0.305958, AVERAGE(ai), 1e-6);

    ASSERT_NOT_NULL(inform_local_active_info((int[]){0,0,0,1,1,1,1,0,0}, 1, 9, 2, 2, ai, NULL));
    ASSERT_DBL_NEAR_TOL(0.305958, AVERAGE(ai), 1e-6);

    ASSERT_NOT_NULL(inform_local_active_info((int[]){0,0,0,0,0,0,1,1,0}, 1, 9, 2, 2, ai, NULL));
    ASSERT_DBL_NEAR_TOL(0.347458, AVERAGE(ai), 1e-6);
}

UNIT(LocalActiveInfoSingleSeries_Base4)
{
    double ai[7];
    ASSERT_NOT_NULL(inform_local_active_info((int[]){3,3,3,2,1,0,0,0,1}, 1, 9, 4, 2, ai, NULL));
    ASSERT_DBL_NEAR_TOL(0.635471, AVERAGE(ai), 1e-6);

    ASSERT_NOT_NULL(inform_local_active_info((int[]){2,2,3,3,3,3,2,1,0}, 1, 9, 4, 2, ai, NULL));
    ASSERT_DBL_NEAR_TOL(0.635471, AVERAGE(ai), 1e-6);

    ASSERT_NOT_NULL(inform_local_active_info((int[]){2,2,2,2,2,2,1,1,1}, 1, 9, 4, 2, ai, NULL));
    ASSERT_DBL_NEAR_TOL(0.234783, AVERAGE(ai), 1e-6);
}

UNIT(LocalActiveInfoEnsemble)
{
    {
        double ai[12];
        int series[16] = {
            1,1,0,0,1,0,0,1,
            0,0,0,1,0,0,0,1,
        };
        ASSERT_NOT_NULL(inform_local_active_info(series, 2, 8, 2, 2, ai, NULL));
        ASSERT_DBL_NEAR_TOL(0.459148, AVERAGE(ai), 1e-6);
    }

    {
        double ai[63];
        int series[81] = {
            1,0,0,0,0,0,0,0,0,
            0,0,1,1,1,1,0,0,0,
            1,0,0,0,0,0,0,1,1,
            1,0,0,0,0,0,0,1,1,
            0,0,0,0,0,1,1,0,0,
            0,0,0,0,1,1,0,0,0,
            1,1,1,0,0,0,0,1,1,
            0,0,0,1,1,1,1,0,0,
            0,0,0,0,0,0,1,1,0,
        };
        ASSERT_NOT_NULL(inform_local_active_info(series, 9, 9, 2, 2, ai, NULL));
        ASSERT_DBL_NEAR_TOL(0.3080467, AVERAGE(ai), 1e-6);
    }
}

UNIT(LocalActiveInfoEnsemble_Base4)
{
    {
        double ai[28];
        int series[36] = {
            3, 3, 3, 2, 1, 0, 0, 0, 1,
            2, 2, 3, 3, 3, 3, 2, 1, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0,
            1, 1, 0, 0, 0, 1, 1, 2, 2,
        };
        ASSERT_NOT_NULL(inform_local_active_info(series, 4, 9, 4, 2, ai, NULL));
        ASSERT_DBL_NEAR_TOL(0.662146, AVERAGE(ai), 1e-6);
    }
}

BEGIN_SUITE(ActiveInformation)
    ADD_UNIT(ActiveInfoSeriesTooShort)
    ADD_UNIT(ActiveInfoHistoryTooLong)
    ADD_UNIT(ActiveInfoEncodingError)
    ADD_UNIT(ActiveInfoSingleSeries_Base2)
    ADD_UNIT(ActiveInfoSingleSeries_Base4)
    ADD_UNIT(ActiveInfoEnsemble)
    ADD_UNIT(ActiveInfoEnsemble_Base4)
    ADD_UNIT(LocalActiveInfoSeriesTooShort)
    ADD_UNIT(LocalActiveInfoHistoryTooLong)
    ADD_UNIT(LocalActiveInfoEncodingError)
    ADD_UNIT(LocalActiveInfoAllocatesOutput)
    ADD_UNIT(LocalActiveInfoSingleSeries_Base2)
    ADD_UNIT(LocalActiveInfoSingleSeries_Base4)
    ADD_UNIT(LocalActiveInfoEnsemble)
    ADD_UNIT(LocalActiveInfoEnsemble_Base4)
END_SUITE
