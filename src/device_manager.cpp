//
// Created by allen on 10/4/20.
//

#include "device_manager.h"
#include <pcap.h>
#include <cstring>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <iostream>

int findAllDevice(pcap_if_t *dev_list) {
    char errBuf[PCAP_ERRBUF_SIZE];
    if (pcap_findalldevs(&dev_list, errBuf) != 0) {
        perror("[Error] [findAllDevice]");
        return -1;
    }
    return 1;
}

void printDevice(pcap_if_t *dev_list) {
    pcap_if_t *head = dev_list;
    while (head->next != nullptr) {
        printf("[Info] [Name: %s] [Description : %s]\n", head->name, head->description);
        head = head->next;
    }
}

bool isContainDevice(pcap_if_t *dev_list, const char *name) {
    pcap_if_t *head = dev_list;
    while (head->next != nullptr) {
        if (strcmp(head->name, name) == 0) {
            return true;
        }
        head = head->next;
    }
    return false;
}

int get_mac(char *mac, const char *name) {
    struct ifreq ifreq;
    int sock;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return -1;
    }
    strcpy(ifreq.ifr_name, name);
    if (ioctl(sock, SIOCGIFADDR, &ifreq) < 0) {
        perror("ioctl");
        return -1;
    }
    return snprintf(mac, ETH_ALEN, "%X:%X:%X:%X:%X:%X", (unsigned char) ifreq.ifr_hwaddr.sa_data[0],
                    (unsigned char) ifreq.ifr_hwaddr.sa_data[1], (unsigned char) ifreq.ifr_hwaddr.sa_data[2],
                    (unsigned char) ifreq.ifr_hwaddr.sa_data[3], (unsigned char) ifreq.ifr_hwaddr.sa_data[4],
                    (unsigned char) ifreq.ifr_hwaddr.sa_data[5]);
}

int addDevice(const char *device, struct DeviceManager &deviceManager) {

    pcap_if_t *dev_list;
    char errBuf[PCAP_ERRBUF_SIZE];
    if (pcap_findalldevs(&dev_list, errBuf) != 0) {
        perror("[Error] [findAllDevice]");
    }
    printDevice(dev_list);
    if (!isContainDevice(dev_list, device)) {
        std::cerr << "Device Not Found" << std::endl;
        return -1;
    }
    if (findDevice(device, deviceManager) != -1) {
        std::cerr << "Device already added" << std::endl;
        return -1;
    }
    struct Device new_device{deviceManager.nxt_id++, device};
    deviceManager.size++;
    deviceManager.devices.push_back(new_device);
    return new_device.id;
}

int findDevice(const char *device, struct DeviceManager &deviceManager) {
    for (auto it = deviceManager.devices.begin(); it < deviceManager.devices.end(); it++) {
        if (it->name == device) {
            return it->id;
        }
    }
    return -1;
}

const char *findDeviceById(const int id, struct DeviceManager &deviceManager) {
    for (auto it = deviceManager.devices.begin(); it < deviceManager.devices.end(); it++) {
        if (it->id == id) {
            return it->name.c_str();
        }
    }
    return nullptr;
}
