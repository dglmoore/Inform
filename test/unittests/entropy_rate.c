// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <inform/entropy_rate.h>
#include <math.h>
#include <unit.h>
#include "random.h"

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

UNIT(EntropyRateSeriesTooShort)
{
    int const series[] = {1,1,0,0,1,0,0,1};
    inform_error err;
    inform_error *errptr = &err;

    *errptr = INFORM_SUCCESS;
    ASSERT_FALSE(inform_failed(errptr));
    ASSERT_TRUE(isnan(inform_entropy_rate(series, 1, 0, 2, 2, errptr)));
    ASSERT_TRUE(inform_failed(errptr));

    *errptr = INFORM_SUCCESS;
    ASSERT_FALSE(inform_failed(errptr));
    ASSERT_TRUE(isnan(inform_entropy_rate(series, 1, 1, 2, 2, errptr)));
    ASSERT_TRUE(inform_failed(errptr));
}

UNIT(EntropyRateHistoryTooLong)
{
    int const series[] = {1,1,0,0,1,0,0,1};
    inform_error err;
    inform_error *errptr = &err;

    *errptr = INFORM_SUCCESS;
    ASSERT_FALSE(inform_failed(errptr));
    ASSERT_TRUE(isnan(inform_entropy_rate(series, 1, 2, 2, 2, errptr)));
    ASSERT_TRUE(inform_failed(errptr));

    *errptr = INFORM_SUCCESS;
    ASSERT_FALSE(inform_failed(errptr));
    ASSERT_FALSE(isnan(inform_entropy_rate(series, 1, 3, 2, 2, errptr)));
    ASSERT_FALSE(inform_failed(errptr));
}

UNIT(EntropyRateEncodingError)
{
    int const series[] = {2,1,0,0,1,0,0,1};
    inform_error err;
    inform_error *errptr = &err;

    *errptr = INFORM_SUCCESS;
    ASSERT_FALSE(inform_failed(errptr));
    ASSERT_FALSE(isnan(inform_entropy_rate(series, 1, 8, 3, 2, errptr)));
    ASSERT_FALSE(inform_failed(errptr));

    *errptr = INFORM_SUCCESS;
    ASSERT_FALSE(inform_failed(errptr));
    ASSERT_TRUE(isnan(inform_entropy_rate(series, 1, 8, 2, 2, errptr)));
    ASSERT_TRUE(inform_failed(errptr));
}

UNIT(EntropyRateSingleSeries_Base2)
{
    ASSERT_DBL_NEAR_TOL(0.000000,
            inform_entropy_rate((int[]){1,1,0,0,1,0,0,1}, 1, 8, 2, 2, NULL),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.000000,
            inform_entropy_rate((int[]){1,0,0,0,0,0,0,0,0}, 1, 9, 2, 2, NULL),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.679270,
            inform_entropy_rate((int[]){0,0,1,1,1,1,0,0,0}, 1, 9, 2, 2, NULL),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.515663,
            inform_entropy_rate((int[]){1,0,0,0,0,0,0,1,1}, 1, 9, 2, 2, NULL),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.515663,
            inform_entropy_rate((int[]){1,0,0,0,0,0,0,1,1}, 1, 9, 2, 2, NULL),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.463587,
            inform_entropy_rate((int[]){0,0,0,0,0,1,1,0,0}, 1, 9, 2, 2, NULL),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.463587,
            inform_entropy_rate((int[]){0,0,0,0,1,1,0,0,0}, 1, 9, 2, 2, NULL),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.679270,
            inform_entropy_rate((int[]){1,1,1,0,0,0,0,1,1}, 1, 9, 2, 2, NULL),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.679270,
            inform_entropy_rate((int[]){0,0,0,1,1,1,1,0,0}, 1, 9, 2, 2, NULL),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.515663,
            inform_entropy_rate((int[]){0,0,0,0,0,0,1,1,0}, 1, 9, 2, 2, NULL),
            1e-6);
}

UNIT(EntropyRateSingleSeries_Base4)
{
    ASSERT_DBL_NEAR_TOL(0.285715,
            inform_entropy_rate((int[]){3,3,3,2,1,0,0,0,1}, 1, 9, 4, 2, NULL),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.196778,
            inform_entropy_rate((int[]){2,2,3,3,3,3,2,1,0}, 1, 9, 4, 2, NULL),
            1e-6);

    ASSERT_DBL_NEAR_TOL(0.257831,
            inform_entropy_rate((int[]){2,2,2,2,2,2,1,1,1}, 1, 9, 4, 2, NULL),
            1e-6);
}

UNIT(EntropyRateEnsemble)
{
    {
        int series[16] = {
            1,1,0,0,1,0,0,1,
            0,0,0,1,0,0,0,1,
        };
        ASSERT_DBL_NEAR_TOL(0.459148,
                inform_entropy_rate(series, 2, 8, 2, 2, NULL),
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
        ASSERT_DBL_NEAR_TOL(0.610249,
                inform_entropy_rate(series, 9, 9, 2, 2, NULL),
                1e-6);
    }
}

UNIT(EntropyRateEnsemble_Base4)
{
    {
        int series[36] = {
            3, 3, 3, 2, 1, 0, 0, 0, 1,
            2, 2, 3, 3, 3, 3, 2, 1, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0,
            1, 1, 0, 0, 0, 1, 1, 2, 2,
        };
        ASSERT_DBL_NEAR_TOL(0.272234,
                inform_entropy_rate(series, 4, 9, 4, 2, NULL),
                1e-6);
    }
}

UNIT(LocalEntropyRateSeriesTooShort)
{
    double er[6];
    int const series[] = {1,1,0,0,1,0,0,1};
    inform_error err;
    inform_error *errptr = &err;

    *errptr = INFORM_SUCCESS;
    ASSERT_FALSE(inform_failed(errptr));
    ASSERT_NULL(inform_local_entropy_rate(series, 1, 0, 2, 2, er, errptr));
    ASSERT_TRUE(inform_failed(errptr));

    *errptr = INFORM_SUCCESS;
    ASSERT_FALSE(inform_failed(errptr));
    ASSERT_NULL(inform_local_entropy_rate(series, 1, 1, 2, 2, er, errptr));
    ASSERT_TRUE(inform_failed(errptr));
}

UNIT(LocalEntropyRateHistoryTooLong)
{
    double er[6];
    int const series[] = {1,1,0,0,1,0,0,1};
    inform_error err;
    inform_error *errptr = &err;

    *errptr = INFORM_SUCCESS;
    ASSERT_FALSE(inform_failed(errptr));
    ASSERT_NULL(inform_local_entropy_rate(series, 1, 2, 2, 2, er, errptr));
    ASSERT_TRUE(inform_failed(errptr));

    *errptr = INFORM_SUCCESS;
    ASSERT_FALSE(inform_failed(errptr));
    ASSERT_NOT_NULL(inform_local_entropy_rate(series, 1, 3, 2, 2, er, errptr));
    ASSERT_FALSE(inform_failed(errptr));
}

UNIT(LocalEntropyRateEncodingError)
{
    double er[6];
    int const series[] = {2,1,0,0,1,0,0,1};
    inform_error err;
    inform_error *errptr = &err;

    *errptr = INFORM_SUCCESS;
    ASSERT_FALSE(inform_failed(errptr));
    ASSERT_NOT_NULL(inform_local_entropy_rate(series, 1, 8, 3, 2, er, errptr));
    ASSERT_FALSE(inform_failed(errptr));

    *errptr = INFORM_SUCCESS;
    ASSERT_FALSE(inform_failed(errptr));
    ASSERT_NULL(inform_local_entropy_rate(series, 1, 8, 2, 2, er, errptr));
    ASSERT_TRUE(inform_failed(errptr));
}

UNIT(LocalEntropyRateAllocatesOutput)
{
    inform_error err = INFORM_SUCCESS;
    double *er = inform_local_entropy_rate((int[]){0,0,1,1,1,1,0,0,0}, 1, 9, 2, 2, NULL, &err);
    ASSERT_NOT_NULL(er);
    ASSERT_EQUAL(INFORM_SUCCESS, err);
    free(er);
}

UNIT(LocalEntropyRateSingleSeries_Base2)
{
    double er[7];
    ASSERT_NOT_NULL(inform_local_entropy_rate((int[]){1,0,0,0,0,0,0,0,0}, 1, 9, 2, 2, er, NULL));
    ASSERT_DBL_NEAR_TOL(0.000000, AVERAGE(er), 1e-6);

    ASSERT_NOT_NULL(inform_local_entropy_rate((int[]){0,0,1,1,1,1,0,0,0}, 1, 9, 2, 2, er, NULL));
    ASSERT_DBL_NEAR_TOL(0.679270, AVERAGE(er), 1e-6);

    ASSERT_NOT_NULL(inform_local_entropy_rate((int[]){1,0,0,0,0,0,0,1,1}, 1, 9, 2, 2, er, NULL));
    ASSERT_DBL_NEAR_TOL(0.515663, AVERAGE(er), 1e-6);

    ASSERT_NOT_NULL(inform_local_entropy_rate((int[]){1,0,0,0,0,0,0,1,1}, 1, 9, 2, 2, er, NULL));
    ASSERT_DBL_NEAR_TOL(0.515663, AVERAGE(er), 1e-6);

    ASSERT_NOT_NULL(inform_local_entropy_rate((int[]){0,0,0,0,0,1,1,0,0}, 1, 9, 2, 2, er, NULL));
    ASSERT_DBL_NEAR_TOL(0.463587, AVERAGE(er), 1e-6);

    ASSERT_NOT_NULL(inform_local_entropy_rate((int[]){0,0,0,0,1,1,0,0,0}, 1, 9, 2, 2, er, NULL));
    ASSERT_DBL_NEAR_TOL(0.463587, AVERAGE(er), 1e-6);

    ASSERT_NOT_NULL(inform_local_entropy_rate((int[]){1,1,1,0,0,0,0,1,1}, 1, 9, 2, 2, er, NULL));
    ASSERT_DBL_NEAR_TOL(0.679270, AVERAGE(er), 1e-6);

    ASSERT_NOT_NULL(inform_local_entropy_rate((int[]){0,0,0,1,1,1,1,0,0}, 1, 9, 2, 2, er, NULL));
    ASSERT_DBL_NEAR_TOL(0.679270, AVERAGE(er), 1e-6);

    ASSERT_NOT_NULL(inform_local_entropy_rate((int[]){0,0,0,0,0,0,1,1,0}, 1, 9, 2, 2, er, NULL));
    ASSERT_DBL_NEAR_TOL(0.515663, AVERAGE(er), 1e-6);
}

UNIT(LocalEntropyRateSingleSeries_Base4)
{
    double er[7];

    ASSERT_NOT_NULL(inform_local_entropy_rate((int[]){3,3,3,2,1,0,0,0,1}, 1, 9, 4, 2, er, NULL));
    ASSERT_DBL_NEAR_TOL(0.285715, AVERAGE(er), 1e-6);

    ASSERT_NOT_NULL(inform_local_entropy_rate((int[]){2,2,3,3,3,3,2,1,0}, 1, 9, 4, 2, er, NULL));
    ASSERT_DBL_NEAR_TOL(0.196778, AVERAGE(er), 1e-6);

    ASSERT_NOT_NULL(inform_local_entropy_rate((int[]){2,2,2,2,2,2,1,1,1}, 1, 9, 4, 2, er, NULL));
    ASSERT_DBL_NEAR_TOL(0.257831, AVERAGE(er), 1e-6);
}

UNIT(LocalEntropyRateEnsemble)
{
    {
        double er[12];
        int series[16] = {
            1,1,0,0,1,0,0,1,
            0,0,0,1,0,0,0,1,
        };
        ASSERT_NOT_NULL(inform_local_entropy_rate(series, 2, 8, 2, 2, er, NULL));
        ASSERT_DBL_NEAR_TOL(0.459148, AVERAGE(er), 1e-6);
    }

    {
        double er[63];
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
        ASSERT_NOT_NULL(inform_local_entropy_rate(series, 9, 9, 2, 2, er, NULL));
        ASSERT_DBL_NEAR_TOL(0.610249, AVERAGE(er), 1e-6);
    }
}

UNIT(LocalEntropyRateEnsemble_Base4)
{
    {
        double er[28];
        int series[36] = {
            3, 3, 3, 2, 1, 0, 0, 0, 1,
            2, 2, 3, 3, 3, 3, 2, 1, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0,
            1, 1, 0, 0, 0, 1, 1, 2, 2,
        };
        ASSERT_NOT_NULL(inform_local_entropy_rate(series, 4, 9, 4, 2, er, NULL));
        ASSERT_DBL_NEAR_TOL(0.272234, AVERAGE(er), 1e-6);
    }
}

BEGIN_SUITE(EntropyRate)
    ADD_UNIT(EntropyRateSeriesTooShort)
    ADD_UNIT(EntropyRateHistoryTooLong)
    ADD_UNIT(EntropyRateEncodingError)
    ADD_UNIT(EntropyRateSingleSeries_Base2)
    ADD_UNIT(EntropyRateSingleSeries_Base4)
    ADD_UNIT(EntropyRateEnsemble)
    ADD_UNIT(EntropyRateEnsemble_Base4)
    ADD_UNIT(LocalEntropyRateSeriesTooShort)
    ADD_UNIT(LocalEntropyRateHistoryTooLong)
    ADD_UNIT(LocalEntropyRateEncodingError)
    ADD_UNIT(LocalEntropyRateAllocatesOutput)
    ADD_UNIT(LocalEntropyRateSingleSeries_Base2)
    ADD_UNIT(LocalEntropyRateSingleSeries_Base4)
    ADD_UNIT(LocalEntropyRateEnsemble)
    ADD_UNIT(LocalEntropyRateEnsemble_Base4)
END_SUITE
