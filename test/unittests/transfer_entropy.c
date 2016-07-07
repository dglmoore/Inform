// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <unit.h>
#include <inform/state_encoding.h>
#include <inform/transfer_entropy.h>
#include "random.h"

UNIT(TransferEntropyTooShort)
{
    uint64_t const series[] = {1,1,1,0,0,1,1,0,0,1};
    ASSERT_TRUE(isnan(inform_transfer_entropy(series, series+5, 0, 2, 2)));
    ASSERT_TRUE(isnan(inform_transfer_entropy(series, series+5, 1, 2, 2)));
}

UNIT(TransferEntropyHistoryTooLong)
{
    {
        uint64_t const series[] = {1,1,1,0,0,1,1,0,0,1};
        ASSERT_TRUE(isnan(inform_transfer_entropy(series, series+5, 2, 2, 2)));
        ASSERT_FALSE(isnan(inform_transfer_entropy(series, series+5, 3, 2, 2)));
    }

    {
        size_t const size = 30;

        uint64_t *xseries = random_series(size, 2);
        uint64_t *yseries = random_series(size, 2);
        ASSERT_TRUE(isnan(inform_transfer_entropy(xseries, yseries, size, 2, 26)));
        free(xseries);
        free(yseries);

        xseries = random_series(size, 3);
        yseries = random_series(size, 3);
        ASSERT_TRUE(isnan(inform_transfer_entropy(xseries, yseries, size, 3, 16)));
        free(xseries);
        free(yseries);

        xseries = random_series(size, 4);
        yseries = random_series(size, 4);
        ASSERT_TRUE(isnan(inform_transfer_entropy(xseries, yseries, size, 4, 13)));
        free(xseries);
        free(yseries);
    }
}

UNIT(TransferEntropyEncodingError)
{
     {
         uint64_t const series[10] = {2,1,1,0,1,0,0,1,0,1};
         ASSERT_FALSE(isnan(inform_transfer_entropy(series+5, series, 5, 3, 2)));
         ASSERT_TRUE(isnan(inform_transfer_entropy(series+5, series, 5, 2, 2)));

         ASSERT_FALSE(isnan(inform_transfer_entropy(series, series+5, 5, 3, 2)));
         ASSERT_TRUE(isnan(inform_transfer_entropy(series, series+5, 5, 2, 2)));
     }

     {
         uint64_t const series[] = {1,1,1,0,2,0,0,1,0,1};
         ASSERT_FALSE(isnan(inform_transfer_entropy(series+5, series, 5, 3, 2)));
         ASSERT_TRUE(isnan(inform_transfer_entropy(series+5, series, 5, 2, 2)));

         ASSERT_FALSE(isnan(inform_transfer_entropy(series, series+5, 5, 3, 2)));
         ASSERT_TRUE(isnan(inform_transfer_entropy(series, series+5, 5, 2, 2)));
     }

     {
         uint64_t const series[] = {1,1,1,2,1,0,0,1,0,1};
         ASSERT_FALSE(isnan(inform_transfer_entropy(series+5, series, 5, 3, 2)));
         ASSERT_TRUE(isnan(inform_transfer_entropy(series+5, series, 5, 2, 2)));

         ASSERT_FALSE(isnan(inform_transfer_entropy(series, series+5, 5, 3, 2)));
         ASSERT_TRUE(isnan(inform_transfer_entropy(series, series+5, 5, 2, 2)));
     }
}


UNIT(TransferEntropySingleSeries_Base2)
{
    {
        uint64_t series[10] = {
                1,1,1,0,0,
                1,1,0,0,1
        };
        ASSERT_DBL_NEAR_TOL(0.000000,
                inform_transfer_entropy(series,   series,   5, 2, 2), 1e-6);
        ASSERT_DBL_NEAR_TOL(0.666666,
                inform_transfer_entropy(series+5, series,   5, 2, 2), 1e-6);
        ASSERT_DBL_NEAR_TOL(0.000000,
                inform_transfer_entropy(series,   series+5, 5, 2, 2), 1e-6);
        ASSERT_DBL_NEAR_TOL(0.000000,
                inform_transfer_entropy(series+5, series+5, 5, 2, 2), 1e-6);
    }

    {
        uint64_t series[20] = {
                0,0,1,1,1,0,0,0,0,1,
                1,1,0,0,0,0,0,0,1,1
        };
        ASSERT_DBL_NEAR_TOL(0.000000,
                inform_transfer_entropy(series,    series,    10, 2, 2), 1e-6);
        ASSERT_DBL_NEAR_TOL(0.500000,
                inform_transfer_entropy(series+10, series,    10, 2, 2), 1e-6);
        ASSERT_DBL_NEAR_TOL(0.106844,
                inform_transfer_entropy(series,    series+10, 10, 2, 2), 1e-6);
        ASSERT_DBL_NEAR_TOL(0.000000,
                inform_transfer_entropy(series+10, series+10, 10, 2, 2), 1e-6);
    }

    {
        uint64_t series[20] = {
                0,1,0,1,0,0,1,1,0,0,
                0,0,1,0,1,1,1,0,1,1
        };
        ASSERT_DBL_NEAR_TOL(0.000000,
                inform_transfer_entropy(series,    series,    10, 2, 2), 1e-6);
        ASSERT_DBL_NEAR_TOL(0.344361,
                inform_transfer_entropy(series+10, series,    10, 2, 2), 1e-6);
        ASSERT_DBL_NEAR_TOL(0.250000,
                inform_transfer_entropy(series,    series+10, 10, 2, 2), 1e-6);
        ASSERT_DBL_NEAR_TOL(0.000000,
                inform_transfer_entropy(series+10, series+10, 10, 2, 2), 1e-6);
    }
}

UNIT(TransferEntropyEnsemble_Base2)
{
    {
        uint64_t xseries[50] = {
            1, 1, 1, 0, 0, 1, 1, 0, 1, 0,
            0, 1, 0, 1, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0, 1, 0, 0,
            0, 0, 1, 0, 0, 0, 1, 0, 0, 1,
            0, 0, 1, 1, 1, 1, 1, 0, 0, 0,
        };
        uint64_t yseries[50] = {
            0, 1, 0, 0, 0, 1, 0, 1, 1, 0,
            0, 0, 0, 1, 1, 1, 0, 1, 0, 0,
            1, 0, 1, 0, 1, 0, 0, 0, 1, 0,
            0, 1, 1, 0, 1, 1, 1, 1, 1, 1,
            0, 0, 1, 1, 0, 0, 0, 0, 0, 1,
        };

        ASSERT_DBL_NEAR_TOL(0.000000,
                inform_transfer_entropy_ensemble(xseries, xseries, 5, 10, 2, 2), 1e-6);
        ASSERT_DBL_NEAR_TOL(0.091141,
                inform_transfer_entropy_ensemble(yseries, xseries, 5, 10, 2, 2), 1e-6);
        ASSERT_DBL_NEAR_TOL(0.107630,
                inform_transfer_entropy_ensemble(xseries, yseries, 5, 10, 2, 2), 1e-6);
        ASSERT_DBL_NEAR_TOL(0.000000,
                inform_transfer_entropy_ensemble(yseries, yseries, 5, 10, 2, 2), 1e-6);

        ASSERT_DBL_NEAR_TOL(0.000000,
                inform_transfer_entropy_ensemble(xseries, xseries, 4, 10, 2, 2), 1e-6);
        ASSERT_DBL_NEAR_TOL(0.134536,
                inform_transfer_entropy_ensemble(yseries, xseries, 4, 10, 2, 2), 1e-6);
        ASSERT_DBL_NEAR_TOL(0.089517,
                inform_transfer_entropy_ensemble(xseries, yseries, 4, 10, 2, 2), 1e-6);
        ASSERT_DBL_NEAR_TOL(0.000000,
                inform_transfer_entropy_ensemble(yseries, yseries, 4, 10, 2, 2), 1e-6);
    }
    {
        uint64_t xseries[50] = {
            0, 1, 0, 1, 0, 0, 1, 1, 1, 1,
            0, 1, 0, 1, 1, 1, 0, 0, 1, 0,
            1, 1, 1, 1, 0, 0, 1, 1, 1, 1,
            1, 0, 0, 0, 0, 0, 0, 0, 1, 0,
            0, 1, 1, 1, 1, 1, 0, 1, 1, 1,
        };
        uint64_t yseries[50] = {
            1, 1, 1, 1, 1, 0, 0, 0, 1, 0,
            0, 1, 1, 0, 0, 1, 1, 0, 0, 0,
            0, 1, 1, 1, 0, 1, 0, 0, 0, 0,
            0, 1, 0, 0, 1, 1, 0, 1, 0, 0,
            0, 1, 1, 1, 1, 0, 1, 1, 1, 1,
        };

        ASSERT_DBL_NEAR_TOL(0.000000,
                inform_transfer_entropy_ensemble(xseries, xseries, 5, 10, 2, 2), 1e-6);
        ASSERT_DBL_NEAR_TOL(0.031471,
                inform_transfer_entropy_ensemble(yseries, xseries, 5, 10, 2, 2), 1e-6);
        ASSERT_DBL_NEAR_TOL(0.152561,
                inform_transfer_entropy_ensemble(xseries, yseries, 5, 10, 2, 2), 1e-6);
        ASSERT_DBL_NEAR_TOL(0.000000,
                inform_transfer_entropy_ensemble(yseries, yseries, 5, 10, 2, 2), 1e-6);

        ASSERT_DBL_NEAR_TOL(0.000000,
                inform_transfer_entropy_ensemble(xseries, xseries, 4, 10, 2, 2), 1e-6);
        ASSERT_DBL_NEAR_TOL(0.172618,
                inform_transfer_entropy_ensemble(yseries, xseries, 4, 10, 2, 2), 1e-6);
        ASSERT_DBL_NEAR_TOL(0.206156,
                inform_transfer_entropy_ensemble(xseries, yseries, 4, 10, 2, 2), 1e-6);
        ASSERT_DBL_NEAR_TOL(0.000000,
                inform_transfer_entropy_ensemble(yseries, yseries, 4, 10, 2, 2), 1e-6);
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
    uint64_t const series[] = {1,1,1,0,0,1,1,0,0,1};
    ASSERT_EQUAL(3, inform_local_transfer_entropy(series, series+5, 0, 2, 2, te));
    ASSERT_EQUAL(3, inform_local_transfer_entropy(series, series+5, 1, 2, 2, te));
}

UNIT(LocalTransferEntropyHistoryTooLong)
{
    {
        double te[8];
        uint64_t const series[] = {1,1,1,0,0,1,1,0,0,1};
        ASSERT_EQUAL(4, inform_local_transfer_entropy(series, series+5, 2, 2, 2, te));
        ASSERT_EQUAL(0, inform_local_transfer_entropy(series, series+5, 3, 2, 2, te));
    }

    {
        size_t const size = 30;

        double te[30];
        uint64_t *xseries = random_series(size, 2);
        uint64_t *yseries = random_series(size, 2);
        ASSERT_EQUAL(5, inform_local_transfer_entropy(xseries, yseries, size, 2, 26, te));
        free(xseries);
        free(yseries);

        xseries = random_series(size, 3);
        yseries = random_series(size, 3);
        ASSERT_EQUAL(5, inform_local_transfer_entropy(xseries, yseries, size, 3, 16, te));
        free(xseries);
        free(yseries);

        xseries = random_series(size, 4);
        yseries = random_series(size, 4);
        ASSERT_EQUAL(5, inform_local_transfer_entropy(xseries, yseries, size, 4, 13, te));
        free(xseries);
        free(yseries);
    }
}

UNIT(LocalTransferEntropyEncodingError)
{
     {
        double te[8];
         uint64_t const series[10] = {2,1,1,0,1,0,0,1,0,1};
        ASSERT_EQUAL(0, inform_local_transfer_entropy(series+5, series, 5, 3, 2, te));
        ASSERT_EQUAL(6, inform_local_transfer_entropy(series+5, series, 5, 2, 2, te));

        ASSERT_EQUAL(0, inform_local_transfer_entropy(series, series+5, 5, 3, 2, te));
        ASSERT_EQUAL(6, inform_local_transfer_entropy(series, series+5, 5, 2, 2, te));
     }

     {
         double te[8];
         uint64_t const series[] = {1,1,1,0,2,0,0,1,0,1};
        ASSERT_EQUAL(0, inform_local_transfer_entropy(series+5, series, 5, 3, 2, te));
        ASSERT_EQUAL(6, inform_local_transfer_entropy(series+5, series, 5, 2, 2, te));

        ASSERT_EQUAL(0, inform_local_transfer_entropy(series, series+5, 5, 3, 2, te));
        ASSERT_EQUAL(6, inform_local_transfer_entropy(series, series+5, 5, 2, 2, te));
     }

     {
        double te[8];
         uint64_t const series[] = {1,1,1,2,1,0,0,1,0,1};
        ASSERT_EQUAL(0, inform_local_transfer_entropy(series+5, series, 5, 3, 2, te));
        ASSERT_EQUAL(6, inform_local_transfer_entropy(series+5, series, 5, 2, 2, te));

        ASSERT_EQUAL(0, inform_local_transfer_entropy(series, series+5, 5, 3, 2, te));
        ASSERT_EQUAL(6, inform_local_transfer_entropy(series, series+5, 5, 2, 2, te));
     }
}

UNIT(LocalTransferEntropySingleSeries_Base2)
{
    {
        double te[3];
        uint64_t series[10] = {
                1,1,1,0,0,
                1,1,0,0,1
        };
        ASSERT_EQUAL(0, inform_local_transfer_entropy(series, series, 5, 2, 2, te));
        ASSERT_DBL_NEAR_TOL(0.000000, AVERAGE(te), 1e-6);
        ASSERT_EQUAL(0, inform_local_transfer_entropy(series+5, series, 5, 2, 2, te));
        ASSERT_DBL_NEAR_TOL(0.666666, AVERAGE(te), 1e-6);
        ASSERT_EQUAL(0, inform_local_transfer_entropy(series, series+5, 5, 2, 2, te));
        ASSERT_DBL_NEAR_TOL(0.000000, AVERAGE(te), 1e-6);
        ASSERT_EQUAL(0, inform_local_transfer_entropy(series+5, series+5, 5, 2, 2, te));
        ASSERT_DBL_NEAR_TOL(0.000000, AVERAGE(te), 1e-6);
    }

    {
        double te[8];
        uint64_t series[20] = {
                0,0,1,1,1,0,0,0,0,1,
                1,1,0,0,0,0,0,0,1,1
        };
        ASSERT_EQUAL(0, inform_local_transfer_entropy(series, series, 10, 2, 2, te));
        ASSERT_DBL_NEAR_TOL(0.000000, AVERAGE(te), 1e-6);
        ASSERT_EQUAL(0, inform_local_transfer_entropy(series+10, series, 10, 2, 2, te));
        ASSERT_DBL_NEAR_TOL(0.500000, AVERAGE(te), 1e-6);
        ASSERT_EQUAL(0, inform_local_transfer_entropy(series, series+10, 10, 2, 2, te));
        ASSERT_DBL_NEAR_TOL(0.106844, AVERAGE(te), 1e-6);
        ASSERT_EQUAL(0, inform_local_transfer_entropy(series+10, series+10, 10, 2, 2, te));
        ASSERT_DBL_NEAR_TOL(0.000000, AVERAGE(te), 1e-6);
    }

    {
        double te[8];
        uint64_t series[20] = {
                0,1,0,1,0,0,1,1,0,0,
                0,0,1,0,1,1,1,0,1,1
        };
        ASSERT_EQUAL(0, inform_local_transfer_entropy(series, series, 10, 2, 2, te));
        ASSERT_DBL_NEAR_TOL(0.000000, AVERAGE(te), 1e-6);
        ASSERT_EQUAL(0, inform_local_transfer_entropy(series+10, series, 10, 2, 2, te));
        ASSERT_DBL_NEAR_TOL(0.344361, AVERAGE(te), 1e-6);
        ASSERT_EQUAL(0, inform_local_transfer_entropy(series, series+10, 10, 2, 2, te));
        ASSERT_DBL_NEAR_TOL(0.250000, AVERAGE(te), 1e-6);
        ASSERT_EQUAL(0, inform_local_transfer_entropy(series+10, series+10, 10, 2, 2, te));
        ASSERT_DBL_NEAR_TOL(0.000000, AVERAGE(te), 1e-6);
    }
}

UNIT(LocalTransferEntropyEnsemble_Base2)
{
    {
        double te[40];
        uint64_t xseries[50] = {
            1, 1, 1, 0, 0, 1, 1, 0, 1, 0,
            0, 1, 0, 1, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0, 1, 0, 0,
            0, 0, 1, 0, 0, 0, 1, 0, 0, 1,
            0, 0, 1, 1, 1, 1, 1, 0, 0, 0,
        };
        uint64_t yseries[50] = {
            0, 1, 0, 0, 0, 1, 0, 1, 1, 0,
            0, 0, 0, 1, 1, 1, 0, 1, 0, 0,
            1, 0, 1, 0, 1, 0, 0, 0, 1, 0,
            0, 1, 1, 0, 1, 1, 1, 1, 1, 1,
            0, 0, 1, 1, 0, 0, 0, 0, 0, 1,
        };

        ASSERT_EQUAL(0, inform_local_transfer_entropy_ensemble(xseries, xseries, 5, 10, 2, 2, te));
        ASSERT_DBL_NEAR_TOL(0.000000, AVERAGE(te), 1e-6);
        ASSERT_EQUAL(0, inform_local_transfer_entropy_ensemble(yseries, xseries, 5, 10, 2, 2, te));
        ASSERT_DBL_NEAR_TOL(0.091141, AVERAGE(te), 1e-6);
        ASSERT_EQUAL(0, inform_local_transfer_entropy_ensemble(xseries, yseries, 5, 10, 2, 2, te));
        ASSERT_DBL_NEAR_TOL(0.107630, AVERAGE(te), 1e-6);
        ASSERT_EQUAL(0, inform_local_transfer_entropy_ensemble(yseries, yseries, 5, 10, 2, 2, te));
        ASSERT_DBL_NEAR_TOL(0.000000, AVERAGE(te), 1e-6);
    }
    {
        double te[32];
        uint64_t xseries[40] = {
            1, 1, 1, 0, 0, 1, 1, 0, 1, 0,
            0, 1, 0, 1, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0, 1, 0, 0,
            0, 0, 1, 0, 0, 0, 1, 0, 0, 1,
        };
        uint64_t yseries[40] = {
            0, 1, 0, 0, 0, 1, 0, 1, 1, 0,
            0, 0, 0, 1, 1, 1, 0, 1, 0, 0,
            1, 0, 1, 0, 1, 0, 0, 0, 1, 0,
            0, 1, 1, 0, 1, 1, 1, 1, 1, 1,
        };

        ASSERT_EQUAL(0, inform_local_transfer_entropy_ensemble(xseries, xseries, 4, 10, 2, 2, te));
        ASSERT_DBL_NEAR_TOL(0.000000, AVERAGE(te), 1e-6);
        ASSERT_EQUAL(0, inform_local_transfer_entropy_ensemble(yseries, xseries, 4, 10, 2, 2, te));
        ASSERT_DBL_NEAR_TOL(0.134536, AVERAGE(te), 1e-6);
        ASSERT_EQUAL(0, inform_local_transfer_entropy_ensemble(xseries, yseries, 4, 10, 2, 2, te));
        ASSERT_DBL_NEAR_TOL(0.089517, AVERAGE(te), 1e-6);
        ASSERT_EQUAL(0, inform_local_transfer_entropy_ensemble(yseries, yseries, 4, 10, 2, 2, te));
        ASSERT_DBL_NEAR_TOL(0.000000, AVERAGE(te), 1e-6);
    }
    {
        double te[40];
        uint64_t xseries[50] = {
            0, 1, 0, 1, 0, 0, 1, 1, 1, 1,
            0, 1, 0, 1, 1, 1, 0, 0, 1, 0,
            1, 1, 1, 1, 0, 0, 1, 1, 1, 1,
            1, 0, 0, 0, 0, 0, 0, 0, 1, 0,
            0, 1, 1, 1, 1, 1, 0, 1, 1, 1,
        };
        uint64_t yseries[50] = {
            1, 1, 1, 1, 1, 0, 0, 0, 1, 0,
            0, 1, 1, 0, 0, 1, 1, 0, 0, 0,
            0, 1, 1, 1, 0, 1, 0, 0, 0, 0,
            0, 1, 0, 0, 1, 1, 0, 1, 0, 0,
            0, 1, 1, 1, 1, 0, 1, 1, 1, 1,
        };

        ASSERT_EQUAL(0, inform_local_transfer_entropy_ensemble(xseries, xseries, 5, 10, 2, 2, te));
        ASSERT_DBL_NEAR_TOL(0.000000, AVERAGE(te), 1e-6);
        ASSERT_EQUAL(0, inform_local_transfer_entropy_ensemble(yseries, xseries, 5, 10, 2, 2, te));
        ASSERT_DBL_NEAR_TOL(0.031471, AVERAGE(te), 1e-6);
        ASSERT_EQUAL(0, inform_local_transfer_entropy_ensemble(xseries, yseries, 5, 10, 2, 2, te));
        ASSERT_DBL_NEAR_TOL(0.152561, AVERAGE(te), 1e-6);
        ASSERT_EQUAL(0, inform_local_transfer_entropy_ensemble(yseries, yseries, 5, 10, 2, 2, te));
        ASSERT_DBL_NEAR_TOL(0.000000, AVERAGE(te), 1e-6);
    }
    {
        double te[32];
        uint64_t xseries[50] = {
            0, 1, 0, 1, 0, 0, 1, 1, 1, 1,
            0, 1, 0, 1, 1, 1, 0, 0, 1, 0,
            1, 1, 1, 1, 0, 0, 1, 1, 1, 1,
            1, 0, 0, 0, 0, 0, 0, 0, 1, 0,
            0, 1, 1, 1, 1, 1, 0, 1, 1, 1,
        };
        uint64_t yseries[50] = {
            1, 1, 1, 1, 1, 0, 0, 0, 1, 0,
            0, 1, 1, 0, 0, 1, 1, 0, 0, 0,
            0, 1, 1, 1, 0, 1, 0, 0, 0, 0,
            0, 1, 0, 0, 1, 1, 0, 1, 0, 0,
            0, 1, 1, 1, 1, 0, 1, 1, 1, 1,
        };

        ASSERT_EQUAL(0, inform_local_transfer_entropy_ensemble(xseries, xseries, 4, 10, 2, 2, te));
        ASSERT_DBL_NEAR_TOL(0.000000, AVERAGE(te), 1e-6);
        ASSERT_EQUAL(0, inform_local_transfer_entropy_ensemble(yseries, xseries, 4, 10, 2, 2, te));
        ASSERT_DBL_NEAR_TOL(0.172618, AVERAGE(te), 1e-6);
        ASSERT_EQUAL(0, inform_local_transfer_entropy_ensemble(xseries, yseries, 4, 10, 2, 2, te));
        ASSERT_DBL_NEAR_TOL(0.206156, AVERAGE(te), 1e-6);
        ASSERT_EQUAL(0, inform_local_transfer_entropy_ensemble(yseries, yseries, 4, 10, 2, 2, te));
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
    ADD_UNIT(LocalTransferEntropySingleSeries_Base2)
    ADD_UNIT(LocalTransferEntropyEnsemble_Base2)
END_SUITE
