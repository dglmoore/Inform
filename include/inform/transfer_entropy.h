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
 * Compute the transfer entropy from one time series to another
 *
 * @param[in] series_y the ensemble of the source node
 * @param[in] series_x the ensemble of the target node
 * @param[in] n        the number initial conditions
 * @param[in] m        the number of time steps in each time series
 * @param[in] b        the base or number of distinct states at each time step
 * @param[in] k        the history length used to calculate the transfer entropy
 * @return the transfer entropy of the ensemble
 */
double inform_transfer_entropy(int const *series_y, int const *series_x,
    size_t n, size_t m, int b, size_t k);

/**
 * Compute the transfer entropy from one time series to another
 *
 * @param[in] series_y the ensemble of the source node
 * @param[in] series_x the ensemble of the target node
 * @param[in] n        the number initial conditions
 * @param[in] m        the number of time steps in each time series
 * @param[in] b        the base or number of distinct states at each time step
 * @param[in] k        the history length used to calculate the transfer entropy
 * @param[out] te      the transfer entropy
 * @return an error code
 */
int inform_local_transfer_entropy(int const *series_y, int const *series_x,
    size_t n, size_t m, int b, size_t k, double *te);

#ifdef __cplusplus
}
#endif
