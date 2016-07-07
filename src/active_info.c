// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <inform/error.h>
#include <inform/active_info.h>

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

        states->histogram[state]++;
        histories->histogram[history]++;
        futures->histogram[future]++;

        history = state - series[i - k]*q;
    }
}

static void accumulate_local_observations(uint64_t const* series, size_t n, uint64_t b,
    uint64_t k, inform_dist *states, inform_dist *histories, inform_dist *futures,
    uint64_t *state, uint64_t *history, uint64_t *future)
{
    history[0] = 0;
    uint64_t q = 1;
    for (uint64_t i = 0; i < k; ++i)
    {
        q *= b;
        history[0] *= b;
        history[0] += series[i];
    }
    for (uint64_t i = k; i < n; ++i)
    {
        uint64_t l = i - k;
        future[l] = series[i];
        state[l] = history[l] * b + future[l];

            states->histogram[state[l]]++;
            histories->histogram[history[l]]++;
            futures->histogram[future[l]]++;

        if (i + 1 != n)
        {
            history[l + 1] = state[l] - series[l]*q;
        }
    }
}

double inform_active_info(uint64_t const *series, size_t n, size_t m, uint64_t b, uint64_t k)
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

    // compute the number of observations to be made
    int const N = n * (m - k);

    // compute the sizes of the various histograms
    int const states_size = b*pow(b,k);
    int const histories_size = states_size / b;
    int const futures_size = b;
    int const total_size = states_size + histories_size + futures_size;

    uint64_t *data = calloc(total_size, sizeof(uint64_t));
    if (data == NULL)
    {
        return inform_nan(6);
    }

    inform_dist states    = { data, states_size, N };
    inform_dist histories = { data + states_size, histories_size, N };
    inform_dist futures   = { data + states_size + histories_size, futures_size, N };

    // for each initial condition
    for (uint64_t i = 0; i < n; ++i, series += m)
    {
        // allocate the observations
        accumulate_observations(series, m, b, k, &states, &histories, &futures);
    }

    // compute the active information
    double ai = inform_mutual_info(&states, &histories, &futures, b);

    // free up the data array
    free(data);

    // return the active information
    return ai;
}

int inform_local_active_info(uint64_t const *series, size_t n, size_t m,
    uint64_t b, uint64_t k, double *ai)
{
    if (series == NULL)
    {
        return 1;
    }
    else if (ai == NULL)
    {
        return 2;
    }
    else if (m <= 1 || n < 1)
    {
        return 3;
    }
    else if (m <= k)
    {
        return 4;
    }
    else if (k > 25/ log2l(b))
    {
        return 5;
    }
    for (size_t i = 0; i < n * m; ++i)
    {
        if (b <= series[i])
        {
            return 6;
        }
    }

    // compute the number of observations to be made
    int const N = n * (m - k);

    // compute the sizes of the various histograms
    int const states_size = b*pow(b,k);
    int const histories_size = states_size / b;
    int const futures_size = b;
    int const total_size = states_size + histories_size + futures_size;

    uint64_t *data = calloc(total_size, sizeof(uint64_t));
    if (data == NULL)
    {
        return 6;
    }

    inform_dist states    = { data, states_size, N };
    inform_dist histories = { data + states_size, histories_size, N };
    inform_dist futures   = { data + states_size + histories_size, futures_size, N };

    uint64_t *state   = malloc(n * (m - k) * sizeof(uint64_t));
    uint64_t *history = malloc(n * (m - k) * sizeof(uint64_t));
    uint64_t *future  = malloc(n * (m - k) * sizeof(uint64_t));

    uint64_t const *series_ptr = series;
    uint64_t *state_ptr = state, *history_ptr = history, *future_ptr = future;
    for (uint64_t i = 0; i < n; ++i)
    {
        accumulate_local_observations(series_ptr, m, b, k, &states, &histories,
            &futures, state_ptr, history_ptr, future_ptr);
        series_ptr += m;
        state_ptr += (m - k);
        history_ptr += (m - k);
        future_ptr += (m - k);
    }

    for (size_t i = 0; i < n * (m - k); ++i)
    {
        ai[i] = inform_shannon_pmi(&states, &histories, &futures, state[i],
            history[i], future[i], b);
    }

    // free up the data array
    free(future);
    free(history);
    free(state);
    free(data);

    // return the error code
    return 0;
}
