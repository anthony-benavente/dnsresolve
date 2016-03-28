#include "dnsrecord.h"
#include "wrappers.h"
#include "dnstypes.h"

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

void dnsrecord_print(dnsrecord_t *record) {
    printf("  NAME:     %s\n", record->name);
    // printf("  TYPE:     %s\n", GET_TYPE(record->type));
    printf("  TYPE:     %x\n", record->type);
    printf("  CLASS:    %s\n", GET_CLASS(record->rclass));
    printf("  TTL:      %-8d\n", record->ttl);
    printf("  RDLENGTH: %-4d\n", record->rdlength);
    printf("  RDATA:    %p\n", record->rdata); // TODO: printing pointer is tmp
}