// Copyright 2016-2017 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <ginger/vector.h>
#include <inform/pid.h>

static pid_source *pid_source_alloc(size_t *name)
{
    pid_source *src = malloc(sizeof(pid_source));
    if (src)
    {
        src->name = gvector_shrink(name);
        src->size = gvector_len(name);

        src->below = NULL;

        src->above = gvector_alloc(0, 0, sizeof(pid_source*));
        if (!src->above)
        {
            pid_source_free(src);
            return NULL;
        }

        src->below = gvector_alloc(0, 0, sizeof(pid_source*));
        if (!src->below)
        {
            pid_source_free(src);
            return NULL;
        }

        src->n_above = src->n_below = 0;
    }
    return src;
}

void pid_source_free(pid_source *src)
{
    if (src)
    {
        gvector_free(src->name);
        gvector_free(src->above);
        gvector_free(src->below);
        free(src);
    }
}

static pid_source **pid_sources_rec(size_t i, size_t m, size_t *c, pid_source **srcs)
{
    if (i < m)
    {
        srcs = pid_sources_rec(i + 1, m, gvector_dup(c), srcs);
    }

    if (i <= m)
    {
        size_t z = 0;
        for (size_t j = 0; j < gvector_len(c); ++j)
        {
            z = i & c[j];
            if (z == i || z == c[j])
            {
                gvector_free(c);
                return srcs;
            }
        }
        gvector_push(c, i);
        gvector_push(srcs, pid_source_alloc(c));
        return pid_sources_rec(i + 1, m, gvector_dup(c), srcs);
    }

    return srcs;
}

pid_source **pid_sources(size_t n)
{
    size_t const m = (1 << n) - 1;
    pid_source **srcs = gvector_alloc(0, 0, sizeof(pid_source*));
    for (size_t i = 1; i <= m; ++i)
    {
        size_t *c = gvector_alloc(1, 1, sizeof(size_t));
        c[0] = i;

        gvector_push(srcs, pid_source_alloc(c));
        srcs = pid_sources_rec(i + 1, m, gvector_dup(c), srcs);
    }
    srcs = gvector_shrink(srcs);
    return srcs;
}

static bool name_below(size_t const *xs, size_t const *ys)
{
    size_t const m = gvector_len(xs);
    size_t const n = gvector_len(ys);

    bool is_below = true;
    for (size_t const *y = ys; y < ys + n && is_below; ++y)
    {
        bool is_valid = false;
        for (size_t const *x = xs; x < xs + m && !is_valid; ++x)
        {
            if ((*x & *y) == *x)
            {
                is_valid = true;
            }
        }
        if (!is_valid)
        {
            is_below = false;
        }
    }
    return is_below;
}

static bool below(pid_source const *a, pid_source const *b)
{
    return name_below(a->name, b->name);
}

void pid_toposort(pid_source **srcs)
{
    size_t const n = gvector_len(srcs);
    size_t u = 0, v = 0;
    while (v < n - 1)
    {
        u = v;
        for (size_t i = u; i < n; ++i)
        {
            bool is_bottom = true;
            for (size_t j = u; j < n; ++j)
            {
                if (i != j && below(srcs[j], srcs[i]))
                {
                    is_bottom = false;
                    break;
                }
            }
            if (is_bottom)
            {
                pid_source *tmp = srcs[v];
                srcs[v] = srcs[i];
                srcs[i] = tmp;
                v += 1;
            }
        }
    }
}

static pid_lattice *pid_lattice_alloc()
{
    pid_lattice *l = malloc(sizeof(pid_lattice));
    if (l)
    {
        l->sources = NULL;
        l->top = NULL;
        l->bottom = NULL;
    }
    return l;
}

void pid_lattice_free(pid_lattice *l)
{
    if (l)
    {
        for (size_t i = 0; i < gvector_len(l->sources); ++i)
        {
            pid_source_free(l->sources[i]);
        }
        gvector_free(l->sources);
        l->sources = NULL;
        l->top = NULL;
        l->bottom = NULL;
        free(l);
    }
}

static pid_lattice *build_hasse(pid_source **srcs)
{
    size_t const n = gvector_len(srcs);
    if (n == 0)
    {
        return NULL;
    }

    pid_lattice *l = pid_lattice_alloc();
    if (l)
    {
        for (size_t i = 0; i < n; ++i)
        {
            for (size_t j = i + 1; j < n; ++j)
            {
                if (below(srcs[i], srcs[j]))
                {
                    bool stop = false;
                    for (size_t k = 0; k < gvector_len(srcs[i]->above); ++k)
                    {
                        pid_source *x = srcs[i]->above[k];
                        if (below(x, srcs[j]))
                        {
                            stop = true;
                            break;
                        }
                    }
                    if (stop)
                    {
                        break;
                    }
                    else
                    {
                        gvector_push(srcs[i]->above, srcs[j]);
                        gvector_push(srcs[j]->below, srcs[i]);
                    }
                }
            }
        }
        l->sources = srcs;
        l->bottom = srcs[0];
        l->top = srcs[n-1];
    }
    return l;
}

pid_lattice *pid_hasse(size_t n)
{
    pid_source **srcs = pid_sources(n);
    pid_toposort(srcs);
    return build_hasse(srcs);
}
