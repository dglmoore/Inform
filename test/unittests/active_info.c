// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <unit.h>
#include <inform/state_encoding.h>
#include <inform/active_info.h>
#include "random.h"

#include <stdio.h>

UNIT(ActiveInfoSeriesTooShort)
{
    uint64_t const series[] = {1,1,0,0,1,0,0,1};
    ASSERT_TRUE(isnan(inform_active_info(series, 1, 0, 2, 2)));
    ASSERT_TRUE(isnan(inform_active_info(series, 1, 1, 2, 2)));
}

UNIT(ActiveInfoHistoryTooLong)
{
    {
        uint64_t const series[] = {1,1,0,0,1,0,0,1};

        ASSERT_TRUE(isnan(inform_active_info(series, 1, 2, 2, 2)));
        ASSERT_FALSE(isnan(inform_active_info(series, 1, 3, 2, 2)));
    }

    {
        size_t const size = 30;

        uint64_t *series = random_series(size, 2);
        ASSERT_TRUE(isnan(inform_active_info(series, 1, size, 2, 26)));
        free(series);

        series = random_series(size, 3);
        ASSERT_TRUE(isnan(inform_active_info(series, 1, size, 3, 16)));
        free(series);

        series = random_series(size, 4);
        ASSERT_TRUE(isnan(inform_active_info(series, 1, size, 4, 13)));
        free(series);
    }
}

UNIT(ActiveInfoEncodingError)
{
    uint64_t const series[] = {2,1,0,0,1,0,0,1};
    ASSERT_FALSE(isnan(inform_active_info(series, 1, 8, 3, 2)));
    ASSERT_TRUE(isnan(inform_active_info(series, 1, 8, 2, 2)));
}

UNIT(ActiveInfoSingleSeries_Base2)
{
    ASSERT_DBL_NEAR_TOL(0.918296,
            inform_active_info((uint64_t[]){1,1,0,0,1,0,0,1}, 1, 8, 2, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.000000,
            inform_active_info((uint64_t[]){1,0,0,0,0,0,0,0,0}, 1, 9, 2, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.305958,
            inform_active_info((uint64_t[]){0,0,1,1,1,1,0,0,0}, 1, 9, 2, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.347458,
            inform_active_info((uint64_t[]){1,0,0,0,0,0,0,1,1}, 1, 9, 2, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.347458,
            inform_active_info((uint64_t[]){1,0,0,0,0,0,0,1,1}, 1, 9, 2, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.399533,
            inform_active_info((uint64_t[]){0,0,0,0,0,1,1,0,0}, 1, 9, 2, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.399533,
            inform_active_info((uint64_t[]){0,0,0,0,1,1,0,0,0}, 1, 9, 2, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.305958,
            inform_active_info((uint64_t[]){1,1,1,0,0,0,0,1,1}, 1, 9, 2, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.305958,
            inform_active_info((uint64_t[]){0,0,0,1,1,1,1,0,0}, 1, 9, 2, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.347458,
            inform_active_info((uint64_t[]){0,0,0,0,0,0,1,1,0}, 1, 9, 2, 2),
            1e-6);
}

UNIT(ActiveInfoSingleSeries_Base4)
{
    ASSERT_DBL_NEAR_TOL(0.635471,
            inform_active_info((uint64_t[]){3,3,3,2,1,0,0,0,1}, 1, 9, 4, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.635471,
            inform_active_info((uint64_t[]){2,2,3,3,3,3,2,1,0}, 1, 9, 4, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.234783,
            inform_active_info((uint64_t[]){2,2,2,2,2,2,1,1,1}, 1, 9, 4, 2),
            1e-6);
}

UNIT(ActiveInfoEnsemble)
{
    {
        uint64_t series[16] = {
            1,1,0,0,1,0,0,1,
            0,0,0,1,0,0,0,1,
        };
        ASSERT_DBL_NEAR_TOL(0.459148,
                inform_active_info(series, 2, 8, 2, 2),
                1e-6);
    }

    {
        uint64_t series[81] = {
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
                inform_active_info(series, 9, 9, 2, 2),
                1e-6);
    }
}

UNIT(ActiveInfoEnsemble_Base4)
{
    {
        uint64_t series[36] = {
            3, 3, 3, 2, 1, 0, 0, 0, 1,
            2, 2, 3, 3, 3, 3, 2, 1, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0,
            1, 1, 0, 0, 0, 1, 1, 2, 2,
        };
        ASSERT_DBL_NEAR_TOL(0.662146,
                inform_active_info(series, 4, 9, 4, 2),
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
    uint64_t const series[] = {1,1,0,0,1,0,0,1};
    ASSERT_EQUAL(3, inform_local_active_info(series, 1, 0, 2, 2, ai));
    ASSERT_EQUAL(3, inform_local_active_info(series, 1, 1, 2, 2, ai));
}

UNIT(LocalActiveInfoHistoryTooLong)
{
    {
        double ai[6];
        uint64_t const series[] = {1,1,0,0,1,0,0,1};

        ASSERT_EQUAL(4, inform_local_active_info(series, 1, 2, 2, 2, ai));
        ASSERT_EQUAL(0, inform_local_active_info(series, 1, 3, 2, 2, ai));
    }

    {
        double ai[30];
        size_t const size = 30;

        uint64_t *series = random_series(size, 2);
        ASSERT_EQUAL(5, inform_local_active_info(series, 1, size, 2, 26, ai));
        free(series);

        series = random_series(size, 3);
        ASSERT_EQUAL(5, inform_local_active_info(series, 1, size, 3, 16, ai));
        free(series);

        series = random_series(size, 4);
        ASSERT_EQUAL(5, inform_local_active_info(series, 1, size, 4, 13, ai));
        free(series);
    }
}

UNIT(LocalActiveInfoEncodingError)
{
    double ai[8];
    uint64_t const series[] = {2,1,0,0,1,0,0,1};
    ASSERT_EQUAL(0, inform_local_active_info(series, 1, 8, 3, 2, ai));
    ASSERT_EQUAL(6, inform_local_active_info(series, 1, 8, 2, 2, ai));
}

UNIT(LocalActiveInfoSingleSeries_Base2)
{
    double ai[7] = {0, 0, 0, 0, 0, 0, 0};

    ASSERT_EQUAL(0, inform_local_active_info((uint64_t[]){1,0,0,0,0,0,0,0,0}, 1, 9, 2, 2, ai));
    ASSERT_DBL_NEAR_TOL(0.000000, AVERAGE(ai), 1e-6);

    ASSERT_EQUAL(0, inform_local_active_info((uint64_t[]){0,0,1,1,1,1,0,0,0}, 1, 9, 2, 2, ai));
    ASSERT_DBL_NEAR_TOL(0.305958, AVERAGE(ai), 1e-6);

    ASSERT_EQUAL(0, inform_local_active_info((uint64_t[]){1,0,0,0,0,0,0,1,1}, 1, 9, 2, 2, ai));
    ASSERT_DBL_NEAR_TOL(0.347458, AVERAGE(ai), 1e-6);

    ASSERT_EQUAL(0, inform_local_active_info((uint64_t[]){1,0,0,0,0,0,0,1,1}, 1, 9, 2, 2, ai));
    ASSERT_DBL_NEAR_TOL(0.347458, AVERAGE(ai), 1e-6);

    ASSERT_EQUAL(0, inform_local_active_info((uint64_t[]){0,0,0,0,0,1,1,0,0}, 1, 9, 2, 2, ai));
    ASSERT_DBL_NEAR_TOL(0.399533, AVERAGE(ai), 1e-6);

    ASSERT_EQUAL(0, inform_local_active_info((uint64_t[]){0,0,0,0,1,1,0,0,0}, 1, 9, 2, 2, ai));
    ASSERT_DBL_NEAR_TOL(0.399533, AVERAGE(ai), 1e-6);

    ASSERT_EQUAL(0, inform_local_active_info((uint64_t[]){1,1,1,0,0,0,0,1,1}, 1, 9, 2, 2, ai));
    ASSERT_DBL_NEAR_TOL(0.305958, AVERAGE(ai), 1e-6);

    ASSERT_EQUAL(0, inform_local_active_info((uint64_t[]){0,0,0,1,1,1,1,0,0}, 1, 9, 2, 2, ai));
    ASSERT_DBL_NEAR_TOL(0.305958, AVERAGE(ai), 1e-6);

    ASSERT_EQUAL(0, inform_local_active_info((uint64_t[]){0,0,0,0,0,0,1,1,0}, 1, 9, 2, 2, ai));
    ASSERT_DBL_NEAR_TOL(0.347458, AVERAGE(ai), 1e-6);
}

UNIT(LocalActiveInfoSingleSeries_Base4)
{
    double ai[7];
    ASSERT_EQUAL(0, inform_local_active_info((uint64_t[]){3,3,3,2,1,0,0,0,1}, 1, 9, 4, 2, ai));
    ASSERT_DBL_NEAR_TOL(0.635471, AVERAGE(ai), 1e-6);

    ASSERT_EQUAL(0, inform_local_active_info((uint64_t[]){2,2,3,3,3,3,2,1,0}, 1, 9, 4, 2, ai));
    ASSERT_DBL_NEAR_TOL(0.635471, AVERAGE(ai), 1e-6);

    ASSERT_EQUAL(0, inform_local_active_info((uint64_t[]){2,2,2,2,2,2,1,1,1}, 1, 9, 4, 2, ai));
    ASSERT_DBL_NEAR_TOL(0.234783, AVERAGE(ai), 1e-6);
}

UNIT(LocalActiveInfoEnsemble)
{
    {
        double ai[12];
        uint64_t series[16] = {
            1,1,0,0,1,0,0,1,
            0,0,0,1,0,0,0,1,
        };
        ASSERT_EQUAL(0, inform_local_active_info(series, 2, 8, 2, 2, ai));
        ASSERT_DBL_NEAR_TOL(0.459148, AVERAGE(ai), 1e-6);
    }

    {
        double ai[63];
        uint64_t series[81] = {
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
        ASSERT_EQUAL(0, inform_local_active_info(series, 9, 9, 2, 2, ai));
        ASSERT_DBL_NEAR_TOL(0.3080467, AVERAGE(ai), 1e-6);
    }
}

UNIT(LocalActiveInfoEnsemble_Base4)
{
    {
        double ai[28];
        uint64_t series[36] = {
            3, 3, 3, 2, 1, 0, 0, 0, 1,
            2, 2, 3, 3, 3, 3, 2, 1, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0,
            1, 1, 0, 0, 0, 1, 1, 2, 2,
        };
        ASSERT_EQUAL(0, inform_local_active_info(series, 4, 9, 4, 2, ai));
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
    ADD_UNIT(LocalActiveInfoSingleSeries_Base2)
    ADD_UNIT(LocalActiveInfoSingleSeries_Base4)
    ADD_UNIT(LocalActiveInfoEnsemble)
    ADD_UNIT(LocalActiveInfoEnsemble_Base4)
END_SUITE
