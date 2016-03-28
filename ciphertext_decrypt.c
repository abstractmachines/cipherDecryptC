 /*
Decrypting a Ciphertext in C
Amanda Falke 2014

OVERVIEW: Decrypt Ciphertext to plaintext in C IA32 using bit manipulation,
no string ASCII manipulation.

REQUIREMENTS: 
1. Hard code username. 

2. Program takes one argument: ciphertext.txt

3. Take in ciphertext.txt through argv, and read file using fread, fseek, ftell, and
related functions. 

4. Do not use string libraries, because NULL bytes are part of this process.

5. Decrypt file " username " and change file name to ciphertext.txt

*/

#include <stdio.h>
#include <stdlib.h>


int checkFile(FILE * fp); // check file not NULL

void stage1(long fileSize, char * buffer);

void stage2(long fileSize, char * buffer);

void stage3(long fileSize, char u3, char * buffer);


int main(int argc, char * argv[])
{

// VARIABLES
 int i;
 long fileSize;
 FILE * theFile;
 char * username =  "machina"; // hard code per requirements
 char u3 = username[2]; // STAGE 3
 char * buffer; // store locally



/* **************** SETUP: OPEN FILE AND COPY INTO LOCAL MEMORY *********** */
 theFile = fopen(argv[1], "r"); // open file

 i = checkFile(theFile); // error check for file open not NULL

 // obtain file size:
 fseek(theFile, 0, SEEK_END); // seek until end of file
 fileSize = ftell(theFile); // tell us how long it is
 rewind(theFile); // go back to beginning of file to read it

 buffer =  malloc (sizeof(char)*fileSize); // malloc memory to contain entire file

 fread(buffer,sizeof(char),fileSize,theFile); // copy that buffer into local memory.



/* ****************** FIRST STAGE ***************** */
 stage1(fileSize, buffer);


/* ****************** SECOND STAGE ***************** */
 stage2(fileSize, buffer);


/* **************** THIRD STAGE ********************** */ 
 stage3(fileSize, u3, buffer);


//****** FINAL STAGE: *************************** */

 printf("%s", buffer); // print the decrypted contents to standard out

 fclose(theFile); // close the file

// FREE DYNAMIC MEMORY
 free(buffer); 


 return 0;
};

int checkFile(FILE * fp)
{

 if ( fp == NULL )
 {
  printf("Error. We could not open your file. Please include the argument as the SECOND command of the command line: ./a.out machina \n");
  return 0;
 }

 else if ( fp != NULL )
 {
  return 1;
 }
 return 1;

}



void stage1(long fileSize, char * buffer)
{
// USERNAME MACHINA: FIRST LETTER M: swap c1 and c3 for every FOUR bytes  
 int i; 
 char tempSwap;

 for ( i = 0; i < fileSize; i=i+4 ) //(FOUR BYTES is i=i+4!!!)
 {
  tempSwap = buffer[i+1];
  buffer[i+1] = buffer[i+3];
  buffer[i+3] = tempSwap;
 }
}



void stage2(long fileSize, char * buffer)
{
// USERNAME MACHINA: SECOND LETTER IS A. 'a' to 'd':  Swap bits b7 and b1, Swap bits b6 and b0. 
 int i;
 char x;
 char y;

 for ( i = 0; i < fileSize; ++i ) // EVERY BYTE is ++i !!
 { 
  x = buffer[i] >> 6; // 0x03. This is the mask for shifting b6 to b0.
  y = buffer[i] << 6; // 0xC0 This is the mask for shifting b1 to b7.

  buffer[i] = ( (x & 0x03) | (y & 0xC0) | (buffer[i] & 0x3C) ); // OR the two masks x and y. or THAT with the mask of buffer and combined mask.
 }
}



void stage3(long fileSize, char u3, char * buffer)
{
// USERNAME MACHINA. THIRD LETTER is C. 'a' to 'd':  XOR bytes c0 and c3 with u3: 
 int i;
 
 for ( i = 0; i < fileSize; i=i+4 ) // EVERY BYTE is ++i !!
 {
  buffer[i] ^= u3;
  buffer[i+3] ^= u3;
 }
}







