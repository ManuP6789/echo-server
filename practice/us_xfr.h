// Program from Michael Kerrisk from the Linux programming interface
#include <sys/un.h>
#include <sys/socket.h>
#include <stdlib.h>
#include "tlpi_hdr.h"

#define SV_SOCK_PATH "/tmp/us_xfr"
#define BUF_SIZE 100

