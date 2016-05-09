// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <ctest.h>

#include <math.h>
#include <inform/error.h>
#include <stdio.h>

CTEST(Error, TagNaN)
{
    double x = inform_nan(0);
    uint64_t *y = (uint64_t*)&x;

    ASSERT_TRUE(isnan(x));
    ASSERT_EQUAL_U(0x7ff8000000000000, *y);

    x = inform_nan(1);
    ASSERT_TRUE(isnan(x));
    ASSERT_EQUAL_U(0x7ff8000000000001, *y);

    x = inform_nan(2);
    ASSERT_TRUE(isnan(x));
    ASSERT_EQUAL_U(0x7ff8000000000002, *y);

    x = inform_nan(15);
    ASSERT_TRUE(isnan(x));
    ASSERT_EQUAL_U(0x7ff800000000000f, *y);

    x = inform_nan(255);
    ASSERT_TRUE(isnan(x));
    ASSERT_EQUAL_U(0x7ff80000000000ff, *y);

    x = inform_nan(2251799813685247);
    ASSERT_TRUE(isnan(x));
    ASSERT_EQUAL_U(0x7fffffffffffffff, *y);

    x = inform_nan(2251799813685248);
    ASSERT_FALSE(isnan(x));
    ASSERT_EQUAL_U(0xbff0000000000000, *y);
}

CTEST(Error, NaNTag)
{
    for (uint64_t x = 0; x <= 0xffff; ++x)
    {
        ASSERT_EQUAL_U(x, inform_nan_tag(inform_nan(x)));
    }

    ASSERT_FALSE(inform_tag_is_valid(inform_nan_tag(0)));
}
