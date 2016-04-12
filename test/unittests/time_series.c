// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <ctest.h>
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

CTEST(TimeSeries, ActiveInfoSingleSeries)
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
