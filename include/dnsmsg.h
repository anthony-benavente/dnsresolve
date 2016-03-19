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
    uint8_t qr;
    uint8_t opcode;
    uint8_t aa;
    uint8_t tc;
    uint8_t rd;
    uint8_t ra;
    uint8_t z;
    uint8_t rcode;
    uint16_t qdcount;
    uint16_t ancount;
    uint16_t nscount;
    uint16_t arcount;
};

struct dnsmsg_query {
    uint8_t *qname;
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

struct dnsmsg_header *new_dnsmsg_header(uint16_t options);
struct dnsmsg_query *new_dnsmsg_query(void);

dnsmsg_t *dnsmsg_new(uint16_t options);

void dnsmsg_print(dnsmsg_t *);

void dnsmsg_free(dnsmsg_t *);

dnsmsg_t *dnsmsg_construct(uint8_t *buf, ssize_t buf_len);

dnsmsg_t *dnsmsg_create_query(const char *name, int qtype, int qclass);

uint8_t *dnsmsg_create_name(const char *name);

int dnsmsg_to_bytes(dnsmsg_t *msg, uint8_t **buf);

int dnsmsg_to_bytes_header(dnsmsg_t *msg, uint8_t **buf);

int dnsmsg_to_bytes_query(dnsmsg_t *msg, uint8_t **buf);
#endif