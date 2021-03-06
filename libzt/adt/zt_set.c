#include "zt_set.h"
#include "../zt_assert.h"


zt_set *
zt_set_init(int (*match)(const void *key1,
                         const void *key2, void *cdata),
            void (*destroy)(void *data),
            void *cdata)
{
    zt_set * set;

    set = XMALLOC(zt_set, 1);


    set->tbl = zt_table_init(NULL, zt_table_hash_int, match, 128, 0, cdata);
    set->match = match;
    set->destroy = destroy;
    set->length = 0;

    return set;
}

static int
_destroy(void *key UNUSED, void *datum, void *param)
{
    void (*destroy)(void *) = ((zt_set *)param)->destroy;

    if (destroy) {
        destroy((void *)datum);
    }

    return 0;
}

void
zt_set_destroy(zt_set *set)
{
    zt_assert(set);

    zt_table_for_each(set->tbl, _destroy, set);
    zt_table_destroy(set->tbl);

    XFREE(set);
}


int
zt_set_insert(zt_set *set, const void *data)
{
    if (zt_set_is_member(set, data)) {
        return 1;
    }

    set->length++;
    return zt_table_set(set->tbl, (void *)data, (void *)data);
}

int zt_set_remove(zt_set *set, void **data)
{
    *data = zt_table_del(set->tbl, *data);
    set->length--;
    return 0;
}

struct _set_pair {
    zt_set * s1;
    zt_set * s2;
};

static int _union(void *key UNUSED, void *datum, void *param)
{
    struct _set_pair * p;

    p = (struct _set_pair *)param;

    return zt_set_insert(p->s2, datum);
}

int zt_set_union(zt_set *setu, const zt_set *set1, const zt_set *set2)
{
    struct _set_pair s = { NULL, NULL };

    s.s2 = setu;

    zt_table_for_each(set1->tbl, _union, &s);
    zt_table_for_each(set2->tbl, _union, &s);

    return 0;
}

static int _intersect(void *key UNUSED, void *datum, void *param)
{
    struct _set_pair * p;

    p = (struct _set_pair *)param;

    if (zt_set_is_member(p->s1, datum)) {
        return zt_set_insert(p->s2, datum);
    }
    return 0;
}

int zt_set_intersection(zt_set *seti, const zt_set *set1, const zt_set *set2)
{
    struct _set_pair p = { NULL, NULL };

    p.s1 = (zt_set *)set2;
    p.s2 = (zt_set *)seti;

    return zt_table_for_each(set1->tbl, _intersect, &p);
}

static int _difference(void *key UNUSED, void *datum, void *param)
{
    struct _set_pair * p;

    p = (struct _set_pair *)param;

    if (!zt_set_is_member(p->s1, datum)) {
        return zt_set_insert(p->s2, datum);
    }
    return 0;
}

int zt_set_difference(zt_set *setd, const zt_set *set1, const zt_set *set2)
{
    struct _set_pair p = { NULL, NULL };

    p.s1 = (zt_set *)set2;
    p.s2 = (zt_set *)setd;

    return zt_table_for_each(set1->tbl, _difference, &p);
}

int zt_set_is_member(const zt_set *set, const void *data)
{
    if (zt_table_get(set->tbl, data) != NULL) {
        return 1;
    }
    return 0;
}

static int _is_subset(void *key UNUSED, void *datum, void *param)
{
    struct _set_pair * p = (struct _set_pair *)param;

    if (zt_set_is_member(p->s1, datum)) {
        return 0;
    }

    return -1;
}

int zt_set_is_subset(const zt_set *set1, const zt_set *set2)
{
    struct _set_pair p = { NULL, NULL };

    p.s1 = (zt_set *)set2;

    if (set1->length > set2->length) {
        return 0;
    }

    return zt_table_for_each(set1->tbl, _is_subset, &p) == 0;
}

int zt_set_is_equal(const zt_set *set1, const zt_set *set2)
{
    struct _set_pair p = { NULL, NULL };

    p.s1 = (zt_set *)set2;

    if (set1->length != set2->length) {
        return -1;
    }

    return zt_table_for_each(set1->tbl, _is_subset, &p);
}


struct _iterator_data {
    zt_set_iterator iterator;
    void          * param;
};

static int _table_to_set_iterator(void *key UNUSED, void *data, void *param)
{
    struct _iterator_data * p = (struct _iterator_data *)param;

    return p->iterator(data, p->param);
}

int
zt_set_for_each(zt_set *set, zt_set_iterator iterator, void *param)
{
    struct _iterator_data d;

    zt_assert(set);
    zt_assert(iterator);

    d.iterator = iterator;
    d.param = param;

    return zt_table_for_each(set->tbl, _table_to_set_iterator, &d);
}

