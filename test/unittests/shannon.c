// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <unit.h>

#include <inform/shannon.h>

#define inform_dist_fill_array(dist, array) \
    ASSERT_NOT_NULL(dist); \
    ASSERT_EQUAL(sizeof(array)/sizeof(uint64_t), inform_dist_size(dist)); \
    for (size_t i = 0; i < inform_dist_size(dist); ++i) \
    { \
        inform_dist_set(dist, i, array[i]); \
    }

#define inform_dist_fill(dist, ...) inform_dist_fill_array(dist, ((uint64_t[]){__VA_ARGS__}))

UNIT(ShannonInvalidDistribution)
{
    inform_dist *dist = NULL;
    ASSERT_TRUE(isnan(inform_shannon(dist, 2)));

    dist = inform_dist_alloc(5);
    ASSERT_TRUE(isnan(inform_shannon(dist, 2)));
    inform_dist_free(dist);
}

UNIT(ShannonDeltaFunction)
{
    inform_dist *dist = inform_dist_alloc(5);
    inform_dist_fill(dist, 0, 1, 0, 0, 0);
    ASSERT_TRUE(isnan(inform_shannon(dist, -1.0)));
    ASSERT_TRUE(isnan(inform_shannon(dist, -0.5)));
    ASSERT_DBL_NEAR_TOL(0.000000, inform_shannon(dist, 0.0), 1e-6);
    ASSERT_DBL_NEAR_TOL(0.000000, inform_shannon(dist, 0.5), 1e-6);
    ASSERT_DBL_NEAR_TOL(0.000000, inform_shannon(dist, 1.5), 1e-6);
    ASSERT_DBL_NEAR_TOL(0.000000, inform_shannon(dist, 2), 1e-6);
    ASSERT_DBL_NEAR_TOL(0.000000, inform_shannon(dist, 3), 1e-6);
    ASSERT_DBL_NEAR_TOL(0.000000, inform_shannon(dist, 4), 1e-6);
    inform_dist_free(dist);
}

UNIT(ShannonUniform)
{
    inform_dist *dist = inform_dist_alloc(5);
    inform_dist_fill(dist, 1, 1, 1, 1, 1);

    ASSERT_TRUE(isnan(inform_shannon(dist, -1.0)));
    ASSERT_TRUE(isnan(inform_shannon(dist, -0.5)));
    ASSERT_DBL_NEAR_TOL(0.000000, inform_shannon(dist, 0.0), 1e-6);
    ASSERT_DBL_NEAR_TOL(-2.321928, inform_shannon(dist, 0.5), 1e-6);
    ASSERT_DBL_NEAR_TOL(3.969362, inform_shannon(dist, 1.5), 1e-6);
    ASSERT_DBL_NEAR_TOL(2.321928, inform_shannon(dist, 2), 1e-6);
    ASSERT_DBL_NEAR_TOL(1.464973, inform_shannon(dist, 3), 1e-6);
    ASSERT_DBL_NEAR_TOL(1.160964, inform_shannon(dist, 4), 1e-6);
    inform_dist_free(dist);
}

UNIT(ShannonNonUniform)
{
    inform_dist *dist = inform_dist_alloc(2);
    inform_dist_fill(dist, 2, 1);

    ASSERT_TRUE(isnan(inform_shannon(dist, -1.0)));
    ASSERT_TRUE(isnan(inform_shannon(dist, -0.5)));
    ASSERT_DBL_NEAR_TOL(0.000000, inform_shannon(dist, 0.0), 1e-6);
    ASSERT_DBL_NEAR_TOL(-0.918295, inform_shannon(dist, 0.5), 1e-6);
    ASSERT_DBL_NEAR_TOL(1.569837, inform_shannon(dist, 1.5), 1e-6);
    ASSERT_DBL_NEAR_TOL(0.918295, inform_shannon(dist, 2), 1e-6);
    ASSERT_DBL_NEAR_TOL(0.579380, inform_shannon(dist, 3), 1e-6);
    ASSERT_DBL_NEAR_TOL(0.459148, inform_shannon(dist, 4), 1e-6);

    dist = inform_dist_realloc(dist, 3);
    inform_dist_fill(dist, 1, 1, 0);

    ASSERT_TRUE(isnan(inform_shannon(dist, -1.0)));
    ASSERT_TRUE(isnan(inform_shannon(dist, -0.5)));
    ASSERT_DBL_NEAR_TOL(0.000000, inform_shannon(dist, 0.0), 1e-6);
    ASSERT_DBL_NEAR_TOL(-1.000000, inform_shannon(dist, 0.5), 1e-6);
    ASSERT_DBL_NEAR_TOL(1.709511, inform_shannon(dist, 1.5), 1e-6);
    ASSERT_DBL_NEAR_TOL(1.000000, inform_shannon(dist, 2), 1e-6);
    ASSERT_DBL_NEAR_TOL(0.630930, inform_shannon(dist, 3), 1e-6);
    ASSERT_DBL_NEAR_TOL(0.500000, inform_shannon(dist, 4), 1e-6);

    inform_dist_fill(dist, 2, 2, 1);
    ASSERT_DBL_NEAR_TOL(1.521928, inform_shannon(dist, 2), 1e-6);

    ASSERT_TRUE(isnan(inform_shannon(dist, -1.0)));
    ASSERT_TRUE(isnan(inform_shannon(dist, -0.5)));
    ASSERT_DBL_NEAR_TOL(0.000000, inform_shannon(dist, 0.0), 1e-6);
    ASSERT_DBL_NEAR_TOL(-1.521928, inform_shannon(dist, 0.5), 1e-6);
    ASSERT_DBL_NEAR_TOL(2.601753, inform_shannon(dist, 1.5), 1e-6);
    ASSERT_DBL_NEAR_TOL(1.521928, inform_shannon(dist, 2), 1e-6);
    ASSERT_DBL_NEAR_TOL(0.960230, inform_shannon(dist, 3), 1e-6);
    ASSERT_DBL_NEAR_TOL(0.760964, inform_shannon(dist, 4), 1e-6);

    inform_dist_free(dist);
}

UNIT(MutualInformationIndependent)
{

    inform_dist *xs = inform_dist_alloc(12);
    inform_dist_fill(xs, 5, 2, 3, 5, 1, 4, 6, 2, 1, 4, 2, 4);

    inform_dist *ys = inform_dist_alloc(12);
    inform_dist_fill(ys, 2, 4, 5, 2, 7, 3, 9, 8, 8, 7, 2, 3);

    inform_dist *dist = inform_dist_alloc(144);
    for (size_t i = 0; i < inform_dist_size(xs); ++i)
    {
        for (size_t j = 0; j < inform_dist_size(ys); ++j)
        {
            inform_dist_set(dist, j+i*12,
                    inform_dist_get(xs, i) * inform_dist_get(ys, j));
        }
    }
    ASSERT_TRUE(isnan(inform_shannon_mi(dist, xs, ys, -1.0)));
    ASSERT_TRUE(isnan(inform_shannon_mi(dist, xs, ys, -0.5)));
    ASSERT_DBL_NEAR_TOL(0.000000, inform_shannon_mi(dist, xs, ys, 0.0), 1e-6);
    ASSERT_DBL_NEAR_TOL(0.000000, inform_shannon_mi(dist, xs, ys, 0.5), 1e-6);
    ASSERT_DBL_NEAR_TOL(0.000000, inform_shannon_mi(dist, xs, ys, 1.5), 1e-6);
    ASSERT_DBL_NEAR_TOL(0.000000, inform_shannon_mi(dist, xs, ys, 2), 1e-6);
    ASSERT_DBL_NEAR_TOL(0.000000, inform_shannon_mi(dist, xs, ys, 3), 1e-6);
    ASSERT_DBL_NEAR_TOL(0.000000, inform_shannon_mi(dist, xs, ys, 4), 1e-6);
    inform_dist_free(dist);
    inform_dist_free(ys);
    inform_dist_free(xs);
}

UNIT(MutualInformationDependent)
{
    inform_dist *dist = inform_dist_alloc(4);
    inform_dist_fill(dist, 10, 70, 15, 5);

    inform_dist *xs = inform_dist_alloc(2);
    inform_dist_fill(xs, 80, 20);

    inform_dist *ys = inform_dist_alloc(2);
    inform_dist_fill(ys, 25, 75);

    ASSERT_TRUE(isnan(inform_shannon_mi(dist, xs, ys, -1.0)));
    ASSERT_TRUE(isnan(inform_shannon_mi(dist, xs, ys, -0.5)));
    ASSERT_DBL_NEAR_TOL(0.000000, inform_shannon_mi(dist, xs, ys, 0.0), 1e-6);
    ASSERT_DBL_NEAR_TOL(-0.214171, inform_shannon_mi(dist, xs, ys, 0.5), 1e-6);
    ASSERT_DBL_NEAR_TOL(0.366128, inform_shannon_mi(dist, xs, ys, 1.5), 1e-6);
    ASSERT_DBL_NEAR_TOL(0.214171, inform_shannon_mi(dist, xs, ys, 2), 1e-6);
    ASSERT_DBL_NEAR_TOL(0.135127, inform_shannon_mi(dist, xs, ys, 3), 1e-6);
    ASSERT_DBL_NEAR_TOL(0.107086, inform_shannon_mi(dist, xs, ys, 4), 1e-6);
    inform_dist_free(ys);
    inform_dist_free(xs);
    inform_dist_free(dist);
}

UNIT(SelfInformationInvalidDist)
{
    inform_dist *dist = NULL;
    ASSERT_TRUE(isnan(inform_shannon_si(dist, 0, 2)));
}

UNIT(SelfInformationImposibleEvent)
{
    inform_dist *dist = inform_dist_alloc(2);
    inform_dist_fill(dist, 1, 0);
    ASSERT_FALSE(isinf(inform_shannon_si(dist, 0, 2)));
    ASSERT_TRUE(isinf(inform_shannon_si(dist, 2, 2)));
    inform_dist_free(dist);
}

UNIT(SelfInformationBase2)
{
    inform_dist *dist = inform_dist_alloc(2);
    inform_dist_fill(dist, 1, 1);
    ASSERT_DBL_NEAR_TOL(1.000000, inform_shannon_si(dist, 0, 2), 1e-6);
    ASSERT_DBL_NEAR_TOL(1.000000, inform_shannon_si(dist, 1, 2), 1e-6);

    inform_dist_fill(dist, 2, 1);
    ASSERT_DBL_NEAR_TOL(0.584963, inform_shannon_si(dist, 0, 2), 1e-6);
    ASSERT_DBL_NEAR_TOL(1.584963, inform_shannon_si(dist, 1, 2), 1e-6);

    inform_dist_fill(dist, 2, 0);
    ASSERT_DBL_NEAR_TOL(0.000000, inform_shannon_si(dist, 0, 2), 1e-6);
    ASSERT_TRUE(isinf(inform_shannon_si(dist, 1, 2)));

    inform_dist_free(dist);
}

UNIT(SelfInformatoinBase3)
{
    inform_dist *dist = inform_dist_alloc(3);
    inform_dist_fill(dist, 1, 2, 3);
    ASSERT_DBL_NEAR_TOL(1.630930, inform_shannon_si(dist, 0, 3), 1e-6);
    ASSERT_DBL_NEAR_TOL(1.000000, inform_shannon_si(dist, 1, 3), 1e-6);
    ASSERT_DBL_NEAR_TOL(0.630930, inform_shannon_si(dist, 2, 3), 1e-6);
    inform_dist_free(dist);
}

UNIT(PointwiseMutualInformationIndependent)
{

    inform_dist *xs = inform_dist_alloc(12);
    inform_dist_fill(xs, 5, 2, 3, 5, 1, 4, 6, 2, 1, 4, 2, 4);

    inform_dist *ys = inform_dist_alloc(12);
    inform_dist_fill(ys, 2, 4, 5, 2, 7, 3, 9, 8, 8, 7, 2, 3);

    inform_dist *dist = inform_dist_alloc(144);
    for (size_t i = 0; i < inform_dist_size(xs); ++i)
    {
        for (size_t j = 0; j < inform_dist_size(ys); ++j)
        {
            inform_dist_set(dist, j+i*12,
                    inform_dist_get(xs, i) * inform_dist_get(ys, j));
        }
    }

    for (size_t i = 0; i < inform_dist_size(xs); ++i)
    {
        for (size_t j = 0; j < inform_dist_size(ys); ++j)
        {
            ASSERT_DBL_NEAR_TOL(0.000000, inform_shannon_pmi(dist, xs, ys, j+i*12, i, j, 0.5), 1e-6);
            ASSERT_DBL_NEAR_TOL(0.000000, inform_shannon_pmi(dist, xs, ys, j+i*12, i, j, 2), 1e-6);
            ASSERT_DBL_NEAR_TOL(0.000000, inform_shannon_pmi(dist, xs, ys, j+i*12, i, j, 3), 1e-6);
        }
    }

    inform_dist_free(dist);
    inform_dist_free(ys);
    inform_dist_free(xs);
}

UNIT(PointwiseMutualInformationDependent)
{
    inform_dist *dist = inform_dist_alloc(4);
    inform_dist_fill(dist, 10, 70, 15, 5);

    inform_dist *xs = inform_dist_alloc(2);
    inform_dist_fill(xs, 80, 20);

    inform_dist *ys = inform_dist_alloc(2);
    inform_dist_fill(ys, 25, 75);

    ASSERT_DBL_NEAR_TOL(-1.000000, inform_shannon_pmi(dist, xs, ys, 0, 0, 0, 2), 1e-6);
    ASSERT_DBL_NEAR_TOL( 0.222392, inform_shannon_pmi(dist, xs, ys, 1, 0, 1, 2), 1e-6);
    ASSERT_DBL_NEAR_TOL( 1.584963, inform_shannon_pmi(dist, xs, ys, 2, 1, 0, 2), 1e-6);
    ASSERT_DBL_NEAR_TOL(-1.584963, inform_shannon_pmi(dist, xs, ys, 3, 1, 1, 2), 1e-6);

    inform_dist_free(ys);
    inform_dist_free(xs);
    inform_dist_free(dist);
}

BEGIN_SUITE(Entropy)
    ADD_UNIT(ShannonInvalidDistribution)
    ADD_UNIT(ShannonDeltaFunction)
    ADD_UNIT(ShannonUniform)
    ADD_UNIT(ShannonNonUniform)
    ADD_UNIT(MutualInformationIndependent)
    ADD_UNIT(MutualInformationDependent)
    ADD_UNIT(SelfInformationInvalidDist)
    ADD_UNIT(SelfInformationImposibleEvent)
    ADD_UNIT(SelfInformationBase2)
    ADD_UNIT(SelfInformatoinBase3)
    ADD_UNIT(PointwiseMutualInformationIndependent)
    ADD_UNIT(PointwiseMutualInformationDependent)
END_SUITE
