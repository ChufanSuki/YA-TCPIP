//
// Created by allen on 10/4/20.
//
/**
 * @file packetio.h
 * @brief Library supporting sending/receiving Ethernet II frames.
 */
#ifndef NET_PACKETIO_H
#define NET_PACKETIO_H


#include <netinet/ether.h>
#include <vector>
#include <cstring>
#include "device_manager.h"


struct buffer {
    int payload_size;
    int true_size;
    struct ether_header etherHeader;
    std::vector<uint8_t> payload;
};

/**
 * @brief Encapsule some data into an Ethernet II frame and send it.
 *
 * @param buf Pointer to the payload.
 * @param len Length of the payload.
 * @param ethtype EtherType field value of this frame.
 * @param destmac MAC address of the destination.
 * @param id ID of the device(returned by `addDevice`) to send on.
 * @return 0 on success, -1 on error.
 * @see addDevice
 */
int convert(struct buffer &buff, uint8_t *plain_buffer);

/**
 * @brief Encapsule some data into an Ethernet II frame and send it.
 *
 * @param buf Pointer to the payload.
 * @param len Length of the payload.
 * @param ethtype EtherType field value of this frame.
 * @param destmac MAC address of the destination.
 * @param id ID of the device(returned by `addDevice`) to send on.
 * @param deviceManager Instance of DeviceManager
 * @return 0 on success, -1 on error.
 * @see addDevice
 */
int sendFrame(const void *buf, int len,
              int ethtype, const void *destmac, int id, struct DeviceManager &deviceManager);

/**
 * @brief host to network string
 * @param buf pointer to string
 * @param len Length of string
 */
void hton_string(uint8_t *buf, int len, uint8_t *res);

/**
 * @brief Process a frame upon receiving it.
 *
 * @param buf Pointer to the frame.
 * @param len Length of the frame.
 * @param id ID of the device(returned by `addDevice`) receiving current
 * frame.
 * @return 0 on success, -1 on error.
 * @see addDevice
 */
typedef int (*frameReceiveCallback)(const void *, int);

/**
 * @brief Register a callback function to be called each time an Ethernet II
 * frame was received.
 *
 * @param callback The callback function.
 * @param deviceManager Instance of DeviceManager
 * @return 0 on success, -1 on error.
 * @see frameReceiveCallback
 */
int setFrameReceiveCallback(frameReceiveCallback callback, struct DeviceManager &deviceManager);

int myOnReceived(const void *buf, int len);


#endif //NET_PACKETIO_H
