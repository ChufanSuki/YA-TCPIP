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

/**
 * @cite https://developer.ibm.com/technologies/systems/articles/au-endianc/
 */
const int i = 1;
#define is_bigendian() ( (*(char*)&i) == 0 )

void hton_string(uint8_t *buff, int len, uint8_t *res) {
    if (!is_bigendian()) {
        for (int i = 0; i < len; i++) {
            res[len - 1 - i] = buff[i];
        }
    }
}

int myOnReceived(const void *buf, int len) {
    printf("\n[Function: myOnReceived]***************\n");
    printf("[Info] [Payload]: ");
    for (int i = 0; i < len; ++i) {
        printf("%02x ", *(u_int8_t *) ((u_char *) buf + i));
    }
    printf("\n");
    return 0;
}

int convert(struct buffer &buff, uint8_t *plain_buffer) {
    memcpy(plain_buffer, buff.etherHeader.ether_dhost, ETH_ALEN);
    memcpy(plain_buffer + ETH_ALEN, buff.etherHeader.ether_shost, ETH_ALEN);
    memcpy(plain_buffer + 2 * ETH_ALEN, &buff.etherHeader.ether_type, ETHER_TYPE_LEN);
    memcpy(plain_buffer + ETHER_HDR_LEN, buff.payload.data(), buff.payload_size);
    return 1;
}

int sendFrame(const void *buf, int len,
              int ethtype, const void *destmac, int id, struct DeviceManager &deviceManager) {
    printf("\n[Function: sendFrame]***************\n");
    int size = len + ETH_HLEN + ETHER_CRC_LEN;
    char srcmac[ETH_ALEN];
    if (!ETHER_IS_VALID_LEN(size)) {
        std::cerr << "payload is too short: " << len << std::endl;
        return -1;
    }
    const char *dev;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *descr;
    struct ether_header etherHeader;  /* net/ethernet.h */

    dev = findDeviceById(id, deviceManager);

    descr = pcap_open_live(dev, BUFSIZ, 0, 100, errbuf);

    if (descr == nullptr) {
        printf("pcap_open_live(): %s\n", errbuf);
        return -1;
    }
    std::vector<uint8_t> data((uint8_t *) buf, (uint8_t *) buf + len);
    memcpy(etherHeader.ether_dhost, destmac, sizeof(uint8_t) * ETH_ALEN);
    get_mac(srcmac, findDeviceById(id, deviceManager));
    memcpy(etherHeader.ether_shost, srcmac, sizeof(uint8_t) * ETH_ALEN);
    etherHeader.ether_type = ethtype;
    struct buffer buff{len, size - ETHER_CRC_LEN, etherHeader, data};
    uint8_t plain_buffer[size - ETHER_CRC_LEN];
    convert(buff, plain_buffer);
    uint8_t mes[size - ETHER_CRC_LEN];
    hton_string(plain_buffer, size - ETHER_CRC_LEN, mes);
    if (pcap_sendpacket(descr, mes, size - ETHER_CRC_LEN) != 0) {
        fprintf(stderr, "\nError sending the packet: %s\n", pcap_geterr(descr));
        return -1;
    }

    return 1;
}


int setFrameReceiveCallback(frameReceiveCallback callback, DeviceManager &deviceManager) {
    printf("\n[Function: setFrameReceiveCallback]***************\n");
    if (callback == nullptr)
        return -1;
    deviceManager.onReceived = callback;
    return 1;
}