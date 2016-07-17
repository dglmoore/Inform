// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <inform/state_encoding.h>
#include <inform/transfer_entropy.h>
#include <math.h>
#include <unit.h>
#include "random.h"

UNIT(TransferEntropyTooShort)
{
    int const series[] = {1,1,1,0,0,1,1,0,0,1};
    inform_error err;
    inform_error *errptr = &err;

    *errptr = INFORM_ERROR_SUCCESS;
    ASSERT_FALSE(INFORM_IS_FAILURE(errptr));
    ASSERT_TRUE(isnan(inform_transfer_entropy(series, series+5, 1, 0, 2, 2, errptr)));
    ASSERT_TRUE(INFORM_IS_FAILURE(errptr));

    *errptr = INFORM_ERROR_SUCCESS;
    ASSERT_FALSE(INFORM_IS_FAILURE(errptr));
    ASSERT_TRUE(isnan(inform_transfer_entropy(series, series+5, 1, 1, 2, 2, errptr)));
    ASSERT_TRUE(INFORM_IS_FAILURE(errptr));
}

UNIT(TransferEntropyHistoryTooLong)
{
    {
        int const series[] = {1,1,1,0,0,1,1,0,0,1};
        inform_error err;
        inform_error *errptr = &err;

        *errptr = INFORM_ERROR_SUCCESS;
        ASSERT_FALSE(INFORM_IS_FAILURE(errptr));
        ASSERT_TRUE(isnan(inform_transfer_entropy(series, series+5, 1, 2, 2, 2, errptr)));
        ASSERT_TRUE(INFORM_IS_FAILURE(errptr));

        *errptr = INFORM_ERROR_SUCCESS;
        ASSERT_FALSE(INFORM_IS_FAILURE(errptr));
        ASSERT_FALSE(isnan(inform_transfer_entropy(series, series+5, 1, 3, 2, 2, errptr)));
        ASSERT_FALSE(INFORM_IS_FAILURE(errptr));
    }
}

UNIT(TransferEntropyEncodingError)
{
    inform_error err;
    inform_error *errptr = &err;

    {
        int const series[10] = {2,1,1,0,1,0,0,1,0,1};

        *errptr = INFORM_ERROR_SUCCESS;
        ASSERT_FALSE(INFORM_IS_FAILURE(errptr));
        ASSERT_FALSE(isnan(inform_transfer_entropy(series+5, series, 1, 5, 3, 2, errptr)));
        ASSERT_FALSE(INFORM_IS_FAILURE(errptr));

        *errptr = INFORM_ERROR_SUCCESS;
        ASSERT_FALSE(INFORM_IS_FAILURE(errptr));
        ASSERT_TRUE(isnan(inform_transfer_entropy(series+5, series, 1, 5, 2, 2, errptr)));
        ASSERT_TRUE(INFORM_IS_FAILURE(errptr));

        *errptr = INFORM_ERROR_SUCCESS;
        ASSERT_FALSE(INFORM_IS_FAILURE(errptr));
        ASSERT_FALSE(isnan(inform_transfer_entropy(series, series+5, 1, 5, 3, 2, errptr)));
        ASSERT_FALSE(INFORM_IS_FAILURE(errptr));

        *errptr = INFORM_ERROR_SUCCESS;
        ASSERT_FALSE(INFORM_IS_FAILURE(errptr));
        ASSERT_TRUE(isnan(inform_transfer_entropy(series, series+5, 1, 5, 2, 2, errptr)));
        ASSERT_TRUE(INFORM_IS_FAILURE(errptr));
    }

    {
        int const series[] = {1,1,1,0,2,0,0,1,0,1};

        *errptr = INFORM_ERROR_SUCCESS;
        ASSERT_FALSE(INFORM_IS_FAILURE(errptr));
        ASSERT_FALSE(isnan(inform_transfer_entropy(series+5, series, 1, 5, 3, 2, errptr)));
        ASSERT_FALSE(INFORM_IS_FAILURE(errptr));

        *errptr = INFORM_ERROR_SUCCESS;
        ASSERT_FALSE(INFORM_IS_FAILURE(errptr));
        ASSERT_TRUE(isnan(inform_transfer_entropy(series+5, series, 1, 5, 2, 2, errptr)));
        ASSERT_TRUE(INFORM_IS_FAILURE(errptr));

        *errptr = INFORM_ERROR_SUCCESS;
        ASSERT_FALSE(INFORM_IS_FAILURE(errptr));
        ASSERT_FALSE(isnan(inform_transfer_entropy(series, series+5, 1, 5, 3, 2, errptr)));
        ASSERT_FALSE(INFORM_IS_FAILURE(errptr));

        *errptr = INFORM_ERROR_SUCCESS;
        ASSERT_FALSE(INFORM_IS_FAILURE(errptr));
        ASSERT_TRUE(isnan(inform_transfer_entropy(series, series+5, 1, 5, 2, 2, errptr)));
        ASSERT_TRUE(INFORM_IS_FAILURE(errptr));
    }

    {
        int const series[] = {1,1,1,2,1,0,0,1,0,1};

        *errptr = INFORM_ERROR_SUCCESS;
        ASSERT_FALSE(INFORM_IS_FAILURE(errptr));
        ASSERT_FALSE(isnan(inform_transfer_entropy(series+5, series, 1, 5, 3, 2, errptr)));
        ASSERT_FALSE(INFORM_IS_FAILURE(errptr));

        *errptr = INFORM_ERROR_SUCCESS;
        ASSERT_FALSE(INFORM_IS_FAILURE(errptr));
        ASSERT_TRUE(isnan(inform_transfer_entropy(series+5, series, 1, 5, 2, 2, errptr)));
        ASSERT_TRUE(INFORM_IS_FAILURE(errptr));

        *errptr = INFORM_ERROR_SUCCESS;
        ASSERT_FALSE(INFORM_IS_FAILURE(errptr));
        ASSERT_FALSE(isnan(inform_transfer_entropy(series, series+5, 1, 5, 3, 2, errptr)));
        ASSERT_FALSE(INFORM_IS_FAILURE(errptr));

        *errptr = INFORM_ERROR_SUCCESS;
        ASSERT_FALSE(INFORM_IS_FAILURE(errptr));
        ASSERT_TRUE(isnan(inform_transfer_entropy(series, series+5, 1, 5, 2, 2, errptr)));
        ASSERT_TRUE(INFORM_IS_FAILURE(errptr));
    }
}


UNIT(TransferEntropySingleSeries_Base2)
{
    {
        int series[10] = {
                1,1,1,0,0,
                1,1,0,0,1
        };
        ASSERT_DBL_NEAR_TOL(0.000000,
                inform_transfer_entropy(series,   series,   1, 5, 2, 2, NULL), 1e-6);
        ASSERT_DBL_NEAR_TOL(0.666666,
                inform_transfer_entropy(series+5, series,   1, 5, 2, 2, NULL), 1e-6);
        ASSERT_DBL_NEAR_TOL(0.000000,
                inform_transfer_entropy(series,   series+5, 1, 5, 2, 2, NULL), 1e-6);
        ASSERT_DBL_NEAR_TOL(0.000000,
                inform_transfer_entropy(series+5, series+5, 1, 5, 2, 2, NULL), 1e-6);
    }

    {
        int series[20] = {
                0,0,1,1,1,0,0,0,0,1,
                1,1,0,0,0,0,0,0,1,1
        };
        ASSERT_DBL_NEAR_TOL(0.000000,
                inform_transfer_entropy(series,    series,    1, 10, 2, 2, NULL), 1e-6);
        ASSERT_DBL_NEAR_TOL(0.500000,
                inform_transfer_entropy(series+10, series,    1, 10, 2, 2, NULL), 1e-6);
        ASSERT_DBL_NEAR_TOL(0.106844,
                inform_transfer_entropy(series,    series+10, 1, 10, 2, 2, NULL), 1e-6);
        ASSERT_DBL_NEAR_TOL(0.000000,
                inform_transfer_entropy(series+10, series+10, 1, 10, 2, 2, NULL), 1e-6);
    }

    {
        int series[20] = {
                0,1,0,1,0,0,1,1,0,0,
                0,0,1,0,1,1,1,0,1,1
        };
        ASSERT_DBL_NEAR_TOL(0.000000,
                inform_transfer_entropy(series,    series,    1, 10, 2, 2, NULL), 1e-6);
        ASSERT_DBL_NEAR_TOL(0.344361,
                inform_transfer_entropy(series+10, series,    1, 10, 2, 2, NULL), 1e-6);
        ASSERT_DBL_NEAR_TOL(0.250000,
                inform_transfer_entropy(series,    series+10, 1, 10, 2, 2, NULL), 1e-6);
        ASSERT_DBL_NEAR_TOL(0.000000,
                inform_transfer_entropy(series+10, series+10, 1, 10, 2, 2, NULL), 1e-6);
    }
}

UNIT(TransferEntropyEnsemble_Base2)
{
    {
        int xseries[50] = {
            1, 1, 1, 0, 0, 1, 1, 0, 1, 0,
            0, 1, 0, 1, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0, 1, 0, 0,
            0, 0, 1, 0, 0, 0, 1, 0, 0, 1,
            0, 0, 1, 1, 1, 1, 1, 0, 0, 0,
        };
        int yseries[50] = {
            0, 1, 0, 0, 0, 1, 0, 1, 1, 0,
            0, 0, 0, 1, 1, 1, 0, 1, 0, 0,
            1, 0, 1, 0, 1, 0, 0, 0, 1, 0,
            0, 1, 1, 0, 1, 1, 1, 1, 1, 1,
            0, 0, 1, 1, 0, 0, 0, 0, 0, 1,
        };

        ASSERT_DBL_NEAR_TOL(0.000000,
                inform_transfer_entropy(xseries, xseries, 5, 10, 2, 2, NULL), 1e-6);
        ASSERT_DBL_NEAR_TOL(0.091141,
                inform_transfer_entropy(yseries, xseries, 5, 10, 2, 2, NULL), 1e-6);
        ASSERT_DBL_NEAR_TOL(0.107630,
                inform_transfer_entropy(xseries, yseries, 5, 10, 2, 2, NULL), 1e-6);
        ASSERT_DBL_NEAR_TOL(0.000000,
                inform_transfer_entropy(yseries, yseries, 5, 10, 2, 2, NULL), 1e-6);

        ASSERT_DBL_NEAR_TOL(0.000000,
                inform_transfer_entropy(xseries, xseries, 4, 10, 2, 2, NULL), 1e-6);
        ASSERT_DBL_NEAR_TOL(0.134536,
                inform_transfer_entropy(yseries, xseries, 4, 10, 2, 2, NULL), 1e-6);
        ASSERT_DBL_NEAR_TOL(0.089517,
                inform_transfer_entropy(xseries, yseries, 4, 10, 2, 2, NULL), 1e-6);
        ASSERT_DBL_NEAR_TOL(0.000000,
                inform_transfer_entropy(yseries, yseries, 4, 10, 2, 2, NULL), 1e-6);
    }
    {
        int xseries[50] = {
            0, 1, 0, 1, 0, 0, 1, 1, 1, 1,
            0, 1, 0, 1, 1, 1, 0, 0, 1, 0,
            1, 1, 1, 1, 0, 0, 1, 1, 1, 1,
            1, 0, 0, 0, 0, 0, 0, 0, 1, 0,
            0, 1, 1, 1, 1, 1, 0, 1, 1, 1,
        };
        int yseries[50] = {
            1, 1, 1, 1, 1, 0, 0, 0, 1, 0,
            0, 1, 1, 0, 0, 1, 1, 0, 0, 0,
            0, 1, 1, 1, 0, 1, 0, 0, 0, 0,
            0, 1, 0, 0, 1, 1, 0, 1, 0, 0,
            0, 1, 1, 1, 1, 0, 1, 1, 1, 1,
        };

        ASSERT_DBL_NEAR_TOL(0.000000,
                inform_transfer_entropy(xseries, xseries, 5, 10, 2, 2, NULL), 1e-6);
        ASSERT_DBL_NEAR_TOL(0.031471,
                inform_transfer_entropy(yseries, xseries, 5, 10, 2, 2, NULL), 1e-6);
        ASSERT_DBL_NEAR_TOL(0.152561,
                inform_transfer_entropy(xseries, yseries, 5, 10, 2, 2, NULL), 1e-6);
        ASSERT_DBL_NEAR_TOL(0.000000,
                inform_transfer_entropy(yseries, yseries, 5, 10, 2, 2, NULL), 1e-6);

        ASSERT_DBL_NEAR_TOL(0.000000,
                inform_transfer_entropy(xseries, xseries, 4, 10, 2, 2, NULL), 1e-6);
        ASSERT_DBL_NEAR_TOL(0.172618,
                inform_transfer_entropy(yseries, xseries, 4, 10, 2, 2, NULL), 1e-6);
        ASSERT_DBL_NEAR_TOL(0.206156,
                inform_transfer_entropy(xseries, yseries, 4, 10, 2, 2, NULL), 1e-6);
        ASSERT_DBL_NEAR_TOL(0.000000,
                inform_transfer_entropy(yseries, yseries, 4, 10, 2, 2, NULL), 1e-6);
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

UNIT(LocalTransferEntropyTooShort)
{
    double te[8];
    int const series[] = {1,1,1,0,0,1,1,0,0,1};
    inform_error err;
    inform_error *errptr = &err;

    *errptr = INFORM_ERROR_SUCCESS;
    ASSERT_FALSE(INFORM_IS_FAILURE(errptr));
    ASSERT_NULL(inform_local_transfer_entropy(series, series+5, 1, 0, 2, 2, te, errptr));
    ASSERT_TRUE(INFORM_IS_FAILURE(errptr));

    *errptr = INFORM_ERROR_SUCCESS;
    ASSERT_FALSE(INFORM_IS_FAILURE(errptr));
    ASSERT_NULL(inform_local_transfer_entropy(series, series+5, 1, 1, 2, 2, te, errptr));
    ASSERT_TRUE(INFORM_IS_FAILURE(errptr));
}

UNIT(LocalTransferEntropyHistoryTooLong)
{
    double te[8];
    int const series[] = {1,1,1,0,0,1,1,0,0,1};
    inform_error err;
    inform_error *errptr = &err;

    *errptr = INFORM_ERROR_SUCCESS;
    ASSERT_FALSE(INFORM_IS_FAILURE(errptr));
    ASSERT_NULL(inform_local_transfer_entropy(series, series+5, 1, 2, 2, 2, te, errptr));
    ASSERT_TRUE(INFORM_IS_FAILURE(errptr));

    *errptr = INFORM_ERROR_SUCCESS;
    ASSERT_FALSE(INFORM_IS_FAILURE(errptr));
    ASSERT_NOT_NULL(inform_local_transfer_entropy(series, series+5, 1, 3, 2, 2, te, errptr));
    ASSERT_FALSE(INFORM_IS_FAILURE(errptr));
}

UNIT(LocalTransferEntropyEncodingError)
{
    inform_error err;
    inform_error *errptr = &err;

    {
        double te[8];
        int const series[10] = {2,1,1,0,1,0,0,1,0,1};

        *errptr = INFORM_ERROR_SUCCESS;
        ASSERT_FALSE(INFORM_IS_FAILURE(errptr));
        ASSERT_NOT_NULL(inform_local_transfer_entropy(series+5, series, 1, 5, 3, 2, te, errptr));
        ASSERT_FALSE(INFORM_IS_FAILURE(errptr));

        *errptr = INFORM_ERROR_SUCCESS;
        ASSERT_FALSE(INFORM_IS_FAILURE(errptr));
        ASSERT_NULL(inform_local_transfer_entropy(series+5, series, 1, 5, 2, 2, te, errptr));
        ASSERT_TRUE(INFORM_IS_FAILURE(errptr));

        *errptr = INFORM_ERROR_SUCCESS;
        ASSERT_FALSE(INFORM_IS_FAILURE(errptr));
        ASSERT_NOT_NULL(inform_local_transfer_entropy(series, series+5, 1, 5, 3, 2, te, errptr));
        ASSERT_FALSE(INFORM_IS_FAILURE(errptr));

        *errptr = INFORM_ERROR_SUCCESS;
        ASSERT_FALSE(INFORM_IS_FAILURE(errptr));
        ASSERT_NULL(inform_local_transfer_entropy(series, series+5, 1, 5, 2, 2, te, errptr));
        ASSERT_TRUE(INFORM_IS_FAILURE(errptr));
    }

    {
        double te[8];
        int const series[] = {1,1,1,0,2,0,0,1,0,1};

        *errptr = INFORM_ERROR_SUCCESS;
        ASSERT_FALSE(INFORM_IS_FAILURE(errptr));
        ASSERT_NOT_NULL(inform_local_transfer_entropy(series+5, series, 1, 5, 3, 2, te, errptr));
        ASSERT_FALSE(INFORM_IS_FAILURE(errptr));

        *errptr = INFORM_ERROR_SUCCESS;
        ASSERT_FALSE(INFORM_IS_FAILURE(errptr));
        ASSERT_NULL(inform_local_transfer_entropy(series+5, series, 1, 5, 2, 2, te, errptr));
        ASSERT_TRUE(INFORM_IS_FAILURE(errptr));

        *errptr = INFORM_ERROR_SUCCESS;
        ASSERT_FALSE(INFORM_IS_FAILURE(errptr));
        ASSERT_NOT_NULL(inform_local_transfer_entropy(series, series+5, 1, 5, 3, 2, te, errptr));
        ASSERT_FALSE(INFORM_IS_FAILURE(errptr));

        *errptr = INFORM_ERROR_SUCCESS;
        ASSERT_FALSE(INFORM_IS_FAILURE(errptr));
        ASSERT_NULL(inform_local_transfer_entropy(series, series+5, 1, 5, 2, 2, te, errptr));
        ASSERT_TRUE(INFORM_IS_FAILURE(errptr));
    }

    {
        double te[8];
        int const series[] = {1,1,1,2,1,0,0,1,0,1};

        *errptr = INFORM_ERROR_SUCCESS;
        ASSERT_FALSE(INFORM_IS_FAILURE(errptr));
        ASSERT_NOT_NULL(inform_local_transfer_entropy(series+5, series, 1, 5, 3, 2, te, errptr));
        ASSERT_FALSE(INFORM_IS_FAILURE(errptr));

        *errptr = INFORM_ERROR_SUCCESS;
        ASSERT_FALSE(INFORM_IS_FAILURE(errptr));
        ASSERT_NULL(inform_local_transfer_entropy(series+5, series, 1, 5, 2, 2, te, errptr));
        ASSERT_TRUE(INFORM_IS_FAILURE(errptr));

        *errptr = INFORM_ERROR_SUCCESS;
        ASSERT_FALSE(INFORM_IS_FAILURE(errptr));
        ASSERT_NOT_NULL(inform_local_transfer_entropy(series, series+5, 1, 5, 3, 2, te, errptr));
        ASSERT_FALSE(INFORM_IS_FAILURE(errptr));

        *errptr = INFORM_ERROR_SUCCESS;
        ASSERT_FALSE(INFORM_IS_FAILURE(errptr));
        ASSERT_NULL(inform_local_transfer_entropy(series, series+5, 1, 5, 2, 2, te, errptr));
        ASSERT_TRUE(INFORM_IS_FAILURE(errptr));
    }
}

UNIT(LocalTransferEntropyAllocatesOutput)
{
    inform_error err = INFORM_ERROR_SUCCESS;
    int source[] = {0,0,1,0,0,1,0,1,0};
    int target[] = {0,1,0,0,1,0,0,1,0};
    double *te = inform_local_transfer_entropy(source, target, 1, 9, 2, 2, NULL, &err);
    ASSERT_NOT_NULL(te);
    ASSERT_EQUAL(INFORM_SUCCESS, err.tag);
    free(te);
}

UNIT(LocalTransferEntropySingleSeries_Base2)
{
    {
        double te[3];
        int series[10] = {
            1,1,1,0,0,
            1,1,0,0,1
        };
        ASSERT_NOT_NULL(inform_local_transfer_entropy(series, series, 1, 5, 2, 2, te, NULL));
        ASSERT_DBL_NEAR_TOL(0.000000, AVERAGE(te), 1e-6);
        ASSERT_NOT_NULL(inform_local_transfer_entropy(series+5, series, 1, 5, 2, 2, te, NULL));
        ASSERT_DBL_NEAR_TOL(0.666666, AVERAGE(te), 1e-6);
        ASSERT_NOT_NULL(inform_local_transfer_entropy(series, series+5, 1, 5, 2, 2, te, NULL));
        ASSERT_DBL_NEAR_TOL(0.000000, AVERAGE(te), 1e-6);
        ASSERT_NOT_NULL(inform_local_transfer_entropy(series+5, series+5, 1, 5, 2, 2, te, NULL));
        ASSERT_DBL_NEAR_TOL(0.000000, AVERAGE(te), 1e-6);
    }

    {
        double te[8];
        int series[20] = {
            0,0,1,1,1,0,0,0,0,1,
            1,1,0,0,0,0,0,0,1,1
        };
        ASSERT_NOT_NULL(inform_local_transfer_entropy(series, series, 1, 10, 2, 2, te, NULL));
        ASSERT_DBL_NEAR_TOL(0.000000, AVERAGE(te), 1e-6);
        ASSERT_NOT_NULL(inform_local_transfer_entropy(series+10, series, 1, 10, 2, 2, te, NULL));
        ASSERT_DBL_NEAR_TOL(0.500000, AVERAGE(te), 1e-6);
        ASSERT_NOT_NULL(inform_local_transfer_entropy(series, series+10, 1, 10, 2, 2, te, NULL));
        ASSERT_DBL_NEAR_TOL(0.106844, AVERAGE(te), 1e-6);
        ASSERT_NOT_NULL(inform_local_transfer_entropy(series+10, series+10, 1, 10, 2, 2, te, NULL));
        ASSERT_DBL_NEAR_TOL(0.000000, AVERAGE(te), 1e-6);
    }

    {
        double te[8];
        int series[20] = {
            0,1,0,1,0,0,1,1,0,0,
            0,0,1,0,1,1,1,0,1,1
        };
        ASSERT_NOT_NULL(inform_local_transfer_entropy(series, series, 1, 10, 2, 2, te, NULL));
        ASSERT_DBL_NEAR_TOL(0.000000, AVERAGE(te), 1e-6);
        ASSERT_NOT_NULL(inform_local_transfer_entropy(series+10, series, 1, 10, 2, 2, te, NULL));
        ASSERT_DBL_NEAR_TOL(0.344361, AVERAGE(te), 1e-6);
        ASSERT_NOT_NULL(inform_local_transfer_entropy(series, series+10, 1, 10, 2, 2, te, NULL));
        ASSERT_DBL_NEAR_TOL(0.250000, AVERAGE(te), 1e-6);
        ASSERT_NOT_NULL(inform_local_transfer_entropy(series+10, series+10, 1, 10, 2, 2, te, NULL));
        ASSERT_DBL_NEAR_TOL(0.000000, AVERAGE(te), 1e-6);
    }
}

UNIT(LocalTransferEntropyEnsemble_Base2)
{
    {
        double te[40];
        int xseries[50] = {
            1, 1, 1, 0, 0, 1, 1, 0, 1, 0,
            0, 1, 0, 1, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0, 1, 0, 0,
            0, 0, 1, 0, 0, 0, 1, 0, 0, 1,
            0, 0, 1, 1, 1, 1, 1, 0, 0, 0,
        };
        int yseries[50] = {
            0, 1, 0, 0, 0, 1, 0, 1, 1, 0,
            0, 0, 0, 1, 1, 1, 0, 1, 0, 0,
            1, 0, 1, 0, 1, 0, 0, 0, 1, 0,
            0, 1, 1, 0, 1, 1, 1, 1, 1, 1,
            0, 0, 1, 1, 0, 0, 0, 0, 0, 1,
        };

        ASSERT_NOT_NULL(inform_local_transfer_entropy(xseries, xseries, 5, 10, 2, 2, te, NULL));
        ASSERT_DBL_NEAR_TOL(0.000000, AVERAGE(te), 1e-6);
        ASSERT_NOT_NULL(inform_local_transfer_entropy(yseries, xseries, 5, 10, 2, 2, te, NULL));
        ASSERT_DBL_NEAR_TOL(0.091141, AVERAGE(te), 1e-6);
        ASSERT_NOT_NULL(inform_local_transfer_entropy(xseries, yseries, 5, 10, 2, 2, te, NULL));
        ASSERT_DBL_NEAR_TOL(0.107630, AVERAGE(te), 1e-6);
        ASSERT_NOT_NULL(inform_local_transfer_entropy(yseries, yseries, 5, 10, 2, 2, te, NULL));
        ASSERT_DBL_NEAR_TOL(0.000000, AVERAGE(te), 1e-6);
    }
    {
        double te[32];
        int xseries[40] = {
            1, 1, 1, 0, 0, 1, 1, 0, 1, 0,
            0, 1, 0, 1, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0, 1, 0, 0,
            0, 0, 1, 0, 0, 0, 1, 0, 0, 1,
        };
        int yseries[40] = {
            0, 1, 0, 0, 0, 1, 0, 1, 1, 0,
            0, 0, 0, 1, 1, 1, 0, 1, 0, 0,
            1, 0, 1, 0, 1, 0, 0, 0, 1, 0,
            0, 1, 1, 0, 1, 1, 1, 1, 1, 1,
        };

        ASSERT_NOT_NULL(inform_local_transfer_entropy(xseries, xseries, 4, 10, 2, 2, te, NULL));
        ASSERT_DBL_NEAR_TOL(0.000000, AVERAGE(te), 1e-6);
        ASSERT_NOT_NULL(inform_local_transfer_entropy(yseries, xseries, 4, 10, 2, 2, te, NULL));
        ASSERT_DBL_NEAR_TOL(0.134536, AVERAGE(te), 1e-6);
        ASSERT_NOT_NULL(inform_local_transfer_entropy(xseries, yseries, 4, 10, 2, 2, te, NULL));
        ASSERT_DBL_NEAR_TOL(0.089517, AVERAGE(te), 1e-6);
        ASSERT_NOT_NULL(inform_local_transfer_entropy(yseries, yseries, 4, 10, 2, 2, te, NULL));
        ASSERT_DBL_NEAR_TOL(0.000000, AVERAGE(te), 1e-6);
    }
    {
        double te[40];
        int xseries[50] = {
            0, 1, 0, 1, 0, 0, 1, 1, 1, 1,
            0, 1, 0, 1, 1, 1, 0, 0, 1, 0,
            1, 1, 1, 1, 0, 0, 1, 1, 1, 1,
            1, 0, 0, 0, 0, 0, 0, 0, 1, 0,
            0, 1, 1, 1, 1, 1, 0, 1, 1, 1,
        };
        int yseries[50] = {
            1, 1, 1, 1, 1, 0, 0, 0, 1, 0,
            0, 1, 1, 0, 0, 1, 1, 0, 0, 0,
            0, 1, 1, 1, 0, 1, 0, 0, 0, 0,
            0, 1, 0, 0, 1, 1, 0, 1, 0, 0,
            0, 1, 1, 1, 1, 0, 1, 1, 1, 1,
        };

        ASSERT_NOT_NULL(inform_local_transfer_entropy(xseries, xseries, 5, 10, 2, 2, te, NULL));
        ASSERT_DBL_NEAR_TOL(0.000000, AVERAGE(te), 1e-6);
        ASSERT_NOT_NULL(inform_local_transfer_entropy(yseries, xseries, 5, 10, 2, 2, te, NULL));
        ASSERT_DBL_NEAR_TOL(0.031471, AVERAGE(te), 1e-6);
        ASSERT_NOT_NULL(inform_local_transfer_entropy(xseries, yseries, 5, 10, 2, 2, te, NULL));
        ASSERT_DBL_NEAR_TOL(0.152561, AVERAGE(te), 1e-6);
        ASSERT_NOT_NULL(inform_local_transfer_entropy(yseries, yseries, 5, 10, 2, 2, te, NULL));
        ASSERT_DBL_NEAR_TOL(0.000000, AVERAGE(te), 1e-6);
    }
    {
        double te[32];
        int xseries[50] = {
            0, 1, 0, 1, 0, 0, 1, 1, 1, 1,
            0, 1, 0, 1, 1, 1, 0, 0, 1, 0,
            1, 1, 1, 1, 0, 0, 1, 1, 1, 1,
            1, 0, 0, 0, 0, 0, 0, 0, 1, 0,
            0, 1, 1, 1, 1, 1, 0, 1, 1, 1,
        };
        int yseries[50] = {
            1, 1, 1, 1, 1, 0, 0, 0, 1, 0,
            0, 1, 1, 0, 0, 1, 1, 0, 0, 0,
            0, 1, 1, 1, 0, 1, 0, 0, 0, 0,
            0, 1, 0, 0, 1, 1, 0, 1, 0, 0,
            0, 1, 1, 1, 1, 0, 1, 1, 1, 1,
        };

        ASSERT_NOT_NULL(inform_local_transfer_entropy(xseries, xseries, 4, 10, 2, 2, te, NULL));
        ASSERT_DBL_NEAR_TOL(0.000000, AVERAGE(te), 1e-6);
        ASSERT_NOT_NULL(inform_local_transfer_entropy(yseries, xseries, 4, 10, 2, 2, te, NULL));
        ASSERT_DBL_NEAR_TOL(0.172618, AVERAGE(te), 1e-6);
        ASSERT_NOT_NULL(inform_local_transfer_entropy(xseries, yseries, 4, 10, 2, 2, te, NULL));
        ASSERT_DBL_NEAR_TOL(0.206156, AVERAGE(te), 1e-6);
        ASSERT_NOT_NULL(inform_local_transfer_entropy(yseries, yseries, 4, 10, 2, 2, te, NULL));
        ASSERT_DBL_NEAR_TOL(0.000000, AVERAGE(te), 1e-6);
    }
}

BEGIN_SUITE(TransferEntropy)
    ADD_UNIT(TransferEntropyTooShort)
    ADD_UNIT(TransferEntropyHistoryTooLong)
    ADD_UNIT(TransferEntropyEncodingError)
    ADD_UNIT(TransferEntropySingleSeries_Base2)
    ADD_UNIT(TransferEntropyEnsemble_Base2)
    ADD_UNIT(LocalTransferEntropyTooShort)
    ADD_UNIT(LocalTransferEntropyHistoryTooLong)
    ADD_UNIT(LocalTransferEntropyEncodingError)
    ADD_UNIT(LocalTransferEntropyAllocatesOutput)
    ADD_UNIT(LocalTransferEntropySingleSeries_Base2)
    ADD_UNIT(LocalTransferEntropyEnsemble_Base2)
END_SUITE
