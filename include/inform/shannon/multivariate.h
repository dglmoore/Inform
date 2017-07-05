// Copyright 2016-2017 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#pragma once

#include <inform/dist.h>
#include <math.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Compute the multivariate pointwise mutual information between a collection of
 * distributions.
 *
 * @param[in] joint           the joint distribution
 * @param[in] marginals       an array of marginal distributions
 * @param[in] n               the number of marginals
 * @param[in] joint_event     the joint event
 * @param[in] marginal_events an array of marginal events
 * @param[in] base            the logarithmic base
 * @return the multivariate pointwise mutual information of the events
 */
EXPORT double inform_shannon_multi_pmi(inform_dist const *joint,
    inform_dist const **marginals, size_t n, size_t joint_event,
    size_t const *marginal_events, double base);

/**
 * Compute the multivariate mutual information between a collection of
 * distributions.
 *
 * @param[in] joint           the joint distribution
 * @param[in] marginals       an array of marginal distributions
 * @param[in] n               the number of marginals
 * @param[in] base            the logarithmic base
 * @return the multivariate mutual information of the distributions
 */
EXPORT double inform_shannon_multi_mi(inform_dist const *joint,
    inform_dist const **marginals, size_t n, double base);

#ifdef __cplusplus
}
#endif
