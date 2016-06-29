// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <inform/error.h>
#include <inform/time_series.h>

static void accumulate_observations(uint64_t const* series, size_t n, uint64_t b,
    uint64_t k, uint64_t *states, uint64_t *histories, uint64_t *futures)
{
    uint64_t history = 0, q = 1, state, future;
    for (uint64_t i = 0; i < k; ++i)
    {
        q *= b;
        history *= b;
        history += series[i];
    }
    for (uint64_t i = k; i < n; ++i)
    {
        future = series[i];
        state  = history * b + future;

        states[state]++;
        histories[history]++;
        futures[future]++;

        history = state - series[i - k]*q;
    }
}

entropy inform_active_info(uint64_t const *series, size_t n, uint64_t b, uint64_t k)
{
    return inform_active_info_ensemble(series, 1, n, b, k);
}

entropy inform_active_info_ensemble(uint64_t const *series, size_t n, size_t m, uint64_t b, uint64_t k)
{
    // ensure that the time series is not NULL
    if (series == NULL)
    {
        return inform_nan(1);
    }
    // ensure that the dimensions of the time series make sense
    else if (m <= 1 || n < 1)
    {
        return inform_nan(2);
    }
    // ensure that the number of time steps greater than the history length
    else if (m <= k)
    {
        return inform_nan(3);
    }
    // ensure that the history length is reasonable given memory constraints
    else if (k > 25/ log2l(b))
    {
        return inform_nan(4);
    }
    // ensure that the base is compatible with the time series
    for (size_t i = 0; i < n * m; ++i)
    {
        if (b <= series[i])
        {
            return inform_nan(5);
        }
    }

    int q = pow(b,k);

    int states_size = b*q;
    int histories_size = q;
    int futures_size = b;

    uint64_t *data = calloc(states_size + histories_size + futures_size, sizeof(uint64_t));
    if (data == NULL)
    {
        return inform_nan(6);
    }	

    uint64_t *states = data;
    uint64_t *histories = states + states_size;
    uint64_t *futures = histories + histories_size;

    // for each initial condition
    for (uint64_t i = 0; i < n; ++i, series += m)
    {
        accumulate_observations(series, m, b, k, states, histories, futures);
    }

    inform_dist *states_dist = inform_dist_create(states, states_size);
    if (states_dist == NULL)
    {
        return inform_nan(7);
    }

    inform_dist *histories_dist = inform_dist_create(histories, histories_size);
    if (histories_dist == NULL)
    {
        inform_dist_free(states_dist);
        return inform_nan(8);
    }

    inform_dist *futures_dist = inform_dist_create(futures, futures_size);
    if (futures_dist == NULL)
    {
        inform_dist_free(histories_dist);
        inform_dist_free(states_dist);
        return inform_nan(9);
    }

    entropy ai = inform_mutual_info(states_dist, histories_dist, futures_dist, b);

    inform_dist_free(futures_dist);
    inform_dist_free(histories_dist);
    inform_dist_free(states_dist);

    free(data);

    // return the active information
    return ai;
}
