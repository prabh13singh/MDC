/**********************************************************************************************

 * FILE          : compress.c

 * PROJECT       : MDC

 * PROGRAMMER    : Prabhjot singh

 * DESCRIPTION   : This program is used to compress a file using its distinct characters

*******************************************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

//enter source as first argument, compressed as second and encryption_key as third //

/*#ifndef DEBUG
#define DEBUG
#endif
*/

/*....Prototypes....*/
int8_t open_read_mode(char *);
int8_t open_write_mode(char *);
char *create_master_array( int8_t );

// FUNCTION      : main
// DESCRIPTION   : It is the main execution function
// PARAMETERS    : argc : argument count
//	 	 : argv : string array of arguments 
// RETURNS       : int : return value
int main(int argc,char *argv[])
{
	/*.....variables........*/ 
	int8_t nibble;                                       
	int8_t file_descriptor[3];				        // array of file descriptors 			  
	int8_t index;							// index of master array 			     
	int8_t distinct_char;						// distinct characters`
	int8_t new_position;						
	unsigned char index_char;					// index as character type
	unsigned char byte;						// byte to be stored in compressed file
	unsigned char read_char;					// charater read from source file
	char *master_array;
	int8_t write_count = 0;
	int8_t byte_count = 1;

	if( argc < 4 )
	{
		printf("\n Insufficient arguments...\n");
		printf(" Please enter source as first argument, file to be compressed as second\n\
		and file to store encryption_key as third...\n");
		return -1;
	}

	/*.. open source file in read mode and other two files to be open in write mode
	  for writing compressed data and encryption key...*/
	file_descriptor[0] = open_read_mode( argv[1] );
	file_descriptor[1] = open_write_mode( argv[2] );
	file_descriptor[2] = open_write_mode( argv[3] );

	master_array = create_master_array( file_descriptor[0] );

	#ifdef DEBUG
	printf("%s",master_array);
	#endif

	/*..write master array to file name given by user..*/	
	for( write_count = 0; write_count < 15; write_count++)
	{
		write( file_descriptor[2], &master_array[write_count], 1);
	}

	distinct_char = strlen( master_array );

	#ifdef DEBUG
	printf( "\n no. of distinct characters is %d", distinct_char );
	#endif
	
	/*..set file to starting position(0) using the lseek function below..*/
	new_position = lseek( file_descriptor[0], 0, SEEK_SET );

	#ifdef DEBUG
	printf( "\n new pos. is:%d",new_position );
	#endif

	/*..repeat till it reads the characters from file..*/
	while( byte_count )
	{
		//clear the byte	
		byte = byte ^ byte;

		for( nibble = 0; nibble < 2; nibble++ )
		{	
			/* read single character from file */	
			byte_count = read( file_descriptor[0], &read_char, 1 );

			#ifdef DEBUG
			printf( "\n input char is %c", read_char );	
			#endif

			/* compare character read from file with characters in master array */
			for( index = 0; index < distinct_char; index++)
			{	
				/* when character in master array matching with character in file */
				if( *( master_array + index ) == read_char )
				{	
					#ifdef DEBUG
					printf("\nindex is %d",index);
					#endif
				
					/* convert int to character */	
					index_char = (char)index;
					break;
				}
			}

			/*..write odd charcters to last 4 bits..*/ 	
			if( nibble == 0)
			{
				index_char = index_char << 4;
				byte = byte | index_char;
			}

			/*..write even charcters to firstt 4 bits..*/ 	
			if( nibble == 1)
			{
				index_char = index_char << 4;
				index_char = index_char >> 4;
				byte = byte | index_char ;
			}
		}

		#ifdef DEBUG
		printf( "\nbyte is %d", byte );
		#endif

		/*..write a character(1 byte) to file..*/
		write( file_descriptor[1], &byte, 1 );
	}

	return 0;

}


// FUNCTION      : open_read_mode
// DESCRIPTION   : It opens the file passed as parameter in read only mode
//		   and returns file descriptor
// PARAMETERS    : char * : file name 
// RETURNS       : int8_t : file descriptor  
int8_t open_read_mode( char *file1 )
{
	int8_t fd1  = open( file1, O_RDONLY );
	
	if( fd1  < 0)
		perror( "open" );

	return fd1;
}

// FUNCTION      : open_write_mode
// DESCRIPTION   : It opens the file passed as parameter in write only mode
//		   and returns file descriptor
// PARAMETERS    : char * : file name 
// RETURNS       : int8_t : return value
int8_t open_write_mode( char *file2 )
{

	/*..O_CREAT will create file if it doesn't exist..*/ 
	int8_t fd2  = open( file2, O_WRONLY|O_CREAT );
	
	if( fd2 < 0 )
		perror( "open" );

	return fd2;
}

// FUNCTION      : create_master_array
// DESCRIPTION   : It creates a master array fo distinct charcacters in file
// PARAMETERS    : int8_t : file_descriptor 
// RETURNS       : char * : master array
char *create_master_array( int8_t fd )
{
	int8_t bytes_read = 1;
	int8_t  stored_chars = 0;
	int8_t flag = 1;
	int8_t index;
	char read_char;
	char *master_array;
	master_array = (char *)malloc( 16 * sizeof(char) );

	/* repeat till it reads the characters from file */
	while( bytes_read )
	{
		/*..reads a character from file & stores return value of function..*/
		bytes_read = read( fd, &read_char, 1 );

		#ifdef DEBUG
		printf("value %c\n", read_char);
		#endif
		
		/*..store first character ..*/
		if( stored_chars == 0 )
		{
			*master_array = read_char;
			stored_chars++;
		}
		
		/*..compare char with those in master array,if matches,then continue
		    otherwise store character in master array */

		for( index = 0; index < stored_chars; index++ )
		{
			if( read_char == *(master_array+index) )
			{
				flag = 0;
				goto out;
			}
			else
			{		
				flag = 1;	
			}
		}

		if( flag == 1 )
		{
			*(master_array + stored_chars) = read_char;
			stored_chars++;
		}
	out:;
	}
	
	*(master_array + stored_chars) = '\0';

	#ifdef DEBUG
	printf("\nindex is %d",stored_chars);
	#endif	

	#ifdef DEBUG
	printf("\n%s", master_array);
	#endif

	return master_array;
}
