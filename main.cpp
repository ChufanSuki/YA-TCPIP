#include "src/device.h"
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
int myOnReceived(const void *buf, int len)
{
    printf("\n[Function: myOnReceived]***************\n");
    printf("[Info] [Payload]: ");
    for (int i = 0; i < len; ++i)
    {
        printf("%02x ", *(u_int8_t *)((u_char *)buf + i));
    }
    printf("\n");
    return 0;
}

int main() {
//    char errBuf[PCAP_ERRBUF_SIZE] = {0};
//    pcap_if_t *alldevs;
//    if (pcap_findalldevs(&alldevs, errBuf) < 0)
//    {
//        perror("[Error] [findalldevs]");
//        return 0;
//    }
//    pcap_if_t *head = alldevs;
//    while (head->next != nullptr) {
//        printf("[Info] [Name: %s] [Description : %s]\n", head->name, head->description);
//        head = head->next;
//    }
    DeviceManager manager;
    int id = addDevice("wlp15s0", manager);
    if (id == -1) {
        std::cerr << "addDevice Failed" << std::endl;
        return 0;
    }
    while (1)
    {
        u_char * content = new u_char[100];
        memset(content, 15, 100);
        u_char * dest_mac = new u_char[6];
        memset(dest_mac, 255, 6);
        sendFrame(content, 100, 0x0800, dest_mac, id, manager);
        sleep(5);
    }
    return 0;
}