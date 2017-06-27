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
    for (size_t i = kpast + kfuture; i < n; ++i)
    {
        state = history * r + future;

        states->histogram[state]++;
        histories->histogram[history]++;
        futures->histogram[future]++;

        history = history * b - series[i - kpast - kfuture]*q + series[i - kfuture];
        future = future * b - series[i - kfuture]*r + series[i];
    }

    state = history * r + future;

    states->histogram[state]++;
    histories->histogram[history]++;
    futures->histogram[future]++;
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

    double ai = inform_shannon_mi(&states, &histories, &futures, 2.0);

    free(data);

    return ai;
}
