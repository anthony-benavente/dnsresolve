#include "dnsessage.h"

struct dnsmsg_header *new_dnsmsg_header(void) {
    struct dnsmsg_header *result = malloc(sizeof(struct dnsmsg_header));
    result->id = rand();
    return result;
}

struct dnsmsg_query *new_dnsmsg_query(void) {
    struct dnsmsg_query *result = malloc(sizeof(struct dnsmsg_query));
    return result;
}

dnsmsg_t *new_dnsmessage(uint8_t options) {
    dnsmsg_t *result = malloc(sizeof(dnsmsg_t));
    result->header = new_dnsmsg_header();
    result->query = new_dnsmsg_query();
    result->answer = NULL;
    result->auth = NULL;
    result->additional = NULL;
}

void dnsmsg_print(dnsmsg_t *msg) {
    printf("-----------------------------------------------");
    printf("  ID: %d\n", msg->id);
    printf("-----------------------------------------------");
}

void dnsmsg_free(dnsmsg_t *msg) {
    free(msg->header);
    free(msg->query);
    free(msg);
}