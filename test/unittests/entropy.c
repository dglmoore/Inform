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
