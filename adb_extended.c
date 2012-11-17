#define  TRACE_TAG  TRACE_ADB
#include <stdio.h>
#include <string.h>

#define ARRAYSIZE(a) ((int)(sizeof(a) / sizeof(*(a))))
#define MAX_TOKENS 255
struct command_shortcut {
		char * short_version;
		int token_total;
		int process_args; 
		int is_shell;
		char * full_version[MAX_TOKENS];
} shortcuts[]  = { { "dev",1,0,0,{"devices"}},
					{ "sh",1,1,0,{"shell"}},
					{ "s",1,1,0,{"shell"}},
					{ "dmesg",1,0,1,{"dmesg"}},
					{ "mount",1,1,1,{"mount"}},
					{ "gp",1,0,1,{"getprop"}},
					{ "recovery",2,0,0,{"reboot","recovery"}},
					{ "ll",2,1,1,{"ls","-l"}},
					{ "lca",9,0,0,{"logcat","-b","system","-b","radio","-b","events","-b","main" }}} ;

static const int shortcut_total = ARRAYSIZE(shortcuts);
int print_args(int argc, char **argv); 
int is_shortcut(int argc, char **argv,int *new_argc ,char ***new_argv);

// Cheeky Debug function 
int print_args(int argc, char **argv)
{
	int counter = 0;
	printf("argc:%d ",argc);	
	for(counter = 0 ; counter < argc;counter++){
		printf("argv[%d]:%s ",counter,argv[counter]);	
	}
	printf("\n");
	return 0;
}
//
int is_shortcut(int argc, char **argv,int *new_argc ,char ***new_argv){

	if(!argc) { // sanity check to make sure nothing slipped through
		// bail early
		*new_argc = argc;
		*new_argv = argv;
		return 0;
	} 
	
	int counter = 0 ; int shortcut_index = -1 ; int argv_zero_strlen = strlen(argv[0]) ;

	for(counter = 0 ; counter < shortcut_total ; counter++){
		int shortcut_strlength = strlen(shortcuts[counter].short_version) ;
		int compare_length =  shortcut_strlength > argv_zero_strlen ?  shortcut_strlength : argv_zero_strlen ;
		int strc = strncmp(shortcuts[counter].short_version,argv[0],compare_length);
		//printf("strncmp:%d argv[0]=%s short_version=%s length:%d\n",strc,argv[0],shortcuts[counter].short_version,compare_length);
		if(!strc){
			shortcut_index = counter;
			break;				
		}
	}
	if(shortcut_index != -1)
	{
		//printf("Shortcut Found %d %s\n",shortcut_index,shortcuts[shortcut_index].full_version[0]);
		// build our argc and argv 		

		// check to see if we should append passed args 
		int token_total = shortcuts[shortcut_index].token_total; int new_argc_position =0;
		*new_argc = token_total;
		if(shortcuts[shortcut_index].process_args) *new_argc += (argc-1);
		if(shortcuts[shortcut_index].is_shell) *new_argc += 1 ; 

		
		printf("new_argc %d argc:%d process:%d is_shell:%d\n",*new_argc,argc,shortcuts[shortcut_index].process_args,shortcuts[shortcut_index].is_shell);
		(*new_argv) = (char**) malloc(*new_argc * sizeof(char**));
		if(shortcuts[shortcut_index].is_shell){
			(*new_argv) [new_argc_position] = "shell";
			new_argc_position += 1;
		}
		for(counter = 0; counter < token_total ; counter ++){
			(*new_argv) [counter+new_argc_position] = shortcuts[shortcut_index].full_version[counter];
		}
		printf("counter:%d argc:%d\n",counter,argc);
		new_argc_position = counter;
		if(shortcuts[shortcut_index].process_args){
			// append args if we need to
			for(counter = 1; counter < argc;counter++){
				(*new_argv)[counter+new_argc_position] = argv[counter];
			}
		}
		print_args(*new_argc,(*new_argv));
		return 1;
	}
	return 0;
}


int adb_extended_commandline(int argc, char **argv,int *new_argc ,char ***new_argv){
	
	if(!argc) {
		// bail early
		*new_argc = argc;
		*new_argv = argv;
		return 0;
	}
		
	//print_args(argc,argv);
	if(is_shortcut(argc,argv,new_argc,new_argv)){
		return 0;	
	}else{
		*new_argc = argc;
		*new_argv = argv;
	}
	//print_args(*new_argc,(*new_argv));
	
	return 0;
}


