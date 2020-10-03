//
// Created by allen on 10/4/20.
//

#include "device_manager.h"
int addDevice(const char* device, struct DeviceManager& deviceManager) {
    if (findDevice(device, deviceManager) != -1)
        return -1;
    struct Device new_device{deviceManager.nxt_id++, device};
    deviceManager.size++;
    deviceManager.devices.push_back(new_device);
    return new_device.id;
}

int findDevice(const char *device, struct DeviceManager& deviceManager) {
    for (auto it = deviceManager.devices.begin(); it < deviceManager.devices.end(); it++) {
        if (it->name == device) {
            return it->id;
        }
    }
    return -1;
}
