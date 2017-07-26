/**********************************************************************************************

 * FILE          : decompression.c

 * PROJECT       : MDC

 * PROGRAMMER    : Prabhjot singh

 * DESCRIPTION   : This program is used to decompress a file using its distinct characters

*******************************************************************************************/

#include"header.h"

/*
#ifndef DEBUG
#define DEBUG
#endif
*/

/* enter encryption key as 1st argument,compressed file as 2nd & final as 3rd */
int main(int argc,char *argv[])
{
	/*..variables..*/
	int8_t index;
	int8_t size;
	int8_t count = 1;
	char buffer[16];
	int8_t read_count = 0;
	int8_t reading = 1;
	unsigned char read_char;
	unsigned char nibble1;
	unsigned char nibble2;
	int8_t file_descriptor[3];				        // array of file descriptors

	if(argc!=4)
		printf("insufficient arguments");

	/*.. open encryption file and compressed in read mode and final file in write mode
	  to recover data in uncompressed form...*/
	file_descriptor[0] = open_read_mode( argv[1] );
	file_descriptor[1] = open_read_mode( argv[2] );
	file_descriptor[2] = open_write_mode( argv[3] );

	/*..store characters in a buffer till they are read from file */
	while(reading)
	{
		reading = read(file_descriptor[0], &buffer[read_count], 1);
		read_count++;
	}
	
	/*..append by null character and find its length..*/
	buffer[read_count]='\0';		
	size=strlen(buffer);
	
	///////////////////decompression/////////////////////////
	/*..repeat loop till it reads the characters from file..*/
	while(count)
	{
		//clear the characters
		nibble1 = nibble1 ^ nibble1;
		nibble2 = nibble2 ^ nibble2;

		/* read single character from file */
		count = read( file_descriptor[1], &read_char, 1);
		printf("\ninput char is :%c %d",read_char,read_char);
		
		/*..if it finds null character or ETX character,then stop reading..*/	
		if( read_char == '\0' || read_char == 2 )
		{
			close( file_descriptor[2] );
			goto last;
		}

		/*..get first charcter by shifting the read character by 4 bits to right..*/ 
		nibble1 = read_char >> 4;

		/*..compare character read from file with index of characters in encryption key
		    and write the corresponding character to the final file..*/

		for( index = 0; index < size; index++ )
		{
			if( index == nibble1 )
			{
				write( file_descriptor[2], &buffer[index], 1 );	
				break;
			}
		}
		
		#ifdef DEBUG
		printf(" & int value is :%d",nibble1);
		#endif

		/*..get second charcter..*/
		nibble2 = read_char << 4;
		nibble2 = nibble2 >> 4;

		#ifdef DEBUG
		printf(" & int value is :%d",nibble2);
		#endif

		
		/*..compare character read from file with index of characters in encryption key
		    and write the corresponding character to the final file..*/
		for( index = 0; index < size; index++ )
		{
			if( index == nibble2 )
			{
				write( file_descriptor[2], &buffer[index], 1);	
				break;
			}
		}
	}

last:
   return 0;
}

// FUNCTION      : open_read_mode
// DESCRIPTION   : It opens the file passed as parameter in read mode
//		   and returns file descriptor
// PARAMETERS    : char * : file name 
// RETURNS       : int8_t : file descriptor  
int8_t open_read_mode( char *file1 )
{
	int8_t fd1  = open( file1, O_RDONLY );
	
	if( fd1 < 0)
		perror( "open" );

	return fd1;
}

// FUNCTION      : open_write_mode
// DESCRIPTION   : It opens the file passed as parameter in write mode
//		   and returns file descriptor
// PARAMETERS    : char * : file name 
// RETURNS       : int8_t : return value
int8_t open_write_mode(char *file2)
{
	int8_t fd2  = open( file2, O_WRONLY|O_CREAT );
	
	if( fd2 < 0 )
		perror( "open" );

	return fd2;
}
