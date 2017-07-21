// For prefix tries
#include <cstdio>
#include <cstdlib>

#include <cedar_config.h>
#include <cedarpp.h>

#include <gtest/gtest.h>

#include "suffix_test.cc"
#include "match_and_predict_test.cc"

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
