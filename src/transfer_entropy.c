// Copyright 2016-2017 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <inform/shannon.h>
#include <inform/transfer_entropy.h>
#include <string.h>

static void accumulate_observations(int const *src, int const *dst, size_t n,
    size_t m, int b, size_t k, inform_dist *states, inform_dist *histories,
    inform_dist *sources, inform_dist *predicates)
{
    for (size_t i = 0; i < n; ++i, src += m, dst += m)
    {
        int history = 0, q = 1, src_state, future, state, source, predicate;
        for (size_t j = 0; j < k; ++j)
        {
            q *= b;
            history *= b;
            history += dst[j];
        }
        for (size_t j = k; j < m; ++j)
        {
            src_state = src[j-1];
            future    = dst[j];
            state     = (history * b + future) * b + src_state;
            source    = history * b + src_state;
            predicate = history * b + future;

            states->histogram[state]++;
            histories->histogram[history]++;
            sources->histogram[source]++;
            predicates->histogram[predicate]++;

            history = predicate - dst[j - k]*q;
        }
    }
}

static void accumulate_local_observations(int const *src, int const *dst,
    size_t n, size_t m, int b, size_t k, inform_dist *states,
    inform_dist *histories, inform_dist *sources, inform_dist *predicates,
    int *state, int *history, int *source, int *predicate)
{
    for (size_t i = 0; i < n; ++i)
    {
        history[0] = 0;
        int q = 1;
        for (size_t j = 0; j < k; ++j)
        {
            q *= b;
            history[0] *= b;
            history[0] += dst[j];
        }
        for (size_t j = k; j < m; ++j)
        {
            size_t l = j - k;
            int src_state = src[j-1];
            int future    = dst[j];
            predicate[l]  = history[l] * b + future;
            state[l]      = predicate[l] * b + src_state;
            source[l]     = history[l] * b + src_state;

            states->histogram[state[l]]++;
            histories->histogram[history[l]]++;
            sources->histogram[source[l]]++;
            predicates->histogram[predicate[l]]++;

            if (j + 1 != m)
            {
                history[l + 1] = predicate[l] - dst[l]*q;
            }
        }
        src += m;
        dst += m;
        state += (m - k);
        history += (m - k);
        source += (m - k);
        predicate += (m - k);
    }
}

static bool check_arguments(int const *src, int const *dst, size_t n, size_t m,
    int b, size_t k, inform_error *err)
{
    if (src == NULL)
    {
        INFORM_ERROR_RETURN(err, INFORM_ETIMESERIES, true);
    }
    else if (dst == NULL)
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
    else if (m <= k)
    {
        INFORM_ERROR_RETURN(err, INFORM_EKLONG, true);
    }
    else if (k == 0)
    {
        INFORM_ERROR_RETURN(err, INFORM_EKZERO, true);
    }
    for (size_t i = 0; i < n * m; ++i)
    {
        if (b <= src[i] || b <= dst[i])
        {
            INFORM_ERROR_RETURN(err, INFORM_EBADSTATE, true);
        }
        else if (src[i] < 0 || dst[i] < 0)
        {
            INFORM_ERROR_RETURN(err, INFORM_ENEGSTATE, true);
        }
    }
    return false;
}

double inform_transfer_entropy(int const *src, int const *dst, size_t n,
    size_t m, int b, size_t k, inform_error *err)
{
    if (check_arguments(src, dst, n, m, b, k, err)) return NAN;

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
        INFORM_ERROR_RETURN(err, INFORM_ENOMEM, NAN);
    }

    inform_dist states     = { data, states_size, N };
    inform_dist histories  = { data + states_size, histories_size, N };
    inform_dist sources    = { data + states_size + histories_size, sources_size, N };
    inform_dist predicates = { data + states_size + histories_size + sources_size, predicates_size, N };

    accumulate_observations(src, dst, n, m, b, k, &states, &histories, &sources,
        &predicates);

    double te = inform_shannon_entropy(&sources, 2.0) +
        inform_shannon_entropy(&predicates, 2.0) -
        inform_shannon_entropy(&states, 2.0) -
        inform_shannon_entropy(&histories, 2.0);

    free(data);

    return te;
}

double *inform_local_transfer_entropy(int const *src, int const *dst, size_t n,
    size_t m, int b, size_t k, double *te, inform_error *err)
{
    if (check_arguments(src, dst, n, m, b, k, err)) return NULL;

    size_t const N = n * (m - k);

    bool allocate_te = (te == NULL);
    if (allocate_te)
    {
        te = malloc(N * sizeof(double));
        if (te == NULL)
        {
            INFORM_ERROR_RETURN(err, INFORM_ENOMEM, NULL);
        }
    }

    size_t const q = (size_t) pow((double) b, (double) k);
    size_t const states_size     = b*b*q;
    size_t const histories_size  = q;
    size_t const sources_size    = b*q;
    size_t const predicates_size = b*q;
    size_t const total_size = states_size + histories_size + sources_size + predicates_size;

    uint32_t *histogram_data = calloc(total_size, sizeof(uint32_t));
    if (histogram_data == NULL)
    {
        INFORM_ERROR_RETURN(err, INFORM_ENOMEM, NULL);
    }

    inform_dist states     = { histogram_data, states_size, N };
    inform_dist histories  = { histogram_data + states_size, histories_size, N };
    inform_dist sources    = { histogram_data + states_size + histories_size, sources_size, N };
    inform_dist predicates = { histogram_data + states_size + histories_size + sources_size, predicates_size, N };

    int *state_data = malloc(4 * N * sizeof(int));
    if (state_data == NULL)
    {
        if (allocate_te) free(te);
        free(histogram_data);
        INFORM_ERROR_RETURN(err, INFORM_ENOMEM, NULL);
    }
    int *state     = state_data;
    int *history   = state + N;
    int *source    = history + N;
    int *predicate = source + N;

    accumulate_local_observations(src, dst, n, m, b, k, &states,
        &histories, &sources, &predicates, state, history, source, predicate);

    double r, s, t, u;
    for (size_t i = 0; i < N; ++i)
    {
        r = states.histogram[state[i]];
        s = sources.histogram[source[i]];
        t = predicates.histogram[predicate[i]];
        u = histories.histogram[history[i]];
        te[i] = log2((r*u)/(s*t));
    }

    free(state_data);
    free(histogram_data);

    return te;
}