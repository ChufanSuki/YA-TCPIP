//
// Created by allen on 10/2/20.
//

#ifndef NET_DEVICE_H
#define NET_DEVICE_H
/**
 * @file device.h
 * @brief Library supporting network device management.
 */

/**
 * Add a device to the library for sending/receiving packets.
 *
 * @param device Name of network device to send/receive packet on.
 * @return An non-negative _device-ID_ on success, -1 on error.
 */
int addDevice(const char *device);

/**
 * Find a device added by `addDevice`.
 *
 * @param device Name of the network device.
 * @return An non-negative _device-ID_ on success, -1 if no such device
 * was found.
 */
int findDevice(const char *device);

#endif //NET_DEVICE_H
