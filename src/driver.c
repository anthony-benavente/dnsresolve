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

#define MAX_LINE 256

int ns_found = 0;

void get_ns_ip(uint8_t *, int, char **);
void usage(void);

int main(int argc, char **argv) {
    srand(time(NULL));
    
    // Check if nameserver is specified in args
    uint8_t ns_ip[4];
    get_ns_ip(ns_ip, argc, argv);
    
    char *query = ns_found ? argv[1] : argv[2];
    
    dnsmsg_t *query_msg = dnsmsg_create_query(query, DNS_TYPE_A, DNS_CLASS_IN);    
    dnsmsg_t *response = dnsclient_resolve(ns_ip, 53, query_msg);
    
    dnsmsg_print(response);
    
    dnsmsg_free(query_msg);
    dnsmsg_free(response);
    
    return 0;
}

void get_ns_ip(uint8_t *buf, int argc, char **argv) {
    int sscanf_rc,
        ns_found = 0, // Used to stop for loop if ns arg was found
        i = 1,  // Used to traverse argv; set to 1 because program name is at 0
        j = 0;  // Used to append to buf
        
    for (; i < argc && !ns_found; i++) {
        if (argv[i][0] == '@') {
            /* 
            We found the name server argument. Make sure it matches the 
            format we are looking for. 
            */
            sscanf(argv[i], "@%d.%d.%d.%d", &buf[0], &buf[1], 
                &buf[2], &buf[3]);
            ns_found = 1;
        }
    }
    
    if (!ns_found) {
        /*
         If the name server argument was not found, try and pull it from 
         etc/resolv.conf
         */
         FILE *fp = fopen("/etc/resolv.conf", "r");
         
         if (fp) { // If resolv.conf was found. If it wasn't, exit with an error
            char line_buf[MAX_LINE];
            while (fgets(line_buf, MAX_LINE, fp) != NULL) {
                if (strstr(line_buf, "nameserver") != NULL) {
                    // Found nameserver in resolv.conf :)
                    
                    // TODO: This line underneath doesn't work... why?
                    sscanf(line_buf, "%d.%d.%d.%d", &buf[0], &buf[1], 
                        &buf[2], &buf[3]);
                    break;
                }
            }
            fclose(fp);             
         } else {
            fprintf(stderr, "Could not find default DNS!\n");
            usage();
            exit(EXIT_FAILURE);
        }
    }
}

void usage(void) {
    printf("usage: dnsresolve [@nameserver] [-t type] <domain>\n");
}