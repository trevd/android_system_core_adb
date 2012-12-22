
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include "sysdeps.h"
#include "adb.h"
#include "adb_extended.h"

#define  TRACE_TAG  TRACE_EXT
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

		if(!is_ipaddress(argv[0]))
			return 0; // not an ipaddress, time time leave time place

		*new_argc =2;		
		(*new_argv) = (char**) malloc(*new_argc * sizeof(char**));
		(*new_argv)[0]="connect";
		(*new_argv)[1]=argv[0];
		return 1;				
}
int process_keyevent_chain(int argc, char **argv,int *new_argc ,char ***new_argv){

	// counters and state management 
	int keyevent_index, repeat_count   , repeat_counter  , check_repeater =0  , processed=0 , argc_counter  =0 ; 
	
	// setup up 3/4 of a keyevent command
	*new_argc = 4 ; (*new_argv) = (char**) malloc(*new_argc * sizeof(char**));
	(*new_argv)[0] = "shell"; (*new_argv)[1] = "input" ; (*new_argv)[2] = "keyevent"; 
	while(argc_counter<argc){		
		if(check_repeater){
			if(!(repeat_count = atol(argv[argc_counter])))
					repeat_count = 1;
			
			for(repeat_counter = 0 ; repeat_counter<repeat_count;repeat_counter++)
				adb_commandline(*new_argc,(*new_argv) );		
			
			check_repeater = 0;
					
		}
		if((keyevent_index = is_keyevent(argv[argc_counter])) != -1){
			processed = 1 ; check_repeater =1 ;
			(*new_argv)[3] = keyevents[keyevent_index].keycode;			
		}
		argc_counter ++; 
	
	}
	if(check_repeater) // we end with a key command so tidy up 
		adb_commandline(*new_argc,(*new_argv) );
	
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

int process_shortcut(int argc, char **argv,int *new_argc ,char ***new_argv){
	if(!argc) { // sanity check to make sure nothing slipped through
		// bail early
		*new_argc = argc;
		*new_argv = argv;
		return 0;
	} 
	int shortcut_index = is_shortcut(argv[0]);
	if(shortcut_index != -1)
	{
		//printf("Shortcut Found %d %s\n",shortcut_index,shortcuts[shortcut_index].full_version[0]);
		// build our argc and argv 		

		// check to see if we should append passed args 
		int counter = 0; int token_total = shortcuts[shortcut_index].token_total; 
		*new_argc = token_total; counter =0;
		if(shortcuts[shortcut_index].process_args) *new_argc += (argc-1);
		if(shortcuts[shortcut_index].is_shell) *new_argc += 1 ; 
		
		(*new_argv) = (char**) malloc(*new_argc * sizeof(char**));
		int new_argc_position =0;
		printf("Total:%d - ",*new_argc); 
		if(shortcuts[shortcut_index].is_shell){
			(*new_argv) [new_argc_position] = "shell";
			printf("[%d:sh]=%s ",new_argc_position,(*new_argv) [new_argc_position]); 
			new_argc_position += 1;
		}

		for(counter = 0; counter < token_total ; counter ++){
			(*new_argv) [new_argc_position] = shortcuts[shortcut_index].full_version[counter];
			printf("[%d:fv]=%s ",new_argc_position,(*new_argv) [new_argc_position]); 
			new_argc_position += 1;
		}
		//printf("counter:%d argc:%d\n",counter,argc);
		if(shortcuts[shortcut_index].process_args){
			// append args if we need to
			for(counter = 1; counter < argc;counter++){
				(*new_argv)[new_argc_position] = argv[counter];
				printf("[%d:pa]=%s ",new_argc_position,(*new_argv) [new_argc_position]); 
				new_argc_position++;
			}
		}
		printf("\n");
		//print_args(*new_argc,(*new_argv));
		return 1;
	}
	return 0;
}

// It's alright We will take over commandline calling from here, 
// thank you very much
int adb_extended_commandline(int argc , char **argv){
	
	D("PreProcess Command Line\n");
	TRACE_INLINE_ARGS
	char **new_argv=NULL  ; int new_argc = 0;
	if(!argc) {
		D("adb_extended_commandline argc:%d\n",argc);
		new_argc = argc; new_argv = argv;
		goto process_now;
	}
	// we'll by checking to ee for ip!
	if(process_ipaddress(argc,argv,&new_argc,&new_argv))
		goto process_now;
			


	// CHECK TO SEE IF THE USER IS LAZY! 
	// &new_argc and &new_argv will contain the real 
	// exploded values
	if(process_shortcut(argc,argv,&new_argc,&new_argv)){ 
		 	goto process_now;
	}else if(process_keyevent_chain(argc,argv,&new_argc,&new_argv)){
		return 0;
	}else{	
		new_argc = argc; new_argv = argv;
	}

process_now:
    return adb_commandline(new_argc, new_argv);

	
}


