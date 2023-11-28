#include "SphKernel2.h"
#include "doctest.h"

TEST_CASE("kernel test") {
	SphKernel2 kernel(1.0f);
	CHECK(kernel.h == 1.0f);
	CHECK(kernel.firstDerivative(2.0f) == 0.0f);
	CHECK(kernel.firstDerivative(0.5f) > 0.0f);
	CHECK(kernel.secondDerivative(1.0f) == 0.0f);
	CHECK(kernel.secondDerivative(0.5f) > 0.0f);
}
