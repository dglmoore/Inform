// Copyright 2016-2017 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <inform/integration.h>
#include <inform/utilities/random.h>
#include <unit.h>

UNIT(IntegrationEvidenceNULLSeries)
{
    inform_error err = INFORM_SUCCESS;
    ASSERT_NULL(inform_integration_evidence(NULL, 1, 3, NULL, NULL, &err));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_ETIMESERIES, err);
}

UNIT(IntegrationEvidenceTooFewSeries)
{
    inform_error err = INFORM_SUCCESS;
    int series[] = {1,1,0,1,0,1};
    ASSERT_NULL(inform_integration_evidence(series, 0, 6, NULL, NULL, &err));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_ENOSOURCES, err);

    err = INFORM_SUCCESS;
    ASSERT_NULL(inform_integration_evidence(series, 1, 6, (int[]){2}, NULL,
        &err));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_ENOSOURCES, err);
}

UNIT(IntegrationEvidenceShortSeries)
{
    inform_error err = INFORM_SUCCESS;
    int series[] = {1,1,0,1,0,1};
    ASSERT_NULL(inform_integration_evidence(series, 2, 0, (int[]){2,2}, NULL,
        &err));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_ESHORTSERIES, err);
}

UNIT(IntegrationEvidenceInvalidBase)
{
    inform_error err = INFORM_SUCCESS;
    int series[] = {1,1,0,1,0,1};
    ASSERT_NULL(inform_integration_evidence(series, 2, 3, (int[]){1,2}, NULL,
        &err));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_EBASE, err);

    err = INFORM_SUCCESS;
    ASSERT_NULL(inform_integration_evidence(series, 2, 3, (int[]){2,1}, NULL,
        &err));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_EBASE, err);

    err = INFORM_SUCCESS;
    ASSERT_NULL(inform_integration_evidence(series, 2, 3, (int[]){0,2}, NULL,
        &err));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_EBASE, err);

    err = INFORM_SUCCESS;
    ASSERT_NULL(inform_integration_evidence(series, 2, 3, (int[]){2,0}, NULL,
        &err));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_EBASE, err);

    err = INFORM_SUCCESS;
    ASSERT_NULL(inform_integration_evidence(series, 2, 3, (int[]){-1,2}, NULL,
        &err));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_EBASE, err);

    err = INFORM_SUCCESS;
    ASSERT_NULL(inform_integration_evidence(series, 2, 3, (int[]){2,-1}, NULL,
        &err));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_EBASE, err);
}

UNIT(IntegrationEvidenceNegativeState)
{
    inform_error err;
    {
        err = INFORM_SUCCESS;
        int series[] = {1,-1,0,1,0,1};
        ASSERT_NULL(inform_integration_evidence(series, 2, 3, (int[]){2,2},
            NULL, &err));
        ASSERT_TRUE(inform_failed(&err));
        ASSERT_EQUAL(INFORM_ENEGSTATE, err);
    }
    {
        err = INFORM_SUCCESS;
        int series[] = {1,1,0,1,0,-1};
        ASSERT_NULL(inform_integration_evidence(series, 2, 3, (int[]){2,2},
            NULL, &err));
        ASSERT_TRUE(inform_failed(&err));
        ASSERT_EQUAL(INFORM_ENEGSTATE, err);
    }
}

UNIT(IntegrationEvidenceBadState)
{
    inform_error err;
    {
        err = INFORM_SUCCESS;
        int series[] = {1,2,0,1,0,1};
        ASSERT_NULL(inform_integration_evidence(series, 2, 3, (int[]){2,2},
            NULL, &err));
        ASSERT_TRUE(inform_failed(&err));
        ASSERT_EQUAL(INFORM_EBADSTATE, err);
    }
    {
        err = INFORM_SUCCESS;
        int series[] = {1,1,0,1,0,2};
        ASSERT_NULL(inform_integration_evidence(series, 2, 3, (int[]){2,2},
            NULL, &err));
        ASSERT_TRUE(inform_failed(&err));
        ASSERT_EQUAL(INFORM_EBADSTATE, err);
    }
}

UNIT(IntegrationEvidenceAllocates)
{
    inform_error err = INFORM_SUCCESS;
    int series[] = {1,1,0,1,0,1};
    double *evidence = inform_integration_evidence(series, 2, 3, (int[]){2,2},
        NULL, &err);
    ASSERT_FALSE(inform_failed(&err));
    ASSERT_NOT_NULL(evidence);
    free(evidence);
}

UNIT(IntegrationEvidenceTwoSeries)
{
    inform_error err = INFORM_SUCCESS;
    double evidence[6];
    {
        int series[] = {1,1,0,1,0,1};
        double expected[3] = {-0.415037, 0.584963, 0.584963};
        inform_integration_evidence(series, 2, 3, (int[]){2,2}, evidence, &err);
        ASSERT_TRUE(inform_succeeded(&err));
        for (size_t i = 0; i < 3; ++i)
        {
            ASSERT_DBL_NEAR_TOL(expected[i], evidence[i], 1e-6);
            ASSERT_DBL_NEAR_TOL(expected[i], evidence[i+3], 1e-6);
        }
    }
    {
        int series[] = {1,1,0,1,0,0};
        double expected[3] = {0.584963, -0.415037, 0.584963};
        inform_integration_evidence(series, 2, 3, (int[]){2,2}, evidence, &err);
        ASSERT_TRUE(inform_succeeded(&err));
        for (size_t i = 0; i < 3; ++i)
        {
            ASSERT_DBL_NEAR_TOL(expected[i], evidence[i], 1e-6);
            ASSERT_DBL_NEAR_TOL(expected[i], evidence[i+3], 1e-6);
        }
    }
    {
        int series[] = {1,1,1,1,0,0};
        double expected[3] = {0.0, 0.0, 0.0};
        inform_integration_evidence(series, 2, 3, (int[]){2,2}, evidence, &err);
        ASSERT_TRUE(inform_succeeded(&err));
        for (size_t i = 0; i < 3; ++i)
        {
            ASSERT_DBL_NEAR_TOL(expected[i], evidence[i], 1e-6);
            ASSERT_DBL_NEAR_TOL(expected[i], evidence[i+3], 1e-6);
        }
    }
    {
        int series[] = {1,0,0,1,0,0};
        double expected[3] = {1.584963, 0.584963, 0.584963};
        inform_integration_evidence(series, 2, 3, (int[]){2,2}, evidence, &err);
        ASSERT_TRUE(inform_succeeded(&err));
        for (size_t i = 0; i < 3; ++i)
        {
            ASSERT_DBL_NEAR_TOL(expected[i], evidence[i], 1e-6);
            ASSERT_DBL_NEAR_TOL(expected[i], evidence[i+3], 1e-6);
        }
    }
}

UNIT(IntegrationEvidenceThreeSeries)
{
    inform_error err = INFORM_SUCCESS;
    double evidence[6];
    {
        int series[] = {1,1,0, 1,0,1, 0,1,1};
        double expected[6] = {0.584963, 0.584963, 0.584963,
                              1.584963, 1.584963, 1.584963};
        inform_integration_evidence(series, 3, 3, (int[]){2,2,2}, evidence, &err);
        ASSERT_TRUE(inform_succeeded(&err));
        for (size_t i = 0; i < 6; ++i)
        {
            ASSERT_DBL_NEAR_TOL(expected[i], evidence[i], 1e-6);
        }
    }
    {
        int series[] = {1,1,0, 1,1,1, 0,1,1};
        double expected[6] = {0.000000, -0.415037, 0.000000,
                              0.584963,  0.000000, 0.584963};
        inform_integration_evidence(series, 3, 3, (int[]){2,2,2}, evidence, &err);
        ASSERT_TRUE(inform_succeeded(&err));
        for (size_t i = 0; i < 6; ++i)
        {
            ASSERT_DBL_NEAR_TOL(expected[i], evidence[i], 1e-6);
        }
    }
    {
        int series[] = {1,1,0,
                        1,0,0,
                        0,1,1};
        double expected[6] = {0.584963, -0.415037, 0.584963,
                              2.169925,  0.584963, 1.169925};
        inform_integration_evidence(series, 3, 3, (int[]){2,2,2}, evidence, &err);
        ASSERT_TRUE(inform_succeeded(&err));
        for (size_t i = 0; i < 6; ++i)
        {
            ASSERT_DBL_NEAR_TOL(expected[i], evidence[i], 1e-6);
        }
    }
    {
        int series[] = {1,1,0,
                        0,1,1,
                        0,1,1};
        double expected[6] = {0.584963, -0.415037, 0.584963,
                              2.169925,  0.584963, 1.169925};
        inform_integration_evidence(series, 3, 3, (int[]){2,2,2}, evidence, &err);
        ASSERT_TRUE(inform_succeeded(&err));
        for (size_t i = 0; i < 6; ++i)
        {
            ASSERT_DBL_NEAR_TOL(expected[i], evidence[i], 1e-6);
        }
    }
    {
        int series[] = {0,1,1,
                        0,1,1,
                        0,1,1};
        double expected[6] = {1.584963, 0.584963, 0.584963,
                              3.169925, 1.169925, 1.169925};
        inform_integration_evidence(series, 3, 3, (int[]){2,2,2}, evidence, &err);
        ASSERT_TRUE(inform_succeeded(&err));
        for (size_t i = 0; i < 6; ++i)
        {
            ASSERT_DBL_NEAR_TOL(expected[i], evidence[i], 1e-6);
        }
    }
}

UNIT(IntegrationEvidenceSynchronized)
{
    srand(1879);
    inform_error err = INFORM_SUCCESS;
    int const b = 4;
    size_t const l = 5;
    size_t const n = 100;
    double *evidence = malloc(2 * n * sizeof(double));
    int *bases = malloc(l * sizeof(int));
    for (size_t i = 0; i < l; ++i) bases[i] = b;
    int *series = malloc(l * n * sizeof(int));
    ASSERT_NOT_NULL(series);
    for (size_t u = 0; u < 10; ++u)
    {
        int *row = inform_random_ints(0, b, n);
        ASSERT_NOT_NULL(row);
        for (size_t i = 0; i < l; ++i)
        {
            for (size_t j = 0; j < n; ++j)
            {
                series[j + n*i] = row[j];
            }
        }
        inform_integration_evidence(series, l, n, bases, evidence, &err);
        ASSERT_TRUE(inform_succeeded(&err));
        for (size_t i = 0; i < 2*n; ++i)
        {
            ASSERT_TRUE(evidence[i] > 0.0);
        }
        free(row);
    }
    for (size_t u = 0; u < 10; ++u)
    {
        int *row = inform_random_ints(0, b, n);
        ASSERT_NOT_NULL(row);
        for (size_t i = 0; i < l; ++i)
        {
            int flip = rand() % 2;
            for (size_t j = 0; j < n; ++j)
            {
                series[j + n*i] = (flip) ? ((row[j] + 1) % 2) : row[j];
            }
        }
        inform_integration_evidence(series, l, n, bases, evidence, &err);
        ASSERT_TRUE(inform_succeeded(&err));
        for (size_t i = 0; i < 2*n; ++i)
        {
            ASSERT_TRUE(evidence[i] > 0.0);
        }
        free(row);
    }
    free(series);
}

UNIT(IntegrationEvidencePartsNULLSeries)
{
    inform_error err = INFORM_SUCCESS;
    ASSERT_NULL(inform_integration_evidence_part(NULL, 1, 3, NULL, NULL, 0,
        NULL, &err));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_ETIMESERIES, err);
}

UNIT(IntegrationEvidencePartsTooFewSeries)
{
    inform_error err = INFORM_SUCCESS;
    int series[] = {1,1,0,1,0,1};
    ASSERT_NULL(inform_integration_evidence_part(series, 0, 6, NULL, NULL, 0,
        NULL, &err));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_ENOSOURCES, err);

    err = INFORM_SUCCESS;
    ASSERT_NULL(inform_integration_evidence_part(series, 1, 6, (int[]){2},
        NULL, 0, NULL, &err));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_ENOSOURCES, err);
}

UNIT(IntegrationEvidencePartsShortSeries)
{
    inform_error err = INFORM_SUCCESS;
    int series[] = {1,1,0,1,0,1};
    ASSERT_NULL(inform_integration_evidence_part(series, 2, 0, (int[]){2,2},
        NULL, 0, NULL, &err));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_ESHORTSERIES, err);
}

UNIT(IntegrationEvidencePartsInvalidBase)
{
    inform_error err = INFORM_SUCCESS;
    int series[] = {1,1,0,1,0,1};
    ASSERT_NULL(inform_integration_evidence_part(series, 2, 3, (int[]){1,2},
        NULL, 0, NULL, &err));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_EBASE, err);

    err = INFORM_SUCCESS;
    ASSERT_NULL(inform_integration_evidence_part(series, 2, 3, (int[]){2,1},
        NULL, 0, NULL, &err));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_EBASE, err);

    err = INFORM_SUCCESS;
    ASSERT_NULL(inform_integration_evidence_part(series, 2, 3, (int[]){0,2},
        NULL, 0, NULL, &err));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_EBASE, err);

    err = INFORM_SUCCESS;
    ASSERT_NULL(inform_integration_evidence_part(series, 2, 3, (int[]){2,0},
        NULL, 0, NULL, &err));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_EBASE, err);

    err = INFORM_SUCCESS;
    ASSERT_NULL(inform_integration_evidence_part(series, 2, 3, (int[]){-1,2},
        NULL, 0, NULL, &err));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_EBASE, err);

    err = INFORM_SUCCESS;
    ASSERT_NULL(inform_integration_evidence_part(series, 2, 3, (int[]){2,-1},
        NULL, 0, NULL, &err));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_EBASE, err);
}

UNIT(IntegrationEvidencePartsNegativeState)
{
    inform_error err;
    {
        err = INFORM_SUCCESS;
        int series[] = {1,-1,0,1,0,1};
        ASSERT_NULL(inform_integration_evidence_part(series, 2, 3, (int[]){2,2},
            NULL, 0, NULL, &err));
        ASSERT_TRUE(inform_failed(&err));
        ASSERT_EQUAL(INFORM_ENEGSTATE, err);
    }
    {
        err = INFORM_SUCCESS;
        int series[] = {1,1,0,1,0,-1};
        ASSERT_NULL(inform_integration_evidence_part(series, 2, 3, (int[]){2,2},
            NULL, 0, NULL, &err));
        ASSERT_TRUE(inform_failed(&err));
        ASSERT_EQUAL(INFORM_ENEGSTATE, err);
    }
}

UNIT(IntegrationEvidencePartsBadState)
{
    inform_error err;
    {
        err = INFORM_SUCCESS;
        int series[] = {1,2,0,1,0,1};
        ASSERT_NULL(inform_integration_evidence_part(series, 2, 3, (int[]){2,2},
            NULL, 0, NULL, &err));
        ASSERT_TRUE(inform_failed(&err));
        ASSERT_EQUAL(INFORM_EBADSTATE, err);
    }
    {
        err = INFORM_SUCCESS;
        int series[] = {1,1,0,1,0,2};
        ASSERT_NULL(inform_integration_evidence_part(series, 2, 3, (int[]){2,2},
            NULL, 0, NULL, &err));
        ASSERT_TRUE(inform_failed(&err));
        ASSERT_EQUAL(INFORM_EBADSTATE, err);
    }
}

UNIT(IntegrationEvidencePartsInvalidPartitioning)
{
    inform_error err;
    int series[] = {1,1,0,1,0,1};
    {
        err = INFORM_SUCCESS;
        ASSERT_NULL(inform_integration_evidence_part(series, 2, 3, (int[]){2,2},
            (size_t[]){0,2}, 2, NULL, &err));
        ASSERT_TRUE(inform_failed(&err));
        ASSERT_EQUAL(INFORM_EPARTS, err);
    }
    {
        err = INFORM_SUCCESS;
        int series[] = {1,1,0,1,0,1};
        ASSERT_NULL(inform_integration_evidence_part(series, 2, 3, (int[]){2,2},
            (size_t[]){1,2}, 2, NULL, &err));
        ASSERT_TRUE(inform_failed(&err));
        ASSERT_EQUAL(INFORM_EPARTS, err);
    }
    {
        err = INFORM_SUCCESS;
        int series[] = {1,1,0,1,0,1};
        ASSERT_NULL(inform_integration_evidence_part(series, 2, 3, (int[]){2,2},
            (size_t[]){2,2}, 2, NULL, &err));
        ASSERT_TRUE(inform_failed(&err));
        ASSERT_EQUAL(INFORM_EPARTS, err);
    }
    {
        err = INFORM_SUCCESS;
        int series[] = {1,1,0,1,0,1};
        ASSERT_NULL(inform_integration_evidence_part(series, 2, 3, (int[]){2,2},
            (size_t[]){0,1,2}, 3, NULL, &err));
        ASSERT_TRUE(inform_failed(&err));
        ASSERT_EQUAL(INFORM_EPARTS, err);
    }
    {
        err = INFORM_SUCCESS;
        int series[] = {1,1,0,1,0,1};
        ASSERT_NULL(inform_integration_evidence_part(series, 2, 3, (int[]){2,2},
            (size_t[]){0,1,1}, 3, NULL, &err));
        ASSERT_TRUE(inform_failed(&err));
        ASSERT_EQUAL(INFORM_EPARTS, err);
    }
}

UNIT(IntegrationEvidencePartsAllocates)
{
    inform_error err = INFORM_SUCCESS;
    int series[] = {1,1,0,1,0,1};
    double *evidence = inform_integration_evidence_part(series, 2, 3,
        (int[]){2,2}, NULL, 0, NULL, &err);
    ASSERT_FALSE(inform_failed(&err));
    ASSERT_NOT_NULL(evidence);
    free(evidence);
}

UNIT(IntegrationEvidencePartsTwoSeries)
{
    inform_error err = INFORM_SUCCESS;
    double evidence[3];
    {
        int series[] = {1,1,0,1,0,1};
        double expected[3] = {-0.415037, 0.584963, 0.584963};
        inform_integration_evidence_part(series, 2, 3, (int[]){2,2},
            NULL, 0, evidence, &err);
        ASSERT_TRUE(inform_succeeded(&err));
        for (size_t i = 0; i < 3; ++i)
        {
            ASSERT_DBL_NEAR_TOL(expected[i], evidence[i], 1e-6);
        }
    }
    {
        int series[] = {1,1,0,1,0,0};
        double expected[3] = {0.584963, -0.415037, 0.584963};
        inform_integration_evidence_part(series, 2, 3, (int[]){2,2},
            NULL, 0, evidence, &err);
        ASSERT_TRUE(inform_succeeded(&err));
        for (size_t i = 0; i < 3; ++i)
        {
            ASSERT_DBL_NEAR_TOL(expected[i], evidence[i], 1e-6);
        }
    }
    {
        int series[] = {1,1,1,1,0,0};
        double expected[3] = {0.0, 0.0, 0.0};
        inform_integration_evidence_part(series, 2, 3, (int[]){2,2},
            NULL, 0, evidence, &err);
        ASSERT_TRUE(inform_succeeded(&err));
        for (size_t i = 0; i < 3; ++i)
        {
            ASSERT_DBL_NEAR_TOL(expected[i], evidence[i], 1e-6);
        }
    }
    {
        int series[] = {1,0,0,1,0,0};
        double expected[3] = {1.584963, 0.584963, 0.584963};
        inform_integration_evidence_part(series, 2, 3, (int[]){2,2},
            NULL, 0, evidence, &err);
        ASSERT_TRUE(inform_succeeded(&err));
        for (size_t i = 0; i < 3; ++i)
        {
            ASSERT_DBL_NEAR_TOL(expected[i], evidence[i], 1e-6);
        }
    }
}

UNIT(IntegrationEvidencePartsThreeSeriesFinest)
{
    inform_error err = INFORM_SUCCESS;
    double evidence[6];
    {
        int series[] = {1,1,0, 1,0,1, 0,1,1};
        double expected[3] = {1.169925, 1.169925, 1.169925};
        inform_integration_evidence_part(series, 3, 3, (int[]){2,2,2},
            NULL, 0, evidence, &err);
        ASSERT_TRUE(inform_succeeded(&err));
        for (size_t i = 0; i < 3; ++i)
        {
            ASSERT_DBL_NEAR_TOL(expected[i], evidence[i], 1e-6);
        }
    }
    {
        int series[] = {1,1,0, 1,1,1, 0,1,1};
        double expected[3] = {0.584963, -0.415037, 0.584963};
        inform_integration_evidence_part(series, 3, 3, (int[]){2,2,2},
            NULL, 0, evidence, &err);
        ASSERT_TRUE(inform_succeeded(&err));
        for (size_t i = 0; i < 3; ++i)
        {
            ASSERT_DBL_NEAR_TOL(expected[i], evidence[i], 1e-6);
        }
    }
    {
        int series[] = {1,1,0,
                        1,0,0,
                        0,1,1};
        double expected[3] = {2.169925, 0.169925, 1.169925};
        inform_integration_evidence_part(series, 3, 3, (int[]){2,2,2},
            NULL, 0, evidence, &err);
        ASSERT_TRUE(inform_succeeded(&err));
        for (size_t i = 0; i < 3; ++i)
        {
            ASSERT_DBL_NEAR_TOL(expected[i], evidence[i], 1e-6);
        }
    }
    {
        int series[] = {1,1,0,
                        0,1,1,
                        0,1,1};
        double expected[3] = {2.169925, 0.169925, 1.169925};
        inform_integration_evidence_part(series, 3, 3, (int[]){2,2,2},
            NULL, 0, evidence, &err);
        ASSERT_TRUE(inform_succeeded(&err));
        for (size_t i = 0; i < 3; ++i)
        {
            ASSERT_DBL_NEAR_TOL(expected[i], evidence[i], 1e-6);
        }
    }
    {
        int series[] = {0,1,1,
                        0,1,1,
                        0,1,1};
        double expected[3] = {3.169925, 1.169925, 1.169925};
        inform_integration_evidence_part(series, 3, 3, (int[]){2,2,2},
            NULL, 0, evidence, &err);
        ASSERT_TRUE(inform_succeeded(&err));
        for (size_t i = 0; i < 3; ++i)
        {
            ASSERT_DBL_NEAR_TOL(expected[i], evidence[i], 1e-6);
        }
    }
}

UNIT(IntegrationEvidencePartsThreeSeries)
{
    inform_error err = INFORM_SUCCESS;
    double evidence[3];
    {
        int series[] = {1,1,0, 1,0,1, 0,1,1};
        size_t parts[] = {0,1,1};
        double expected[3] = {0.584963, 0.584963, 1.584963};
        inform_integration_evidence_part(series, 3, 3, (int[]){2,2,2}, parts, 2,
            evidence, &err);
        ASSERT_TRUE(inform_succeeded(&err));
        for (size_t i = 0; i < 3; ++i)
        {
            ASSERT_DBL_NEAR_TOL(expected[i], evidence[i], 1e-6);
        }
    }
    {
        int series[] = {1,1,0, 1,0,1, 0,1,1};
        size_t parts[] = {1,0,1};
        double expected[3] = {0.584963, 1.584963, 0.584963};
        inform_integration_evidence_part(series, 3, 3, (int[]){2,2,2}, parts, 2,
            evidence, &err);
        ASSERT_TRUE(inform_succeeded(&err));
        for (size_t i = 0; i < 3; ++i)
        {
            ASSERT_DBL_NEAR_TOL(expected[i], evidence[i], 1e-6);
        }
    }
    {
        int series[] = {1,1,0, 1,0,1, 0,1,1};
        size_t parts[] = {1,1,0};
        double expected[3] = {1.584963, 0.584963, 0.584963};
        inform_integration_evidence_part(series, 3, 3, (int[]){2,2,2}, parts, 2,
            evidence, &err);
        ASSERT_TRUE(inform_succeeded(&err));
        for (size_t i = 0; i < 3; ++i)
        {
            ASSERT_DBL_NEAR_TOL(expected[i], evidence[i], 1e-6);
        }
    }
}

UNIT(IntegrationEvidencePartsSynchronizedFinest)
{
    srand(1879);
    inform_error err = INFORM_SUCCESS;
    int const b = 4;
    size_t const l = 5;
    size_t const n = 100;
    double *evidence = malloc(n * sizeof(double));
    int *bases = malloc(l * sizeof(int));
    for (size_t i = 0; i < l; ++i) bases[i] = b;
    int *series = malloc(l * n * sizeof(int));
    ASSERT_NOT_NULL(series);
    for (size_t u = 0; u < 10; ++u)
    {
        int *row = inform_random_ints(0, b, n);
        ASSERT_NOT_NULL(row);
        for (size_t i = 0; i < l; ++i)
        {
            for (size_t j = 0; j < n; ++j)
            {
                series[j + n*i] = row[j];
            }
        }
        inform_integration_evidence_part(series, l, n, bases, NULL, 0, evidence,
            &err);
        ASSERT_TRUE(inform_succeeded(&err));
        for (size_t i = 0; i < n; ++i)
        {
            ASSERT_TRUE(evidence[i] > 0.0);
        }
        free(row);
    }
    for (size_t u = 0; u < 10; ++u)
    {
        int *row = inform_random_ints(0, b, n);
        ASSERT_NOT_NULL(row);
        for (size_t i = 0; i < l; ++i)
        {
            int flip = rand() % 2;
            for (size_t j = 0; j < n; ++j)
            {
                series[j + n*i] = (flip) ? ((row[j] + 1) % 2) : row[j];
            }
        }
        inform_integration_evidence_part(series, l, n, bases, NULL, 0, evidence,
            &err);
        ASSERT_TRUE(inform_succeeded(&err));
        for (size_t i = 0; i < n; ++i)
        {
            ASSERT_TRUE(evidence[i] > 0.0);
        }
        free(row);
    }
    free(series);
}

UNIT(IntegrationEvidencePartsSynchronized)
{
    srand(1879);
    inform_error err = INFORM_SUCCESS;
    int const b = 4;
    size_t const l = 5;
    size_t const n = 100;
    double *evidence = malloc(n * sizeof(double));
    int *bases = malloc(l * sizeof(int));
    for (size_t i = 0; i < l; ++i) bases[i] = b;
    int *series = malloc(l * n * sizeof(int));
    ASSERT_NOT_NULL(series);

    size_t parts[] = {0,1,1,0,2};
    for (size_t u = 0; u < 10; ++u)
    {
        int *row = inform_random_ints(0, b, n);
        ASSERT_NOT_NULL(row);
        for (size_t i = 0; i < l; ++i)
        {
            for (size_t j = 0; j < n; ++j)
            {
                series[j + n*i] = row[j];
            }
        }
        inform_integration_evidence_part(series, l, n, bases, parts, 3,
            evidence, &err);
        ASSERT_TRUE(inform_succeeded(&err));
        for (size_t i = 0; i < n; ++i)
        {
            ASSERT_TRUE(evidence[i] > 0.0);
        }
        free(row);
    }
    for (size_t u = 0; u < 10; ++u)
    {
        int *row = inform_random_ints(0, b, n);
        ASSERT_NOT_NULL(row);
        for (size_t i = 0; i < l; ++i)
        {
            int flip = rand() % 2;
            for (size_t j = 0; j < n; ++j)
            {
                series[j + n*i] = (flip) ? ((row[j] + 1) % 2) : row[j];
            }
        }
        inform_integration_evidence_part(series, l, n, bases, parts, 3,
            evidence, &err);
        ASSERT_TRUE(inform_succeeded(&err));
        for (size_t i = 0; i < n; ++i)
        {
            ASSERT_TRUE(evidence[i] > 0.0);
        }
        free(row);
    }
    free(series);
}

BEGIN_SUITE(Integration)
    ADD_UNIT(IntegrationEvidenceNULLSeries)
    ADD_UNIT(IntegrationEvidenceTooFewSeries)
    ADD_UNIT(IntegrationEvidenceShortSeries)
    ADD_UNIT(IntegrationEvidenceNegativeState)
    ADD_UNIT(IntegrationEvidenceBadState)
    ADD_UNIT(IntegrationEvidenceAllocates)
    ADD_UNIT(IntegrationEvidenceTwoSeries)
    ADD_UNIT(IntegrationEvidenceThreeSeries)
    ADD_UNIT(IntegrationEvidenceSynchronized)

    ADD_UNIT(IntegrationEvidencePartsNULLSeries)
    ADD_UNIT(IntegrationEvidencePartsTooFewSeries)
    ADD_UNIT(IntegrationEvidencePartsShortSeries)
    ADD_UNIT(IntegrationEvidencePartsNegativeState)
    ADD_UNIT(IntegrationEvidencePartsBadState)
    ADD_UNIT(IntegrationEvidencePartsInvalidPartitioning)
    ADD_UNIT(IntegrationEvidencePartsAllocates)
    ADD_UNIT(IntegrationEvidencePartsTwoSeries)
    ADD_UNIT(IntegrationEvidencePartsThreeSeriesFinest)
    ADD_UNIT(IntegrationEvidencePartsThreeSeries)
    ADD_UNIT(IntegrationEvidencePartsSynchronizedFinest)
    ADD_UNIT(IntegrationEvidencePartsSynchronized)
END_SUITE