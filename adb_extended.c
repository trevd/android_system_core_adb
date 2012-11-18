#define  TRACE_TAG  TRACE_EXT
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include "sysdeps.h"
#include "adb.h"

#define ARRAYSIZE(a) ((int)(sizeof(a) / sizeof(*(a))))
#define MAX_TOKENS 255
struct input_keyevents{
	char * name;
	char * alt_name;
	char * keycode;
} keyevents[] = { { "MENU","unlock","82"},
		{ "EXPLORER", "browser","64"},
		{ "BACK","bk","4"},
		{ "POWER","pwr","26"},
		{ "DPAD_UP","up","19"},
		{ "DPAD_DOWN","down","20"},
		{ "DPAD_LEFT","left","21"},
		{ "DPAD_RIGHT","right","23"},
		{ "TAB","tab","61"},
		{ "ENTER","ent","66"} };


struct command_shortcut {
		char * short_version;
		int token_total;
		int process_args; 
		int is_shell;
		char * full_version[MAX_TOKENS];
} shortcuts[]  = { { "dev",1,1,0,{"devices"}},
					{ "kill",1,0,0,{"kill-server"}},
					{ "sh",0,1,1,{}},
					{ "s",0,1,1,{}},
					{ "st",1,1,1,{"start"}},
					{ "ks",1,1,1,{"stop"}},
					{ "dmesg",1,0,1,{"dmesg"}},
					{ "mount",1,1,1,{"mount"}},
					{ "umount",1,1,1,{"umount"}},
					{ "lspart",1,1,1,{"mount | grep"}},
					{ "uname",2,0,1,{"cat","proc/version"}},
					{ "gp",1,1,1,{"getprop"}},
					{ "du",1,1,1,{"du"}},
					{ "df",1,1,1,{"df"}},
					{ "dfh",2,1,1,{"df","-h"}},
					{ "sp",1,1,1,{"setprop"}},
					{ "cat",1,1,1,{"cat"}},
					{ "c",1,1,1,{"cat"}},
					{ "echo",1,1,1,{"echo"}},
					{ "ps",1,0,1,{"ps"}},
					{ "mv",1,1,1,{"mv"}},
					{ "cp",1,1,1,{"cp"}},
					{ "rm",1,1,1,{"rm"}},
					{ "touch",1,1,1,{"touch"}},
					{ "lsusb",1,1,1,{"lsusb"}},
					{ "grp",1,1,1,{"getprop | grep"}},
					{ "wp",1,0,1,{"watchprops"}},
					{ "chmod",1,1,1,{"chmod"}},
					{ "insmod",1,1,1,{"insmod"}},
					{ "gevt",1,1,1,{"getevent"}},
					{ "sevt",1,1,1,{"sendevent"}},
					{ "lsmod",1,0,1,{"lsmod"}},
					{ "pl",1,1,0,{"pull"}},
					{ "pu",1,1,0,{"push"}},
					// reboot commands, some device specific
					{ "recovery",2,0,0,{"reboot","recovery"}},
					{ "fastboot",2,0,0,{"reboot","bootloader"}},
					{ "rec",2,0,0,{"reboot","recovery"}},
					// archos specialness
					{ "sde",2,0,1,{"reboot_into","sde"}},
					{ "arec",2,0,1,{"reboot_into","recovery"}},
					{ "android",2,0,1,{"reboot_into","android"}},
					// samsung download mode
					{ "download",2,0,0,{"reboot","download"}}, 
					{ "ll",2,1,1,{"ls","-l"}},
					{ "lha",2,1,1,{"ls","-lha"}},
					{ "lca",9,0,0,{"logcat","-b","system","-b","radio","-b","events","-b","main" }},
					{ "lc",1,1,0,{"logcat"}},
					{ "temp-root",1,0,1,{"echo 'ro.kernel.qemu=1'>/data/local.prop"}},
					{ "un-root",1,0,1,{"echo '#'>/data/local.prop"}},
					// Input				
					{ "key",2,1,1,{"input","keyevent"}},
					{ "tw",2,1,1,{"input","text"}},
					{ "tap",2,1,1,{"input","tap"}},
					{ "swipe",2,1,1,{"input","swipe"}},
					// Activity Manager Startups
					{ "vending",6,0,1,{"am", "start","-a" ,"android.intent.action.MAIN","-n","com.android.vending/.AssetBrowserActivity"}},
					{ "settings",6,0,1,{"am", "start","-a" ,"android.intent.action.MAIN","-n","com.android.settings/.Settings"}}
} ;

static const int shortcut_total = ARRAYSIZE(shortcuts);
static const int keyevents_total = ARRAYSIZE(keyevents);
int print_args(int argc, char **argv); 
int process_shortcut(int argc, char **argv,int *new_argc ,char ***new_argv);
int process_keyevent_chain(int argc, char **argv,int *new_argc ,char ***new_argv);
int is_keyevent(char* test_string);
int is_shortcut(char* test_string);
// Cheeky Debug function 
int print_args(int argc, char **argv)
{
	int counter = 0;
	D("argc:%d\n",argc);	
	for(counter = 0 ; counter < argc;counter++){
		D("argv[%d]:%s\n",counter,argv[counter]);	
	}

	return 0;
}
int is_keyevent(char* test_string){

	int counter = 0 ; int keyevent_index = -1 ; int test_strlen = strlen(test_string) ;
	for(counter = 0 ; counter < keyevents_total ; counter++){
		int keyevent_altname_strlength = strlen(keyevents[counter].alt_name) ;
		int compare_length =  keyevent_altname_strlength > test_strlen ?  keyevent_altname_strlength : test_strlen ;
		int strc = strncmp(keyevents[counter].alt_name,test_string,compare_length);
		if(!strc){
			keyevent_index = counter;
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

int process_keyevent_chain(int argc, char **argv,int *new_argc ,char ***new_argv){

	int argc_position = 0 ; int processed =0;
	// check the first argv for a keyevent. we will only process if there is
	// one present
	
	int keyevent_index = is_keyevent(argv[argc_position]);
	if(keyevent_index != -1){
		// check to see if next is numeric
		D("Keyevent Index:%d argc=%d\n",keyevent_index,argc);
		if(argc>1) argc_position += 1;
		int repeat_count =0 ; int repeat_counter =0;  
		*new_argc = 4 ;
		(*new_argv) = (char**) malloc(*new_argc * sizeof(char**));
		
		(*new_argv)[0] = "shell"; (*new_argv)[1] = "input" ; (*new_argv)[2] = "keyevent"; (*new_argv)[3] = keyevents[keyevent_index].keycode;
		int argc_counter = argc_position; 
		for(argc_counter = argc_position;argc_counter<(argc); argc_counter++){
			printf("counter=%d : pos=%d : argv[%d]=%s\n",argc_counter,argc_position,argc_position,argv[argc_position]);
			if(argc_position==argc) // we've only got one to process 
				repeat_count = 1;
			else
	 			repeat_count =atoi(argv[argc_position]); 
			// set  up a command line		

			if(repeat_count == 0)
				repeat_count = 1;
			else if(repeat_count > 0)
				argc_position += 1; 	
			printf("repeat_count:%d\n",repeat_count);			
			for(repeat_counter = 0 ; repeat_counter<repeat_count;repeat_counter++){
				//printf("
				printf("repeat_counter:%d\n",repeat_counter);
				adb_commandline(*new_argc,(*new_argv) );		
			}
			processed = 1 ;
			//return 1;
		}
		
	}
		
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
	print_args(argc,argv);
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


int adb_extended_commandline(int argc, char **argv){
	
	int new_argc = 0;
	D("adb_extended_commandline\n");
	char **new_argv=NULL;		
	if(!argc) {
		D("adb_extended_commandline argc:%d\n",argc);
		new_argc = argc;
		new_argv = argv;
	    return adb_commandline(new_argc, new_argv);
	}
	
	if(process_shortcut(argc,argv,&new_argc,&new_argv)){
		
		 return adb_commandline(new_argc, new_argv);
	}else if(process_keyevent_chain(argc,argv,&new_argc,&new_argv)){
		return 0;
	}else{	
		//printf("Here we are ");		
		new_argc = argc;
		new_argv = argv;
	}

	//print_args(argc,argv);
    return adb_commandline(new_argc, new_argv);

	
}


