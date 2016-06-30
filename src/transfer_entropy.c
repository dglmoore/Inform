// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <inform/error.h>
#include <inform/time_series.h>

int inform_transfer_entropy_dist(uint64_t const *series_y,
                                 uint64_t const *series_x, size_t n,
                                 uint64_t b, uint64_t k,
                                 inform_dist *states,
                                 inform_dist *histories,
                                 inform_dist *sources,
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

		inform_dist_tick(states, state);
		inform_dist_tick(histories, history);
		inform_dist_tick(sources, source);
		inform_dist_tick(predicates, predicate);

		history = predicate - series_x[i - k]*q;
	}

    return 0;
}

entropy inform_transfer_entropy(uint64_t const *node_y, uint64_t const *node_x, size_t n, uint64_t b, uint64_t k)
{
    return inform_transfer_entropy_ensemble(node_y, node_x, 1, n, b, k);
}

entropy inform_transfer_entropy_ensemble(uint64_t const *node_y, uint64_t const *node_x, size_t n, size_t m, uint64_t b, uint64_t k)
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

    // allocate a distribution for the observed states, histories,
    // sources and predicates
    // clear memory and return NaN if any of the allocations fail
    inform_dist *states = inform_dist_alloc(powl(b,k+2));
    if (states == NULL)
    {
        return inform_nan(7);
    }
    inform_dist *histories  = inform_dist_alloc(powl(b,k));
    if (histories == NULL)
    {
        inform_dist_free(states);
        return inform_nan(7);
    }
    inform_dist *sources = inform_dist_alloc(powl(b,k+1));
    if (sources == NULL)
    {
        inform_dist_free(states);
        inform_dist_free(histories);
        return inform_nan(7);
    }
    inform_dist *predicates = inform_dist_alloc(powl(b,k+1));
    if (predicates == NULL)
    {
        inform_dist_free(states);
        inform_dist_free(histories);
        inform_dist_free(sources);
        return inform_nan(8);
    }

    // for each initial condition
    for (uint64_t i = 0; i < n; ++i, node_x += m, node_y += m)
    {
        // accumulate observations and return NaN if there is an encoding error
        inform_transfer_entropy_dist(node_y, node_x, m, b, k, states, histories, sources, predicates);
    }
    // compute the transfer entropy from the distributions
    entropy te = inform_shannon(sources, b) + inform_shannon(predicates, b) -
        inform_shannon(states, b) - inform_shannon(histories, b);

    // free up the distributions (otherwise there would be memory leaks)
    inform_dist_free(predicates);
    inform_dist_free(sources);
    inform_dist_free(histories);
    inform_dist_free(states);

    // return the transfer entropy
    return te;
}
