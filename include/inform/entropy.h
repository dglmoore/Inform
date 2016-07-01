// Copyright 2016 ELIFE. All rights reserved.
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
 * Compute the self-information of an event given some distribution.
 *
 * This function will return `NaN` if the distribution is not valid.
 *
 * @param[in] dist  the probability distribution
 * @param[in] event the event in question
 * @param[in] base  the logarithmic base
 * @return the self-information of the event
 */
double inform_self_info(inform_dist const *dist, uint64_t event, double base);

/**
 * Compute the Shannon entropy of a distribution.
 *
 * This function will return `NaN` if the distribution is not valid,
 * i.e. `!inform_dist_is_valid(dist)`.
 *
 * @param[in] dist the probability distribution
 * @param[in] base the logarithmic base
 * @return the shannon entropy
 */
double inform_shannon(inform_dist const *dist, double base);

/**
 * Compute the point-wise mutual information of an combination of events
 *
 * @param[in] joint       the joint distribution
 * @param[in] marginal_x  the x-marginal
 * @param[in] marginal_y  the y-marginal
 * @param[in] event_joint the joint event
 * @param[in] event_x     the corresponding event in the x-marginal
 * @param[in] event_y     the corresponding event in the y-marginal
 * @param[in] base        the logarithmic base
 * @return the pointwise mutual information of the events
 */
double inform_pointwise_mutual_info(inform_dist const *joint,
    inform_dist const *marginal_x, inform_dist const *marginal_y,
    uint64_t event_joint, uint64_t event_x, uint64_t enent_y, double base);

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
 * @param[in] base       the logarithmic base
 * @return the mutual information
 *
 * @see inform_shannon
 */
double inform_mutual_info(inform_dist const *joint,
        inform_dist const *marginal_x,
        inform_dist const *marginal_y,
        double base);

/**
 * Compute the Shannon-based conditional entropy of a joint distribution and
 * a marginal.
 *
 * This function will return `NaN` if `inform_shannon` returns `NaN`
 * when applied to any of the distribution arguments.
 *
 * @param[in] joint    the joint probability distribution
 * @param[in] marginal a marginal distribution
 * @param[in] base     the logarithmic base
 * @return the conditional entropy
 *
 * @see inform_shannon
 */
double inform_conditional_entropy(inform_dist const* joint,
        inform_dist const *marginal,
        double base);

#ifdef __cplusplus
}
#endif
