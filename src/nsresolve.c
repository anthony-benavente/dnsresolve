#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "dnstypes.h"
#include "dnsmessage.h"

int main(void) {
    srand(time(NULL));
    dnsmsg_t *msg = new_dnsmessage(DNSMSG_OPT_QR | DNSMSG_OPT_RD);
    dnsmsg_print(msg);
    dnsmsg_free(msg);
    
    return 0;
}