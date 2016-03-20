#ifndef DNSCLIENT_H
#define DNSCLIENT_H

/**
 * This file holds the function prototypes for the UDP client used to query
 * the desired DNS server.
 *
 * @author Anthony Benavente
 * @version 03/19/2016
 */


#include <stdint.h>
#include "dnsmsg.h"

/**
 * This function sends a query to the specified ip address of a DNS server
 *
 * @param uint8_t [4] - This is an array of the 4 octets making up the IP
 *                      address of a DNS server
 * @param dnsmsg_t * -  This is the message to send to the DNS server
 * @return The dnsmsg_t * that was returned back from the DNS server
 */
dnsmsg_t *dnsclient_resolve(uint8_t ip[4], int port, dnsmsg_t *query);

#endif