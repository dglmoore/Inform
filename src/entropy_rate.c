// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <inform/error.h>
#include <inform/state_encoding.h>
#include <inform/entropy_rate.h>

static void accumulate_observations(int const* series, size_t n,
    int b, size_t k, inform_dist *states, inform_dist *histories)
{
    int history = 0, q = 1, state, future;
    for (size_t i = 0; i < k; ++i)
    {
        q *= b;
        history *= b;
        history += series[i];
    }
    for (size_t i = k; i < n; ++i)
    {
        future = series[i];
        state  = history * b + future;

        states->histogram[state]++;
        histories->histogram[history]++;

        history = state - series[i - k]*q;
    }
}

static void accumulate_local_observations(int const* series, size_t n,
    int b, size_t k, inform_dist *states, inform_dist *histories,
    int *state, int *history)
{
    int q = 1;
    history[0] = 0;
    for (size_t i = 0; i < k; ++i)
    {
        q *= b;
        history[0] *= b;
        history[0] += series[i];
    }
    for (size_t i = k; i < n; ++i)
    {
        size_t l = i - k;
        state[l]  = history[l] * b + series[i];

        states->histogram[state[l]]++;
        histories->histogram[history[l]]++;

        if (i + 1 != n)
        {
            history[l + 1] = state[l] - series[l]*q;
        }
    }
}

double inform_entropy_rate(int const *series, size_t n, size_t m, int b, size_t k)
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
    // ensure that the base is at least 2
    else if (b < 2)
    {
        return inform_nan(4);
    }
    // ensure that the history length is reasonable given memory constraints
    else if (k == 0 || k > 25 / log2((double) b))
    {
        return inform_nan(5);
    }
    // ensure that the base is compatible with the time series
    for (size_t i = 0; i < n * m; ++i)
    {
        if (b <= series[i] || series[i] < 0)
        {
            return inform_nan(6);
        }
    }

    size_t const N = n * (m - k);

    // compute the sizes of the histograms
    size_t const states_size = (size_t) (b * pow((double) b, (double) k));
    size_t const histories_size = states_size / b;
    size_t const total_size = states_size + histories_size;

    // allocate memory to store the histograms
    uint64_t *data = calloc(total_size, sizeof(uint64_t));
    // ensure that the memory was allocated
    if (data == NULL)
    {
        return inform_nan(7);
    }

    // create the distributions
    inform_dist states    = { data, states_size, N };
    inform_dist histories = { data + states_size, histories_size, N };

    // for each initial condition
    for (size_t i = 0; i < n; ++i, series += m)
    {
        // accumulate the observations
        accumulate_observations(series, m, b, k, &states, &histories);
    }

    // compute the entropy rate
    double er = inform_shannon_ce(&states, &histories, (double) b);

    // free up the data array
    free(data);

    // return the active information
    return er;
}

int inform_local_entropy_rate(int const *series, size_t n, size_t m, int b, size_t k, double *er)
{
    // ensure that the time series is not NULL
    if (series == NULL)
    {
        return 1;
    }
    // ensure that the entropy rate array is not NULL
    else if (er == NULL)
    {
        return 2;
    }
    // ensure that the dimensions of the time series make sense
    else if (m <= 1 || n < 1)
    {
        return 3;
    }
    // ensure that the number of time steps greater than the history length
    else if (m <= k)
    {
        return 4;
    }
    // ensure that the base is at least 2
    else if (b < 2)
    {
        return 5;
    }
    // ensure that the history length is reasonable given memory constraints
    else if (k == 0 || k > 25 / log2((double) b))
    {
        return 6;
    }
    // ensure that the base is compatible with the time series
    for (size_t i = 0; i < n * m; ++i)
    {
        if (b <= series[i] || series[i] < 0)
        {
            return 7;
        }
    }

    size_t const N = n * (m - k);

    // compute the sizes of the histograms
    size_t const states_size = (size_t) (b * pow((double) b, (double) k));
    size_t const histories_size = states_size / b;
    size_t const total_size = states_size + histories_size;

    // allocate memory to store the histograms
    uint64_t *data = calloc(total_size, sizeof(uint64_t));
    // ensure that the memory was allocated
    if (data == NULL)
    {
        return 7;
    }

    // create the distributions
    inform_dist states    = { data, states_size, N };
    inform_dist histories = { data + states_size, histories_size, N };

    int *state = malloc(N * sizeof(uint64_t));
    int *history = malloc(N * sizeof(uint64_t));

    // for each initial condition
    int const *series_ptr = series;
    int *state_ptr = state, *history_ptr = history;
    for (size_t i = 0; i < n; ++i)
    {
        // accumulate the observations
        accumulate_local_observations(series_ptr, m, b, k, &states, &histories,
            state_ptr, history_ptr);
        series_ptr += m;
        state_ptr += (m - k);
        history_ptr += (m - k);
    }

    // compute the entropy rate
    for (size_t i = 0; i < N; ++i)
    {
        er[i] = inform_shannon_pce(&states, &histories, state[i], history[i],
            (double) b);
    }

    // free up the data array
    free(data);

    // return the active information
    return 0;
}
