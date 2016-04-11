// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <ctest.h>

#include <inform/entropy.h>

#define inform_dist_fill_array(dist, array) \
    ASSERT_NOT_NULL(dist); \
    ASSERT_EQUAL(sizeof(array)/sizeof(uint64_t), inform_dist_size(dist)); \
    for (size_t i = 0; i < inform_dist_size(dist); ++i) \
    { \
        inform_dist_set(dist, i, array[i]); \
    }

#define inform_dist_fill(dist, ...) inform_dist_fill_array(dist, ((uint64_t[]){__VA_ARGS__}))

CTEST_DATA(Entropy)
{
    inform_dist *dist;
};

CTEST_SETUP(Entropy)
{
    data->dist = NULL;
}

CTEST_TEARDOWN(Entropy)
{
    inform_dist_free(data->dist);
}

CTEST2(Entropy, ShannonInvalidDistribution)
{
    ASSERT_TRUE(isnan(inform_shannon(data->dist)));

    data->dist = inform_dist_alloc(5);
    ASSERT_TRUE(isnan(inform_shannon(data->dist)));
}

CTEST2(Entropy, ShannonDeltaFunction)
{
    data->dist = inform_dist_alloc(5);
    inform_dist_fill(data->dist, 0, 1, 0, 0, 0);
    ASSERT_DBL_NEAR_TOL(0.000000, inform_shannon(data->dist), 1e-6);
}

CTEST2(Entropy, ShannonUniform)
{
    data->dist = inform_dist_alloc(5);
    inform_dist_fill(data->dist, 1, 1, 1, 1, 1);
    ASSERT_DBL_NEAR_TOL(2.321928, inform_shannon(data->dist), 1e-6);
}

CTEST2(Entropy, ShannonNonUniform)
{
    data->dist = inform_dist_alloc(2);
    inform_dist_fill(data->dist, 2, 1);
    ASSERT_DBL_NEAR_TOL(0.918295, inform_shannon(data->dist), 1e-6);

    data->dist = inform_dist_realloc(data->dist, 3);
    inform_dist_fill(data->dist, 1, 1, 0);
    ASSERT_DBL_NEAR_TOL(1.000000, inform_shannon(data->dist), 1e-6);

    inform_dist_fill(data->dist, 2, 2, 1);
    ASSERT_DBL_NEAR_TOL(1.521928, inform_shannon(data->dist), 1e-6);
}

CTEST2(Entropy, MutualInformationIndependent)
{

    inform_dist *xs = inform_dist_alloc(12);
    inform_dist_fill(xs, 5, 2, 3, 5, 1, 4, 6, 2, 1, 4, 2, 4);

    inform_dist *ys = inform_dist_alloc(12);
    inform_dist_fill(ys, 2, 4, 5, 2, 7, 3, 9, 8, 8, 7, 2, 3);

    data->dist = inform_dist_alloc(144);
    for (size_t i = 0; i < inform_dist_size(xs); ++i)
    {
        for (size_t j = 0; j < inform_dist_size(ys); ++j)
        {
            inform_dist_set(data->dist, j+i*12,
                    inform_dist_get(xs, i) * inform_dist_get(ys, j));
        }
    }
    ASSERT_DBL_NEAR_TOL(0.000000, inform_mutual_info(data->dist, xs, ys), 1e-6);
    inform_dist_free(ys);
    inform_dist_free(xs);
}

CTEST2(Entropy, MutualInformationDependent)
{
    data->dist = inform_dist_alloc(4);
    inform_dist_fill(data->dist, 10, 70, 15, 5);

    inform_dist *xs = inform_dist_alloc(2);
    inform_dist_fill(xs, 80, 20);

    inform_dist *ys = inform_dist_alloc(2);
    inform_dist_fill(ys, 25, 75);

    ASSERT_DBL_NEAR_TOL(0.214171, inform_mutual_info(data->dist, xs, ys), 1e-6);
    inform_dist_free(ys);
    inform_dist_free(xs);
}
