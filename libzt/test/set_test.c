#include <libzt/zt.h>
#include <libzt/adt/zt_set.h>
#include "test.h"

int values0_19[] =  { 0, 1, 2, 3, 4, 5,
		      6, 7, 8, 9, 10,
		      11, 12, 13, 14, 15,
		      16, 17, 18, 19 };

#define MAX_LEN sizeof_array(values0_19)

int values0_19_fill[MAX_LEN];

static int union_test(void *data, void *param)
{
	int		  result = (int) data;
	static int	  t = 0;

	TEST_N("union", t, values0_19_fill[result] == 0);
	
	values0_19_fill[result] = result;
	
	return 0;
}

static int intersection_test(void *data, void *param)
{
	int		  result = (int) data;
	static int	  t = 0;

	TEST_N("intersection", t,
	       (values0_19_fill[result] == 0) &&
	       result >= MAX_LEN / 2);

	values0_19_fill[result] = result;
	
	return 0;
}

static int difference_test(void *data, void *param)
{
	int		  result = (int) data;
	static int	  t = 0;

	TEST_N("difference", t,
	       (values0_19_fill[result] == 0) &&
	       result <  MAX_LEN / 2);

	values0_19_fill[result] = result;
	
	return 0;
}


int
main(int argc, char *argv[])
{
	int	  i;
	zt_set	* result;
	zt_set	* set1;
	zt_set	* set2;

	
	set1 = zt_set_init(table_compare_int, NULL);
	set2 = zt_set_init(table_compare_int, NULL);

	for(i = 0; i < MAX_LEN; i++){
		zt_set_insert(set1, (void *)values0_19[i]);
		if(i >= MAX_LEN / 2) {
			zt_set_insert(set2, (void *)values0_19[i]);
		}
	}

	
	memset(values0_19_fill, 0, (MAX_LEN + 1) * sizeof(int));
	result = zt_set_init(table_compare_int, NULL);
	zt_set_union(result, set1, set2);
	zt_set_for_each(result, union_test, NULL);
	zt_set_destroy(result);

	memset(values0_19_fill, 0, (MAX_LEN + 1)  * sizeof(int));
	result = zt_set_init(table_compare_int, NULL);
	zt_set_intersection(result, set1, set2);
	zt_set_for_each(result, intersection_test, NULL);
	zt_set_destroy(result);

	memset(values0_19_fill, 0, (MAX_LEN + 1) * sizeof(int));
	result = zt_set_init(table_compare_int, NULL);
	zt_set_difference(result, set1, set2);
	zt_set_for_each(result, difference_test, NULL);
	zt_set_destroy(result);

	TEST("is_subset[0]", zt_set_is_subset(set2, set1));
	TEST("is_subset[1]", zt_set_is_subset(set1, set2) == 0);
	TEST("is_equal[0]", zt_set_is_equal(set1, set1));

	zt_set_destroy(set1);
	zt_set_destroy(set2);
	
	return 0;
}