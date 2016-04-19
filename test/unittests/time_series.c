// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <ctest.h>
#include <inform/state_encoding.h>
#include <inform/time_series.h>
#include <math.h>

CTEST(TimeSeries, ActiveInfoSeriesTooShort)
{
    int const series[] = {1,1,0,0,1,0,0,1};
    ASSERT_TRUE(isnan(inform_active_info(series, 0, 2)));
    ASSERT_TRUE(isnan(inform_active_info(series, 1, 2)));
}

CTEST(TimeSeries, ActiveInfoHistoryTooLong)
{
    int const series[] = {1,1,0,0,1,0,0,1};

    ASSERT_TRUE(isnan(inform_active_info(series, 2, 2)));
    ASSERT_FALSE(isnan(inform_active_info(series, 3, 2)));
}

CTEST(TimeSeries, ActiveInfoSingleSeries_Base2)
{
    ASSERT_DBL_NEAR_TOL(0.918296,
            inform_active_info((int[]){1,1,0,0,1,0,0,1}, 8, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.000000,
            inform_active_info((int[]){1,0,0,0,0,0,0,0,0}, 9, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.305958,
            inform_active_info((int[]){0,0,1,1,1,1,0,0,0}, 9, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.347458,
            inform_active_info((int[]){1,0,0,0,0,0,0,1,1}, 9, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.347458,
            inform_active_info((int[]){1,0,0,0,0,0,0,1,1}, 9, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.399533,
            inform_active_info((int[]){0,0,0,0,0,1,1,0,0}, 9, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.399533,
            inform_active_info((int[]){0,0,0,0,1,1,0,0,0}, 9, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.305958,
            inform_active_info((int[]){1,1,1,0,0,0,0,1,1}, 9, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.305958,
            inform_active_info((int[]){0,0,0,1,1,1,1,0,0}, 9, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.347458,
            inform_active_info((int[]){0,0,0,0,0,0,1,1,0}, 9, 2),
            1e-6);
}

#include <stdio.h>

CTEST(TimeSeries, ActiveInfoSingleSeries_Base4)
{
    ASSERT_DBL_NEAR_TOL(1.270942,
            inform_active_info((int[]){3,3,3,2,1,0,0,0,1}, 9, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(1.270942,
            inform_active_info((int[]){2,2,3,3,3,3,2,1,0}, 9, 2),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.469565,
            inform_active_info((int[]){2,2,2,2,2,2,1,1,1}, 9, 2),
            1e-6);
}

CTEST(TimeSeries, ActiveInfoEnsemble)
{
    {
        int series[16] = {
            1,1,0,0,1,0,0,1,
            0,0,0,1,0,0,0,1,
        };
        ASSERT_DBL_NEAR_TOL(0.459148,
                inform_active_info_ensemble(series, 2, 8, 2), 1e-6);
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
                inform_active_info_ensemble(series, 9, 9, 2), 1e-6);
    }
}

CTEST(TimeSeries, ActiveInfoEnsemble_Base4)
{
    {
        int series[36] = {
            3, 3, 3, 2, 1, 0, 0, 0, 1,
            2, 2, 3, 3, 3, 3, 2, 1, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0,
            1, 1, 0, 0, 0, 1, 1, 2, 2,
        };
        ASSERT_DBL_NEAR_TOL(1.324291,
                inform_active_info_ensemble(series, 4, 9, 2), 1e-6);
    }
}
