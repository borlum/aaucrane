#include <stdio.h>
#include "../cranelib.h"
#include "minunit.h"

int tests_run = 0;

static char * test_initialize_crane()
{
	int ret = 0;
	if (initialize_crane() == 0)
		ret = 1;
	assert("Error; initialize did not return 0 as expected", ret != 0);
	return 0;
}

static char * test_run_motorx()
{
	int ret = 0;

	/*Testing edges*/
	int out = run_motorx(14);
	if (out == 4096) {
		ret = 1;
	}

	out = run_motorx(-14);
	if (out == 0) {
		ret = 1;
	}

	/*Testing out of bounds (+/- 14V)*/
	out = run_motorx(20);
	if (out == 4096) {
		ret = 1;
	}

	out = run_motorx(-20);
	if (out == 0) {
		ret = 1;
	}

	assert("Error; run_motorx not behaving!", ret != 0);
	return 0;
}

static char * test_run_motory()
{
	int ret = 0;

	/*Testing edges*/
	int out = run_motory(14);
	if (out == 4096) {
		ret = 1;
	}

	out = run_motory(-14);
	if (out == 0) {
		ret = 1;
	}

	/*Testing out of bounds (+/- 14V)*/
	out = run_motory(20);
	if (out == 4096) {
		ret = 1;
	}

	out = run_motory(-20);
	if (out == 0) {
		ret = 1;
	}

	assert("Error; run_motory not behaving!", ret != 0);
	return 0;
}

static char * testsuite()
{
	run_test(test_initialize_crane);
	run_test(test_run_motorx);
	run_test(test_run_motory);
	return 0;
}

int main(int argc, char **argv)
{
	char *result = testsuite();
	if (result != 0) {
	    printf("%s\n", result);
	} else {
	    printf("ALL TESTS PASSED\n");
	}
	printf("Tests run: %d\n", tests_run);
	return result != 0;
}