
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifdef _WIN32
#include <winsock2.h>
#endif
#ifdef  __LINUX__
#include <sys/sysinfo.h>
#endif
#include <arpa/inet.h>
#include <sys/types.h>

#include "sysdeps.h"
#include "adb.h"
#include "usb_vendors.h"
#include "adb_extended.h"

#define  TRACE_TAG  TRACE_EXT
char *format_shortcut(int shortcut_index,int argc, char **argv)
{
	
	return NULL;
}
// Get Your Fucking Winsocks On!!! Because they have to be different
#ifdef HAVE_WINSOCK
int is_ipaddress(char *ipaddress)
{
    WSADATA wsaData;
    int iResult;
    unsigned long ulAddr = INADDR_NONE;
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        return 0;
    }
	//--------------------------------
	// Call inet_addr(). If the call succeeds,
	// the result variable will hold a IN_ADDR
    ulAddr = inet_addr(ipaddress);
    if ( ulAddr == INADDR_NONE ) {
        WSACleanup();
        return 0;
    }   
    if (ulAddr == INADDR_ANY) {
        WSACleanup();
        return 0;  
    }    
    WSACleanup();
    return 1;
}
#else
int is_ipaddress(char *ipaddress)
{

    struct sockaddr_in sa;
    int result = 0;
    char* ipcpy= (char*) malloc(strlen(ipaddress) * sizeof(char));
    strcpy(ipcpy,ipaddress);
    char * lineptr = strsep(&ipcpy,":");
    
    result = inet_pton(AF_INET, lineptr, &(sa.sin_addr));	
    return result != 0;
}
#endif

// is_keyevent : Test test_string to see if it contains either input_keyevents.name or 
// input_keyevents.alt_name. 
// Return: When no match is found -1 is retruned, otherwise the input_keyevents.keycode is returned
// input_keyevents.keycode is the value of the internal android keyevent code, a full list can be found
// at frameworks/base/core/java/android/view/KeyEvent.java
int is_keyevent(char* test_string){

	int counter = 0 ; int keyevent_index = -1 ; int test_strlen = strlen(test_string) ;
	for(counter = 0 ; counter < keyevents_total ; counter++){
		int keyevent_altname_strlength = strlen(keyevents[counter].alt_name) ;
		int compare_length =  keyevent_altname_strlength > test_strlen ?  keyevent_altname_strlength : test_strlen ;
		int strc = strncmp(keyevents[counter].alt_name,test_string,compare_length);
		if(!strc){
			keyevent_index = counter;
			//printf("	keyevent_index  %d\n",	keyevent_index );
			break;				
		}else{
			int keyevent_name_strlength = strlen(keyevents[counter].name) ;
			compare_length =  keyevent_name_strlength> test_strlen ?  keyevent_name_strlength: test_strlen ;
			strc = strncmp(keyevents[counter].name,test_string,compare_length);
			if(!strc){
				keyevent_index  = counter;
				break;
			}
		}
	}
	return keyevent_index;
}

int process_ipaddress(int argc, char **argv,int *new_argc ,char ***new_argv){
		
		if(!is_ipaddress(argv[0])){
			D("Test:IP_ADDRESS=FALSE\n");
			return 0; // not an ipaddress, time time leave time place
		}
		*new_argc =2;		
		(*new_argv) = (char**) malloc(*new_argc * sizeof(char**));
		(*new_argv)[0]="connect";
		(*new_argv)[1]=argv[0];
		D("Test:IP_ADDRESS=TRUE\n");
		return 1;				
}
int process_keyevent_chain(int argc, char **argv){

	// counters and state management 
	int keyevent_index, repeat_count   , repeat_counter  , check_repeater =0  , processed=0 , argc_counter  =0 ; 
	// Because we call adb_commandline direct from here we need to Setup private storage for passing arguments
	char **internal_argv = NULL;
	// setup up 3/4 of a keyevent command
	int internal_argc = 4 ; (internal_argv) = (char**) malloc(internal_argc * sizeof(char**));
	(internal_argv)[0] = "shell"; (internal_argv)[1] = "input" ; (internal_argv)[2] = "keyevent"; 
	while(argc_counter<argc){		
		if(check_repeater){
			if(!(repeat_count = atol(argv[argc_counter])))
					repeat_count = 1;
			
			for(repeat_counter = 0 ; repeat_counter<repeat_count;repeat_counter++)
				adb_commandline(internal_argc,(internal_argv) );		
			
			check_repeater = 0;
					
		}
		if((keyevent_index = is_keyevent(argv[argc_counter])) != -1){
			processed = 1 ; check_repeater =1 ;
			(internal_argv)[3] = keyevents[keyevent_index].keycode;			
		}
		argc_counter ++; 
	
	}
	if(check_repeater) // we end with a key command so tidy up 
		adb_commandline(internal_argc,(internal_argv) );
		
	D("Test:KEYEVENT_CHAIN=%d\n",processed);
	return processed;	
}
int is_shortcut(char* test_string){
	D("test_string:%s\n",test_string);
	int counter = 0 ; int shortcut_index = -1 ; int test_strlen = strlen(test_string) ;
	for(counter = 0 ; counter < shortcut_total ; counter++){
		int shortcut_strlength = strlen(shortcuts[counter].short_version) ;
		int compare_length =  shortcut_strlength > test_strlen ?  shortcut_strlength : test_strlen ;
		int strc = strncmp(shortcuts[counter].short_version,test_string,compare_length);
		if(!strc){
			shortcut_index = counter;
			break;				
		}
	}
	D("shortcut_index:%d:\n",shortcut_index);
	return shortcut_index;
}
/* 
 * process_shortcut : 
 * <in> argc 			- original argument count
 * <in> **argv 			- original argument values
 * <out> *new_argc 		- new argument count
 * <out> ***new_argv 	- new argument count
 * Returns 
 * 1. Call is_shortcut to test the string at index 0 in argv for a valid command_shortcut.shortversion 
 * 2. 
 * */
int process_shortcut(int argc, char **argv,int *new_argc ,char ***new_argv){
	if(!argc) { // sanity check to make sure nothing slipped through
		// bail early
		*new_argc = argc;
		*new_argv = argv;
		return 0;
	} 
	
	int shortcut_index = is_shortcut(argv[0]);
	if(shortcut_index != COMMAND_SHORTCUT_NOT_FOUND)
	{
		D("Test:SHORTCUT_COMMAND_INDEX=%d\n",shortcut_index);
		// check to see if we should append or format passed argv values
		int counter = 0;  *new_argc = 0; char *concat_command=NULL;
		switch(shortcuts[shortcut_index].process_args_type){
			case COMMAND_ARGS_CONCAT:{
				/* COMMAND_FORMAT_ARGS RULES OF ENGAGEMENT
				 * 1. shortcuts[shortcut_index].token_total must be greater 1. 
				 * 2. shortcuts[shortcut_index].full_version[0] contains a formatted 
				*/
				// Token Total includes format string , we only want the args
				*new_argc = (shortcuts[shortcut_index].token_total);
				concat_command =malloc( sizeof(char*) *4096);
				strncpy(concat_command,shortcuts[shortcut_index].full_version[0],strlen(shortcuts[shortcut_index].full_version[0]));
				
				for(counter=1;counter<*new_argc;counter++){
					char * token_string = shortcuts[shortcut_index].full_version[counter];
					D("Test:COMMAND_FORMAT_ARGS *new_argc:%d argc:%d %s\n",*new_argc,argc, token_string);
					if(strlen(token_string)==1 && isdigit(token_string[0])){
						int use_arg = atoi(token_string);
						strncat(concat_command,argv[use_arg],strlen(argv[use_arg]));
					}else{
						strncat(concat_command,token_string,strlen(token_string ));
					}
					//printf("%s\n",concat_command);
				}
				
				// Done cating, reset new_argc to 1 because we send it as one parameter
				*new_argc=1;
				//printf("11%s %d\n",concat_command,*new_argc);
				break;
			} 
			case COMMAND_ARGS_APPEND: {
				D("Test:COMMAND_HAS_ARGS *new_argc:%d argc:%d\n",*new_argc,argc);
				*new_argc += (shortcuts[shortcut_index].token_total + (argc-1));
				D("Test:COMMAND_HAS_ARGS *new_argc:%d argc:%d\n",*new_argc,argc);
				break;
			}
			case COMMAND_ARGS_NONE:
				// The Command Doesnot support additional arguments, we only need to
				// the process the tokens in the shortcuts[shortcut_index].full_version array
				D("Test:COMMAND_HAS_ARGS *new_argc:%d argc:%d\n",*new_argc,argc);
				*new_argc = shortcuts[shortcut_index].token_total; 
			default:
				break;
		}
		
		//printf("Total:%d\n",shortcuts[shortcut_index].command_type); 
		// If the shortcut is a shell command then we need to make room 
		// for the shell keyword.
		if(shortcuts[shortcut_index].command_type == COMMAND_TYPE_SHELL) {
			*new_argc += 1 ;
			//printf("Shell:%d\n",*new_argc); 
		}
		//printf("Total:%d\n",*new_argc); 
		// We Now now what size our argument value array needs to
		// so we can alloc the memory.
		(*new_argv) = (char**) malloc(*new_argc * sizeof(char**));
		int new_argc_position =0;
		printf("Total:%d - ",*new_argc); 
		if(shortcuts[shortcut_index].command_type == COMMAND_TYPE_SHELL){
			(*new_argv) [new_argc_position] = "shell";
			printf("[%d:sh]=%s ",new_argc_position,(*new_argv) [new_argc_position]); 
			new_argc_position += 1;
		}
		switch(shortcuts[shortcut_index].process_args_type){
			case COMMAND_ARGS_CONCAT:{
				//(*new_argv) [new_argc_position]=concat_command;
				
				(*new_argv) [new_argc_position]  =malloc( sizeof(char*) *strlen(concat_command)+1);
				strcpy((*new_argv) [new_argc_position],concat_command);
				printf("[%d:pa]=%s ",new_argc_position,(*new_argv) [new_argc_position]); 
				free(concat_command);
				new_argc_position += 1;
				break;
			}
			case COMMAND_ARGS_NONE:{
				for(counter = 0; counter < shortcuts[shortcut_index].token_total ; counter ++){
					(*new_argv) [new_argc_position] = shortcuts[shortcut_index].full_version[counter];
					printf("[%d:fv]=%s ",new_argc_position,(*new_argv) [new_argc_position]); 
					new_argc_position += 1;
				}break;
			}
			case COMMAND_ARGS_APPEND:{
				for(counter = 0; counter < shortcuts[shortcut_index].token_total ; counter ++){
					(*new_argv) [new_argc_position] = shortcuts[shortcut_index].full_version[counter];
					printf("[%d:fv]=%s ",new_argc_position,(*new_argv) [new_argc_position]); 
					new_argc_position += 1;
				}
				if(shortcuts[shortcut_index].process_args_type ==COMMAND_ARGS_APPEND)
				for(counter = 1; counter < argc;counter++){
					(*new_argv)[new_argc_position] = argv[counter];
					printf("[%d:pa]=%s ",new_argc_position,(*new_argv) [new_argc_position]); 
					new_argc_position++;
				}
				break;
			}
			
		} 
		
		
		
		//if(shortcuts[shortcut_index].process_args_type == COMMAND_ARGS_APPEND){
			
		//}
		printf("\n");
		//print_args(*new_argc,(*new_argv));
		return 1;
	}
	D("Test:SHORTCUT_COMMAND_INDEX=%d\n",shortcut_index);
	return 0;
}

// It's alright We will take over commandline calling from here, 
// thank you very much
int adb_extended_commandline(int argc , char **argv){
	
	
	D("PreProcess Command Line\n");
	//TRACE_INLINE_ARGS
	
	char **new_argv=NULL; int new_argc = -1;
	new_argv=argv; new_argc =argc;
	if(!argc) {
		D("Nothing To Process!!! Returning Control!\n");
		goto process_now;
	}
	
	/* 
	 * Pre-Process Checking Order
	 * 1. Check if we have been passed an ip-address
	 * 2. Check for pre-process shortcut
	 * 3. Check for Keyevent(s) chain   
	 */
	
	if(process_ipaddress(argc,argv,&new_argc,&new_argv)){
		// We have a valid ipaddress. The new_argv[] should now be "connect <ipaddress>"
		goto process_now;
	}
	 D("new_argc:%d\n",new_argc);
	if(process_shortcut(argc,argv,&new_argc,&new_argv)){ 
		/* 	Got ourselves a shortcut &new_argc and &new_argv will contain the real 
		 *	exploded command line values
		 */
		 D("new_argc:%d\n",new_argc);
		// TRACE_NEW_INLINE_ARGS;
		 goto process_now;
	}
	if(process_keyevent_chain(argc,argv)){
		/* Process a collection of keyevents, the process_keyevent_chain
		 * functions handles the adb_commandline as it may have been called
		 * multiple time
		*/ 
			D("Argc:\n" );//%d\n",new_argc);
		return COMMAND_LINE_PROCESS_DONE;
	}
	D("Argc:%d\n",new_argc);
process_now:
    return adb_commandline(new_argc, new_argv);

	
}


