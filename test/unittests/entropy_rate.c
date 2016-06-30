// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <ctest.h>
#include <inform/state_encoding.h>
#include <inform/entropy_rate.h>
#include "random.h"

CTEST(TimeSeries, EntropyRateSeriesTooShort)
{
    uint64_t const series[] = {1,1,0,0,1,0,0,1};
    ASSERT_TRUE(isnan(inform_entropy_rate(series, 0, 2, 2)));
    ASSERT_TRUE(isnan(inform_entropy_rate(series, 1, 2, 2)));
}

CTEST(TimeSeries, EntropyRateHistoryTooLong)
{
    {
        uint64_t const series[] = {1,1,0,0,1,0,0,1};

        ASSERT_TRUE(isnan(inform_entropy_rate(series, 2, 2, 2)));
        ASSERT_FALSE(isnan(inform_entropy_rate(series, 3, 2, 2)));
    }

    {
        size_t const size = 30;

        uint64_t *series = random_series(size, 2);
        ASSERT_TRUE(isnan(inform_entropy_rate(series, size, 2, 26)));
        free(series);

        series = random_series(size, 3);
        ASSERT_TRUE(isnan(inform_entropy_rate(series, size, 3, 16)));
        free(series);

        series = random_series(size, 4);
        ASSERT_TRUE(isnan(inform_entropy_rate(series, size, 4, 13)));
        free(series);
    }
}

CTEST(TimeSeries, EntropyRateEncodingError)
{
    uint64_t const series[] = {2,1,0,0,1,0,0,1};
    ASSERT_FALSE(isnan(inform_entropy_rate(series, 8, 3, 2)));
    ASSERT_TRUE(isnan(inform_entropy_rate(series, 8, 2, 2)));
}

CTEST(TimeSeries, EntropyRateSingleSeries_Base2)
{
    ASSERT_DBL_NEAR_TOL(0.000000,
            inform_entropy_rate((uint64_t[]){1,1,0,0,1,0,0,1}, 8, 2, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.000000,
            inform_entropy_rate((uint64_t[]){1,0,0,0,0,0,0,0,0}, 9, 2, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.679270,
            inform_entropy_rate((uint64_t[]){0,0,1,1,1,1,0,0,0}, 9, 2, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.515663,
            inform_entropy_rate((uint64_t[]){1,0,0,0,0,0,0,1,1}, 9, 2, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.515663,
            inform_entropy_rate((uint64_t[]){1,0,0,0,0,0,0,1,1}, 9, 2, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.463587,
            inform_entropy_rate((uint64_t[]){0,0,0,0,0,1,1,0,0}, 9, 2, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.463587,
            inform_entropy_rate((uint64_t[]){0,0,0,0,1,1,0,0,0}, 9, 2, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.679270,
            inform_entropy_rate((uint64_t[]){1,1,1,0,0,0,0,1,1}, 9, 2, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.679270,
            inform_entropy_rate((uint64_t[]){0,0,0,1,1,1,1,0,0}, 9, 2, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.515663,
            inform_entropy_rate((uint64_t[]){0,0,0,0,0,0,1,1,0}, 9, 2, 2),
            1e-6);
}

CTEST(TimeSeries, EntropyRateSingleSeries_Base4)
{
    ASSERT_DBL_NEAR_TOL(0.285715,
            inform_entropy_rate((uint64_t[]){3,3,3,2,1,0,0,0,1}, 9, 4, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.196778,
            inform_entropy_rate((uint64_t[]){2,2,3,3,3,3,2,1,0}, 9, 4, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.257831,
            inform_entropy_rate((uint64_t[]){2,2,2,2,2,2,1,1,1}, 9, 4, 2),
            1e-6);
}

CTEST(TimeSeries, EntropyRateEnsemble)
{
    {
        uint64_t series[16] = {
            1,1,0,0,1,0,0,1,
            0,0,0,1,0,0,0,1,
        };
        ASSERT_DBL_NEAR_TOL(0.459148,
                inform_entropy_rate_ensemble(series, 2, 8, 2, 2), 1e-6);
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
        ASSERT_DBL_NEAR_TOL(0.610249,
                inform_entropy_rate_ensemble(series, 9, 9, 2, 2), 1e-6);
    }
}

CTEST(TimeSeries, EntropyRateEnsemble_Base4)
{
    {
        uint64_t series[36] = {
            3, 3, 3, 2, 1, 0, 0, 0, 1,
            2, 2, 3, 3, 3, 3, 2, 1, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0,
            1, 1, 0, 0, 0, 1, 1, 2, 2,
        };
        ASSERT_DBL_NEAR_TOL(0.272234,
                inform_entropy_rate_ensemble(series, 4, 9, 4, 2), 1e-6);
    }
}
