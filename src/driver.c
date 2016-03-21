/** 
 * This file is the entry point to the program.
 *
 * @author Anthony Benavente
 * @version 
 */

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
    dnsmsg_t *response = dnsclient_resolve(dns_ip, 53, query);
    
    dnsmsg_print(response);
    
    dnsmsg_free(query);
    dnsmsg_free(response);
    
    return 0;
}