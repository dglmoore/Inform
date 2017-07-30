// Copyright 2016-2017 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <inform/integration.h>
#include <inform/mutual_info.h>
#include <inform/utilities.h>
#include <math.h>

static bool check_arguments(int const *series, size_t l, inform_error *err)
{
    if (series == NULL)
    {
        INFORM_ERROR_RETURN(err, INFORM_ETIMESERIES, true);
    }
    else if (l < 2)
    {
        INFORM_ERROR_RETURN(err, INFORM_ENOSOURCES, true);
    }
    // All other checks will be caught by inform_black_box_parts or
    // inform_local_mutual_info
    return false;
}

double *inform_integration_evidence(int const *series, size_t l, size_t n,
    int const *b, double *evidence, inform_error *err)
{
    if (check_arguments(series, l, err))
    {
        return NULL;
    }
    int allocate = (evidence == NULL);
    if (allocate)
    {
        evidence = malloc(2 * n * sizeof(double));
        if (evidence == NULL)
        {
            INFORM_ERROR_RETURN(err, INFORM_ENOMEM, NULL);
        }
    }
    double *lmi = malloc(n * sizeof(double));
    if (lmi == NULL)
    {
        if (allocate) free(evidence);
        INFORM_ERROR_RETURN(err, INFORM_ENOMEM, NULL);
    }
    double *minimum = evidence;
    double *maximum = minimum + n;
    for (size_t i = 0; i < n; ++i)
    {
        minimum[i] = INFINITY;
        maximum[i] = -INFINITY;
    }

    size_t *parts = inform_first_partitioning(l);
    size_t nparts = 1;
    while ((nparts = inform_next_partitioning(parts, l)))
    {
        int *partitioned = inform_black_box_parts(series, l, n, b, parts,
            nparts, NULL, err);
        if (inform_failed(err))
        {
            free(partitioned);
            break;
        }
        inform_local_mutual_info(partitioned, nparts, n, partitioned + nparts*n,
            lmi, err);
        if (inform_failed(err))
        {
            free(partitioned);
            break;
        }
        for (size_t i = 0; i < n; ++i)
        {
            minimum[i] = min(minimum[i], lmi[i]);
            maximum[i] = max(maximum[i], lmi[i]);
        }
        free(partitioned);
    }
    free(parts);
    free(lmi);
    
    if (inform_failed(err))
    {
        if (allocate)
        {
            free(evidence);
        }
        return NULL;
    }

    return evidence;
}
