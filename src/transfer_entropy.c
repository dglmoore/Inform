// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <inform/transfer_entropy.h>

static void accumulate_observations(int const *series_y, int const *series_x,
     size_t n, uint64_t b, size_t k, inform_dist *states,
     inform_dist *histories, inform_dist *sources, inform_dist *predicates)
{
    int history = 0, q = 1, y_state, future, state, source, predicate;
    for (size_t i = 0; i < k; ++i)
    {
        q *= b;
        history *= b;
        history += series_x[i];
    }
    for (size_t i = k; i < n; ++i)
    {
        y_state   = series_y[i-1];
        future    = series_x[i];
        state     = (history * b + future) * b + y_state;
        source    = history * b + y_state;
        predicate = history * b + future;

        states->histogram[state]++;
        histories->histogram[history]++;
        sources->histogram[source]++;
        predicates->histogram[predicate]++;

        history = predicate - series_x[i - k]*q;
    }
}

static void accumulate_local_observations(int const *series_y,
    int const *series_x, size_t n, int b, size_t k,
    inform_dist *states, inform_dist *histories, inform_dist *sources,
    inform_dist *predicates, int *state, int *history,
    int *source, int *predicate)
{
    history[0] = 0;
    size_t q = 1;
    for (size_t i = 0; i < k; ++i)
    {
        q *= b;
        history[0] *= b;
        history[0] += series_x[i];
    }
    for (size_t i = k; i < n; ++i)
    {
        size_t l = i - k;
        int y_state   = series_y[i-1];
        int future    = series_x[i];
        predicate[l]  = history[l] * b + future;
        state[l]      = predicate[l] * b + y_state;
        source[l]     = history[l] * b + y_state;

        states->histogram[state[l]]++;
        histories->histogram[history[l]]++;
        sources->histogram[source[l]]++;
        predicates->histogram[predicate[l]]++;

        if (i + 1 != n)
        {
            history[l + 1] = predicate[l] - series_x[l]*q;
        }
    }
}

static bool check_arguments(int const *node_y, int const *node_x,
    size_t n, size_t m, int b, size_t k, inform_error *err)
{
    if (node_y == NULL)
    {
        INFORM_ERROR_RETURN(err, INFORM_EINVAL, "source node's time series is NULL", true);
    }
    else if (node_x == NULL)
    {
        INFORM_ERROR_RETURN(err, INFORM_EINVAL, "target node's time series is NULL", true);
    }
    else if (n < 1)
    {
        INFORM_ERROR_RETURN(err, INFORM_EINVAL, "time series has no initial conditions", true);
    }
    else if (m < 2)
    {
        INFORM_ERROR_RETURN(err, INFORM_EINVAL, "time series has less than two timesteps", true);
    }
    else if (m <= k)
    {
        INFORM_ERROR_RETURN(err, INFORM_EINVAL, "history length is too long for the timeseries", true);
    }
    else if (b < 2)
    {
        INFORM_ERROR_RETURN(err, INFORM_EINVAL, "base is less than two", true);
    }
    else if (k == 0)
    {
        INFORM_ERROR_RETURN(err, INFORM_EINVAL, "history length is zero", true);
    }
    for (size_t i = 0; i < n * m; ++i)
    {
        if (b <= node_y[i])
        {
            INFORM_ERROR_RETURN(err, INFORM_EINVAL, "source node's time series has negative states", true);
        }
        else if (node_y[i] < 0)
        {
            INFORM_ERROR_RETURN(err, INFORM_EINVAL, "source node's time series has states inconsistent with the expected base", true);
        }
        if (b <= node_x[i])
        {
            INFORM_ERROR_RETURN(err, INFORM_EINVAL, "source node's time series has negative states", true);
        }
        else if (node_x[i] < 0)
        {
            INFORM_ERROR_RETURN(err, INFORM_EINVAL, "source node's time series has states inconsistent with the expected base", true);
        }
    }
    return false;
}

double inform_transfer_entropy(int const *node_y, int const *node_x, size_t n,
    size_t m, int b, size_t k, inform_error *err)
{
    if (check_arguments(node_y, node_x, n, m, b, k, err)) return NAN;

    size_t const N = n * (m - k);

    size_t const q = (size_t) pow((double) b, (double) k);
    size_t const states_size     = b*b*q;
    size_t const histories_size  = q;
    size_t const sources_size    = b*q;
    size_t const predicates_size = b*q;
    size_t const total_size = states_size + histories_size + sources_size + predicates_size;

    uint32_t *data = calloc(total_size, sizeof(uint32_t));
    if (data == NULL)
    {
        INFORM_ERROR_RETURN(err, INFORM_ENOMEM, "failed to allocate distribution histograms", NAN);
    }

    inform_dist states     = { data, states_size, N };
    inform_dist histories  = { data + states_size, histories_size, N };
    inform_dist sources    = { data + states_size + histories_size, sources_size, N };
    inform_dist predicates = { data + states_size + histories_size + sources_size, predicates_size, N };

    for (size_t i = 0; i < n; ++i, node_x += m, node_y += m)
    {
        accumulate_observations(node_y, node_x, m, b, k, &states, &histories, &sources, &predicates);
    }

    double te = inform_shannon(&sources, (double) b) +
        inform_shannon(&predicates, (double) b) -
        inform_shannon(&states, (double) b) -
        inform_shannon(&histories, (double) b);

    free(data);

    return te;
}

double *inform_local_transfer_entropy(int const *node_y, int const *node_x,
    size_t n, size_t m, int b, size_t k, double *te, inform_error *err)
{
    if (check_arguments(node_y, node_x, n, m, b, k, err)) return NULL;

    size_t const N = n * (m - k);

    if (te == NULL)
    {
        te = malloc(N * sizeof(double));
        if (te == NULL)
        {
            INFORM_ERROR_RETURN(err, INFORM_ENOMEM, "failed to allocate TE output array", NULL);
        }
    }

    size_t const q = (size_t) pow((double) b, (double) k);
    size_t const states_size     = b*b*q;
    size_t const histories_size  = q;
    size_t const sources_size    = b*q;
    size_t const predicates_size = b*q;
    size_t const total_size = states_size + histories_size + sources_size + predicates_size;

    uint32_t *data = calloc(total_size, sizeof(uint32_t));
    if (data == NULL)
    {
        INFORM_ERROR_RETURN(err, INFORM_ENOMEM, "failed to allocate distribution histograms", NULL);
    }

    inform_dist states     = { data, states_size, N };
    inform_dist histories  = { data + states_size, histories_size, N };
    inform_dist sources    = { data + states_size + histories_size, sources_size, N };
    inform_dist predicates = { data + states_size + histories_size + sources_size, predicates_size, N };

    int *state      = malloc(N * sizeof(int));
    if (state == NULL)
    {
        INFORM_ERROR_RETURN(err, INFORM_ENOMEM, "failed to allocate state array", NULL);
    }
    int *history    = malloc(N * sizeof(int));
    if (history == NULL)
    {
        INFORM_ERROR_RETURN(err, INFORM_ENOMEM, "failed to allocate history array", NULL);
    }
    int *source     = malloc(N * sizeof(int));
    if (source == NULL)
    {
        INFORM_ERROR_RETURN(err, INFORM_ENOMEM, "failed to allocate source array", NULL);
    }
    int *predicate  = malloc(N * sizeof(int));
    if (predicate == NULL)
    {
        INFORM_ERROR_RETURN(err, INFORM_ENOMEM, "failed to allocate predicate array", NULL);
    }

    int const *node_y_ptr = node_y, *node_x_ptr = node_x;
    int *state_ptr = state, *source_ptr = source, *history_ptr = history, *predicate_ptr = predicate;
    for (size_t i = 0; i < n; ++i)
    {
        accumulate_local_observations(node_y_ptr, node_x_ptr, m, b, k, &states,
            &histories, &sources, &predicates, state_ptr, history_ptr,
            source_ptr, predicate_ptr);
        node_y_ptr += m;
        node_x_ptr += m;
        state_ptr += (m - k);
        history_ptr += (m - k);
        source_ptr += (m - k);
        predicate_ptr += (m - k);
    }

    for (size_t i = 0; i < N; ++i)
    {
        te[i] = inform_shannon_pcmi(&states, &sources, &predicates, &histories,
            state[i], source[i], predicate[i], history[i], (double) b);
    }

    free(predicate);
    free(source);
    free(history);
    free(state);
    free(data);

    return te;
}
