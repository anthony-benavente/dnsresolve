#ifndef DNSTYPES_H
#define DNSTYPES_H

/**
 * This file holds the macros used throughout the program.
 *
 * @author Anthony Benavente
 * @version 03/20/2016
 */ 

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

#define GET_OP(arg) arg == 0 ? "QUERY" : arg == 1 ? "IQUERY" : "STATUS"
#define GET_RCODE(arg) arg == 0 ? "No Error" :  \
    arg == 1 ? "Format error" : \
    arg == 2 ? "Server error" : \
    arg == 3 ? "Name error" : \
    arg == 4 ? "Implementation error" : \
    arg == 5 ? "Refused error" : \
    "Unknown error"
        
#define GET_TYPE(arg) arg == 1 ? "A" : \
    arg == 2 ? "NS" : \
    arg == 5 ? "CNAME" : \
    arg == 6 ? "SOA" : \
    arg == 12 ? "PTR" : \
    arg == 13 ? "HINFO" : \
    arg == 14 ? "MINFO" : \
    arg == 15 ? "MX" : \
    arg == 16 ? "TXT" : \
    "UNKNOWN"   
    
#define GET_CLASS(arg) arg == 1 ? "IN" : \
    arg == 3 ? "CH" : \
    arg == 4 ? "HS" : \
    "UNKNOWN"

#endif