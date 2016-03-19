#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "dnstypes.h"
#include "dnsclient.h"
#include "dnsmsg.h"

int main(void) {
    srand(time(NULL));
    
    uint8_t dns_ip[] = { 8,8,8,8 };
    dnsmsg_t *query = dnsmsg_create_query("www.google.com", 
        DNS_TYPE_A, DNS_CLASS_IN);
    
    // int i = 1;
    // int new_ln = 0;
    // for (; i <= n; i++) {
    //     printf("%2x ", beginning[i - 1], beginning[i - 1]);
        
    //     new_ln = i % 8;
    //     if (!new_ln) {
    //         printf("\n");
    //     }
    // }
    // printf("\n");    
    
    dnsmsg_t *response = dnsclient_resolve(dns_ip, 53, query);
    
    dnsmsg_free(query);
    dnsmsg_free(response);
    
    return 0;
}