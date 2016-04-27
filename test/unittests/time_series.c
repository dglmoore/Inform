// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <ctest.h>
#include <inform/state_encoding.h>
#include <inform/time_series.h>
#include <math.h>

CTEST(TimeSeries, ActiveInfoSeriesTooShort)
{
    uint64_t const series[] = {1,1,0,0,1,0,0,1};
    ASSERT_TRUE(isnan(inform_active_info(series, 0, 2, 2)));
    ASSERT_TRUE(isnan(inform_active_info(series, 1, 2, 2)));
}

CTEST(TimeSeries, ActiveInfoHistoryTooLong)
{
    uint64_t const series[] = {1,1,0,0,1,0,0,1};

    ASSERT_TRUE(isnan(inform_active_info(series, 2, 2, 2)));
    ASSERT_FALSE(isnan(inform_active_info(series, 3, 2, 2)));
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
