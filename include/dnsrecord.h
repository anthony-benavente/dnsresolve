#ifndef DNS_RECORD_H
#define DNS_RECORD_H

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

struct dnsrecord_params *new_dnsrecord_params(void);
dnsrecord_t *new_dnsrecord(struct dnsrecord_params *params);

#endif