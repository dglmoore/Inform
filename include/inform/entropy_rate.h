// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#pragma once

#include <inform/entropy.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Compute the entropy rate of a time series.
 *
 * `NaN` is returned in at least the following erroneous situtations
 *   - `series == NULL`
 *   - `n == 0`
 *   - `n <= k`
 *   - `series[i] >= b` for some `0 <= i && i < n`
 *
 * @param[in] series the time series of values
 * @param[in] n      the number of time steps in the time series
 * @param[in] b      the base or number of distinct states at each time step
 * @param[in] k      the history length used to calculate the entropy rate
 * @return the entropy rate
 *
 * @see inform_entropy_rate_ensemble
 */
double inform_entropy_rate(uint64_t const *series, size_t n, uint64_t b, uint64_t k);

/**
 * Compute the entropy rate of an ensemble of time series.
 *
 * `NaN` is returned in at least the following erroneous situtations
 *   - `series == NULL`
 *   - `n == 0`
 *   - `m <= 1`
 *   - `m <= k`
 *   - `series[i] >= b` for some `0 <= i && i < n*m`
 *
 * @param[in] series the ensemble of time series
 * @param[in] n      the number of initial conditions
 * @param[in] m      the number of time steps in each time series
 * @param[in] b      the base or number of distinct states at each time step
 * @param[in] k      the history length used to calculate the entropy rate
 * @return the entropy rate for the ensemble
 *
 * @see inform_entropy_rate
 */
double inform_entropy_rate_ensemble(uint64_t const *series, size_t n, size_t m, uint64_t b, uint64_t k);

/**
 * @brief Compute the entropy rate of a time series.
 *
 * @param[in] series the time series of values
 * @param[in] n      the number of time steps in the time series
 * @param[in] b      the base or number of distinct states at each time step
 * @param[in] k      the history length used to calculate the entropy rate
 * @param[out] er    the array of local entropy rates
 * @return an error code
 *
 * @see inform_entropy_rate_ensemble
 */
int inform_local_entropy_rate(uint64_t const *series, size_t n, uint64_t b, uint64_t k, double *er);

/**
 * Compute the entropy rate of an ensemble of time series.
 *
 * @param[in] series the ensemble of time series
 * @param[in] n      the number of initial conditions
 * @param[in] m      the number of time steps in each time series
 * @param[in] b      the base or number of distinct states at each time step
 * @param[in] k      the history length used to calculate the entropy rate
 * @param[out] err   the local entropy rate of the ensemble
 * @return an error code
 *
 * @see inform_entropy_rate
 */
int inform_local_entropy_rate_ensemble(uint64_t const *series, size_t n, size_t m, uint64_t b, uint64_t k, double *er);

#ifdef __cplusplus
}
#endif
