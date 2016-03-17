#ifndef DNSMESSAGE_H
#define DNSMESSAGE_H

#include <stdint.h>
#include <stdlib.h>

#include "dnsrecord.h"

#define DNSMSG_OPT_QR 0x8000
#define DNSMSG_OPT_AA 0x400
#define DNSMSG_OPT_TC 0x200
#define DNSMSG_OPT_RD 0x100
#define DNSMSG_OPT_RA 0x80

struct dnsmsg_header {
    uint16_t id;
    uint8_t qr : 1;
    uint8_t opcode  : 4;
    uint8_t aa : 1;
    uint8_t tc : 1;
    uint8_t rd : 1;
    uint8_t ra : 1;
    uint8_t z : 3;
    uint8_t rcode : 4;
    uint16_t qdcount;
    uint16_t ancount;
    uint16_t nscount;
    uint16_t arcount;
};

struct dnsmsg_query {
    char *qname;
    uint16_t qtype;
    uint16_t qclass;  
};

typedef struct {
    struct dnsmsg_header *header;
    struct dnsmsg_query *query; 
    dnsrecord_t *answer;  // TODO Make sure these are freed
    dnsrecord_t *auth; 
    dnsrecord_t *additional; 
} dnsmsg_t;

struct dnsmsg_header *new_dnsmsg_header(void);
struct dnsmsg_query *new_dnsmsg_query(void);

dnsmsg_t *new_dnsmessage(uint16_t options);

void dnsmsg_print(dnsmsg_t *);

void dnsmsg_free(dnsmsg_t *);

#endif