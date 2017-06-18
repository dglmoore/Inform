// Copyright 2016-2017 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include "util.h"
#include <inform/mutual_info.h>
#include <math.h>
#include <unit.h>

UNIT(MutualInfoNULLSeries)
{
    inform_error err = INFORM_SUCCESS;
    ASSERT_TRUE(isnan(inform_mutual_info(NULL, NULL, 3, 2, 2, &err)));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_ETIMESERIES, err);

    err = INFORM_SUCCESS;
    ASSERT_TRUE(isnan(inform_mutual_info((int[]){0,0,1}, NULL, 3, 2, 2, &err)));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_ETIMESERIES, err);
}

UNIT(MutualInfoSeriesTooShort)
{
    int const xs[] = {1,1,0,0,1,0,0,1};
    inform_error err = INFORM_SUCCESS;
    ASSERT_TRUE(isnan(inform_mutual_info(xs, xs, 0, 2, 2, &err)));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_ESHORTSERIES, err);
}

UNIT(MutualInfoInvalidBase)
{
    int const xs[] = {1,1,0,0,1,0,0,1};
    inform_error err = INFORM_SUCCESS;
    for (int i = 0; i < 2; ++i)
    {
        err = INFORM_SUCCESS;
        ASSERT_TRUE(isnan(inform_mutual_info(xs, xs, 8, i, 2, &err)));
        ASSERT_TRUE(inform_failed(&err));
        ASSERT_EQUAL(INFORM_EBASE, err);

        err = INFORM_SUCCESS;
        ASSERT_TRUE(isnan(inform_mutual_info(xs, xs, 8, 2, i, &err)));
        ASSERT_TRUE(inform_failed(&err));
        ASSERT_EQUAL(INFORM_EBASE, err);
    }
}

UNIT(MutualInfoNegativeState)
{
    int const xs[] = {1,1,0,0,-1,0,0,1};
    int const ys[] = {1,1,0,0, 1,0,0,1};
    inform_error err = INFORM_SUCCESS;
    ASSERT_TRUE(isnan(inform_mutual_info(xs, ys, 8, 2, 2, &err)));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_ENEGSTATE, err);

    err = INFORM_SUCCESS;
    ASSERT_TRUE(isnan(inform_mutual_info(ys, xs, 8, 2, 2, &err)));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_ENEGSTATE, err);
}

UNIT(MutualInfoBadState)
{
    int const xs[] = {1,2,0,0,1,0,0,1};
    int const ys[] = {1,1,0,0,1,0,0,1};
    inform_error err = INFORM_SUCCESS;
    ASSERT_TRUE(isnan(inform_mutual_info(xs, ys, 8, 2, 2, &err)));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_EBADSTATE, err);

    err = INFORM_SUCCESS;
    ASSERT_TRUE(isnan(inform_mutual_info(xs, ys, 8, 2, 2, &err)));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_EBADSTATE, err);
}

UNIT(MutualInfo)
{
    inform_error err = INFORM_SUCCESS;
    ASSERT_DBL_NEAR_TOL(1.000000, inform_mutual_info((int[]){0,0,0,0,1,1,1,1},
        (int[]){1,1,1,1,0,0,0,0}, 8, 2, 2, &err), 1e-6);
    ASSERT_EQUAL(INFORM_SUCCESS, err);

    ASSERT_DBL_NEAR_TOL(0.991076, inform_mutual_info((int[]){0,0,1,1,1,1,0,0,0},
        (int[]){1,1,0,0,0,0,1,1,1}, 9, 2, 2, &err), 1e-6);
    ASSERT_EQUAL(INFORM_SUCCESS, err);

    ASSERT_DBL_NEAR_TOL(0.072780, inform_mutual_info((int[]){1,1,0,1,0,1,1,1,0},
        (int[]){1,1,0,0,0,1,0,1,1}, 9, 2, 2, &err), 1e-6);
    ASSERT_EQUAL(INFORM_SUCCESS, err);

    ASSERT_DBL_NEAR_TOL(0.000000, inform_mutual_info((int[]){0,0,0,0,0,0,0,0,0},
        (int[]){1,1,1,0,0,0,1,1,1}, 9, 2, 2, &err), 1e-6);
    ASSERT_EQUAL(INFORM_SUCCESS, err);

    ASSERT_DBL_NEAR_TOL(0.072780, inform_mutual_info((int[]){1,1,1,1,0,0,0,0,1},
        (int[]){1,1,1,0,0,0,1,1,1}, 9, 2, 2, &err), 1e-6);
    ASSERT_EQUAL(INFORM_SUCCESS, err);

    ASSERT_DBL_NEAR_TOL(0.018311, inform_mutual_info((int[]){1,1,0,0,1,1,0,0,1},
        (int[]){1,1,1,0,0,0,1,1,1}, 9, 2, 2, &err), 1e-6);
    ASSERT_EQUAL(INFORM_SUCCESS, err);

    ASSERT_DBL_NEAR_TOL(1.000000, inform_mutual_info((int[]){0,1,0,1,0,1,0,1},
        (int[]){0,2,0,2,0,2,0,2}, 8, 2, 3, &err), 1e-6);
    ASSERT_EQUAL(INFORM_SUCCESS, err);

    ASSERT_DBL_NEAR_TOL(0.666667,
        inform_mutual_info((int[]){0,0,0,0,0,0,1,1,1,1,1,1},
            (int[]){0,0,0,0,1,1,1,1,2,2,2,2}, 12, 2, 3, &err), 1e-6);
    ASSERT_EQUAL(INFORM_SUCCESS, err);

    ASSERT_DBL_NEAR_TOL(0.473851, inform_mutual_info((int[]){0,0,1,1,2,1,1,0,0},
        (int[]){0,0,0,1,1,1,0,0,0}, 9, 3, 2, &err), 1e-6);
    ASSERT_EQUAL(INFORM_SUCCESS, err);

    ASSERT_DBL_NEAR_TOL(0.251629, inform_mutual_info((int[]){0,1,0,0,1,0,0,1,0},
        (int[]){1,0,0,1,0,0,1,0,0}, 9, 2, 2, &err), 1e-6);
    ASSERT_EQUAL(INFORM_SUCCESS, err);

    ASSERT_DBL_NEAR_TOL(0.954434, inform_mutual_info((int[]){1,0,0,1,0,0,1,0},
        (int[]){2,0,1,2,0,1,2,0}, 8, 2, 3, &err), 1e-6);
    ASSERT_EQUAL(INFORM_SUCCESS, err);
}

UNIT(LocalMutualInfoNULLSeries)
{
    double mi[8];
    inform_error err = INFORM_SUCCESS;
    ASSERT_NULL(inform_local_mutual_info(NULL, NULL, 3, 2, 2, mi, &err));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_ETIMESERIES, err);

    err = INFORM_SUCCESS;
    ASSERT_NULL(inform_local_mutual_info((int[]){0,0,1}, NULL, 3, 2, 2, mi, &err));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_ETIMESERIES, err);
}

UNIT(LocalMutualInfoSeriesTooShort)
{
    double mi[8];
    int const xs[] = {1,1,0,0,1,0,0,1};
    inform_error err = INFORM_SUCCESS;
    ASSERT_NULL(inform_local_mutual_info(xs, xs, 0, 2, 2, mi, &err));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_ESHORTSERIES, err);
}

UNIT(LocalMutualInfoInvalidBase)
{
    double mi[8];
    int const xs[] = {1,1,0,0,1,0,0,1};
    inform_error err = INFORM_SUCCESS;
    for (int i = 0; i < 2; ++i)
    {
        err = INFORM_SUCCESS;
        ASSERT_NULL(inform_local_mutual_info(xs, xs, 8, i, 2, mi, &err));
        ASSERT_TRUE(inform_failed(&err));
        ASSERT_EQUAL(INFORM_EBASE, err);

        err = INFORM_SUCCESS;
        ASSERT_NULL(inform_local_mutual_info(xs, xs, 8, 2, i, mi, &err));
        ASSERT_TRUE(inform_failed(&err));
        ASSERT_EQUAL(INFORM_EBASE, err);
    }
}

UNIT(LocalMutualInfoNegativeState)
{
    double mi[8];
    int const xs[] = {1,1,0,0,-1,0,0,1};
    int const ys[] = {1,1,0,0, 1,0,0,1};
    inform_error err = INFORM_SUCCESS;
    ASSERT_NULL(inform_local_mutual_info(xs, ys, 8, 2, 2, mi, &err));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_ENEGSTATE, err);

    err = INFORM_SUCCESS;
    ASSERT_NULL(inform_local_mutual_info(ys, xs, 8, 2, 2, mi, &err));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_ENEGSTATE, err);
}

UNIT(LocalMutualInfoBadState)
{
    double mi[8];
    int const xs[] = {1,2,0,0,1,0,0,1};
    int const ys[] = {1,1,0,0,1,0,0,1};
    inform_error err = INFORM_SUCCESS;
    ASSERT_NULL(inform_local_mutual_info(xs, ys, 8, 2, 2, mi, &err));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_EBADSTATE, err);

    err = INFORM_SUCCESS;
    ASSERT_NULL(inform_local_mutual_info(xs, ys, 8, 2, 2, mi, &err));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_EBADSTATE, err);
}

UNIT(LocalMutualInfoAllocatesOutput)
{
    int const xs[] = {0,0,1,1,1,1,0,0,0};
    int const ys[] = {1,1,0,0,0,0,1,1,1};
    inform_error err = INFORM_SUCCESS;
    double *mi = inform_local_mutual_info(xs, ys, 9, 2, 2, NULL, &err);
    ASSERT_NOT_NULL(mi);
    ASSERT_EQUAL(INFORM_SUCCESS, err);
    free(mi);
}

UNIT(LocalMutualInfo)
{
    inform_error err = INFORM_SUCCESS;
    double mi_8[8];
    double mi_9[9];
    double mi_12[12];

    inform_local_mutual_info((int[]){0,0,0,0,1,1,1,1}, (int[]){1,1,1,1,0,0,0,0}, 8, 2, 2, mi_8, &err);
    ASSERT_DBL_NEAR_TOL(1.000000, AVERAGE(mi_8), 1e-6);
    ASSERT_EQUAL(INFORM_SUCCESS, err);

    inform_local_mutual_info((int[]){0,0,1,1,1,1,0,0,0}, (int[]){1,1,0,0,0,0,1,1,1}, 9, 2, 2, mi_9, &err);
    ASSERT_DBL_NEAR_TOL(0.991076, AVERAGE(mi_9), 1e-6);
    ASSERT_EQUAL(INFORM_SUCCESS, err);

    inform_local_mutual_info((int[]){1,1,0,1,0,1,1,1,0}, (int[]){1,1,0,0,0,1,0,1,1}, 9, 2, 2, mi_9, &err);
    ASSERT_DBL_NEAR_TOL(0.072780, AVERAGE(mi_9), 1e-6);
    ASSERT_EQUAL(INFORM_SUCCESS, err);

    inform_local_mutual_info((int[]){0,0,0,0,0,0,0,0,0}, (int[]){1,1,1,0,0,0,1,1,1}, 9, 2, 2, mi_9, &err);
    ASSERT_DBL_NEAR_TOL(0.000000, AVERAGE(mi_9), 1e-6);
    ASSERT_EQUAL(INFORM_SUCCESS, err);

    inform_local_mutual_info((int[]){1,1,1,1,0,0,0,0,1}, (int[]){1,1,1,0,0,0,1,1,1}, 9, 2, 2, mi_9, &err);
    ASSERT_DBL_NEAR_TOL(0.072780, AVERAGE(mi_9), 1e-6);
    ASSERT_EQUAL(INFORM_SUCCESS, err);

    inform_local_mutual_info((int[]){1,1,0,0,1,1,0,0,1}, (int[]){1,1,1,0,0,0,1,1,1}, 9, 2, 2, mi_9, &err);
    ASSERT_DBL_NEAR_TOL(0.018311, AVERAGE(mi_9), 1e-6);
    ASSERT_EQUAL(INFORM_SUCCESS, err);

    inform_local_mutual_info((int[]){0,1,0,1,0,1,0,1}, (int[]){0,2,0,2,0,2,0,2}, 8, 2, 3, mi_8, &err);
    ASSERT_DBL_NEAR_TOL(1.000000, AVERAGE(mi_8), 1e-6);
    ASSERT_EQUAL(INFORM_SUCCESS, err);

    inform_local_mutual_info((int[]){0,0,0,0,0,0,1,1,1,1,1,1}, (int[]){0,0,0,0,1,1,1,1,2,2,2,2}, 12, 2, 3, mi_12, &err);
    ASSERT_DBL_NEAR_TOL(0.666667, AVERAGE(mi_12), 1e-6);
    ASSERT_EQUAL(INFORM_SUCCESS, err);

    inform_local_mutual_info((int[]){0,0,1,1,2,1,1,0,0}, (int[]){0,0,0,1,1,1,0,0,0}, 9, 3, 2, mi_9, &err);
    ASSERT_DBL_NEAR_TOL(0.473851, AVERAGE(mi_9), 1e-6);
    ASSERT_EQUAL(INFORM_SUCCESS, err);
    
    inform_local_mutual_info((int[]){0,1,0,0,1,0,0,1,0}, (int[]){1,0,0,1,0,0,1,0,0}, 9, 2, 2, mi_9, &err);
    ASSERT_DBL_NEAR_TOL(0.251629, AVERAGE(mi_9), 1e-6);
    ASSERT_EQUAL(INFORM_SUCCESS, err);

    inform_local_mutual_info((int[]){1,0,0,1,0,0,1,0}, (int[]){2,0,1,2,0,1,2,0}, 8, 2, 3, mi_8, &err);
    ASSERT_DBL_NEAR_TOL(0.954434, AVERAGE(mi_8), 1e-6);
    ASSERT_EQUAL(INFORM_SUCCESS, err);
}

BEGIN_SUITE(MutualInfo)
    ADD_UNIT(MutualInfoNULLSeries)
    ADD_UNIT(MutualInfoSeriesTooShort)
    ADD_UNIT(MutualInfoInvalidBase)
    ADD_UNIT(MutualInfoNegativeState)
    ADD_UNIT(MutualInfoBadState)
    ADD_UNIT(MutualInfo)
    ADD_UNIT(LocalMutualInfoNULLSeries)
    ADD_UNIT(LocalMutualInfoSeriesTooShort)
    ADD_UNIT(LocalMutualInfoInvalidBase)
    ADD_UNIT(LocalMutualInfoNegativeState)
    ADD_UNIT(LocalMutualInfoBadState)
    ADD_UNIT(LocalMutualInfoAllocatesOutput)
    ADD_UNIT(LocalMutualInfo)
END_SUITE
