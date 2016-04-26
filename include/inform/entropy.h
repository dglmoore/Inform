// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#pragma once

#include <inform/dist.h>
#include <math.h>

/// an entropy is just a double
typedef double entropy;

/**
 * Compute the Shannon entropy of a distribution.
 * 
 * This function will return `NaN` if the distribution is not valid,
 * i.e. `!inform_dist_is_valid(dist)`.
 *
 * @param[in] dist the probability distribution
 * @return the base-2 shannon entropy
 */
entropy inform_shannon(inform_dist const *dist);

/**
 * Compute the Shannon-based mutual information of a distribution and
 * two marginals.
 *
 * This function will return `NaN` if `inform_shannon` returns `NaN`
 * when applied to any of the distribution arguments.
 *
 * @param[in] joint      the joint probability distribution
 * @param[in] marginal_x a marginal distribution
 * @param[in] marginal_y a marginal distribution
 * @return the mutual information
 *
 * @see inform_shannon
 */
entropy inform_mutual_info(inform_dist const *joint,
        inform_dist const *marginal_x,
        inform_dist const *marginal_y);
