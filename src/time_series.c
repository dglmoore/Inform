// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <inform/state_encoding.h>
#include <inform/time_series.h>

static void inform_active_info_dist(int const* series, size_t n,
                                    uint64_t k, int base, inform_dist *states,
                                    inform_dist *histories,
                                    inform_dist *futures)
{
    int const *last   = series + n;
    int const *future = series + k;
    while (future != last)
    {
        uint64_t const history = inform_encode_base(series, k, base);
        uint64_t const state   = history + (*future * pow(base,k));

        inform_dist_tick(states, state);
        inform_dist_tick(histories, history);
        inform_dist_tick(futures, *future);

        ++series;
        ++future;
    }
}

entropy inform_active_info(int const *series, size_t n, int base, uint64_t k)
{
    return inform_active_info_ensemble(series, 1, n, base, k);
}

entropy inform_active_info_ensemble(int const *series, size_t n, size_t m, int base, uint64_t k)
{
    if (m <= 1 || n < 1)
    {
        return nan("1");
    }
    else if (m <= k)
    {
        return nan("2");
    }

    inform_dist *states    = inform_dist_alloc(pow(base,k+1));
    inform_dist *histories = inform_dist_alloc(pow(base,k));
    inform_dist *futures   = inform_dist_alloc(base);

    for (size_t i = 0; i < n; ++i, series += m)
    {
        inform_active_info_dist(series, m, k, base, states, histories, futures);
    }
    entropy ai = inform_mutual_info(states, histories, futures);

    inform_dist_free(futures);
    inform_dist_free(histories);
    inform_dist_free(states);

    return ai;
}
