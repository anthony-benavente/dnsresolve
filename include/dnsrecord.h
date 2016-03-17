#ifndef DNS_RECORD_H
#define DNS_RECORD_H

#include <stdint.h>

struct dnsrecord_params {
    char *name;
    uint16_t type;
    uint16_t class;
    uint32_t ttl;
    uint16_t rdlength;
    uint8_t *rdata;
};

typedef struct {
    struct dnsrecord_params *params;
} dnsrecord_t;

dnsrecord_t *dnsrecord_new(void);
void dnsrecord_free(dnsrecord_t *);

#endif