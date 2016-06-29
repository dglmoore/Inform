// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <inform/error.h>
#include <inform/time_series.h>

static void accumulate_observations(uint64_t const* series, size_t n, uint64_t b,
    uint64_t k, inform_dist *states, inform_dist *histories, inform_dist *futures)
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

        inform_dist_tick(states, state);
        inform_dist_tick(histories, history);
        inform_dist_tick(futures, future);

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
    for (size_t i = 0; i < n *m; ++i)
    {
        if (b <= series[i])
        {
            return inform_nan(5);
        }
    }

    // allocate a distribution for the observed states, histories and futures
    // clear memory and return NaN if any of the allocations fail
    inform_dist *states = inform_dist_alloc(powl(b,k+1));
    if (states == NULL)
    {
        return inform_nan(6);
    }
    inform_dist *histories = inform_dist_alloc(powl(b,k));
    if (histories == NULL)
    {
        inform_dist_free(states);
        return inform_nan(6);
    }
    inform_dist *futures = inform_dist_alloc(b);
    if (futures == NULL)
    {
        inform_dist_free(states);
        inform_dist_free(histories);
        return inform_nan(6);
    }

    // for each initial condition
    for (uint64_t i = 0; i < n; ++i, series += m)
    {
        // accumulate observations
        accumulate_observations(series, m, b, k, states, histories, futures);
    }
    // compute the mututal information between the states, histories and futures,
    // i.e. the active information
    entropy ai = inform_mutual_info(states, histories, futures, b);

    // free up the distributions (otherwise there would be memory leaks)
    inform_dist_free(futures);
    inform_dist_free(histories);
    inform_dist_free(states);

    // return the active information
    return ai;
}
