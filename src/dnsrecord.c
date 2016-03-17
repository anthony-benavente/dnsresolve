#include "dnsrecord.h"
#include "wrappers.h"

dnsrecord_t *dnsrecord_new(void) {
    dnsrecord_t *result = _malloc(sizeof(dnsrecord_t));  
    result->params = _malloc(sizeof(dnsrecord_t));
    return result;
}

void dnsrecord_free(dnsrecord_t *record) {
    free(record->params);
    free(record);
}