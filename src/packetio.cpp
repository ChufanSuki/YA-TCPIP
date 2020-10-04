//
// Created by allen on 10/4/20.
//

#include "packetio.h"
#include <net/ethernet.h>
#include <netinet/if_ether.h>
#include <net/if_packet.h>
#include <pcap.h>
#include <iostream>
#include <pcap.h>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <netinet/if_ether.h>
#include "device_manager.h"
#include <cstring>

int convert(struct buffer& buff, uint8_t* plain_buffer) {
    memcpy(plain_buffer, buff.etherHeader.ether_dhost, ETH_ALEN);
    memcpy(plain_buffer + ETH_ALEN, buff.etherHeader.ether_shost, ETH_ALEN);
    memcpy(plain_buffer + 2*ETH_ALEN, &buff.etherHeader.ether_type, ETHER_TYPE_LEN);
    memcpy(plain_buffer + ETHER_HDR_LEN, buff.payload.data(), buff.payload_size);
    return 1;
}

int sendFrame(const void* buf, int len,
              int ethtype, const void* destmac, int id, struct DeviceManager& deviceManager) {
    printf("\n[Function: sendFrame]***************\n");
    int size = len + ETH_HLEN + ETHER_CRC_LEN;
    char srcmac[ETH_ALEN];
    if (!ETHER_IS_VALID_LEN(size)) {
        std::clog << "payload is too short: " << len << std::endl;
        return -1;
    }
    const char *dev;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* descr;
    struct ether_header etherHeader;  /* net/ethernet.h */

    dev = findDeviceById(id, deviceManager);

    descr = pcap_open_live(dev, BUFSIZ, 0, 100, errbuf);

    if(descr == nullptr)
    {
        printf("pcap_open_live(): %s\n",errbuf);
        return -1;
    }
    std::vector<uint8_t> data((uint8_t*)buf, (uint8_t*)buf + len);
    memcpy(etherHeader.ether_dhost, destmac, sizeof(uint8_t)*ETH_ALEN);
    get_mac(srcmac, findDeviceById(id, deviceManager));
    memcpy(etherHeader.ether_shost, srcmac, sizeof(uint8_t) * ETH_ALEN);
    memcpy(reinterpret_cast<void *>(etherHeader.ether_type), reinterpret_cast<const void *>(ethtype), sizeof(uint16_t));

    struct buffer buff{len, size-ETHER_CRC_LEN, etherHeader, data};
    uint8_t plain_buffer[size-ETHER_CRC_LEN];
    convert(buff, plain_buffer);
    if (pcap_sendpacket(descr, plain_buffer , size-ETHER_CRC_LEN) != 0) {
        fprintf(stderr,"\nError sending the packet: %s\n", pcap_geterr(descr));
        return -1;
    }

    return 1;
}


int setFrameReceiveCallback(frameReceiveCallback callback, DeviceManager deviceManager) {
    printf("\n[Function: setFrameReceiveCallback]***************\n");
    if (callback == nullptr)
        return -1;
    deviceManager.onReceived = callback;
    return 1;
}