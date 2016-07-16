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
 * Compute the active information of an ensemble of time series
 *
 * @param[in] series the ensemble of time series
 * @param[in] n      the number of initial conditions
 * @param[in] m      the number of time steps in each time series
 * @param[in] b      the base or number of distinct states at each time step
 * @param[in] k      the history length used to calculate the active information
 * @return the active information for the ensemble
 */
double inform_active_info(int const *series, size_t n, size_t m, int b, size_t k);

/**
 * Compute the local active information of a ensemble of time series
 *
 * @param[in] series the ensemble of time series
 * @param[in] n      the number of initial conditions
 * @param[in] m      the number of time steps in each time series
 * @param[in] b      the base or number of distinct states at each time step
 * @param[in] k      the history length used to calculate the active information
 * @param[out] ai    the local active information
 * @return an error code
 */
int inform_local_active_info(int const *series, size_t n, size_t m, int b,
        size_t k, double *ai);

#ifdef __cplusplus
}
#endif
