// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <inform/error.h>
#include <inform/transfer_entropy.h>

static void accumulate_observations(uint64_t const *series_y,
    uint64_t const *series_x, size_t n, uint64_t b, uint64_t k,
    inform_dist *states, inform_dist *histories, inform_dist *sources,
    inform_dist *predicates)

{
    uint64_t history = 0, q = 1, y_state, future, state, source, predicate;
    for (uint64_t i = 0; i < k; ++i)
    {
        q *= b;
        history *= b;
        history += series_x[i];
    }
    for (uint64_t i = k; i < n; ++i)
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

static void accumulate_local_observations(uint64_t const *series_y,
    uint64_t const *series_x, size_t n, uint64_t b, uint64_t k,
    inform_dist *states, inform_dist *histories, inform_dist *sources,
    inform_dist *predicates, uint64_t *state, uint64_t *history,
    uint64_t *source, uint64_t *predicate)
{
    history[0] = 0;
    uint64_t q = 1;
    for (uint64_t i = 0; i < k; ++i)
    {
        q *= b;
        history[0] *= b;
        history[0] += series_x[i];
    }
    for (uint64_t i = k; i < n; ++i)
    {
        uint64_t l = i - k;
        uint64_t y_state   = series_y[i-1];
        uint64_t future    = series_x[i];
        predicate[l]       = history[l] * b + future;
        state[l]           = predicate[l] * b + y_state;
        source[l]          = history[l] * b + y_state;

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

double inform_transfer_entropy(uint64_t const *node_y, uint64_t const *node_x,
    size_t n, uint64_t b, uint64_t k)
{
    return inform_transfer_entropy_ensemble(node_y, node_x, 1, n, b, k);
}

double inform_transfer_entropy_ensemble(uint64_t const *node_y,
    uint64_t const *node_x, size_t n, size_t m, uint64_t b, uint64_t k)
{
    // ensure that neither of the time series are NULL
    if (node_x == NULL || node_y == NULL)
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
    // ensure that the history is reasonable given the history length
    else if (k > 25 / log2l(b))
    {
        return inform_nan(4);
    }
    for (size_t i = 0; i < n * m; ++i)
    {
        if (b <= node_x[i] || b <= node_y[i])
        {
            return inform_nan(6);
        }
    }

    // compute the number of observations to be made
    int const N = n * (m - k);

    // compute the sizes of the various histograms
    int const q = pow(b,k);
    int const states_size     = b*b*q;
    int const histories_size  = q;
    int const sources_size    = b*q;
    int const predicates_size = b*q;
    int const total_size = states_size + histories_size + sources_size
        + predicates_size;

    // allocate memory to store the basic histograms
    uint64_t *data = calloc(total_size, sizeof(uint64_t));
    if (data == NULL)
    {
        return inform_nan(7);
    }

    // create some pointers to facilitate observation accumulation
    inform_dist states     = { data, states_size, N };
    inform_dist histories  = { data + states_size, histories_size, N };
    inform_dist sources    = { data + states_size + histories_size, sources_size, N };
    inform_dist predicates = { data + states_size + histories_size + sources_size, predicates_size, N };

    // for each initial condition
    for (uint64_t i = 0; i < n; ++i, node_x += m, node_y += m)
    {
        // accumulate the observations
        accumulate_observations(node_y, node_x, m, b, k, &states, &histories, &sources, &predicates);
    }

    // compute the transfer entropy from the distributions
    double te = inform_shannon(&sources, b) + inform_shannon(&predicates, b) -
        inform_shannon(&states, b) - inform_shannon(&histories, b);

    // free up the data array
    free(data);

    // return the transfer entropy
    return te;
}

int inform_local_transfer_entropy(uint64_t const *node_y, uint64_t const *node_x,
    size_t n, uint64_t b, uint64_t k, double *te)
{
    return inform_local_transfer_entropy_ensemble(node_y, node_x, 1, n, b, k, te);
}

int inform_local_transfer_entropy_ensemble(uint64_t const *node_y,
    uint64_t const *node_x, size_t n, size_t m, uint64_t b, uint64_t k,
    double *te)
{
    // ensure that neither of the time series are NULL
    if (node_x == NULL || node_y == NULL)
    {
        return 1;
    }
    // ensure that the transfer entropy array is not NULL
    else if (te == NULL)
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
    // ensure that the history is reasonable given the history length
    else if (k > 25 / log2l(b))
    {
        return 5;
    }
    for (size_t i = 0; i < n * m; ++i)
    {
        if (b <= node_x[i] || b <= node_y[i])
        {
            return 6;
        }
    }

    // compute the number of observations to be made
    size_t const N = n * (m - k);

    // compute the sizes of the various histograms
    int const q = pow(b,k);
    int const states_size     = b*b*q;
    int const histories_size  = q;
    int const sources_size    = b*q;
    int const predicates_size = b*q;
    int const total_size = states_size + histories_size + sources_size
        + predicates_size;

    // allocate memory to store the basic histograms
    uint64_t *data = calloc(total_size, sizeof(uint64_t));
    if (data == NULL)
    {
        return 7;
    }

    // create some pointers to facilitate observation accumulation
    inform_dist states     = { data, states_size, N };
    inform_dist histories  = { data + states_size, histories_size, N };
    inform_dist sources    = { data + states_size + histories_size, sources_size, N };
    inform_dist predicates = { data + states_size + histories_size + sources_size, predicates_size, N };

    uint64_t *state      = malloc(N * sizeof(uint64_t));
    uint64_t *history    = malloc(N * sizeof(uint64_t));
    uint64_t *source     = malloc(N * sizeof(uint64_t));
    uint64_t *predicate  = malloc(N * sizeof(uint64_t));

    // for each initial condition
    uint64_t const *node_y_ptr = node_y, *node_x_ptr = node_x;
    uint64_t *state_ptr = state, *source_ptr = source, *history_ptr = history, *predicate_ptr = predicate;
    for (uint64_t i = 0; i < n; ++i)
    {
        // accumulate the observations
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

    // compute the transfer entropy from the distributions
    for (size_t i = 0; i < N; ++i)
    {
        te[i] = inform_pointwise_conditional_mutual_info(&states,
            &sources, &predicates, &histories, state[i], source[i],
            predicate[i], history[i], b);
    }

    // free up the data array
    free(predicate);
    free(source);
    free(history);
    free(state);
    free(data);

    // return the transfer entropy
    return 0;
}
