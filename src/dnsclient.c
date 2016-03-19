#include "dnsclient.h"

#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_BUF_SIZE 512

dnsmsg_t *nsclient_resolve(uint8_t ip[4], int port, dnsmsg_t *query) {
    // This buffer is used to send AND receive information
    uint8_t buf[MAX_BUF_SIZE];
    uint8_t *buf_ptr = buf;
    uint8_t *buf_ptr_start = buf_ptr;
    int buf_len = dnsmsg_to_bytes(query, &buf_ptr);
    int bytes_sent, bytes_rcvd;
    unsigned int addrlen;
    
    // Set up UDP socket that we will be sending the query out of
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    struct sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port   = htons(port);
    dest_addr.sin_addr.s_addr = 
        (ip[0] << 24) | (ip[1] << 16) | (ip[2] << 8)  | ip[3];
    addrlen = sizeof(dest_addr);
    
    // Send query to the appropriate place
    printf("Sending data to %d.%d.%d.%d:%d\n", ip[0], ip[1], ip[2], ip[3], port);
    bytes_sent = sendto(sockfd, buf, buf_len, 0, 
        (struct sockaddr *) &dest_addr, addrlen);
    if (bytes_sent < 0) {
        perror("sendto");
        _exit(EXIT_FAILURE);
    }
        
    // Block until we receive a response from the DNS server or we timeout
    bytes_rcvd = recvfrom(sockfd, buf, MAX_BUF_SIZE, 0, 
        (struct sockaddr *) &dest_addr, &addrlen);
    if (bytes_rcvd < 0) {
        perror("recvfrom");
        _exit(EXIT_FAILURE);
    }
    
    // Close UDP socket
    close(sockfd);
    
    // Return parsed information packaged in a dnsmsg_t
    return dnsmsg_construct(buf, bytes_rcvd);
}