// Copyright 2016-2017 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <unit.h>

#include <inform/shannon.h>
#include <inform/utilities/random.h>

#define inform_dist_fill_array(dist, array) \
    ASSERT_NOT_NULL(dist); \
    ASSERT_EQUAL(sizeof(array)/sizeof(int), inform_dist_size(dist)); \
    for (size_t i = 0; i < inform_dist_size(dist); ++i) \
    { \
        inform_dist_set(dist, i, array[i]); \
    }

#define inform_dist_fill(dist, ...) inform_dist_fill_array(dist, ((int[]){__VA_ARGS__}))

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

UNIT(RelativeEntropyInvalidDistributions)
{
    inform_dist *p = NULL, *q = NULL;
    ASSERT_TRUE(isnan(inform_shannon_re(p, q, 2)));

    p = inform_dist_alloc(5);
    ASSERT_TRUE(isnan(inform_shannon_re(p, q, 2)));
    ASSERT_TRUE(isnan(inform_shannon_re(q, p, 2)));

    inform_dist_tick(p, 0);
    ASSERT_TRUE(isnan(inform_shannon_re(p, q, 2)));
    ASSERT_TRUE(isnan(inform_shannon_re(q, p, 2)));

    q = inform_dist_alloc(5);
    ASSERT_TRUE(isnan(inform_shannon_re(p, q, 2)));
    ASSERT_TRUE(isnan(inform_shannon_re(q, p, 2)));

    inform_dist_free(q);
    inform_dist_free(p);
}

UNIT(RelativeEntropyIncompatibleSizes)
{
    inform_dist *p = inform_dist_alloc(5);
    inform_dist *q = inform_dist_alloc(4);

    inform_dist_tick(p, 0);
    inform_dist_tick(q, 1);

    ASSERT_TRUE(isnan(inform_shannon_re(p, q, 2)));
    ASSERT_TRUE(isnan(inform_shannon_re(q, p, 2)));

    inform_dist_free(q);
    inform_dist_free(p);
}

UNIT(RelativeEntropyUndefined)
{
    inform_dist *p = inform_dist_alloc(5);
    inform_dist *q = inform_dist_alloc(5);
    inform_dist_fill(p, 1, 1, 1, 1, 1);
    inform_dist_fill(q, 1, 1, 1, 2, 0);

    ASSERT_TRUE(isnan(inform_shannon_re(p, q, 2)));
    ASSERT_FALSE(isnan(inform_shannon_re(q, p, 2)));

    inform_dist_free(q);
    inform_dist_free(p);
}

UNIT(RelativeEntropySameDist)
{
    inform_dist *p = inform_dist_alloc(20);
    for (size_t i = 0; i < inform_dist_size(p); ++i)
        inform_dist_set(p, i, inform_random_int(0, 100));
    ASSERT_TRUE(isnan(inform_shannon_re(p, p, -1.0)));
    ASSERT_TRUE(isnan(inform_shannon_re(p, p, -0.5)));
    ASSERT_DBL_NEAR_TOL(0.000000, inform_shannon_re(p, p, 0.0), 1e-6);
    ASSERT_DBL_NEAR_TOL(0.000000, inform_shannon_re(p, p, 0.5), 1e-6);
    ASSERT_DBL_NEAR_TOL(0.000000, inform_shannon_re(p, p, 1.5), 1e-6);
    ASSERT_DBL_NEAR_TOL(0.000000, inform_shannon_re(p, p, 2.0), 1e-6);
    ASSERT_DBL_NEAR_TOL(0.000000, inform_shannon_re(p, p, 3.0), 1e-6);
    ASSERT_DBL_NEAR_TOL(0.000000, inform_shannon_re(p, p, 4.0), 1e-6);
    inform_dist_free(p);
}

UNIT(RelativeEntropyDefined)
{
    inform_dist *p = inform_dist_alloc(3);
    inform_dist *q = inform_dist_alloc(3);

    inform_dist_fill(p, 1, 0, 0);
    inform_dist_fill(q, 1, 1, 1);
    for (double b = 0.0; b <= 4.0; b += 0.5)
    {
        ASSERT_DBL_NEAR(log2(3)/log2(b), inform_shannon_re(p, q, b));
    }

    inform_dist_fill(p, 1, 1, 0);
    for (double b = 0.0; b <= 4.0; b += 0.5)
    {
        ASSERT_DBL_NEAR(log2(3.0/2.0)/log2(b), inform_shannon_re(p, q, b));
    }

    inform_dist_fill(p, 2, 2, 1);
    for (double b = 0.0; b <= 4.0; b += 0.5)
    {
        ASSERT_DBL_NEAR_TOL((4*log2(6./5.) + log2(3./5.))/(5 * log2(b)), inform_shannon_re(p, q, b), 5e-16);
    }

    inform_dist_fill(q, 1, 2, 2);
    for (double b = 0.0; b <= 4.0; b += 0.5)
    {
        ASSERT_DBL_NEAR(1.0/(5.0*log2(b)), inform_shannon_re(p, q, b));
    }

    inform_dist_fill(p, 1, 0, 1);
    inform_dist_fill(q, 2, 1, 2);
    for (double b = 0.0; b <= 4.0; b += 0.5)
    {
        ASSERT_DBL_NEAR(log2(5.0/4.0)/log2(b), inform_shannon_re(p, q, b));
    }

    inform_dist_fill(p, 1, 0, 0);
    inform_dist_fill(q, 4, 1, 0);
    for (double b = 0.0; b <= 4.0; b += 0.5)
    {
        ASSERT_DBL_NEAR(log2(5.0/4.0)/log2(b), inform_shannon_re(p, q, b));
    }

    inform_dist_fill(p, 1, 0, 0);
    inform_dist_fill(q, 1, 4, 0);
    for (double b = 0.0; b <= 4.0; b += 0.5)
    {
        ASSERT_DBL_NEAR(log2(5.0)/log2(b), inform_shannon_re(p, q, b));
    }

    inform_dist_free(q);
    inform_dist_free(p);
}

UNIT(CrossEntropyInvalidDistributions)
{
    inform_dist *p = NULL, *q = NULL;
    ASSERT_TRUE(isnan(inform_shannon_cross(p, q, 2)));

    p = inform_dist_alloc(5);
    ASSERT_TRUE(isnan(inform_shannon_cross(p, q, 2)));
    ASSERT_TRUE(isnan(inform_shannon_cross(q, p, 2)));

    inform_dist_tick(p, 0);
    ASSERT_TRUE(isnan(inform_shannon_cross(p, q, 2)));
    ASSERT_TRUE(isnan(inform_shannon_cross(q, p, 2)));

    q = inform_dist_alloc(5);
    ASSERT_TRUE(isnan(inform_shannon_cross(p, q, 2)));
    ASSERT_TRUE(isnan(inform_shannon_cross(q, p, 2)));

    inform_dist_free(q);
    inform_dist_free(p);
}

UNIT(CrossEntropyIncompatibleSizes)
{
    inform_dist *p = inform_dist_alloc(5);
    inform_dist *q = inform_dist_alloc(4);

    inform_dist_tick(p, 0);
    inform_dist_tick(q, 1);

    ASSERT_TRUE(isnan(inform_shannon_cross(p, q, 2)));
    ASSERT_TRUE(isnan(inform_shannon_cross(q, p, 2)));

    inform_dist_free(q);
    inform_dist_free(p);
}

UNIT(CrossEntropyUndefined)
{
    inform_dist *p = inform_dist_alloc(5);
    inform_dist *q = inform_dist_alloc(5);
    inform_dist_fill(p, 1, 1, 1, 1, 1);
    inform_dist_fill(q, 1, 1, 1, 2, 0);

    ASSERT_TRUE(isinf(inform_shannon_cross(p, q, 2)));
    ASSERT_FALSE(isnan(inform_shannon_cross(q, p, 2)));
    ASSERT_FALSE(isinf(inform_shannon_cross(q, p, 2)));

    inform_dist_free(q);
    inform_dist_free(p);
}

UNIT(CrossEntropySameDist)
{
    inform_dist *p = inform_dist_alloc(20);
    for (size_t i = 0; i < inform_dist_size(p); ++i)
        inform_dist_set(p, i, inform_random_int(0, 100));
    ASSERT_TRUE(isnan(inform_shannon_cross(p, p, -1.0)));
    ASSERT_TRUE(isnan(inform_shannon_cross(p, p, -0.5)));
    ASSERT_DBL_NEAR_TOL(inform_shannon(p, 0.0), inform_shannon_cross(p, p, 0.0), 1e-6);
    ASSERT_DBL_NEAR_TOL(inform_shannon(p, 0.5), inform_shannon_cross(p, p, 0.5), 1e-6);
    ASSERT_DBL_NEAR_TOL(inform_shannon(p, 1.5), inform_shannon_cross(p, p, 1.5), 1e-6);
    ASSERT_DBL_NEAR_TOL(inform_shannon(p, 2.0), inform_shannon_cross(p, p, 2.0), 1e-6);
    ASSERT_DBL_NEAR_TOL(inform_shannon(p, 3.0), inform_shannon_cross(p, p, 3.0), 1e-6);
    ASSERT_DBL_NEAR_TOL(inform_shannon(p, 4.0), inform_shannon_cross(p, p, 4.0), 1e-6);
    inform_dist_free(p);
}

UNIT(CrossEntropyDefined)
{
    inform_dist *p = inform_dist_alloc(3);
    inform_dist *q = inform_dist_alloc(3);

    inform_dist_fill(p, 1, 0, 0);
    inform_dist_fill(q, 2, 1, 1);
    for (double b = 0.0; b <= 4.0; b += 0.5)
    {
        double got = inform_shannon_cross(p, q, b);
        ASSERT_FALSE(isnan(got));
        ASSERT_DBL_NEAR(1/log2(b), got);
    }

    inform_dist_fill(p, 1, 1, 0);
    for (double b = 0.0; b <= 4.0; b += 0.5)
    {
        double got = inform_shannon_cross(p, q, b);
        ASSERT_FALSE(isnan(got));
        ASSERT_DBL_NEAR(3/(2*log2(b)), got);
    }

    inform_dist_fill(p, 2, 2, 1);
    for (double b = 0.0; b <= 4.0; b += 0.5)
    {
        double got = inform_shannon_cross(p, q, b);
        ASSERT_FALSE(isnan(got));
        ASSERT_DBL_NEAR(8/(5*log2(b)), got);
    }

    inform_dist_fill(q, 1, 2, 2);
    for (double b = 0.0; b <= 4.0; b += 0.5)
    {
        double got = inform_shannon_cross(p, q, b);
        ASSERT_FALSE(isnan(got));
        ASSERT_DBL_NEAR(((5*log2(5)-3)/(5*log2(b))), got);
    }

    inform_dist_fill(p, 1, 0, 1);
    inform_dist_fill(q, 2, 1, 2);
    for (double b = 0.0; b <= 4.0; b += 0.5)
    {
        double got = inform_shannon_cross(p, q, b);
        ASSERT_FALSE(isnan(got));
        ASSERT_DBL_NEAR((log2(5)-1)/log2(b), got);
    }

    inform_dist_fill(p, 1, 0, 0);
    inform_dist_fill(q, 4, 1, 0);
    for (double b = 0.0; b <= 4.0; b += 0.5)
    {
        double got = inform_shannon_cross(p, q, b);
        ASSERT_FALSE(isnan(got));
        ASSERT_DBL_NEAR((log2(5)-2)/log2(b), got);
    }

    inform_dist_fill(p, 1, 0, 0);
    inform_dist_fill(q, 1, 4, 0);
    for (double b = 0.0; b <= 4.0; b += 0.5)
    {
        double got = inform_shannon_cross(p, q, b);
        ASSERT_FALSE(isnan(got));
        ASSERT_DBL_NEAR(log2(5.0)/log2(b), got);
    }

    inform_dist_free(q);
    inform_dist_free(p);
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

    ADD_UNIT(RelativeEntropyInvalidDistributions)
    ADD_UNIT(RelativeEntropyIncompatibleSizes)
    ADD_UNIT(RelativeEntropyUndefined)
    ADD_UNIT(RelativeEntropySameDist)
    ADD_UNIT(RelativeEntropyDefined)

    ADD_UNIT(CrossEntropyInvalidDistributions)
    ADD_UNIT(CrossEntropyIncompatibleSizes)
    ADD_UNIT(CrossEntropyUndefined)
    ADD_UNIT(CrossEntropySameDist)
    ADD_UNIT(CrossEntropyDefined)
END_SUITE
