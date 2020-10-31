# Protocol Stack

In this lab, you will implement a userspace C/C++ program based on [`libpcap`](http://www.tcpdump.org/) to replace layer 2/3/4 of kernel protocol stack from scratch and let applications talk with other Linux machines using your own protocol stack. 

After finishing this lab, you are expected to:

- Deeply understand:

    * Classic TCP/IP standards and their implementation
    * How to implement a usable system by programming

- Be (more) familiar with:

    * Bash scripts
    * Ethernet frames
    * `man` pages & RFCs
    * Usage of Linux network tools

## Handin Instructions

- `src/`

    Source code of your programs, including the protocol stack and self-built programs for evaluating.

- `Makefile`

    Makefile for building your program. It's default target should generate a program named `protocol-stack` in `bin/`.

- `codelist.[pdf|html|md|docx|txt]`

    A single document describing where you placed your solution for each programming tasks at.

- `writing-task.[pdf|html|md|docx|txt]`

    A single document including your solution to all writing tasks.

- `not-implemented.[pdf|html|md|docx|txt]`

    A single document listing the features that specified by this document/the standards but you didn't implemented in non-optional programming tasks. You also need to explicitly give the reason why you are not implementing it for each feature. Submit an empty file if you completely finished all non-optional tasks.

## Targets


### 1. Link-layer: Packet I/O On Ethernet

For many reasons, Link-layer is quite complex. One of them is that you can translate bit stream into frames in many different ways. In this part, we will work with [Ethernet II](https://en.wikipedia.org/wiki/Ethernet_frame#Ethernet_II) frames. Ethernet II is supported by [Linux virtual ethernet device](http://man7.org/linux/man-pages/man4/veth.4.html) and almost all [GigE](https://en.wikipedia.org/wiki/Gigabit_Ethernet) NICs. In other words, you can use Ethernet II frames to communicate with your wired router/switch, or other nodes in a virtual network on your Linux machine.

- Tasks

    1. (__PT__)Implement following methods to support network device management.

        ```c
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
        int addDevice(const char* device);

        /**
         * Find a device added by `addDevice`.
         *
         * @param device Name of the network device.
         * @return An non-negative _device-ID_ on success, -1 if no such device 
         * was found.
         */
        int findDevice(const char* device);
        ````

    2. (__PT__)Use `libpcap` to implement the following methods to support sending/receiving Ethernet II frames.

        ```c
        /** 
         * @file packetio.h
         * @brief Library supporting sending/receiving Ethernet II frames.
         */

        #include <netinet/ether.h>

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
        int sendFrame(const void* buf, int len, 
            int ethtype, const void* destmac, int id);

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
        typedef int (*frameReceiveCallback)(const void*, int, int);

        /**
         * @brief Register a callback function to be called each time an Ethernet II 
         * frame was received.
         *
         * @param callback The callback function.
         * @return 0 on success, -1 on error.
         * @see frameReceiveCallback
         */
        int setFrameReceiveCallback(frameReceiveCallback callback);
        ```

- Hints & Instructions 

    * Check your byte order!

    * In this part, you don't need to compute the CRC bits when building a ethernet frame.

### 2. Network-layer: IP Protocol

After completing part 1 you should be able to send something to some other place via Ethernet(if there're no fragmented packets!), but as you've learnt, the range is strictly limited. In this part, you will implement a simplified version of [IP protocol, version 4](https://tools.ietf.org/html/rfc791) to expand this range to the whole Internet.

- Tasks

    1. (__PT__)Update the method in `device.h`, Use the library in `packetio.h` to implement the following methods to support sending/receving IP packets.

        You should follow [RFC791](https://tools.ietf.org/html/rfc791) when working on this.

        ```c
        /** 
         * @file ip.h
         * @brief Library supporting sending/receiving IP packets encapsuled in an 
         * Ethernet II frame.
         */
        
        #include <netinet/ip.h>
        
        /**
         * @brief Send an IP packet to specified host. 
         *
         * @param src Source IP address.
         * @param dest Destination IP address.
         * @param proto Value of `protocol` field in IP header.
         * @param buf pointer to IP payload
         * @param len Length of IP payload
         * @return 0 on success, -1 on error.
         */
        int sendIPPacket(const struct in_addr src, const struct in_addr dest, 
            int proto, const void *buf, int len);
        
        /** 
         * @brief Process a IP packet upon receiving it.
         *
         * @param buf Pointer to the packet.
         * @param len Length of the packet.
         * @return 0 on success, -1 on error.
         * @see addDevice
         */
        typedef int (*IPPacketReceiveCallback)(const void* buf, int len);
        
        /**
         * @brief Register a callback function to be called each time an IP packet
         * was received.
         *
         * @param callback The callback function.
         * @return 0 on success, -1 on error.
         * @see IPPacketReceiveCallback
         */
        int setIPPacketReceiveCallback(IPPacketReceiveCallback callback);
        
        /**
         * @brief Manully add an item to routing table. Useful when talking with real 
         * Linux machines.
         * 
         * @param dest The destination IP prefix.
         * @param mask The subnet mask of the destination IP prefix.
         * @param nextHopMAC MAC address of the next hop.
         * @param device Name of device to send packets on.
         * @return 0 on success, -1 on error
         */
        int setRoutingTable(const struct in_addr dest, const struct in_addr mask, 
            const void* nextHopMAC, const char *device);
        ```

    2. (__WT__)`sendFrame()` requires the caller to provide the destination MAC address when sending IP packets, but users of IP layer won't provide the address to you. Explain how you addressed this problem when implementing IP protocol.

    3. (__WT__)Describe your routing algorithm. 

    4. (__WT__)To implement routing properly, you need to detect other hosts/let other hosts know about you. Since we are to communicate with other Linux machines, you can't assume that all hosts in the network are running your protocol stack. In this lab, you are not required to detect hosts not running your protocol stack automatically/let them know about you, but you must not make them complain about strange incoming packets. Describe how your IP implementation achieved this goal.

- Hints & Instructions

    * Check your byte order!!

    * When designing a usable system, you should always take all corner cases into consideration. Carefully describe which corner cases will your system meet and what's your solution when writing for Tasks 2/3.

    * You are not required to implement IP packet fragmentation, simply drop fragmented packets is OK.

    * You are not required to support TOS and IP options. Use a default value when sending, ignore them when receiving.

    * Your implementation should be reentrant.

### 3. Transport-layer: TCP Protocol

With the help of IP protocol, you can talk with any host on the Internet now. In this part, you will implement a simplified version of [TCP protocol](https://tools.ietf.org/html/rfc793), provide a subset of [POSIX-compatible socket interfaces](https://en.wikipedia.org/wiki/Berkeley_sockets) to the applications.

- Tasks

    1. (__PT__)Use the interfaces provided by `ip.h` to implement the following POSIX-compatible interfaces.

        You should follow [RFC793](https://tools.ietf.org/html/rfc793) when working on this. You are also expected to keep compatibility with [POSIX.1-2017 standard](http://pubs.opengroup.org/onlinepubs/9699919799) when implementing your socket interfaces, but that's just for your applications to run correctly, you will __NOT__ lose credits if your interfaces behave slightly different.

        Note: You can use a file descriptor allocating algorithm slightly different from the standardized one to avoid conflicts with fds allocated by the system.

        ```c
        /** 
         * @file socket.h
         * @brief POSIX-compatble socket library supporting TCP protocol on IPv4.
         */
         
        #include <sys/types.h>
        #include <sys/socket.h>
        #include <netdb.h>

        /**
         * @see [POSIX.1-2017:socket](http://pubs.opengroup.org/onlinepubs/
         * 9699919799/functions/socket.html)
         */
        int __wrap_socket(int domain, int type, int protocol);
        
        /**
         * @see [POSIX.1-2017:bind](http://pubs.opengroup.org/onlinepubs/
         * 9699919799/functions/bind.html)
         */
         int __wrap_bind(int socket, const struct sockaddr *address,
            socklen_t address_len);
         
        /**
         * @see [POSIX.1-2017:listen](http://pubs.opengroup.org/onlinepubs/
         * 9699919799/functions/listen.html)
         */
        int __wrap_listen(int socket, int backlog);

        /**
         * @see [POSIX.1-2017:connect](http://pubs.opengroup.org/onlinepubs/
         * 9699919799/functions/connect.html)
         */
        int __wrap_connect(int socket, const struct sockaddr *address,
            socklen_t address_len);

        /**
         * @see [POSIX.1-2017:accept](http://pubs.opengroup.org/onlinepubs/
         * 9699919799/functions/accept.html)
         */
        int __wrap_accept(int socket, struct sockaddr *address,
            socklen_t *address_len);
        
        /**
         * @see [POSIX.1-2017:read](http://pubs.opengroup.org/onlinepubs/
         * 9699919799/functions/read.html)
         */
        ssize_t __wrap_read(int fildes, void *buf, size_t nbyte);
        
        /**
         * @see [POSIX.1-2017:write](http://pubs.opengroup.org/onlinepubs/
         * 9699919799/functions/write.html)
         */
        ssize_t __wrap_write(int fildes, const void *buf, size_t nbyte);

        /**
         * @see [POSIX.1-2017:close](http://pubs.opengroup.org/onlinepubs/
         * 9699919799/functions/close.html)
         */
        ssize_t __wrap_close(int fildes);

        /** 
         * @see [POSIX.1-2017:getaddrinfo](http://pubs.opengroup.org/onlinepubs/
         * 9699919799/functions/getaddrinfo.html)
         */
        int __wrap_getaddrinfo(const char *node, const char *service,
            const struct addrinfo *hints,
            struct addrinfo **res);
        ```

    2. (__PT, Optional__)Implement your own TCP flow control algorithm.

    3. (__PT, Optional__)Implement your own TCP congestion control algorithm.

    4. (__WT__)Describe how you correctly handled TCP state changes.

    5. (__WT__)Describe how you implemented in-order data transferring.

    6. (__WT, Optional__)Describe your TCP flow control algorithm.

    7. (__WT, Optional__)Describe your TCP congestion control algorithm.

- Hints & Instructions

    * Check your byte order!!!

    * Carefully describe which corner cases will your system meet and what's your solution when writing for Tasks 4/5/6/7.

    * You are not required to support TCP options other than those specified in RFC793, but you may be interested in these topics when implemeting flow control algorithms and congestion control algorithms:

        + [TCP Selective-ACK Option](https://tools.ietf.org/html/rfc2018)
        + [TCP Timestamp Option, TCP Window Scale Option](https://tools.ietf.org/html/rfc7323) and [TCP RTTM Mechanism](https://tools.ietf.org/html/rfc6298)

    * You are not required to support any socket option.

    * You are not required to support TCP out-of-band data.

    * You are not required to implement any sort of URL resolving mechanism. When implementing `getaddrinfo`, you only need to let it work when:
    
        + `node` is a valid IPv4 address or `NULL`
        + `service` is a valid port number or `NULL`
        + `hints` has `.ai_family == AF_INET`, `.ai_socktype == IPPROTO_TCP`, `.ai_flags == 0` or `hints == NULL`

    * Your implementation should be reentrant, as specified by POSIX standard.

    * Although TCP flow control is an optional task, you still need to set `window` field in TCP header properly when sending and try not to make bytes-in-flight value larger than the receive window to let your TCP work correctly. Try to implement something significantly better than approach like using a default value for the TCP flow control tasks if you would like to get the credits.

    * To be robust, your implementation of `read`, `write` and `close` should fallback to the real library functions when processing fds not allocated by yourself.

    * Being POSIX-compatible doesn't necessarily require you to implement all functionalities implemented by Linux. Just check the arguments and fail if required feature is not to be implemented.

    * You are not required to develop a brand new flow control algorithm or congestion control algorithm. Feel free to design, but implementing something already exists will be enough for this lab.

### 4. Test/Evaluation

This is an open task. In this part, you will come up with a way to prove that your program works robustly and efficiently by yourself and do evaluation, just like what you will do for your works to be submitted:). There's only one constraint: when using applications to test your TCP implementation, your application must use standard POSIX socket interfaces. In other words, the applications should run correctly on Linux machines.

- Tasks

    1. (__PT__)Implement a method to initialize your evaluating environment.

    2. (__WT__)Describe your evaluating methodology, result and usage of your evaluating method. Hosts running your protocol stack should be able to communicate with each other and real machines running an off-the-shelf OS. 

## Testing(Playing With) Your Program

As mentioned above, we won't provide a standardized way for you to test/evaluate your implementation, it's your responsibility to complete this part by yourself. But unfortunately, this requires quite a handful of knowledge that shouldn't be required in a network lab focusing on TCP/IP. This section thus describes some useful techniques to enpower you to force network programs to use your TCP interface, test your program in a emulated network, and communicate with real Linux machines. However, you are encouraged to explore how those things work and get your hands on them.

### Hijack Library Functions

We won't be happy if we must develop some brand new example program using our TCP interface just to test it. So here comes the problem: If a network program uses functionalities already implemented by us only, can we force it to use our implementations?

Yes, by using a alternate library function when compiling a program from source:

By specifing `--wrap [fun]` when invoking [`ld`](http://man7.org/linux/man-pages/man1/ld.1.html), any undefined reference to `[fun]` will be resolved to `__wrap_[fun]`, any undefined reference to `__real_[fun]` will be resolved to `[fun]`. By specifing `-Wl` option when invoking [`gcc`](http://man7.org/linux/man-pages/man1/gcc.1.html), you can pass options to the linker. 

We offer a simple homemade TCP traffic generating tool, `mperf`, which uses functionalities already implemented by yourself to you for doing tests. See its [`README.md`](mperf/README.md) for detailed usage. (You can use whatever program other than `mperf` to do evaluation if you like. We are offering it just for your convenience)

Example: to hijack `socket()`, implement a function named `__wrap_socket` with desired functionality, then specify `-Wl,--wrap,socket` will let the program use your version of `socket()`, you can still call the real `socket()` by calling `__real_socket()`.

Note: You may want to implement `send`/`recv`, `sendto`/`recvfrom` as a wrapper of `write`/`read` to support programs using those functions.

### Create Virtual Networks

It's naturally impossible for us to test our implementation in a large real network: just because we don't have that many computers. For network system designers, it's often essential to be able to create some sort of virtual networks for testing. Here we present a homemade tool, `vnetUtils`, to create & use virtual networks with specified topology, with the constraint that there are no redundant links.

`vnetUtils` is a set of small bash scripts, providing the functionalities of creating virtual hosts, connecting them & running commands on them. with the help of it you can easily develop scripts to create virtual networks with desired topology. See its [`README.md`](vnetUtils/README.md) for detailed usage.

### Disable The Kernel Protocol Stack 

By using `libpcap`, we can monitor the packet flow and send packets. But the kernel protocol stack won't be disabled, which is not desired by us. To disable the kernel network protocol stack, we can disable the IP module By filtering packets using [`iptables`](http://man7.org/linux/man-pages/man8/iptables.8.html).

To disable the IP module on specified host, use `filter` table to drop all packets:

```bash
# Drop all packets
iptables -t filter -I FORWARD -j DROP
iptables -t filter -I INPUT -j DROP
iptables -t filter -I OUTPUT -j DROP
# Revert
iptables -t filter -D FORWARD -j DROP
iptables -t filter -D INPUT -j DROP
iptables -t filter -D OUTPUT -j DROP
```

Note: This method is integrated in `vnetUtils`. 

### Emulate _Bad_ Links

It's hard to test if your program can survive specific corner case on a typical local/wired link: packet losses and congestions are very unlikely to happen. To emulate such a link, you can use [`tc-netem`](http://man7.org/linux/man-pages/man8/tc-netem.8.html) provided by Linux itself. You can thus add delay, loss to a link, or limit its bandwidth.

You can find more examples at [networking:netem [Linux Foundation Wiki]](https://wiki.linuxfoundation.org/networking/netem).

### Talk With Real Linux Machine

To talk with a Linux machine(or machine running Windows, BSD, etc), you need to configure routing table on all hosts on the path correctly. Implementing an automatic(and often distributed) solution using packets is hard, but things can be easier with some manual methods. The problem comes in two directions: for you, you need to know how to reach the peer. This can be easily done by calling `setRoutingTable()` implemented by yourself earlier to set routing table manually. Similarily, for others, they need to know how to reach you. This can be done using [`ip route`](http://man.he.net/man8/ip-route).

```bash
# Set routing table
ip route add [your IP/subnet] via [next hop IP] dev [device name]
# Revert
ip route del [your IP/subnet] via [next hop IP] dev [device name]
```

Specially, if the host is in your LAN, you should use the script below instead:

```bash
# Set routing table
ip route add [your IP/subnet] dev [device name]
# Revert
ip route del [your IP/subnet] dev [device name]
```

Note: Carefully check your implementation before talking to a real machine. Linux network protocol stack is filled with sanity checks, the kernel may drop a strange-looked packet without notifying anybody. 

