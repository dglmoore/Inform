// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <inform/shannon.h>
#include <inform/error.h>

double inform_shannon_si(inform_dist const *dist, uint64_t event, double base)
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
        // get the size of the distribution's support
        size_t const n = inform_dist_size(dist);
        double h = 0.;
        // for each element of the distribution's support
        for (size_t i = 0; i < n; ++i)
        {
            // get the probability
            double const p = inform_dist_prob(dist, i);
            // the the probability is non-zero
            if (p != 0)
            {
                // accumulate the weighted self-information of the event
                h -= p * log2(p);
            }
        }
        // return the entropy
        return h / log2(base);
    }
    // return NaN if the distribution is invalid
    return inform_nan(1);
}

double inform_shannon_pmi(inform_dist const *joint,
    inform_dist const * marginal_x, inform_dist const *marginal_y,
    uint64_t event_joint, uint64_t event_x, uint64_t event_y, double base)
{
    return inform_shannon_si(marginal_x, event_x, base) +
        inform_shannon_si(marginal_y, event_y, base) -
        inform_shannon_si(joint, event_joint, base);
}

double inform_shannon_mi(inform_dist const *joint,
    inform_dist const *marginal_x, inform_dist const *marginal_y, double base)
{
    return inform_shannon(marginal_x,base) + inform_shannon(marginal_y,base)
        - inform_shannon(joint,base);
}

double inform_shannon_pce(inform_dist const *joint, inform_dist const *marginal,
    uint64_t event_joint,uint64_t event_marginal, double base)
{
    return inform_shannon_si(joint, event_joint, base) -
        inform_shannon_si(marginal, event_marginal, base);
}

double inform_shannon_ce(inform_dist const *joint, inform_dist const *marginal,
    double base)
{
    return inform_shannon(joint, base) - inform_shannon(marginal, base);
}

double inform_shannon_pcmi(inform_dist const *joint,
    inform_dist const *marginal_xz, inform_dist const *marginal_yz,
    inform_dist const *marginal_z, uint64_t event_joint,
    uint64_t event_marginal_xz, uint64_t event_marginal_yz,
    uint64_t event_marginal_z, double base)
{
    return inform_shannon_si(marginal_xz, event_marginal_xz, base) +
        inform_shannon_si(marginal_yz, event_marginal_yz, base) -
        inform_shannon_si(joint, event_joint, base) -
        inform_shannon_si(marginal_z, event_marginal_z, base);
}

double inform_shannon_cmi(inform_dist const *joint,
    inform_dist const *marginal_xz, inform_dist const *marginal_yz,
    inform_dist const *marginal_z, double base)
{
    return inform_shannon(marginal_xz, base) +
        inform_shannon(marginal_yz, base) -
        inform_shannon(joint, base) -
        inform_shannon(marginal_z, base);
}
