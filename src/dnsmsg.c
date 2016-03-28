#include "dnsmsg.h"
#include "wrappers.h"
#include "dnstypes.h"

#include <stdio.h>
#include <string.h>

#define MAX_NAME_LEN 256

#define DNS_OPCODE_MASK 0x7800
#define DNS_RCODE_MASK 0xf
#define DNS_Z_MASK 0x70

void append_to_buf8(uint8_t **buf, uint8_t val);
void append_to_buf16(uint8_t **buf, uint16_t val);
uint16_t dnsmsg_header_get_opt(struct dnsmsg_header *);
uint16_t get_uint16(uint8_t **buf);
uint8_t get_uint8(uint8_t **buf);
struct dnsmsg_header *dnsmsg_parse_header(uint8_t **buf);
struct dnsmsg_query *dnsmsg_parse_query(uint8_t **buf, uint8_t *original);
char *dnsmsg_parse_name(uint8_t **buf, uint8_t *original);
dnsrecord_t *dnsmsg_parse_record(uint8_t **buf, uint8_t *original);

struct dnsmsg_header *new_dnsmsg_header(uint16_t options) {
    struct dnsmsg_header *result = _malloc(sizeof(struct dnsmsg_header));
    result->id = rand();
    result->qr = (options & DNSMSG_OPT_QR) > 0;
    result->tc = (options & DNSMSG_OPT_TC) > 0;
    result->rd = (options & DNSMSG_OPT_RD) > 0;
    result->ra = (options & DNSMSG_OPT_RA) > 0;
    return result;
}

struct dnsmsg_query *new_dnsmsg_query(void) {
    struct dnsmsg_query *result = _malloc(sizeof(struct dnsmsg_query));
    return result;
}

dnsmsg_t *dnsmsg_new(uint16_t options) {
    dnsmsg_t *result = _malloc(sizeof(dnsmsg_t));
    result->header = new_dnsmsg_header(options);
    result->query = new_dnsmsg_query();
    result->answer = NULL;
    result->auth = NULL;
    result->additional = NULL;
    return result;
}

void dnsmsg_print(dnsmsg_t *msg) {
    printf("-----------------------------------------------\n");
    printf(";; Header\n");
    printf("  ID:             %-#4x\n", msg->header->id);
    printf("  Response Code:  %s\n", GET_RCODE(msg->header->rcode));
    printf("  Flags:          %-#4x\n", dnsmsg_header_get_opt(msg->header));
    printf("  Questions:      %-4d\n", msg->header->qdcount);
    printf("  Answers:        %-4d\n", msg->header->ancount);
    printf("  Authority:      %-4d\n", msg->header->nscount);
    printf("  Additional:     %-4d\n", msg->header->arcount);
    printf("\n");
    printf(";; Question Section\n");
    printf("  QNAME: %s\n", msg->query->qname);
    printf("  QTYPE: %s\n", GET_TYPE(msg->query->qtype));
    printf("  QCLASS: %s\n", GET_CLASS(msg->query->qclass));
    printf("\n");
    printf(";; Answer Section\n");
    int i = 0;
    for (; i < msg->header->ancount; i++) {
        dnsrecord_print(msg->answer[i]);
    }
    printf("-----------------------------------------------\n");
}

void dnsmsg_free(dnsmsg_t *msg) {
    if (msg->header) free(msg->header); 
    if (msg->query) {
        if (msg->query->qname) free(msg->query->qname);
        free(msg->query);
    }
    free(msg);
}

dnsmsg_t *dnsmsg_parse(uint8_t **buf, ssize_t buf_len) {
    uint8_t *original = *buf;
    
    dnsmsg_t *result = malloc(sizeof(dnsmsg_t));
    int i = 0;
    
    result->header = dnsmsg_parse_header(buf);
    result->query = dnsmsg_parse_query(buf, original);
    
    result->answer = malloc(sizeof(dnsrecord_t *) * result->header->ancount);
    for (i = 0; i < result->header->ancount; i++) {
        result->answer[i] = dnsmsg_parse_record(buf, original);
    }
    
    result->auth = malloc(sizeof(dnsrecord_t) * result->header->nscount);
    for (i = 0; i < result->header->nscount; i++) {
        result->auth[i] = dnsmsg_parse_record(buf, original);
    }
    
    result->additional = malloc(sizeof(dnsrecord_t) * result->header->arcount);
    for (i = 0; i < result->header->arcount; i++) {
        result->additional[i] = dnsmsg_parse_record(buf, original);
    }
    
    return result;
}

struct dnsmsg_header *dnsmsg_parse_header(uint8_t **buf) {
    struct dnsmsg_header *result = malloc(sizeof(struct dnsmsg_header));
    result->id = get_uint16(buf);
    uint16_t row_2 = get_uint16(buf);
    result->qr = (row_2 & DNSMSG_OPT_QR) >> 15;
    result->opcode = (row_2 & DNS_OPCODE_MASK) >> 11;
    result->aa = (row_2 & DNSMSG_OPT_AA) >> 10;
    result->tc = (row_2 & DNSMSG_OPT_TC) >> 9;
    result->rd = (row_2 & DNSMSG_OPT_RD) >> 8;
    result->ra = (row_2 & DNSMSG_OPT_RA) >> 7;
    result->z = (row_2 & DNS_Z_MASK) >> 4;
    result->rcode = (row_2 & DNS_RCODE_MASK);
    result->qdcount = get_uint16(buf);
    result->ancount = get_uint16(buf);
    result->nscount = get_uint16(buf);
    result->arcount = get_uint16(buf);
    return result;
}

struct dnsmsg_query *dnsmsg_parse_query(uint8_t **buf, uint8_t *original_buf) {
    struct dnsmsg_query *result = malloc(sizeof(struct dnsmsg_query));
    result->qname = dnsmsg_parse_name(buf, original_buf);
    result->qtype = get_uint16(buf);
    result->qclass = get_uint16(buf);
    return result;
}

dnsrecord_t *dnsmsg_parse_record(uint8_t **buf, uint8_t *original_buf) {
    int i;
    dnsrecord_t *result = dnsrecord_new();
    result->name = dnsmsg_parse_name(buf, original_buf);
    result->type = get_uint16(buf);
    result->rclass = get_uint16(buf);
    result->ttl = (get_uint16(buf) << 16) | get_uint16(buf);
    result->rdlength = get_uint16(buf);
    result->rdata = malloc(sizeof(uint8_t *) * result->rdlength);
    for (i = 0; i < result->rdlength; i++) {
        result->rdata[i] = get_uint8(buf);
    }
    return result;
}

// char *dnsmsg_parse_name(uint8_t **buf, uint8_t *original_buf) {
//     int i;
    
//     // Read name from buffer
//     uint8_t label_len = get_uint8(buf);
//     char *name = calloc(sizeof(char *), MAX_NAME_LEN);
//     char *name_ptr = name; // This variable is used like a string builder
    
//     do {
        
//     } 
    
//     // If encountered pointer, recurse
//     if ((label_len & 0xc0) == 0xc0) {
//     }
// }

char *dnsmsg_parse_name(uint8_t **buf, uint8_t *original_buf) {
    int i = 0;
    uint8_t label_len = get_uint8(buf);
    char *name = calloc(sizeof(char *), MAX_NAME_LEN);
    char *name_ptr = name;
    do {
        if ((label_len & 0xc0) == 0xc0) {
            // Get new buf position
            int offset = ((label_len & 0x3f << 8) | get_uint8(buf));
            uint8_t *new_buf_pos = original_buf + offset;
            char *pointed = dnsmsg_parse_name(&new_buf_pos, original_buf);
            for (i = 0; pointed[i] != '\0'; i++) {
                *(name_ptr++) = pointed[i];
            }
            *(name_ptr++) = '\0';
            free(pointed); // pointed is no longer needed. Goodbye
        } else {
            for (i = 0; i < label_len; i++) {
                *(name_ptr++) = (char) get_uint8(buf);
            }
            *(name_ptr++) = '.';
        }
        label_len = get_uint8(buf);
    } while (label_len != 0);
    return name;
}

dnsmsg_t *dnsmsg_create_query(const char *name, int qtype, int qclass) {
    dnsmsg_t *result = dnsmsg_new(DNSMSG_OPT_RD);
    result->header->opcode = DNS_OPCODE_QUERY;
    result->header->qdcount = 1;
    
    result->query->qname = calloc(sizeof(char), strlen(name) + 1);
    strcpy(result->query->qname, name);
    result->query->qtype = qtype;
    result->query->qclass = qclass;
    return result;
}

uint8_t *dnsmsg_create_name(const char *name) {
    int i = 0, 
        next_len = 0,
        buf_index = 1,
        label_len = 0;
    uint8_t *buf = malloc(sizeof(uint8_t) * MAX_NAME_LEN);
    
    size_t namelen = strlen(name);
    for (; i <= namelen; i++) {
        if (name[i] == '.' || name[i] == '\0') {
            buf[next_len] = label_len;
            label_len = 0;
            next_len = buf_index++;
        } else {
            buf[buf_index++] = name[i];
            label_len++;
        }
    }
    buf[buf_index++] = 0;
    
    return buf;
}

int dnsmsg_to_bytes(dnsmsg_t *msg, uint8_t **buf) {
    int num_bytes = 0;
    
    num_bytes += dnsmsg_to_bytes_header(msg, buf);    
    num_bytes += dnsmsg_to_bytes_query(msg, buf);
    
    return num_bytes;
}

int dnsmsg_to_bytes_header(dnsmsg_t *msg, uint8_t **buf) {
    int num_bytes = 0;
    
    struct dnsmsg_header *head = msg->header;

    append_to_buf16(buf, head->id);
    append_to_buf16(buf, dnsmsg_header_get_opt(head));
    append_to_buf16(buf, head->qdcount);
    append_to_buf16(buf, head->ancount);
    append_to_buf16(buf, head->nscount);
    append_to_buf16(buf, head->arcount);
    
    num_bytes += 12;
    
    return num_bytes;
}

int dnsmsg_to_bytes_query(dnsmsg_t *msg, uint8_t **buf) {
    int num_bytes = 0,
        i = 0;
    
    // Convert qname
    uint8_t *qname = dnsmsg_create_name(msg->query->qname);
    for (; qname[i] != 0; i++) {
        append_to_buf8(buf, qname[i]);
        num_bytes++;
    }
    append_to_buf8(buf, qname[i]); // Add the last 0 in there too
    num_bytes++;
    
    // Convert qtype
    append_to_buf16(buf, msg->query->qtype);
    num_bytes += 2;
    
    // Convert qclass
    append_to_buf16(buf, msg->query->qclass);
    num_bytes += 2;
    free(qname);
    
    return num_bytes;
}

void append_to_buf8(uint8_t **buf, uint8_t val) {
    *(*buf)++ = val;
}

void append_to_buf16(uint8_t **buf, uint16_t val) {
    append_to_buf8(buf, (val & 0xff00) >> 8);
    append_to_buf8(buf, val & 0xff);
}

uint16_t dnsmsg_header_get_opt(struct dnsmsg_header *h) {
    return  h->qr << 15 | h->opcode << 14 | h->aa << 10 | h->tc << 9 | 
        h->rd << 8 | h->ra << 7 |  h->z << 6 | h->rcode;
}

uint16_t get_uint16(uint8_t **buf) {
    return (*(*buf)++ << 8) | *(*buf)++; // I'm so sorry if you have to read this line
}

uint8_t get_uint8(uint8_t **buf) {
    return *(*buf)++;
}