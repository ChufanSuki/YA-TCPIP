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

int main() {
    std::vector<struct Device> devices;
    DeviceManager deviceManager{0, 0, devices};
    printf("%d", addDevice("eth0", deviceManager));
    printf("%d\n", findDevice("eth0", deviceManager));
    printf("%d", addDevice("eth1", deviceManager));
    printf("%d", findDevice("eth1", deviceManager));
}