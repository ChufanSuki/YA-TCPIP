#include "src/util.h"
#include "src/device_manager.h"
#include <pcap.h>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <netinet/if_ether.h>
#include <unistd.h>
#include <iostream>

/*
Description of this file:
1) find all devices and try to add them to device_list;
2) each device will send a packet to "ff:ff:ff:ff:ff:ff" every 5 seconds;
3) During Device initianlizing, it will create a thread using "pcap_loop" to receive packets;
*/
int main() {
    char errBuf[PCAP_ERRBUF_SIZE] = {0};
    pcap_if_t *alldevs;
    if (pcap_findalldevs(&alldevs, errBuf) < 0) {
        perror("[Error] [findalldevs]");
        return 0;
    }
    pcap_if_t *head = alldevs;
    DeviceManager manager;
    setFrameReceiveCallback(myOnReceived, manager);
    while (head->next != nullptr) {
        printf("[Info] [Name: %s] [Description : %s]\n", head->name, head->description);
        if (addDevice(head->name, manager) < 0) {
            std::cerr << "[Info] [Name:] " << head->name << " add failed!\n";
        } else {
            std::cout << "[Info] [Name:] " << head->name << " add succeeded!\n";
        }
        std::cout << "\n";
        head = head->next;
    }
    while (1) {
        for (auto &device : manager.devices) {
            fprintf(stdout, "[name: %s] [id: %d]\n", device.name.c_str(), device.id);
            u_char *content = new u_char[100];
            memset(content, 15, 100);
            u_char *dest_mac = new u_char[6];
            memset(dest_mac, 255, 6);
            sendFrame(content, 100, 0x0800, dest_mac, device.id, manager);
            fprintf(stdin, "CRC of this frame is %0X\n", calculateCRC32(content, 100));
            delete[] content;
            delete[] dest_mac;
        }
        sleep(5);
    }
    return 0;
}