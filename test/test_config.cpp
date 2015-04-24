#include <iostream>
using namespace std;
#include "gtest/gtest.h"

#include "src/config.h"
#include "config.h"


// The fixture for testing class Config. From google test primer.
class ConfigTest : public ::testing::Test {
protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    ConfigTest() {
        // You can do set-up work for each test here.
    }

    virtual ~ConfigTest() {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:
    virtual void SetUp() {
        // Code here will be called immediately after the constructor (right
        // before each test).
    }

    virtual void TearDown() {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }

    // Objects declared here can be used by all tests in the test case for Project1.
    // config->init("./template/config/eth1.conf");
};

// Test case must be called the class above
// Also note: use TEST_F instead of TEST to access the test fixture (from google test primer)
TEST_F(ConfigTest, GetConfig) {
    string device = config->get("device");
    EXPECT_EQ(device, "eth1");
}
