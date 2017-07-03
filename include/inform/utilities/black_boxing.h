// Copyright 2016-2017 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#pragma once

#include <inform/error.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Black box a collection of time series, of various bases, history lengths,
 * and future lengths.
 *
 * @param[in] series    the time series
 * @param[in] l         the number of time series
 * @param[in] n         the number of initial conditions in each time series
 * @param[in] m         the number of time steps for each initial condition
 * @param[in] b         the base of each time series
 * @param[in] r         the history length for each time series
 * @param[in] s         the future length for each time series
 * @param[in,out] box   the array in which to put the black boxed time series
 * @param[in,out] err   an error code
 * @return the black boxed time series
 */
EXPORT int *inform_black_box(int const *series, size_t l, size_t n, size_t m,
    int const *b, size_t const *r, size_t const *s, int *box,
    inform_error *err);

#ifdef __cplusplus
}
#endif
