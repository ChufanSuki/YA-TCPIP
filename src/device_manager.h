//
// Created by allen on 10/4/20.
//

#ifndef NET_DEVICE_MANAGER_H
#define NET_DEVICE_MANAGER_H

#include <vector>
#include <string>
#include <net/if.h>
#include <net/ethernet.h>
#include "packetio.h"
#include <pcap.h>

typedef int (*frameReceiveCallback)(const void *, int);

/**
 * Find all device
 *
 * @param dev_list List of device.
 * @return 1 on success, -1 on error.
 */
int findAllDevice(pcap_if_t *dev_list);

/**
 * Print all device
 *
 * @param dev_list List of device.
 */
void printDevice(pcap_if_t *dev_list);

/**
 * Tell if the device exists
 *
 * @param dev_list List of device.
 * @param name Device name
 * @return true on success, false on error.
 */
bool isContainDevice(pcap_if_t *dev_list, const char *name);

struct Device {
    int id;
    std::string name;
    ether_addr mac;
};

struct DeviceManager {
    int nxt_id; // increment by 1 after adding a new device
    int size;   // equals to nxt_id
    std::vector<struct Device> devices;
    frameReceiveCallback onReceived;
};

/**
 * Get the mac address
 *
 * @param mac Mac address to filled in
 * @param name Device name.
 * @return 1 on success, -1 on error.
 */
int get_mac(char *mac, const char *name);

/**
 * Add a device to the library for sending/receiving packets.
 *
 * @param device Name of network device to send/receive packet on.
 * @param deviceManager Instance of DeviceManager
 * @return An non-negative _device-ID_ on success, -1 on error.
 */
int addDevice(const char *device, struct DeviceManager &deviceManager);

/**
 * Find a device added by `addDevice`.
 *
 * @param device Name of the network device.
 * @param deviceManager Instance of DeviceManager
 * @return An non-negative _device-ID_ on success, -1 if no such device
 * was found.
 */
int findDevice(const char *device, struct DeviceManager &deviceManager);

/**
 * Find a device added by `addDevice`.
 *
 * @param id Device ID
 * @param deviceManager Instance of DeviceManager
 * @return device name
 */
const char *findDeviceById(const int id, struct DeviceManager &deviceManager);

#endif //NET_DEVICE_MANAGER_H
