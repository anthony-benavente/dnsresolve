#include "dnsrecord.h"
#include "wrappers.h"

dnsrecord_t *dnsrecord_new(void) {
    dnsrecord_t *result = _malloc(sizeof(dnsrecord_t));
    result->name = NULL;
    result->type = 0;
    result->rclass = 0;
    result->ttl = 0;
    result->rdlength = 0;
    result->rdata = NULL;
    return result;
}

void dnsrecord_free(dnsrecord_t *record) {
    free(record);
}