// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#pragma once

#include <inform/shannon.h>

#include <assert.h>
static_assert(sizeof(int) >= 4, "int must be at least 32-bits");

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Compute the entropy rate of an ensemble of time series
 *
 * @param[in] series the ensemble of time series
 * @param[in] n      the number of initial conditions
 * @param[in] m      the number of time steps in each time series
 * @param[in] b      the base or number of distinct states at each time step
 * @param[in] k      the history length used to calculate the entropy rate
 * @return the entropy rate for the ensemble
 */
double inform_entropy_rate(int const *series, size_t n, size_t m, int b,
    size_t k);

/**
 * Compute the entropy rate of an ensemble of time series
 *
 * @param[in] series the ensemble of time series
 * @param[in] n      the number of initial conditions
 * @param[in] m      the number of time steps in each time series
 * @param[in] b      the base or number of distinct states at each time step
 * @param[in] k      the history length used to calculate the entropy rate
 * @param[out] err   the local entropy rate of the ensemble
 * @return an error code
 */
int inform_local_entropy_rate(int const *series, size_t n, size_t m, int b,
    size_t k, double *er);

#ifdef __cplusplus
}
#endif
