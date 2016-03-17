#include "dnsmsg.h"

#include "wrappers.h"

#include <stdio.h>

struct dnsmsg_header *new_dnsmsg_header(void) {
    struct dnsmsg_header *result = _malloc(sizeof(struct dnsmsg_header));
    result->id = rand();
    return result;
}

struct dnsmsg_query *new_dnsmsg_query(void) {
    struct dnsmsg_query *result = _malloc(sizeof(struct dnsmsg_query));
    return result;
}

dnsmsg_t *new_dnsmessage(uint16_t options) {
    dnsmsg_t *result = _malloc(sizeof(dnsmsg_t));
    result->header = new_dnsmsg_header();
    result->query = new_dnsmsg_query();
    result->answer = NULL;
    result->auth = NULL;
    result->additional = NULL;
    return result;
}

void dnsmsg_print(dnsmsg_t *msg) {
    printf("-----------------------------------------------\n");
    printf("  ID: %d\n", msg->header->id);
    printf("-----------------------------------------------\n");
}

void dnsmsg_free(dnsmsg_t *msg) {
    free(msg->header);
    free(msg->query);
    free(msg);
}