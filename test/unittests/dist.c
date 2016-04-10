// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <ctest.h>
#include <inform/dist.h>

CTEST(Distribution, AllocZero)
{
    inform_dist *dist = inform_dist_alloc(0);
    ASSERT_NULL(dist);
}

CTEST(Distribution, AllocOne)
{
    inform_dist *dist = inform_dist_alloc(1);
    ASSERT_NOT_NULL(dist);
    ASSERT_EQUAL(1, inform_dist_size(dist));
    ASSERT_EQUAL(0, inform_dist_counts(dist));
    ASSERT_FALSE(inform_dist_is_valid(dist));
    inform_dist_free(dist);
}

CTEST(Distribution, AllocTwo)
{
    inform_dist *dist = inform_dist_alloc(2);
    ASSERT_NOT_NULL(dist);
    ASSERT_EQUAL(2, inform_dist_size(dist));
    ASSERT_EQUAL(0, inform_dist_counts(dist));
    ASSERT_FALSE(inform_dist_is_valid(dist));
    inform_dist_free(dist);
}

CTEST(Distribution, GetNull)
{
    inform_dist *dist = NULL;
    ASSERT_EQUAL(0, inform_dist_get(dist, 0));
    ASSERT_EQUAL(0, inform_dist_get(dist, 1));
    ASSERT_EQUAL(0, inform_dist_get(dist, 2));
}

CTEST(Distribution, SetNull)
{
    inform_dist *dist = NULL;
    ASSERT_EQUAL(0, inform_dist_set(dist, 0, 1));
    ASSERT_EQUAL(0, inform_dist_set(dist, 1, 1));
    ASSERT_EQUAL(0, inform_dist_set(dist, 2, 1));
}

CTEST(Distribution, Set)
{
    inform_dist *dist = inform_dist_alloc(3);
    ASSERT_NOT_NULL(dist);
    ASSERT_FALSE(inform_dist_is_valid(dist));

    ASSERT_EQUAL(1, inform_dist_set(dist, 0, 1));
    ASSERT_TRUE(inform_dist_is_valid(dist));
    ASSERT_EQUAL(2, inform_dist_set(dist, 1, 2));
    ASSERT_TRUE(inform_dist_is_valid(dist));
    ASSERT_EQUAL(3, inform_dist_set(dist, 2, 3));
    ASSERT_TRUE(inform_dist_is_valid(dist));

    ASSERT_EQUAL(3, inform_dist_size(dist));
    ASSERT_EQUAL(6, inform_dist_counts(dist));

    for (uint64_t i = 0; i < inform_dist_size(dist); ++i)
    {
        inform_dist_set(dist, i, 0); 
    }
    ASSERT_FALSE(inform_dist_is_valid(dist));
    ASSERT_EQUAL(3, inform_dist_size(dist));
    ASSERT_EQUAL(0, inform_dist_counts(dist));
    inform_dist_free(dist);
}

CTEST(Distribution, Get)
{
    inform_dist *dist = inform_dist_alloc(3);
    ASSERT_NOT_NULL(dist);
    ASSERT_FALSE(inform_dist_is_valid(dist));

    ASSERT_EQUAL(1, inform_dist_set(dist, 0, 1));
    ASSERT_EQUAL(2, inform_dist_set(dist, 1, 2));
    ASSERT_EQUAL(3, inform_dist_set(dist, 2, 3));

    for (size_t i = 0; i < inform_dist_size(dist); ++i)
    {
        ASSERT_EQUAL(i+1, inform_dist_get(dist, i));
    }
    inform_dist_free(dist);
}

CTEST(Distribution, DupNull)
{
    inform_dist *dist = NULL;
    inform_dist *dist2 = inform_dist_dup(dist);
    ASSERT_NULL(dist2);
}

CTEST(Distribution, Dup)
{
    inform_dist *dist = inform_dist_alloc(5);
    ASSERT_NOT_NULL(dist);
    for (size_t i = 0; i < inform_dist_size(dist); ++i)
    {
        inform_dist_set(dist, i, i*i);
    }
    inform_dist *dup = inform_dist_dup(dist);
    ASSERT_NOT_NULL(dup);
    ASSERT_EQUAL(inform_dist_size(dist), inform_dist_size(dup));
    ASSERT_EQUAL(inform_dist_counts(dist), inform_dist_counts(dup));
    for (size_t i = 0; i < inform_dist_size(dist); ++i)
    {
        ASSERT_EQUAL(inform_dist_get(dist,i), inform_dist_get(dup,i));
    }
    inform_dist_free(dup);
    inform_dist_free(dist);
}

CTEST(Distribution, Tick)
{
    inform_dist *dist = inform_dist_alloc(3);
    ASSERT_NOT_NULL(dist);
    ASSERT_FALSE(inform_dist_is_valid(dist));

    ASSERT_EQUAL(0, inform_dist_get(dist,0));

    ASSERT_EQUAL(1, inform_dist_tick(dist,0));
    ASSERT_EQUAL(1, inform_dist_get(dist,0));
    ASSERT_TRUE(inform_dist_is_valid(dist));

    ASSERT_EQUAL(1, inform_dist_tick(dist,1));
    ASSERT_EQUAL(1, inform_dist_get(dist,1));

    ASSERT_EQUAL(2, inform_dist_tick(dist,0));
    ASSERT_EQUAL(2, inform_dist_get(dist,0));

    ASSERT_EQUAL(1, inform_dist_tick(dist,2));
    ASSERT_EQUAL(1, inform_dist_get(dist,2));

    ASSERT_EQUAL(3, inform_dist_size(dist));
    ASSERT_EQUAL(4, inform_dist_counts(dist));

    inform_dist_free(dist);
}

CTEST(Distribution, Prob)
{
    inform_dist* dist = inform_dist_alloc(5);
    ASSERT_NOT_NULL(dist);

    for (size_t i = 0; i < inform_dist_size(dist); ++i)
    {
        inform_dist_set(dist, i, i+1);
    }
    ASSERT_EQUAL(15, inform_dist_counts(dist));
    for (size_t i = 0; i < inform_dist_size(dist); ++i)
    {
        ASSERT_DBL_NEAR((double)(i+1)/15, inform_dist_prob(dist,i));
    }

    inform_dist_free(dist);
}

CTEST(Distribution, Dump)
{
    inform_dist* dist = NULL;
    ASSERT_NULL(inform_dist_dump(dist));

    dist = inform_dist_alloc(5);
    ASSERT_NOT_NULL(dist);

    for (size_t i = 1; i < inform_dist_size(dist); ++i)
    {
        inform_dist_set(dist, i, i+1);
    }
    ASSERT_EQUAL(14, inform_dist_counts(dist));
    double expect[5] = {0., 2./14, 3/14., 4/14., 5/14.};
    double* probs = inform_dist_dump(dist);
    ASSERT_NOT_NULL(probs);
    for (size_t i = 0; i < inform_dist_size(dist); ++i)
    {
        ASSERT_DBL_NEAR(expect[i], probs[i]);
    }
    free(probs);
    inform_dist_free(dist);
}
