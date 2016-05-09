// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <ctest.h>
#include <inform/state_encoding.h>
#include <inform/time_series.h>
#include <math.h>

static uint64_t* random_series(size_t size, uint64_t base)
{
    uint64_t *series = calloc(size, sizeof(uint64_t));
    if (series != NULL)
    {
        for (size_t i = 0; i < size; ++i)
        {
            series[i] = rand() % base;
        }
    }
    return series;
}

CTEST(TimeSeries, ActiveInfoSeriesTooShort)
{
    uint64_t const series[] = {1,1,0,0,1,0,0,1};
    ASSERT_TRUE(isnan(inform_active_info(series, 0, 2, 2)));
    ASSERT_TRUE(isnan(inform_active_info(series, 1, 2, 2)));
}

CTEST(TimeSeries, ActiveInfoHistoryTooLong)
{
    {
        uint64_t const series[] = {1,1,0,0,1,0,0,1};

        ASSERT_TRUE(isnan(inform_active_info(series, 2, 2, 2)));
        ASSERT_FALSE(isnan(inform_active_info(series, 3, 2, 2)));
    }

    {
        size_t const size = 30;

        uint64_t *series = random_series(size, 2);
        ASSERT_TRUE(isnan(inform_active_info(series, size, 2, 26)));
        free(series);

        series = random_series(size, 3);
        ASSERT_TRUE(isnan(inform_active_info(series, size, 3, 16)));
        free(series);

        series = random_series(size, 4);
        ASSERT_TRUE(isnan(inform_active_info(series, size, 4, 13)));
        free(series);
    }
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

CTEST(TimeSeries, TransferEntropyTooShort)
{
    uint64_t const series[] = {1,1,1,0,0,1,1,0,0,1};
    ASSERT_TRUE(isnan(inform_transfer_entropy(series, series+5, 0, 2, 2)));
    ASSERT_TRUE(isnan(inform_transfer_entropy(series, series+5, 1, 2, 2)));
}

CTEST(TimeSeries, TransferEntropyHistoryTooLong)
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

CTEST(TimeSeries, TransferEntropyEncodingError)
{
     {
         uint64_t const series[10] = {2,1,1,0,1,0,0,1,0,1};
         ASSERT_FALSE(isnan(inform_transfer_entropy(series+5, series, 5, 3, 2)));
         ASSERT_TRUE(isnan(inform_transfer_entropy(series+5, series, 5, 2, 2)));

         ASSERT_FALSE(isnan(inform_transfer_entropy(series, series+5, 5, 3, 2)));
         ASSERT_FALSE(isnan(inform_transfer_entropy(series, series+5, 5, 2, 2)));
     }

     {
         uint64_t const series[] = {1,1,1,0,2,0,0,1,0,1};
         ASSERT_FALSE(isnan(inform_transfer_entropy(series+5, series, 5, 3, 2)));
         ASSERT_TRUE(isnan(inform_transfer_entropy(series+5, series, 5, 2, 2)));

         ASSERT_FALSE(isnan(inform_transfer_entropy(series, series+5, 5, 3, 2)));
         ASSERT_FALSE(isnan(inform_transfer_entropy(series, series+5, 5, 2, 2)));
     }

     {
         uint64_t const series[] = {1,1,1,2,1,0,0,1,0,1};
         ASSERT_FALSE(isnan(inform_transfer_entropy(series+5, series, 5, 3, 2)));
         ASSERT_TRUE(isnan(inform_transfer_entropy(series+5, series, 5, 2, 2)));

         ASSERT_FALSE(isnan(inform_transfer_entropy(series, series+5, 5, 3, 2)));
         ASSERT_TRUE(isnan(inform_transfer_entropy(series, series+5, 5, 2, 2)));
     }
}

CTEST(TimeSeries, TransferEntropySingleSeries_Base2)
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

CTEST(TimeSeries, TransferEntropyEnsemble_Base2)
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
