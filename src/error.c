// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <inform/error.h>

#include <math.h>

/// The largest admissable NaN tag
static uint64_t const inform_max_tag = 0x0007ffffffffffff;
/// The binary encoding of the zero-tagged NaN
static uint64_t const inform_raw_nan = 0x7ff8000000000000;

bool inform_tag_is_valid(uint64_t tag)
{
    return tag <= inform_max_tag;
}

double inform_nan(uint64_t tag)
{
    if (!inform_tag_is_valid(tag))
    {
        return -1.;
    }
    tag ^= inform_raw_nan;
    double *nan = (double*)&tag;
    return *nan;
}

uint64_t inform_nan_tag(double x)
{
    if (!isnan(x))
    {
        return inform_max_tag + 1;
    }
    else
    {
        uint64_t *y = (uint64_t*)&x;
        return *y ^ inform_raw_nan;
    }
}
