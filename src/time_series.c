// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <inform/state_encoding.h>
#include <inform/time_series.h>

static int inform_active_info_dist(uint64_t const* series, size_t n,
                                    uint64_t k, uint64_t base, inform_dist *states,
                                    inform_dist *histories,
                                    inform_dist *futures)
{
    // store a pointer to the end of the time series
    uint64_t const *last   = series + n;
    // store a pointer to the future state
    uint64_t const *future = series + k;
    // while the future state is in the time series
    while (future != last)
    {
        // encode the k-length history as an integer
        uint64_t const history = inform_encode(series, k, base);
        // if the encoding failed, return an error code
        if (history >= INFORM_ENCODING_ERROR(0))
        {
            return 1;
        }
        // construct an encoding of the state (history + future) as an integer
        uint64_t const state   = history + (*future * powl(base, k));

        // log observations of the state, history and future
        inform_dist_tick(states, state);
        inform_dist_tick(histories, history);
        inform_dist_tick(futures, *future);

        // move to the next time step
        ++series;
        ++future;
    }
    return 0;
}

entropy inform_active_info(uint64_t const *series, size_t n, uint64_t base, uint64_t k)
{
    return inform_active_info_ensemble(series, 1, n, base, k);
}

entropy inform_active_info_ensemble(uint64_t const *series, size_t n, size_t m, uint64_t base, uint64_t k)
{
    // ensure that the time series is not NULL
    if (series == NULL)
    {
        return nan("1");
    }
    // ensure that the dimensions of the time series make sense
    else if (m <= 1 || n < 1)
    {
        return nan("2");
    }
    // ensure that the number of time steps greater than the history length
    else if (m <= k)
    {
        return nan("3");
    }
    
    // allocate a distribution for the observed states, histories and futures
    inform_dist *states    = inform_dist_alloc(powl(base,k+1));
    inform_dist *histories = inform_dist_alloc(powl(base,k));
    inform_dist *futures   = inform_dist_alloc(base);

    // for each initial condition
    for (uint64_t i = 0; i < n; ++i, series += m)
    {
        // accumulate observations
        if (inform_active_info_dist(series, m, k, base, states, histories, futures) != 0)
        {
            inform_dist_free(futures);
            inform_dist_free(histories);
            inform_dist_free(states);
            return nan("4");
        }
    }
    // compute the mututal information between the states, histories and futures, 
    // i.e. the active information
    entropy ai = inform_mutual_info(states, histories, futures, base);

    // free up the distributions (otherwise there would be memory leaks)
    inform_dist_free(futures);
    inform_dist_free(histories);
    inform_dist_free(states);

    // return the active information
    return ai;
}
