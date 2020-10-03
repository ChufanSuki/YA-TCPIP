//
// Created by allen on 10/4/20.
//
#include "gtest/gtest.h"
#include "../src/device_manager.h"
#include "../src/device.h"

TEST(DeviceTest, AddDevice) {
    std::vector<struct Device> devices;
    DeviceManager deviceManager{0, 0, devices};
    addDevice("eth0", deviceManager);
    EXPECT_EQ(deviceManager.size, 1);
    ASSERT_STREQ(deviceManager.devices[0].name.c_str(), "eth0");
}

