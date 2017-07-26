/**********************************************************************************************

 * FILE          : header.h

 * PROJECT       : MDC

 * PROGRAMMER    : Prabhjot singh

 * DESCRIPTION   : This file contains library files and prototypes used in decompression file

*******************************************************************************************/

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>

/*..prototypes..*/
int8_t open_read_mode(char *);
int8_t open_write_mode(char *);

