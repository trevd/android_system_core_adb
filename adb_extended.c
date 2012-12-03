#define  TRACE_TAG  TRACE_EXT
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include "sysdeps.h"
#include "adb.h"

#define ARRAYSIZE(a) ((int)(sizeof(a) / sizeof(*(a))))
#define MAX_TOKENS 255
struct input_keyevents{
	char * name;
	char * alt_name;
	char * keycode;
} keyevents[] = { 
		{ "MENU","menu","82"},
		{ "UNLOCK","unlock","82"},
		{ "EXPLORER", "browser","64"},
		{ "BACK","bk","4"},
		{ "POWER","pwr","26"},
		{ "DPAD_UP","up","19"},
		{ "DPAD_DOWN","down","20"},
		{ "DPAD_LEFT","left","21"},
		{ "DPAD_RIGHT","right","22"},
		{ "TAB","tab","61"},
		{ "ENTER","ent","66"} };

#define COMMAND_HAS_ARGS 1
#define COMMAND_NO_ARGS 0
#define COMMAND_TYPE_SHELL 1
#define COMMAND_TYPE_ADB 0
struct command_shortcut {
		char * short_version;
		int token_total;
		int process_args; 
		int is_shell;
		char * full_version[MAX_TOKENS];
} shortcuts[]  = { 
					{ "ver",COMMAND_HAS_ARGS ,COMMAND_NO_ARGS ,COMMAND_TYPE_ADB,{"version"}},
					{ "remount",2,COMMAND_HAS_ARGS ,COMMAND_TYPE_SHELL,{"mount","-rw,remount"}},
					{ "dev",1,COMMAND_HAS_ARGS ,COMMAND_TYPE_ADB,{"devices"}},
					{ "d",1,COMMAND_HAS_ARGS ,COMMAND_TYPE_ADB,{"devices"}},
					{ "kill",COMMAND_HAS_ARGS ,COMMAND_NO_ARGS ,COMMAND_TYPE_ADB,{"kill-server"}},
					{ "sh",0,COMMAND_HAS_ARGS ,COMMAND_TYPE_SHELL,{}},
					{ "s",0,COMMAND_HAS_ARGS ,COMMAND_TYPE_SHELL,{}},
					{ "st",1,COMMAND_HAS_ARGS ,COMMAND_TYPE_SHELL,{"start"}},
					{ "ks",1,COMMAND_HAS_ARGS ,COMMAND_TYPE_SHELL,{"stop"}},
					{ "dmesg",COMMAND_HAS_ARGS ,COMMAND_NO_ARGS ,COMMAND_TYPE_SHELL,{"dmesg"}},
					{ "mount",1,COMMAND_HAS_ARGS ,COMMAND_TYPE_SHELL,{"mount"}},
					{ "umount",1,COMMAND_HAS_ARGS ,COMMAND_TYPE_SHELL,{"umount"}},
					{ "lspart",1,COMMAND_HAS_ARGS ,COMMAND_TYPE_SHELL,{"mount | grep"}},
					{ "uname",2,COMMAND_NO_ARGS ,COMMAND_TYPE_SHELL,{"cat","proc/version"}},
					{ "gp",1,COMMAND_HAS_ARGS ,COMMAND_TYPE_SHELL,{"getprop"}},
					{ "du",1,COMMAND_HAS_ARGS ,COMMAND_TYPE_SHELL,{"du"}},
					{ "df",1,COMMAND_HAS_ARGS ,COMMAND_TYPE_SHELL,{"df"}},
					{ "dfh",2,COMMAND_HAS_ARGS ,COMMAND_TYPE_SHELL,{"df","-h"}},
					{ "sp",1,COMMAND_HAS_ARGS ,COMMAND_TYPE_SHELL,{"setprop"}},
					{ "cat",1,COMMAND_HAS_ARGS ,COMMAND_TYPE_SHELL,{"cat"}},
					{ "c",1,COMMAND_HAS_ARGS ,COMMAND_TYPE_SHELL,{"cat"}},
					{ "echo",1,COMMAND_HAS_ARGS ,COMMAND_TYPE_SHELL,{"echo"}},
					{ "ps",COMMAND_HAS_ARGS ,COMMAND_NO_ARGS ,COMMAND_TYPE_SHELL,{"ps"}},
					{ "mv",1,COMMAND_HAS_ARGS ,COMMAND_TYPE_SHELL,{"mv"}},
					{ "cp",1,COMMAND_HAS_ARGS ,COMMAND_TYPE_SHELL,{"cp"}},
					{ "rm",1,COMMAND_HAS_ARGS ,COMMAND_TYPE_SHELL,{"rm"}},
					{ "rmf",2,COMMAND_HAS_ARGS ,COMMAND_TYPE_SHELL,{"rm","-f"}},
					{ "symlink",2,COMMAND_HAS_ARGS ,COMMAND_TYPE_SHELL,{"ln","-s"}},
					{ "netcfg",1,COMMAND_HAS_ARGS ,COMMAND_TYPE_SHELL,{"netcfg"}},
					
					{ "touch",1,COMMAND_HAS_ARGS ,COMMAND_TYPE_SHELL,{"touch"}},
					{ "lsusb",1,COMMAND_HAS_ARGS ,COMMAND_TYPE_SHELL,{"lsusb"}},
					{ "mkdir",1,COMMAND_HAS_ARGS ,COMMAND_TYPE_SHELL,{"mkdir"}},
					{ "grp",1,COMMAND_HAS_ARGS ,COMMAND_TYPE_SHELL,{"getprop | grep"}},
					{ "wp",1,COMMAND_NO_ARGS ,COMMAND_TYPE_SHELL,{"watchprops"}},
					{ "chmod",1,COMMAND_HAS_ARGS ,COMMAND_TYPE_SHELL,{"chmod"}},
					{ "insmod",1,COMMAND_HAS_ARGS ,COMMAND_TYPE_SHELL,{"insmod"}},
					{ "gevt",1,COMMAND_HAS_ARGS ,COMMAND_TYPE_SHELL,{"getevent"}},
					{ "sevt",1,COMMAND_HAS_ARGS ,COMMAND_TYPE_SHELL,{"sendevent"}},
					{ "lsmod",1,COMMAND_NO_ARGS ,COMMAND_TYPE_SHELL,{"lsmod"}},
					{ "pl",1,COMMAND_HAS_ARGS ,COMMAND_TYPE_ADB,{"pull"}},
					{ "pu",1,COMMAND_HAS_ARGS ,COMMAND_TYPE_ADB,{"push"}},
					{ "wfd",1,COMMAND_NO_ARGS ,COMMAND_TYPE_ADB,{"wait-for-device"}},
					// reboot commands, some device specific
					{ "recovery",2,COMMAND_NO_ARGS ,COMMAND_TYPE_ADB,{"reboot","recovery"}},
					{ "fastboot",2,COMMAND_NO_ARGS ,COMMAND_TYPE_ADB,{"reboot","bootloader"}},
					{ "rec",2,COMMAND_NO_ARGS ,COMMAND_TYPE_ADB,{"reboot","recovery"}},
					// archos specialness
					{ "kdf",1,COMMAND_HAS_ARGS ,COMMAND_TYPE_SHELL,{"kd_flasher"}},
					{ "updaterd",1,COMMAND_NO_ARGS ,COMMAND_TYPE_SHELL,{"updaterd"}},
					{ "sde",2,COMMAND_NO_ARGS ,COMMAND_TYPE_SHELL,{"reboot_into","sde"}},
					{ "into",2,COMMAND_HAS_ARGS ,COMMAND_TYPE_SHELL,{"reboot_into","-s"}},
					{ "arec",2,COMMAND_NO_ARGS ,COMMAND_TYPE_SHELL,{"reboot_into","recovery"}},
					{ "android",2,COMMAND_NO_ARGS ,COMMAND_TYPE_SHELL,{"reboot_into","android"}},
					// samsung download mode
					{ "download",2,COMMAND_NO_ARGS ,COMMAND_TYPE_ADB,{"reboot","download"}}, 
					{ "ll",2,COMMAND_HAS_ARGS ,COMMAND_TYPE_SHELL,{"ls","-l"}},
					{ "l",2,COMMAND_HAS_ARGS ,COMMAND_TYPE_SHELL,{"ls","-la"}},
					{ "lha",2,COMMAND_HAS_ARGS ,COMMAND_TYPE_SHELL,{"ls","-lha"}},
					{ "lca",9,COMMAND_NO_ARGS ,COMMAND_TYPE_ADB,{"logcat","-b","system","-b","radio","-b","events","-b","main" }},
					{ "lc",1,COMMAND_HAS_ARGS ,COMMAND_TYPE_ADB,{"logcat"}},
					{ "temp-root",1,COMMAND_NO_ARGS ,COMMAND_TYPE_SHELL,{"echo 'ro.kernel.qemu=1'>/data/local.prop"}},
					{ "un-root",1,COMMAND_NO_ARGS ,COMMAND_TYPE_SHELL,{"echo '#'>/data/local.prop"}},
					// Input				
					{ "key",2,COMMAND_HAS_ARGS ,COMMAND_TYPE_SHELL,{"input","keyevent"}},
					{ "tw",2,COMMAND_HAS_ARGS ,COMMAND_TYPE_SHELL,{"input","text"}},
					{ "tap",2,COMMAND_HAS_ARGS ,COMMAND_TYPE_SHELL,{"input","tap"}},
					{ "swipe",2,COMMAND_HAS_ARGS ,COMMAND_TYPE_SHELL,{"input","swipe"}},
					// Activity Manager Startups
					{ "vending",6,COMMAND_NO_ARGS ,COMMAND_TYPE_SHELL,{"am", "start","-a" ,"android.intent.action.MAIN","-n","com.android.vending/.AssetBrowserActivity"}},
					{ "settings",6,COMMAND_NO_ARGS ,COMMAND_TYPE_SHELL,{"am", "start","-a" ,"android.intent.action.MAIN","-n","com.android.settings/.Settings"}}
} ;

static const int shortcut_total = ARRAYSIZE(shortcuts);
static const int keyevents_total = ARRAYSIZE(keyevents);
int print_args(int argc, char **argv); 
int process_shortcut(int argc, char **argv,int *new_argc ,char ***new_argv);
int process_keyevent_chain(int argc, char **argv,int *new_argc ,char ***new_argv);
int process_ipaddress(int argc, char **argv,int *new_argc ,char ***new_argv);
int is_keyevent(char* test_string);
int is_shortcut(char* test_string);
int is_ipaddress(char *ipaddress);
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
int process_ipaddress(int argc, char **argv,int *new_argc ,char ***new_argv){

		if(!is_ipaddress(argv[0]))
			return 0; // not an ipaddress, time time leave time place

		*new_argc =2;		
		(*new_argv) = (char**) malloc(*new_argc * sizeof(char**));
		(*new_argv)[0]="connect";
		(*new_argv)[1]=argv[0];
		return 1;		
		
}
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

// It's alright We will take over commandline calling from here, 
// thank you very much
int adb_extended_commandline(int argc , char **argv){
	
	D("adb_extended_commandline\n");
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


