// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <inform/active_info.h>

static void accumulate_observations(int const* series, size_t n, int b,
    size_t k, inform_dist *states, inform_dist *histories, inform_dist *futures)
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
        futures->histogram[future]++;

        history = state - series[i - k]*q;
    }
}

static void accumulate_local_observations(int const* series, size_t n, int b,
    size_t k, inform_dist *states, inform_dist *histories, inform_dist *futures,
    int *state, int *history, int *future)
{
    history[0] = 0;
    int q = 1;
    for (size_t i = 0; i < k; ++i)
    {
        q *= b;
        history[0] *= b;
        history[0] += series[i];
    }
    for (size_t i = k; i < n; ++i)
    {
        size_t l = i - k;
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

double inform_active_info(int const *series, size_t n, size_t m, int b, size_t k, inform_error *err)
{
    if (series == NULL)
    {
        INFORM_ERROR_RETURN(err, INFORM_EINVAL, "time series is NULL", NAN);
    }
    else if (n < 1)
    {
        INFORM_ERROR_RETURN(err, INFORM_EINVAL, "time series has no initial conditions", NAN);
    }
    else if (m < 2)
    {
        INFORM_ERROR_RETURN(err, INFORM_EINVAL, "time series has less than two timesteps", NAN);
    }
    else if (m <= k)
    {
        INFORM_ERROR_RETURN(err, INFORM_EINVAL, "history length is too long for the timeseries", NAN);
    }
    else if (b < 2)
    {
        INFORM_ERROR_RETURN(err, INFORM_EINVAL, "base is less than two", NAN);
    }
    else if (k == 0)
    {
        INFORM_ERROR_RETURN(err, INFORM_EINVAL, "history length is zero", NAN);
    }
    for (size_t i = 0; i < n * m; ++i)
    {
        if (series[i] < 0)
        {
            INFORM_ERROR_RETURN(err, INFORM_EINVAL, "time series has negative states", NAN);
        }
        else if (b <= series[i])
        {
            INFORM_ERROR_RETURN(err, INFORM_EINVAL, "time series has states inconsistent with the expected base", NAN);
        }
    }

    // compute the number of observations to be made
    size_t const N = n * (m - k);

    // compute the sizes of the various histograms
    size_t const states_size = (size_t) (b * pow((double) b,(double) k));
    size_t const histories_size = states_size / b;
    size_t const futures_size = b;
    size_t const total_size = states_size + histories_size + futures_size;

    uint32_t *data = calloc(total_size, sizeof(uint32_t));
    if (data == NULL)
    {
        INFORM_ERROR_RETURN(err, INFORM_ENOMEM, "failed to allocate distribution histograms", NAN);
    }

    inform_dist states    = { data, states_size, N };
    inform_dist histories = { data + states_size, histories_size, N };
    inform_dist futures   = { data + states_size + histories_size, futures_size, N };

    // for each initial condition
    for (size_t i = 0; i < n; ++i, series += m)
    {
        // allocate the observations
        accumulate_observations(series, m, b, k, &states, &histories, &futures);
    }

    // compute the active information
    double ai = inform_shannon_mi(&states, &histories, &futures, (double) b);

    // free up the data array
    free(data);

    // return the active information
    return ai;
}

int inform_local_active_info(int const *series, size_t n, size_t m,
    int b, size_t k, double *ai)
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
    else if (b < 2)
    {
        return 5;
    }
    else if (k == 0 || k > 25 / log2((double) b))
    {
        return 6;
    }
    for (size_t i = 0; i < n * m; ++i)
    {
        if (b <= series[i] || series[i] < 0)
        {
            return 7;
        }
    }

    // compute the number of observations to be made
    size_t const N = n * (m - k);

    // compute the sizes of the various histograms
    size_t const states_size = (size_t) (b*pow((double) b,(double) k));
    size_t const histories_size = states_size / b;
    size_t const futures_size = b;
    size_t const total_size = states_size + histories_size + futures_size;

    uint32_t *data = calloc(total_size, sizeof(uint32_t));
    if (data == NULL)
    {
        return 6;
    }

    inform_dist states    = { data, states_size, N };
    inform_dist histories = { data + states_size, histories_size, N };
    inform_dist futures   = { data + states_size + histories_size, futures_size, N };

    int *state   = malloc(N * sizeof(int));
    int *history = malloc(N * sizeof(int));
    int *future  = malloc(N * sizeof(int));

    int const *series_ptr = series;
    int *state_ptr = state, *history_ptr = history, *future_ptr = future;
    for (size_t i = 0; i < n; ++i)
    {
        accumulate_local_observations(series_ptr, m, b, k, &states, &histories,
            &futures, state_ptr, history_ptr, future_ptr);
        series_ptr += m;
        state_ptr += (m - k);
        history_ptr += (m - k);
        future_ptr += (m - k);
    }

    for (size_t i = 0; i < N; ++i)
    {
        ai[i] = inform_shannon_pmi(&states, &histories, &futures, state[i],
            history[i], future[i], (double) b);
    }

    // free up the data array
    free(future);
    free(history);
    free(state);
    free(data);

    // return the error code
    return 0;
}
