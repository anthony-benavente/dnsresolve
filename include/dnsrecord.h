#ifndef DNS_RECORD_H
#define DNS_RECORD_H

/** 
 * This file holds the definition for the dnsrecord_t struct as well as 
 * accompanying functions to aide in its use. We really should only be using
 * the parse function in this file since we are not a DNS server creating 
 * records.
 *
 * @author Anthony Benavente
 * @version 03/20/2016
 */

#include <stdint.h>

/**
 * Represents resource records distributed by DNS servers.
 *
 * This struct holds the components that make up resource records as they are
 * specified in RFC 1035. There are 5 components:
 *  1. The domain name
 *  2. The type of record this is (A, AAAA, CNAME, etc...)
 *  3. The class associated with this record (IN, CH, etc...)
 *  4. The ttl or time-to-live. This is used to determine how long the DNS 
 *     packet should stay "alive" on a line
 *  5. A number saying how much data extra data is stored in the record
 *  6. The actual extra data stored in the record
 */
typedef struct {
    /** The domain name of this resource record. */
    char *name;
    
    /** The type of record this is (A, CNAME, etc...) */
    uint16_t type;
    
    /** The class associated with this record */
    uint16_t rclass;
    
    /** Used to determine how many "hops" this packet can make */
    uint32_t ttl;
    
    /** Number of bytes that the data takes up. */
    uint16_t rdlength;
    
    /** The extra data stored in the resource record (changes based on type) */
    uint8_t *rdata;
} dnsrecord_t;

/**
 * Creates a new dnsrecord_t object and returns a pointer to said object.
 *
 * @return a pointer to the allocated space for the dnsrecord_t object. Be 
 * sure to free this.
 */
dnsrecord_t *dnsrecord_new(void);

/** 
 * Frees the dnsrecord_t pointer and any data stored inside that was 
 * dynamically allocated.
 *
 * @param dnsrecord_t * - the dnsrecord_t to free
 */
void dnsrecord_free(dnsrecord_t *);

/**
 * Prints a pretty version of this dnsrecord.
 *
 * @param record The record to print
 */
void dnsrecord_print(dnsrecord_t *record);

#endif