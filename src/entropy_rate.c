// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <inform/error.h>
#include <inform/state_encoding.h>
#include <inform/time_series.h>

int inform_entropy_rate_dist(uint64_t const* series, size_t n,
        uint64_t b, uint64_t k, inform_dist *states, inform_dist *histories)
{
    // store a pointer to the end of the time series
    uint64_t const *last   = series + n;
    // store a pointer to the future state
    uint64_t const *future = series + k;
    // while the future state is in the time series
    while (future != last)
    {
        // encode the k-length history as an integer
        uint64_t const history = inform_encode(series, k, b);
        // if the encoding failed, return an error code
        if (history >= INFORM_ENCODING_ERROR(0))
        {
            return 1;
        }
        // construct an encoding of the state (history + future) as an integer
        uint64_t const state = history + (*future * powl(b, k));

        // log observations of the state, history and future
        inform_dist_tick(states, state);
        inform_dist_tick(histories, history);

        // move to the next time step
        ++series;
        ++future;
    }
    return 0;
}

entropy inform_entropy_rate(uint64_t const *series, size_t n, uint64_t b, uint64_t k)
{
    return inform_entropy_rate_ensemble(series, 1, n, b, k);
}

entropy inform_entropy_rate_ensemble(uint64_t const *series, size_t n, size_t m, uint64_t b, uint64_t k)
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

    // allocate a distribution for the observed states and histories
    // clear memory and return NaN if any of the allocations fail
    inform_dist *states = inform_dist_alloc(powl(b,k+1));
    if (states == NULL)
    {
        return inform_nan(5);
    }
    inform_dist *histories = inform_dist_alloc(powl(b,k));
    if (histories == NULL)
    {
        inform_dist_free(states);
        return inform_nan(5);
    }

    // for each initial condition
    for (uint64_t i = 0; i < n; ++i, series += m)
    {
        // accumulate observations
        if (inform_entropy_rate_dist(series, m, b, k, states, histories) != 0)
        {
            inform_dist_free(histories);
            inform_dist_free(states);
            return inform_nan(6);
        }
    }
    // compute the conditional entropy etween the states and histories,
    // i.e. the entropy rate
    entropy er = inform_conditional_entropy(states, histories, b);

    // free up the distributions (otherwise there would be memory leaks)
    inform_dist_free(histories);
    inform_dist_free(states);

    // return the active information
    return er;
}
