//
// Created by allen on 10/13/20.
//

#ifndef NET_UTIL_H
#define NET_UTIL_H

/**
 * @brief Calculate CRC-32
 *
 * @param buffer Pointer to the frame.
 * @param len Length of the frame.
 * @return crc32
 */
unsigned int calculateCRC32(unsigned char *buffer, unsigned int len);


#endif //NET_UTIL_H
