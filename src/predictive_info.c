// Copyright 2016-2017 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <inform/predictive_info.h>
#include <inform/shannon.h>

static void accumulate_observations(int const* series, size_t n, int b,
    size_t kpast, size_t kfuture, inform_dist *states, inform_dist *histories,
    inform_dist *futures)
{
    int history = 0, q = 1, r = 1, state, future = 0;
    for (size_t i = 0; i < kpast; ++i)
    {
        q *= b;
        history *= b;
        history += series[i];
    }

    for (size_t i = kpast; i < kpast + kfuture; ++i)
    {
        r *= b;
        future *= b;
        future += series[i];
    }

    size_t i = kpast + kfuture;
    do
    {
        state = history * r + future;

        states->histogram[state]++;
        histories->histogram[history]++;
        futures->histogram[future]++;

        history = history * b - series[i - kpast - kfuture]*q + series[i - kfuture];
        future = future * b - series[i - kfuture]*r + series[i];
    } while (++i <= n);
}

static void accumulate_local_observations(int const* series, size_t n, int b,
    size_t kpast, size_t kfuture, inform_dist *states, inform_dist *histories,
    inform_dist *futures, int *state, int *history, int *future)
{
    history[0] = 0;
    int q = 1;
    for (size_t i = 0; i < kpast; ++i)
    {
        q *= b;
        history[0] *= b;
        history[0] += series[i];
    }
    
    future[0] = 0;
    int r = 1;
    for (size_t i = kpast; i < kpast + kfuture; ++i)
    {
        r *= b;
        future[0] *= b;
        future[0] += series[i];
    }

    size_t i = kpast + kfuture;
    do
    {
        size_t l = i - kpast - kfuture;
        state[l] = history[l] * r + future[l];

        states->histogram[state[l]]++;
        histories->histogram[history[l]]++;
        futures->histogram[future[l]]++;

        if (i != n)
        {
            history[l + 1] = history[l] * b - series[l]*q + series[i - kfuture];
            future[l + 1] = future[l] * b - series[i - kfuture]*r + series[i];
        }
    } while (++i <= n);
}

static bool check_arguments(int const *series, size_t n, size_t m, int b,
    size_t kpast, size_t kfuture, inform_error *err)
{
    if (series == NULL)
    {
        INFORM_ERROR_RETURN(err, INFORM_ETIMESERIES, true);
    }
    else if (n < 1)
    {
        INFORM_ERROR_RETURN(err, INFORM_ENOINITS, true);
    }
    else if (m < 2)
    {
        INFORM_ERROR_RETURN(err, INFORM_ESHORTSERIES, true);
    }
    else if (b < 2)
    {
        INFORM_ERROR_RETURN(err, INFORM_EBASE, true);
    }
    else if (kpast == 0 || kfuture == 0)
    {
        INFORM_ERROR_RETURN(err, INFORM_EKZERO, true);
    }
    else if (m <= kpast + kfuture)
    {
        INFORM_ERROR_RETURN(err, INFORM_EKLONG, true);
    }
    for (size_t i = 0; i < n * m; ++i)
    {
        if (series[i] < 0)
        {
            INFORM_ERROR_RETURN(err, INFORM_ENEGSTATE, true);
        }
        else if (b <= series[i])
        {
            INFORM_ERROR_RETURN(err, INFORM_EBADSTATE, true);
        }
    }
    return false;
}

double inform_predictive_info(int const *series, size_t n, size_t m, int b,
    size_t kpast, size_t kfuture, inform_error *err)
{
    if (check_arguments(series, n, m, b, kpast, kfuture, err)) return NAN;

    size_t const N = n * (m - kpast - kfuture + 1);

    size_t const histories_size = (size_t) pow((double) b, (double) kpast);
    size_t const futures_size = (size_t) pow((double) b, (double) kfuture);
    size_t const states_size = histories_size * futures_size;
    size_t const total_size = states_size + histories_size + futures_size;

    uint32_t *data = calloc(total_size, sizeof(uint32_t));
    if (data == NULL)
    {
        INFORM_ERROR_RETURN(err, INFORM_ENOMEM, NAN);
    }

    inform_dist states    = { data, states_size, N };
    inform_dist histories = { data + states_size, histories_size, N };
    inform_dist futures   = { data + states_size + histories_size, futures_size, N };

    for (size_t i = 0; i < n; ++i, series += m)
    {
        accumulate_observations(series, m, b, kpast, kfuture, &states,
            &histories, &futures);
    }

    double pi = inform_shannon_mi(&states, &histories, &futures, 2.0);

    free(data);

    return pi;
}

double *inform_local_predictive_info(int const *series, size_t n, size_t m,
    int b, size_t kpast, size_t kfuture, double *pi, inform_error *err)
{
    if (check_arguments(series, n, m, b, kpast, kfuture, err)) return NULL;

    size_t const N = n * (m - kpast - kfuture + 1);

    bool allocate_pi = (pi == NULL);
    if (allocate_pi)
    {
        pi = malloc(N * sizeof(double));
        if (pi == NULL)
        {
            INFORM_ERROR_RETURN(err, INFORM_ENOMEM, NULL);
        }
    }

    size_t const histories_size = (size_t) pow((double) b, (double) kpast);
    size_t const futures_size = (size_t) pow((double) b, (double) kfuture);
    size_t const states_size = histories_size * futures_size;
    size_t const total_size = states_size + histories_size + futures_size;

    uint32_t *histogram_data = calloc(total_size, sizeof(uint32_t));
    if (histogram_data == NULL)
    {
        if (allocate_pi) free(pi);
        INFORM_ERROR_RETURN(err, INFORM_ENOMEM, NULL);
    }

    inform_dist states    = { histogram_data, states_size, N };
    inform_dist histories = { histogram_data + states_size, histories_size, N };
    inform_dist futures   = { histogram_data + states_size + histories_size, futures_size, N };

    int *state_data = malloc(3 * N * sizeof(int));
    if (state_data == NULL)
    {
        if (allocate_pi) free(pi);
        free(histogram_data);
        INFORM_ERROR_RETURN(err, INFORM_ENOMEM, NULL);
    }
    int *state   = state_data;
    int *history = state + N;
    int *future  = history + N;

    int const *series_ptr = series;
    int *state_ptr = state, *history_ptr = history, *future_ptr = future;
    for (size_t i = 0; i < n; ++i)
    {
        accumulate_local_observations(series_ptr, m, b, kpast, kfuture, &states,
            &histories, &futures, state_ptr, history_ptr, future_ptr);
        series_ptr += m;
        state_ptr += (m - kpast - kfuture + 1);
        history_ptr += (m - kpast - kfuture + 1);
        future_ptr += (m - kpast - kfuture + 1);
    }

    for (size_t i = 0; i < N; ++i)
    {
        pi[i] = inform_shannon_pmi(&states, &histories, &futures, state[i],
            history[i], future[i], 2.0);
    }

    free(state_data);
    free(histogram_data);

    return pi;
}
