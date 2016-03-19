#ifndef DNSTYPES_H
#define DNSTYPES_H

#define DNS_TYPE_A 1
#define DNS_TYPE_NS 2
#define DNS_TYPE_CNAME 5
#define DNS_TYPE_SOA 6    
#define DNS_TYPE_PTR 12    
#define DNS_TYPE_HINFO 13    
#define DNS_TYPE_MINFO 14    
#define DNS_TYPE_MX 15
#define DNS_TYPE_TXT 16

#define DNS_QTYPE_AXFR 252    
#define DNS_QTYPE_MAILB 253    
#define DNS_QTYPE_MAILA 254    

#define DNS_CLASS_IN 1
#define DNS_CLASS_CH 3
#define DNS_CLASS_HS 4

#define DNS_RCODE_NOERR 0
#define DNS_RCODE_FMTERR 1
#define DNS_RCODE_SERV 2
#define DNS_RCODE_NAMERR 3
#define DNS_RCODE_IMPL 4
#define DNS_RCODE_REFUSED 5

#define DNS_OPCODE_QUERY 0
#define DNS_OPCODE_IQUERY 1
#define DNS_OPCODE_STATUS 2

#endif