// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <inform/entropy.h>
#include <inform/error.h>

double inform_self_info(inform_dist const *dist, uint64_t event, double base)
{
    if (inform_dist_is_valid(dist))
    {
        return -log2(inform_dist_prob(dist, event)) / log2(base);
    }
    return inform_nan(1);
}

double inform_shannon(inform_dist const *dist, double base)
{
    // ensure that the distribution is valid
    if (inform_dist_is_valid(dist))
    {
        double h = 0.;
        // for each element of the distribution's support
        for (size_t i = 0; i < inform_dist_size(dist); ++i)
        {
            // get the probability
            double const p = inform_dist_prob(dist, i);
            // the the probability is non-zero
            if (p != 0)
            {
                // accumulate the weighted self-information of the event
                h -= p * log2(p)/log2(base);
            }
        }
        // return the entropy
        return h;
    }
    // return NaN if the distribution is invalid
    return inform_nan(1);
}

double inform_pointwise_mutual_info(inform_dist const *joint,
    inform_dist const * marginal_x, inform_dist const *marginal_y,
    uint64_t event_joint, uint64_t event_x, uint64_t event_y, double base)
{
    return inform_self_info(marginal_x, event_x, base) +
        inform_self_info(marginal_y, event_y, base) -
        inform_self_info(joint, event_joint, base);
}

double inform_mutual_info(inform_dist const *joint,
        inform_dist const *marginal_x,
        inform_dist const *marginal_y,
        double base)
{
    // Simply farm out the computation to the inform_shannon function.
    // This sidesteps the possibility that the joint and the marginals
    // may all have different support.
    return inform_shannon(marginal_x,base) + inform_shannon(marginal_y,base)
        - inform_shannon(joint,base);
}

double inform_pointwise_conditional_entropy(inform_dist const *joint,
    inform_dist const *marginal, uint64_t event_joint,uint64_t event_marginal,
    double base)
{
    return inform_self_info(joint, event_joint, base) -
        inform_self_info(marginal, event_marginal, base);
}

double inform_conditional_entropy(inform_dist const *joint,
        inform_dist const *marginal,
        double base)
{
    return inform_shannon(joint, base) - inform_shannon(marginal, base);
}
