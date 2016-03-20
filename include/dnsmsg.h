#ifndef DNSMESSAGE_H
#define DNSMESSAGE_H

/**
 * This file holds the definition for the dnsmsg_t struct, structs for its 
 * components (dnsmsg_header and dnsmsg_query), as well as function 
 * prototypes used in junction with these structs.
 *
 * @author Anthony Benavente
 * @version 03/19/2016 
 */

#include <stdint.h>
#include <stdlib.h>

#include "dnsrecord.h"

#define DNSMSG_OPT_QR 0x8000
#define DNSMSG_OPT_AA 0x400
#define DNSMSG_OPT_TC 0x200
#define DNSMSG_OPT_RD 0x100
#define DNSMSG_OPT_RA 0x80

/** 
 * This struct holds information found in the DNS packet header all specified
 * in RFC 1035. 
 */
struct dnsmsg_header {
    /** The ID of the DNS message. When being created,this is random */
    uint16_t id;
    
    /** Whether this DNS message is a query or not (1 or 0 respectively) */
    uint8_t qr;
    
    /** 
     * This is the opcode for the DNS message. Opcodes can be found in the
     * "dnstypes.h" file.
     */
    uint8_t opcode;
    
    /** Whether this query is an authoritative answer or not (1 or 0) */
    uint8_t aa;
    
    /** Whether this DNS message has truncated data */
    uint8_t tc;
    
    /** Whether recursion is desired if this is a query */
    uint8_t rd;
    
    /** Whether recursion is available if this is a repsonse */
    uint8_t ra;
    
    /** 
     * The z code (3 bits) for this query (usually 000). Only the least 
     * significant bits of this field are used. 
     */
    uint8_t z;
    
    /**
     * This is the response code of the DNS message (if it was a response).
     * Only the least 4 sig bits are used.
     */
    uint8_t rcode;
    
    /**
     * 2 octet number showing the number of questions associated with this 
     * message
     */
    uint16_t qdcount;
    
    /**
     * 2 octet number showing the number of answers associated with this
     * message.
     */
    uint16_t ancount;
    
    /** 
     * 2 octet field showing the number of authoritative records associated 
     * with this message.
     */
    uint16_t nscount;
    
    /** 
     * 2 octet field showing the number of additional records associated 
     * with this message.
     */
    uint16_t arcount;
};

/** 
 * This struct holds information found in the question section of DNS 
 * messages as specified in RFC 1035.
 */
struct dnsmsg_query {
    /**
     * This is the byte buffer holding the name in question
     */
    uint8_t *qname;
    
    /**
     * This is the query type which can be specified through the "dnstypes.h"
     * file macros.
     */
    uint16_t qtype;
    
    /**
     * This is the class of query for this message. This is typically 
     * DNS_QCLASS_IN (1)
     */
    uint16_t qclass;
};

/**
 * This struct is used to store information received or being sent in DNS
 * packets. 
 */
typedef struct {
    struct dnsmsg_header *header;
    struct dnsmsg_query *query; 
    dnsrecord_t *answer;  // TODO Make sure these are freed
    dnsrecord_t *auth; 
    dnsrecord_t *additional; 
} dnsmsg_t;

/**
 * Creates a dnsmsg_header struct object and returns a pointer to the struct
 * in memory. The parameter passed in is a list of options found at the top of
 * file (DNSMSG_OPT_X).
 *
 * @param options The options desired to use when creating the dnsmsg_header
 * @return a pointer to the struct object in memory created with the desired
 * options.
 */
struct dnsmsg_header *new_dnsmsg_header(uint16_t options);

/**
 * Creates a dnsmsg_query struct object and returns a pointer to the object in
 * memory.
 *
 * @return a pointer to the struct object in memory created with the desired
 * options
 */
struct dnsmsg_query *new_dnsmsg_query(void);

/**
 * Creates a dnsmsg_t struct object and returns a pointer to the object in
 * memory. The parameter is the options desired when creating the message
 * and are utilitized in the header.
 *
 * @param options The options desired to use when creating the dnsmsg_header
 * @return a pointer to the struct object in memory created with the desired
 * options.
 */
dnsmsg_t *dnsmsg_new(uint16_t options);

/**
 * Prints out a pretty version of the passed in dnsmsg_t struct object
 *
 * @param dnsmsg A pointer to a dnsmsg_t struct object -- the one that
 *               will be printed all "pretty-like".
 */
void dnsmsg_print(dnsmsg_t *dnsmsg);

/**
 * Frees up memory used to store the dnsmsg_t struct object. Use this function
 * instead of just free because it also frees pointer inside of the struct.
 *
 * @param dnsmsg The message to set free
 */
void dnsmsg_free(dnsmsg_t *dnsmsg);

/**
 * Creates a dnsmsg_t object using the specified byte buffer and returns a 
 * pointer to the newly created dnsmsg_t. The byte buffer passed in is 
 * typically the response from a DNS server.
 *
 * @param buf An array of uint8_t's used to create the dnsmsg_t
 * @param buf_len The number of elements in the buffer
 * @return a pointer to the created dnsmsg_t object or NULL if an error 
 * occured.
 */
dnsmsg_t *dnsmsg_construct(uint8_t *buf, ssize_t buf_len);

/**
 * Creates a dnsmsg_t object that is a query for a name. This is what should
 * be used when trying to send a query to a DNS server.
 *
 * @param name      The name the user wishes to resolve
 * @param qtype     This is the type of query to make. The list of qtypes are found
 *                  in the "dnstypes.h" file
 * @param qclass    This is the class of query to make. The list of qclasses are 
 *                  found in the "dnstypes.h" file
 * @return a pointer to the created dnsmsg_t object
 */
dnsmsg_t *dnsmsg_create_query(const char *name, int qtype, int qclass);

/**
 * Creates an array used when converting a name to DNS formatted byte array
 * used when making a query.
 *
 * @param name The desired name to convert to bytes
 * @return the converted name. Be sure to free this result when finished. Or
 * don't, I don't care if you leak.
 */
uint8_t *dnsmsg_create_name(const char *name);

/** 
 * Converts a dnsmsg_t to an array of bytes and stores them in the passed in
 * buffer. This should typically be used when wanting to send a query to a
 * DNS server.
 *
 * @param msg the message that the user wishes to turn into bytes
 * @param buf the buffer the message's bytes will be stored in
 * @return the number of bytes that were placed into the buffer. This 
 * indicates the size of the dnsmsg_t
 */
int dnsmsg_to_bytes(dnsmsg_t *msg, uint8_t **buf);

/** 
 * Converts a dnsmsg_t to an array of bytes and stores them in the passed in
 * buffer. This should typically be used when wanting to send a query to a
 * DNS server.
 *
 * @param msg the message that the user wishes to turn into bytes
 * @param buf the buffer the message's bytes will be stored in
 * @return the number of bytes that were placed into the buffer. This 
 * indicates the size of the dnsmsg_t
 */
int dnsmsg_to_bytes_header(dnsmsg_t *msg, uint8_t **buf);

/** 
 * Converts a dnsmsg_t to an array of bytes and stores them in the passed in
 * buffer. This should typically be used when wanting to send a query to a
 * DNS server.
 *
 * @param msg the message whose header the user desires to be 
 *            converted to bytes
 * @param buf the buffer the message's header's bytes will be 
 *            stored in
 * @return the number of bytes that were placed into the buffer. This 
 * indicates the size of the dnsmsg_t's header
 */
int dnsmsg_to_bytes_query(dnsmsg_t *msg, uint8_t **buf);
#endif