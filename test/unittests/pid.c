// Copyright 2016-2017 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <inform/pid.h>
#include <ginger/unit.h>
#include <ginger/vector.h>

#define TestPIDSources(N,M) \
{ \
    pid_source **srcs = pid_sources((N)); \
    ASSERT_NOT_NULL(srcs); \
    ASSERT_EQUAL_U((M), gvector_cap(srcs)); \
    ASSERT_EQUAL_U((M), gvector_len(srcs)); \
    for (size_t i = 0; i < gvector_len(srcs); ++i) \
    { \
        ASSERT_NOT_NULL(srcs[i]); \
        ASSERT_NOT_NULL(srcs[i]->name); \
        ASSERT_EQUAL_U(gvector_len(srcs[i]->name), gvector_cap(srcs[i]->name)); \
 \
        ASSERT_NOT_NULL(srcs[i]->above); \
        ASSERT_EQUAL_U(0, gvector_len(srcs[i]->above)); \
 \
        ASSERT_NOT_NULL(srcs[i]->below); \
        ASSERT_EQUAL_U(0, gvector_len(srcs[i]->below)); \
    } \
 \
    for (size_t i = 0; i < gvector_len(srcs); ++i) \
    { \
        pid_source_free(srcs[i]); \
    } \
    gvector_free(srcs); \
}

UNIT(PIDSources)
{
    TestPIDSources(1,1);
    TestPIDSources(2,4);
    TestPIDSources(3,18);
    TestPIDSources(4,166);
    TestPIDSources(5,7579);

    size_t expected[18][4] = {
        {1,1,0,0}, {2,1,6,0}, {2,1,4,0}, {2,1,2,0},
        {3,1,2,4}, {1,2,0,0}, {2,2,5,0}, {2,2,4,0},
        {1,3,0,0}, {2,3,6,0}, {2,3,5,0}, {3,3,5,6},
        {2,3,4,0}, {1,4,0,0}, {1,5,0,0}, {2,5,6,0},
        {1,6,0,0}, {1,7,0,0}
    };

    pid_source **srcs = pid_sources(3);
    ASSERT_NOT_NULL(srcs);
    ASSERT_EQUAL_U(18, gvector_len(srcs));
    for (size_t i = 0; i < gvector_len(srcs); ++i)
    {
        ASSERT_EQUAL_U(expected[i][0], gvector_len(srcs[i]->name));
        for (size_t j = 0; j < expected[i][0]; ++j)
        {
            ASSERT_EQUAL_U(expected[i][j+1], srcs[i]->name[j]);
        }
        pid_source_free(srcs[i]);
    }
    gvector_free(srcs);
}

BEGIN_SUITE(PID)
    ADD_UNIT(PIDSources)
END_SUITE
