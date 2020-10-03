//
// Created by allen on 10/4/20.
//

#ifndef NET_DEVICE_MANAGER_H
#define NET_DEVICE_MANAGER_H
#include "device.h"
#include <vector>
#include <string>
struct Device {
    int id;
    std::string name;
};
struct DeviceManager {
    int nxt_id; // increment by 1 after adding a new device
    int size;   // equals to nxt_id
    std::vector<struct Device> devices;
};


/**
 * Add a device to the library for sending/receiving packets.
 *
 * @param device Name of network device to send/receive packet on.
 * @return An non-negative _device-ID_ on success, -1 on error.
 */
int addDevice(const char *device, struct DeviceManager& deviceManager);

/**
 * Find a device added by `addDevice`.
 *
 * @param device Name of the network device.
 * @return An non-negative _device-ID_ on success, -1 if no such device
 * was found.
 */
int findDevice(const char *device, struct DeviceManager& deviceManager);
#endif //NET_DEVICE_MANAGER_H
