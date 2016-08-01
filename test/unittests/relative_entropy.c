// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a reT
// license that can be found in the LICENSE file.
#include "util.h"
#include <inform/relative_entropy.h>
#include <math.h>
#include <unit.h>

UNIT(RelativeEntropyNULLSeries)
{
    inform_error err = INFORM_SUCCESS;
    ASSERT_TRUE(isnan(inform_relative_entropy(NULL, NULL, 3, 2, 2, &err)));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_ETIMESERIES, err);

    err = INFORM_SUCCESS;
    ASSERT_TRUE(isnan(inform_relative_entropy((int[]){0,0,1}, NULL, 3, 2, 2, &err)));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_ETIMESERIES, err);
}

UNIT(RelativeEntropySeriesTooShort)
{
    int const xs[] = {1,1,0,0,1,0,0,1};
    inform_error err = INFORM_SUCCESS;
    ASSERT_TRUE(isnan(inform_relative_entropy(xs, xs, 0, 2, 2, &err)));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_ESHORTSERIES, err);
}

UNIT(RelativeEntropyInvalidBase)
{
    int const xs[] = {1,1,0,0,1,0,0,1};
    inform_error err = INFORM_SUCCESS;
    for (int i = 0; i < 2; ++i)
    {
        err = INFORM_SUCCESS;
        ASSERT_TRUE(isnan(inform_relative_entropy(xs, xs, 8, i, 2, &err)));
        ASSERT_TRUE(inform_failed(&err));
        ASSERT_EQUAL(INFORM_EBASE, err);
    }
}

UNIT(RelativeEntropyNegativeState)
{
    int const xs[] = {1,1,0,0,-1,0,0,1};
    int const ys[] = {1,1,0,0, 1,0,0,1};
    inform_error err = INFORM_SUCCESS;
    ASSERT_TRUE(isnan(inform_relative_entropy(xs, ys, 8, 2, 2, &err)));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_ENEGSTATE, err);

    err = INFORM_SUCCESS;
    ASSERT_TRUE(isnan(inform_relative_entropy(ys, xs, 8, 2, 2, &err)));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_ENEGSTATE, err);
}

UNIT(RelativeEntropyBadState)
{
    int const xs[] = {1,2,0,0,1,0,0,1};
    int const ys[] = {1,1,0,0,1,0,0,1};
    inform_error err = INFORM_SUCCESS;
    ASSERT_TRUE(isnan(inform_relative_entropy(xs, ys, 8, 2, 2, &err)));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_EBADSTATE, err);

    err = INFORM_SUCCESS;
    ASSERT_TRUE(isnan(inform_relative_entropy(xs, ys, 8, 2, 2, &err)));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_EBADSTATE, err);
}

UNIT(RelativeEntropy)
{
    inform_error err = INFORM_SUCCESS;

    ASSERT_DBL_NEAR_TOL(0.038330, inform_relative_entropy((int[]){0,0,1,1,1,1,0,0,0},
        (int[]){1,0,0,1,0,0,1,0,0}, 9, 2, 2, &err), 1e-6); // == 0.038330
    ASSERT_EQUAL(INFORM_SUCCESS, err);

    ASSERT_DBL_NEAR_TOL(0.037010, inform_relative_entropy((int[]){1,0,0,1,0,0,1,0,0},
        (int[]){0,0,1,1,1,1,0,0,0}, 9, 2, 2, &err), 1e-6); // == 0.037010
    ASSERT_EQUAL(INFORM_SUCCESS, err);

    ASSERT_DBL_NEAR_TOL(0.000000, inform_relative_entropy((int[]){0,0,0,0,1,1,1,1},
        (int[]){1,1,1,1,0,0,0,0}, 8, 2, 2, &err), 1e-6);
    ASSERT_EQUAL(INFORM_SUCCESS, err);

    ASSERT_DBL_NEAR_TOL(0.035770, inform_relative_entropy((int[]){0,0,1,1,1,1,0,0,0},
        (int[]){1,1,0,0,0,0,1,1,1}, 9, 2, 2, &err), 1e-6);
    ASSERT_EQUAL(INFORM_SUCCESS, err);

    ASSERT_DBL_NEAR_TOL(0.037010, inform_relative_entropy((int[]){1,1,0,1,0,1,1,1,0},
        (int[]){1,1,0,0,0,1,0,1,1}, 9, 2, 2, &err), 1e-6);
    ASSERT_EQUAL(INFORM_SUCCESS, err);

    ASSERT_DBL_NEAR_TOL(1.584963, inform_relative_entropy((int[]){0,0,0,0,0,0,0,0,0},
        (int[]){1,1,1,0,0,0,1,1,1}, 9, 2, 2, &err), 1e-6);
    ASSERT_EQUAL(INFORM_SUCCESS, err);

    ASSERT_DBL_NEAR_TOL(0.038331, inform_relative_entropy((int[]){1,1,1,1,0,0,0,0,1},
        (int[]){1,1,1,0,0,0,1,1,1}, 9, 2, 2, &err), 1e-6);
    ASSERT_EQUAL(INFORM_SUCCESS, err);

    ASSERT_DBL_NEAR_TOL(0.038331, inform_relative_entropy((int[]){1,1,0,0,1,1,0,0,1},
        (int[]){1,1,1,0,0,0,1,1,1}, 9, 2, 2, &err), 1e-6);
    ASSERT_EQUAL(INFORM_SUCCESS, err);

    ASSERT_TRUE(isnan(inform_relative_entropy((int[]){0,1,0,1,0,1,0,1},
        (int[]){0,2,0,2,0,2,0,2}, 8, 3, 2, &err)));
    ASSERT_EQUAL(INFORM_SUCCESS, err);

    ASSERT_DBL_NEAR_TOL(0.584963,
        inform_relative_entropy((int[]){0,0,0,0,0,0,1,1,1,1,1,1},
            (int[]){0,0,0,0,1,1,1,1,2,2,2,2}, 12, 3, 2, &err), 1e-6);
    ASSERT_EQUAL(INFORM_SUCCESS, err);

    ASSERT_TRUE(isnan(inform_relative_entropy((int[]){0,0,1,1,2,1,1,0,0},
        (int[]){0,0,0,1,1,1,0,0,0}, 9, 3, 2, &err)));
    ASSERT_EQUAL(INFORM_SUCCESS, err);

    ASSERT_DBL_NEAR_TOL(0.000000, inform_relative_entropy((int[]){0,1,0,0,1,0,0,1,0},
        (int[]){1,0,0,1,0,0,1,0,0}, 9, 2, 2, &err), 1e-6);
    ASSERT_EQUAL(INFORM_SUCCESS, err);

    ASSERT_DBL_NEAR_TOL(0.679964, inform_relative_entropy((int[]){1,0,0,1,0,0,1,0},
        (int[]){2,0,1,2,0,1,2,0}, 8, 3, 2, &err), 1e-6);
    ASSERT_EQUAL(INFORM_SUCCESS, err);
}

BEGIN_SUITE(RelativeEntropy)
    ADD_UNIT(RelativeEntropyNULLSeries)
    ADD_UNIT(RelativeEntropySeriesTooShort)
    ADD_UNIT(RelativeEntropyInvalidBase)
    ADD_UNIT(RelativeEntropyNegativeState)
    ADD_UNIT(RelativeEntropyBadState)
    ADD_UNIT(RelativeEntropy)
END_SUITE
