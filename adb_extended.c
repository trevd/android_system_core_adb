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
					{ "kill",1,0,0,{"kill-server"}},
					{ "sh",0,1,1,{}},
					{ "s",0,1,1,{}},
					{ "st",1,1,1,{"start"}},
					{ "ks",1,1,1,{"stop"}},
					{ "dmesg",1,0,1,{"dmesg"}},
					{ "mount",1,1,1,{"mount"}},
					{ "umount",1,1,1,{"umount"}},
					{ "gp",1,0,1,{"getprop"}},
					{ "cat",1,1,1,{"cat"}},
					{ "echo",1,1,1,{"echo"}},
					{ "ps",1,0,1,{"ps"}},
					{ "grp",1,1,1,{"getprop | grep"}},
					{ "wp",1,0,1,{"watchprops"}},
					{ "chmod",1,1,1,{"chmod"}},
					{ "insmod",1,1,1,{"insmod"}},
					{ "lsmod",1,0,1,{"lsmod"}},
					{ "pl",1,1,0,{"pull"}},
					{ "pu",1,1,0,{"push"}},
					{ "recovery",2,0,0,{"reboot","recovery"}},
					{ "sde",2,0,1,{"reboot_into","sde"}},
					{ "rec",2,0,0,{"reboot","recovery"}},
					{ "ll",2,1,1,{"ls","-l"}},
					{ "lha",2,1,1,{"ls","-lha"}},
					{ "lca",9,0,0,{"logcat","-b","system","-b","radio","-b","events","-b","main" }},
					{ "lc",1,1,0,{"logcat"}},
					{ "temp-root",1,0,1,{"echo 'ro.kernel.qemu=1'>/data/local.prop"}},
					{ "un-root",1,0,1,{"echo '#'>/data/local.prop"}},
					// Input KeyEvents					
					{ "key",2,1,1,{"input","keyevent"}},
					{ "tap",2,1,1,{"input","tap"}},
					{ "unlock",3,0,1,{"input","keyevent","82"}},
					{ "back",3,0,1,{"input","keyevent","4"}},
					{ "bk",3,0,1,{"input","keyevent","4"}},
					// Activity Manager Startups
					{ "vending",6,0,1,{"am", "start","-a" ,"android.intent.action.MAIN","-n","com.android.vending/.AssetBrowserActivity"}},
					{ "settings",6,0,1,{"am", "start","-a" ,"android.intent.action.MAIN","-n","com.android.settings/.Settings"}}
} ;

static const int shortcut_total = ARRAYSIZE(shortcuts);
int print_args(int argc, char **argv); 
int process_shortcut(int argc, char **argv,int *new_argc ,char ***new_argv);
int is_shortcut(char* test_string);
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
int is_shortcut(char* test_string){

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
	if(process_shortcut(argc,argv,new_argc,new_argv)){
		return 0;	
	}else{
		*new_argc = argc;
		*new_argv = argv;
	}
	//print_args(*new_argc,(*new_argv));
	
	return 0;
}


