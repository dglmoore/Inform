// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <inform/utilities.h>
#include <float.h>
#include <unit.h>

UNIT(RangeNullSeries)
{
    inform_error err = INFORM_SUCCESS;
    ASSERT_DBL_NEAR(0.0, inform_range(NULL, 0, NULL, NULL, &err));
    ASSERT_EQUAL(INFORM_ETIMESERIES, err);
}

UNIT(RangeEmpty)
{
    inform_error err = INFORM_SUCCESS;
    ASSERT_DBL_NEAR(0.0, inform_range((double[]){1,2,3}, 0, NULL, NULL, &err));
    ASSERT_EQUAL(INFORM_ESHORTSERIES, err);
}

UNIT(RangeNullMinMax)
{
    inform_error err = INFORM_SUCCESS;
    ASSERT_DBL_NEAR(2.0, inform_range((double[]){1,2,3}, 3, NULL, NULL, &err));
    ASSERT_EQUAL(INFORM_SUCCESS, err);
}

UNIT(RangeMinMax)
{
    inform_error err = INFORM_SUCCESS;
    double min, max;
    ASSERT_DBL_NEAR(2.0, inform_range((double[]){1,2,3}, 3, &min, &max, &err));
    ASSERT_EQUAL(INFORM_SUCCESS, err);
    ASSERT_DBL_NEAR(1.0, min);
    ASSERT_DBL_NEAR(3.0, max);
}

UNIT(BinNullSeries)
{
    inform_error err = INFORM_SUCCESS;
    ASSERT_DBL_NEAR(0.0, inform_bin(NULL, 0, 2, NULL, &err));
    ASSERT_EQUAL(INFORM_ETIMESERIES, err);
}

UNIT(BinEmpty)
{
    inform_error err = INFORM_SUCCESS;
    ASSERT_DBL_NEAR(0.0, inform_bin((double[]){1,2,3,4,5,6}, 0, 2, NULL, &err));
    ASSERT_EQUAL(INFORM_ESHORTSERIES, err);
}

UNIT(BinInvalidBining)
{
    inform_error err = INFORM_SUCCESS;
    ASSERT_DBL_NEAR(0.0, inform_bin((double[]){1,2,3,4,5,6}, 6, -1, NULL, &err));
    ASSERT_EQUAL(INFORM_EBIN, err);

    err = INFORM_SUCCESS;
    ASSERT_DBL_NEAR(0.0, inform_bin((double[]){1,2,3,4,5,6}, 6, 0, NULL, &err));
    ASSERT_EQUAL(INFORM_EBIN, err);
    
    err = INFORM_SUCCESS;
    ASSERT_DBL_NEAR(0.0, inform_bin((double[]){1,2,3,4,5,6}, 6, 1, NULL, &err));
    ASSERT_EQUAL(INFORM_EBIN, err);
}

UNIT(BinNullBinned)
{
    inform_error err = INFORM_SUCCESS;
    ASSERT_DBL_NEAR(0.0, inform_bin((double[]){1,2,3,4,5,6}, 6, 2, NULL, &err));
    ASSERT_EQUAL(INFORM_ETIMESERIES, err);
}

UNIT(BinTwo)
{
    inform_error err = INFORM_SUCCESS;
    int binned[6];
    int expect[6] = {0,0,0,1,1,1};
    ASSERT_DBL_NEAR(2.5, inform_bin((double[]){1,2,3,4,5,6}, 6, 2, binned, &err));
    ASSERT_EQUAL(INFORM_SUCCESS, err);
    for (size_t i = 0; i < 6; ++i)
    {
        ASSERT_EQUAL(expect[i], binned[i]);
    }
}

UNIT(BinThree)
{
    inform_error err = INFORM_SUCCESS;
    int binned[6];
    int expect[6] = {0,0,1,1,2,2};
    ASSERT_DBL_NEAR(5.0/3, inform_bin((double[]){1,2,3,4,5,6}, 6, 3, binned, &err));
    ASSERT_EQUAL(INFORM_SUCCESS, err);
    for (size_t i = 0; i < 6; ++i)
    {
        ASSERT_EQUAL(expect[i], binned[i]);
    }
}

UNIT(BinSix)
{
    inform_error err = INFORM_SUCCESS;
    int binned[6];
    int expect[6] = {0,1,2,3,4,5};
    ASSERT_DBL_NEAR(5.0/6, inform_bin((double[]){1,2,3,4,5,6}, 6, 6, binned, &err));
    ASSERT_EQUAL(INFORM_SUCCESS, err);
    for (size_t i = 0; i < 6; ++i)
    {
        ASSERT_EQUAL(expect[i], binned[i]);
    }
}

UNIT(BinStepNullSeries)
{
    inform_error err = INFORM_SUCCESS;
    ASSERT_DBL_NEAR(0.0, inform_bin_step(NULL, 0, 2, NULL, &err));
    ASSERT_EQUAL(INFORM_ETIMESERIES, err);
}

UNIT(BinStepEmpty)
{
    inform_error err = INFORM_SUCCESS;
    ASSERT_DBL_NEAR(0.0, inform_bin_step((double[]){1,2,3,4,5,6}, 0, 2, NULL, &err));
    ASSERT_EQUAL(INFORM_ESHORTSERIES, err);
}

UNIT(BinStepInvalidBining)
{
    inform_error err = INFORM_SUCCESS;
    ASSERT_DBL_NEAR(0.0, inform_bin_step((double[]){1,2,3,4,5,6}, 6, -1., NULL, &err));
    ASSERT_EQUAL(INFORM_EBIN, err);

    err = INFORM_SUCCESS;
    ASSERT_DBL_NEAR(0.0, inform_bin_step((double[]){1,2,3,4,5,6}, 6, 0., NULL, &err));
    ASSERT_EQUAL(INFORM_EBIN, err);

    err = INFORM_SUCCESS;
    ASSERT_DBL_NEAR(0.0, inform_bin_step((double[]){1,2,3,4,5,6}, 6, DBL_EPSILON, NULL, &err));
    ASSERT_EQUAL(INFORM_EBIN, err);
}

UNIT(BinStepNullBinned)
{
    inform_error err = INFORM_SUCCESS;
    ASSERT_DBL_NEAR(0.0, inform_bin_step((double[]){1,2,3,4,5,6}, 6, 2., NULL, &err));
    ASSERT_EQUAL(INFORM_ETIMESERIES, err);
}

UNIT(BinStepTwo)
{
    inform_error err = INFORM_SUCCESS;
    int binned[6];
    int expect[6] = {0,0,1,1,2,2};
    ASSERT_DBL_NEAR(3, inform_bin_step((double[]){1,2,3,4,5,6}, 6, 2., binned, &err));
    ASSERT_EQUAL(INFORM_SUCCESS, err);
    for (size_t i = 0; i < 6; ++i)
    {
        ASSERT_EQUAL(expect[i], binned[i]);
    }
}

UNIT(BinStepFiveHalves)
{
    inform_error err = INFORM_SUCCESS;
    int binned[6];
    int expect[6] = {0,0,0,1,1,2};
    ASSERT_DBL_NEAR(3, inform_bin_step((double[]){1,2,3,4,5,6}, 6, 5./2, binned, &err));
    ASSERT_EQUAL(INFORM_SUCCESS, err);
    for (size_t i = 0; i < 6; ++i)
    {
        ASSERT_EQUAL(expect[i], binned[i]);
    }
}

UNIT(BinStepOne)
{
    inform_error err = INFORM_SUCCESS;
    int binned[6];
    int expect[6] = {0,1,2,3,4,5};
    ASSERT_DBL_NEAR(6, inform_bin_step((double[]){1,2,3,4,5,6}, 6, 1.0, binned, &err));
    ASSERT_EQUAL(INFORM_SUCCESS, err);
    for (size_t i = 0; i < 6; ++i)
    {
        ASSERT_EQUAL(expect[i], binned[i]);
    }
}

UNIT(BinStepHalf)
{
    inform_error err = INFORM_SUCCESS;
    int binned[6];
    int expect[6] = {0,2,4,6,8,10};
    ASSERT_DBL_NEAR(11, inform_bin_step((double[]){1,2,3,4,5,6}, 6, 0.5, binned, &err));
    ASSERT_EQUAL(INFORM_SUCCESS, err);
    for (size_t i = 0; i < 6; ++i)
    {
        ASSERT_EQUAL(expect[i], binned[i]);
    }
}

UNIT(BinBoundsNullSeries)
{
    inform_error err = INFORM_SUCCESS;
    ASSERT_EQUAL(0, inform_bin_bounds(NULL, 0, NULL, 0, NULL, &err));
    ASSERT_EQUAL(INFORM_ETIMESERIES, err);
}

UNIT(BinBoundsEmpty)
{
    inform_error err = INFORM_SUCCESS;
    ASSERT_EQUAL(0, inform_bin_bounds((double[]){1,2,3,4,5,6}, 0,
                NULL, 0, NULL, &err));
    ASSERT_EQUAL(INFORM_ESHORTSERIES, err);
}

UNIT(BinBoundsInvalidBining)
{
    inform_error err = INFORM_SUCCESS;
    ASSERT_EQUAL(0, inform_bin_bounds((double[]){1,2,3,4,5,6}, 6,
                NULL, 0, NULL, &err));
    ASSERT_EQUAL(INFORM_EBIN, err);

    err = INFORM_SUCCESS;
    ASSERT_EQUAL(0, inform_bin_bounds((double[]){1,2,3,4,5,6}, 6,
                (double[]){3.0}, 0, NULL, &err));
    ASSERT_EQUAL(INFORM_EBIN, err);
}

UNIT(BinBoundsNullBinned)
{
    inform_error err = INFORM_SUCCESS;
    ASSERT_EQUAL(0, inform_bin_bounds((double[]){1,2,3,4,5,6}, 6,
                (double[]){3}, 1, NULL, &err));
    ASSERT_EQUAL(INFORM_ETIMESERIES, err);
}

UNIT(BinBoundsOne)
{
    inform_error err = INFORM_SUCCESS;
    int binned[6];
    int expect[6] = {0,0,1,1,1,1};
    ASSERT_EQUAL(2, inform_bin_bounds((double[]){1,2,3,4,5,6}, 6,
                (double[]){3}, 1, binned, &err));
    ASSERT_EQUAL(INFORM_SUCCESS, err);
    for (size_t i = 0; i < 6; ++i)
    {
        ASSERT_EQUAL(expect[i], binned[i]);
    }
}

UNIT(BinBoundsTwo)
{
    inform_error err = INFORM_SUCCESS;
    int binned[6];
    int expect[6] = {0,0,1,1,1,2};
    ASSERT_EQUAL(3, inform_bin_bounds((double[]){1,2,3,4,5,6}, 6,
                (double[]){2.5, 5.5}, 2, binned, &err));
    ASSERT_EQUAL(INFORM_SUCCESS, err);
    for (size_t i = 0; i < 6; ++i)
    {
        ASSERT_EQUAL(expect[i], binned[i]);
    }
}

UNIT(BinBoundsNone)
{
    inform_error err = INFORM_SUCCESS;
    int binned[6];
    int expect[6] = {0,0,0,0,0,0};
    ASSERT_EQUAL(1, inform_bin_bounds((double[]){1,2,3,4,5,6}, 6,
                (double[]){6.1}, 1, binned, &err));
    ASSERT_EQUAL(INFORM_SUCCESS, err);
    for (size_t i = 0; i < 6; ++i)
    {
        ASSERT_EQUAL(expect[i], binned[i]);
    }
}

UNIT(BinBoundsAll)
{
    inform_error err = INFORM_SUCCESS;
    int binned[6];
    int expect[6] = {1,1,1,1,1,1};
    ASSERT_EQUAL(2, inform_bin_bounds((double[]){1,2,3,4,5,6}, 6,
                (double[]){0.}, 1, binned, &err));
    ASSERT_EQUAL(INFORM_SUCCESS, err);
    for (size_t i = 0; i < 6; ++i)
    {
        ASSERT_EQUAL(expect[i], binned[i]);
    }
}

UNIT(CoalesceNullSeries)
{
    inform_error err = INFORM_SUCCESS;
    ASSERT_EQUAL(0, inform_coalesce(NULL, 0, NULL, &err));
    ASSERT_EQUAL(INFORM_ETIMESERIES, err);
}

UNIT(CoalesceEmpty)
{
    inform_error err = INFORM_SUCCESS;
    ASSERT_EQUAL(0, inform_coalesce((int[]){1,2,3,4}, 0, NULL, &err));
    ASSERT_EQUAL(INFORM_ESHORTSERIES, err);
}

UNIT(CoalesceNullCoalesce)
{
    inform_error err = INFORM_SUCCESS;
    ASSERT_EQUAL(0, inform_coalesce((int[]){1,2,3,4}, 4, NULL, &err));
    ASSERT_EQUAL(INFORM_ETIMESERIES, err);
}

UNIT(CoalesceUnchanged)
{
    inform_error err = INFORM_SUCCESS;
    int series[6] = {0,2,2,1,2,3};
    int binned[6];
    int *expect = series;
    ASSERT_EQUAL(4, inform_coalesce(series, 6, binned, &err));
    ASSERT_EQUAL(INFORM_SUCCESS, err);
    for (size_t i = 0; i < 6; ++i)
    {
        ASSERT_EQUAL(expect[i], binned[i]);
    }
}

UNIT(CoalesceShifted)
{
    inform_error err = INFORM_SUCCESS;
    int series[6] = {1,3,3,2,3,4};
    int binned[6];
    int expect[6] = {0,2,2,1,2,3};
    ASSERT_EQUAL(4, inform_coalesce(series, 6, binned, &err));
    ASSERT_EQUAL(INFORM_SUCCESS, err);
    for (size_t i = 0; i < 6; ++i)
    {
        ASSERT_EQUAL(expect[i], binned[i]);
    }
}

UNIT(CoalesceNoGaps)
{
    inform_error err = INFORM_SUCCESS;
    int series[6] = {2,8,7,2,0,0};
    int binned[6];
    int expect[6] = {1,3,2,1,0,0};
    ASSERT_EQUAL(4, inform_coalesce(series, 6, binned, &err));
    ASSERT_EQUAL(INFORM_SUCCESS, err);
    for (size_t i = 0; i < 6; ++i)
    {
        ASSERT_EQUAL(expect[i], binned[i]);
    }
}

BEGIN_SUITE(Utilities)
    ADD_UNIT(RangeNullSeries)
    ADD_UNIT(RangeEmpty)
    ADD_UNIT(RangeNullMinMax)
    ADD_UNIT(RangeMinMax)

    ADD_UNIT(BinNullSeries)
    ADD_UNIT(BinEmpty)
    ADD_UNIT(BinInvalidBining)
    ADD_UNIT(BinNullBinned)
    ADD_UNIT(BinTwo)
    ADD_UNIT(BinThree)
    ADD_UNIT(BinSix)

    ADD_UNIT(BinStepNullSeries)
    ADD_UNIT(BinStepEmpty)
    ADD_UNIT(BinStepInvalidBining)
    ADD_UNIT(BinStepNullBinned)
    ADD_UNIT(BinStepTwo)
    ADD_UNIT(BinStepFiveHalves)
    ADD_UNIT(BinStepOne)
    ADD_UNIT(BinStepHalf)

    ADD_UNIT(BinBoundsNullSeries)
    ADD_UNIT(BinBoundsEmpty)
    ADD_UNIT(BinBoundsInvalidBining)
    ADD_UNIT(BinBoundsNullBinned)
    ADD_UNIT(BinBoundsOne)
    ADD_UNIT(BinBoundsTwo)
    ADD_UNIT(BinBoundsNone)
    ADD_UNIT(BinBoundsAll)

    ADD_UNIT(CoalesceNullSeries)
    ADD_UNIT(CoalesceEmpty)
    ADD_UNIT(CoalesceNullCoalesce)
    ADD_UNIT(CoalesceUnchanged)
    ADD_UNIT(CoalesceShifted)
    ADD_UNIT(CoalesceNoGaps)
END_SUITE
