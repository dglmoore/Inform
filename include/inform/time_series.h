// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#pragma once

#include <inform/entropy.h>

/**
 * @brief Compute the active information of a time series.
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
 * @param[in] k      the history length used to calculate the active information
 * @return the active information
 *
 * @see inform_active_info_ensemble
 */
entropy inform_active_info(uint64_t const *series, size_t n, uint64_t base, uint64_t k);

/**
 * Compute the active information of an ensemble of time series.
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
 * @param[in] k      the history length used to calculate the active information
 * @return the active information for the ensemble
 *
 * @see inform_active_info
 */
entropy inform_active_info_ensemble(uint64_t const *series, size_t n, size_t m, uint64_t base, uint64_t k);

/**
 * Compute the transfer entropy from one time series to another.
 *
 * `NaN` is returned in at least the following erroneous situtations
 *   - `series_x == NULL || series_y == NULL`
 *   - `n == 0`
 *   - `m <= 1`
 *   - `m <= k`
 *   - `series_x[i] >= b` for some `0 <= i && i < n`
 *   - `series_y[i] >= b` for some `0 <= k-1 && i < n-1`
 *
 * @param[in] series_y the time series of the source node
 * @param[in] series_x the time series of the target node
 * @param[in] n        the number of time steps in each time series
 * @param[in] b        the base or number of distinct states at each time step
 * @param[in] k        the history length used to calculate the transfer entropy
 * @return the transfer entropy
 */
entropy inform_transfer_entropy(uint64_t const *series_y, uint64_t const *series_x, size_t n, uint64_t b, uint64_t k);
/**
 * Compute the transfer entropy from one time series to another.
 *
 * `NaN` is returned in at least the following erroneous situtations
 *   - `series_x == NULL || series_y == NULL`
 *   - `n == 0`
 *   - `m <= 1`
 *   - `m <= k`
 *   - `series_x[i] >= b` for some `0 <= i && i < n`
 *   - `series_y[i] >= b` for some `0 <= k-1 && i < n-1`
 *
 * @param[in] series_y the ensemble of the source node
 * @param[in] series_x the ensemble of the target node
 * @param[in] n        the number initial conditions
 * @param[in] m        the number of time steps in each time series
 * @param[in] b        the base or number of distinct states at each time step
 * @param[in] k        the history length used to calculate the transfer entropy
 * @return the transfer entropy of the ensemble
 */
entropy inform_transfer_entropy_ensemble(uint64_t const *series_y, uint64_t const *series_x, size_t n, size_t m, uint64_t b, uint64_t k);