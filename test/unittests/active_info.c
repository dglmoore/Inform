// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <ctest.h>
#include <inform/state_encoding.h>
#include <inform/active_info.h>
#include "random.h"

#include <stdio.h>

CTEST(TimeSeries, ActiveInfoSeriesTooShort)
{
    uint64_t const series[] = {1,1,0,0,1,0,0,1};
    ASSERT_TRUE(isnan(inform_active_info(series, 0, 2, 2)));
    ASSERT_TRUE(isnan(inform_active_info(series, 1, 2, 2)));
}

CTEST(TimeSeries, ActiveInfoHistoryTooLong)
{
    {
        uint64_t const series[] = {1,1,0,0,1,0,0,1};

        ASSERT_TRUE(isnan(inform_active_info(series, 2, 2, 2)));
        ASSERT_FALSE(isnan(inform_active_info(series, 3, 2, 2)));
    }

    {
        size_t const size = 30;

        uint64_t *series = random_series(size, 2);
        ASSERT_TRUE(isnan(inform_active_info(series, size, 2, 26)));
        free(series);

        series = random_series(size, 3);
        ASSERT_TRUE(isnan(inform_active_info(series, size, 3, 16)));
        free(series);

        series = random_series(size, 4);
        ASSERT_TRUE(isnan(inform_active_info(series, size, 4, 13)));
        free(series);
    }
}

CTEST(TimeSeries, ActiveInfoEncodingError)
{
    uint64_t const series[] = {2,1,0,0,1,0,0,1};
    ASSERT_FALSE(isnan(inform_active_info(series, 8, 3, 2)));
    ASSERT_TRUE(isnan(inform_active_info(series, 8, 2, 2)));
}

CTEST(TimeSeries, ActiveInfoSingleSeries_Base2)
{
    ASSERT_DBL_NEAR_TOL(0.918296,
            inform_active_info((uint64_t[]){1,1,0,0,1,0,0,1}, 8, 2, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.000000,
            inform_active_info((uint64_t[]){1,0,0,0,0,0,0,0,0}, 9, 2, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.305958,
            inform_active_info((uint64_t[]){0,0,1,1,1,1,0,0,0}, 9, 2, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.347458,
            inform_active_info((uint64_t[]){1,0,0,0,0,0,0,1,1}, 9, 2, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.347458,
            inform_active_info((uint64_t[]){1,0,0,0,0,0,0,1,1}, 9, 2, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.399533,
            inform_active_info((uint64_t[]){0,0,0,0,0,1,1,0,0}, 9, 2, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.399533,
            inform_active_info((uint64_t[]){0,0,0,0,1,1,0,0,0}, 9, 2, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.305958,
            inform_active_info((uint64_t[]){1,1,1,0,0,0,0,1,1}, 9, 2, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.305958,
            inform_active_info((uint64_t[]){0,0,0,1,1,1,1,0,0}, 9, 2, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.347458,
            inform_active_info((uint64_t[]){0,0,0,0,0,0,1,1,0}, 9, 2, 2),
            1e-6);
}

CTEST(TimeSeries, ActiveInfoSingleSeries_Base4)
{
    ASSERT_DBL_NEAR_TOL(0.635471,
            inform_active_info((uint64_t[]){3,3,3,2,1,0,0,0,1}, 9, 4, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.635471,
            inform_active_info((uint64_t[]){2,2,3,3,3,3,2,1,0}, 9, 4, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.234783,
            inform_active_info((uint64_t[]){2,2,2,2,2,2,1,1,1}, 9, 4, 2),
            1e-6);
}

CTEST(TimeSeries, ActiveInfoEnsemble)
{
    {
        uint64_t series[16] = {
            1,1,0,0,1,0,0,1,
            0,0,0,1,0,0,0,1,
        };
        ASSERT_DBL_NEAR_TOL(0.459148,
                inform_active_info_ensemble(series, 2, 8, 2, 2), 1e-6);
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
                inform_active_info_ensemble(series, 9, 9, 2, 2), 1e-6);
    }
}

CTEST(TimeSeries, ActiveInfoEnsemble_Base4)
{
    {
        uint64_t series[36] = {
            3, 3, 3, 2, 1, 0, 0, 0, 1,
            2, 2, 3, 3, 3, 3, 2, 1, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0,
            1, 1, 0, 0, 0, 1, 1, 2, 2,
        };
        ASSERT_DBL_NEAR_TOL(0.662146,
                inform_active_info_ensemble(series, 4, 9, 4, 2), 1e-6);
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

CTEST(TimeSeries, LocalActiveInfoSeriesTooShort)
{
    double ai[8];
    uint64_t const series[] = {1,1,0,0,1,0,0,1};
    ASSERT_EQUAL(3, inform_local_active_info(series, 0, 2, 2, ai));
    ASSERT_EQUAL(3, inform_local_active_info(series, 1, 2, 2, ai));
}

CTEST(TimeSeries, LocalActiveInfoHistoryTooLong)
{
    {
        double ai[6];
        uint64_t const series[] = {1,1,0,0,1,0,0,1};

        ASSERT_EQUAL(4, inform_local_active_info(series, 2, 2, 2, ai));
        ASSERT_EQUAL(0, inform_local_active_info(series, 3, 2, 2, ai));
    }

    {
        double ai[30];
        size_t const size = 30;

        uint64_t *series = random_series(size, 2);
        ASSERT_EQUAL(5, inform_local_active_info(series, size, 2, 26, ai));
        free(series);

        series = random_series(size, 3);
        ASSERT_EQUAL(5, inform_local_active_info(series, size, 3, 16, ai));
        free(series);

        series = random_series(size, 4);
        ASSERT_EQUAL(5, inform_local_active_info(series, size, 4, 13, ai));
        free(series);
    }
}

CTEST(TimeSeries, LocalActiveInfoEncodingError)
{
    double ai[8];
    uint64_t const series[] = {2,1,0,0,1,0,0,1};
    ASSERT_EQUAL(0, inform_local_active_info(series, 8, 3, 2, ai));
    ASSERT_EQUAL(6, inform_local_active_info(series, 8, 2, 2, ai));
}

CTEST(TimeSeries, LocalActiveInfoSingleSeries_Base2)
{
    double ai[7] = {0, 0, 0, 0, 0, 0, 0};

    ASSERT_EQUAL(0, inform_local_active_info((uint64_t[]){1,0,0,0,0,0,0,0,0}, 9, 2, 2, ai));
    ASSERT_DBL_NEAR_TOL(0.000000, AVERAGE(ai), 1e-6);

    ASSERT_EQUAL(0, inform_local_active_info((uint64_t[]){0,0,1,1,1,1,0,0,0}, 9, 2, 2, ai));
    ASSERT_DBL_NEAR_TOL(0.305958, AVERAGE(ai), 1e-6);

    ASSERT_EQUAL(0, inform_local_active_info((uint64_t[]){1,0,0,0,0,0,0,1,1}, 9, 2, 2, ai));
    ASSERT_DBL_NEAR_TOL(0.347458, AVERAGE(ai), 1e-6);

    ASSERT_EQUAL(0, inform_local_active_info((uint64_t[]){1,0,0,0,0,0,0,1,1}, 9, 2, 2, ai));
    ASSERT_DBL_NEAR_TOL(0.347458, AVERAGE(ai), 1e-6);

    ASSERT_EQUAL(0, inform_local_active_info((uint64_t[]){0,0,0,0,0,1,1,0,0}, 9, 2, 2, ai));
    ASSERT_DBL_NEAR_TOL(0.399533, AVERAGE(ai), 1e-6);

    ASSERT_EQUAL(0, inform_local_active_info((uint64_t[]){0,0,0,0,1,1,0,0,0}, 9, 2, 2, ai));
    ASSERT_DBL_NEAR_TOL(0.399533, AVERAGE(ai), 1e-6);

    ASSERT_EQUAL(0, inform_local_active_info((uint64_t[]){1,1,1,0,0,0,0,1,1}, 9, 2, 2, ai));
    ASSERT_DBL_NEAR_TOL(0.305958, AVERAGE(ai), 1e-6);

    ASSERT_EQUAL(0, inform_local_active_info((uint64_t[]){0,0,0,1,1,1,1,0,0}, 9, 2, 2, ai));
    ASSERT_DBL_NEAR_TOL(0.305958, AVERAGE(ai), 1e-6);

    ASSERT_EQUAL(0, inform_local_active_info((uint64_t[]){0,0,0,0,0,0,1,1,0}, 9, 2, 2, ai));
    ASSERT_DBL_NEAR_TOL(0.347458, AVERAGE(ai), 1e-6);
}

CTEST(TimeSeries, LocalActiveInfoSingleSeries_Base4)
{
    double ai[7];
    ASSERT_EQUAL(0, inform_local_active_info((uint64_t[]){3, 3, 3, 2, 1, 0, 0, 0, 1}, 9, 4, 2, ai));
    ASSERT_DBL_NEAR_TOL(0.635471, AVERAGE(ai), 1e-6);

    ASSERT_EQUAL(0, inform_local_active_info((uint64_t[]){2,2,3,3,3,3,2,1,0}, 9, 4, 2, ai));
    ASSERT_DBL_NEAR_TOL(0.635471, AVERAGE(ai), 1e-6);

    ASSERT_EQUAL(0, inform_local_active_info((uint64_t[]){2,2,2,2,2,2,1,1,1}, 9, 4, 2, ai));
    ASSERT_DBL_NEAR_TOL(0.234783, AVERAGE(ai), 1e-6);
}

CTEST(TimeSeries, LocalActiveInfoEnsemble)
{
    {
        double ai[12];
        uint64_t series[16] = {
            1,1,0,0,1,0,0,1,
            0,0,0,1,0,0,0,1,
        };
        ASSERT_EQUAL(0, inform_local_active_info_ensemble(series, 2, 8, 2, 2, ai));
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
        ASSERT_EQUAL(0, inform_local_active_info_ensemble(series, 9, 9, 2, 2, ai));
        ASSERT_DBL_NEAR_TOL(0.3080467, AVERAGE(ai), 1e-6);
    }
}

CTEST(TimeSeries, LocalActiveInfoEnsemble_Base4)
{
    {
        double ai[28];
        uint64_t series[36] = {
            3, 3, 3, 2, 1, 0, 0, 0, 1,
            2, 2, 3, 3, 3, 3, 2, 1, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0,
            1, 1, 0, 0, 0, 1, 1, 2, 2,
        };
        ASSERT_EQUAL(0, inform_local_active_info_ensemble(series, 4, 9, 4, 2, ai));
        ASSERT_DBL_NEAR_TOL(0.662146, AVERAGE(ai), 1e-6);
    }
}
