#ifndef __ADB_EXTENDED_H
#define __ADB_EXTENDED_H	

#define ARRAYSIZE(a) ((int)(sizeof(a) / sizeof(*(a))))
#define TRACE_INLINE_ARGS D("argc:%d\n",argc);int counter = 0 ;for(counter = 0 ; counter < argc;counter++){D("argv[%d]:%s\n",counter,argv[counter]);}	

#define COMMAND_MAX_TOKENS 255
#define COMMAND_HAS_ARGS 1
#define COMMAND_NO_ARGS 0
#define COMMAND_TYPE_SHELL 1
#define COMMAND_TYPE_ADB 0

static struct command_shortcut {
		char * short_version;
		int token_total;
		int process_args; 
		int is_shell;
		char * full_version[COMMAND_MAX_TOKENS];
} shortcuts[]  = { 
					{ "ver",		1,COMMAND_NO_ARGS 	,COMMAND_TYPE_ADB,{"version"}},
					{ "dev",		1,COMMAND_HAS_ARGS 	,COMMAND_TYPE_ADB,{"devices"}},
					{ "d",			1,COMMAND_HAS_ARGS 	,COMMAND_TYPE_ADB,{"devices"}},
					{ "dl",			2,COMMAND_HAS_ARGS 	,COMMAND_TYPE_ADB,{"devices","-l"}},
					{ "kill",		1,COMMAND_NO_ARGS 	,COMMAND_TYPE_ADB,{"kill-server"}},
					{ "pl",			1,COMMAND_HAS_ARGS 	,COMMAND_TYPE_ADB,{"pull"}},
					{ "pu",			1,COMMAND_HAS_ARGS 	,COMMAND_TYPE_ADB,{"push"}},
					{ "wfd",		1,COMMAND_NO_ARGS 	,COMMAND_TYPE_ADB,{"wait-for-device"}},
					// reboot commands, some device specific
					{ "recovery",	2,COMMAND_NO_ARGS 	,COMMAND_TYPE_ADB,{"reboot","recovery"}},
					{ "rec",		2,COMMAND_NO_ARGS 	,COMMAND_TYPE_ADB,{"reboot","recovery"}},
					{ "fastboot",	2,COMMAND_NO_ARGS 	,COMMAND_TYPE_ADB,{"reboot","bootloader"}},
					{ "fb",			2,COMMAND_NO_ARGS 	,COMMAND_TYPE_ADB,{"reboot","bootloader"}},
					// samsung download mode
					{ "download",	2,COMMAND_NO_ARGS	,COMMAND_TYPE_ADB,{"reboot","download"}}, 

								
					// l
					{ "lca",		9,COMMAND_NO_ARGS 	,COMMAND_TYPE_ADB,{"logcat","-b","system","-b","radio","-b","events","-b","main" }},
					{ "lc",			1,COMMAND_HAS_ARGS	,COMMAND_TYPE_ADB,{"logcat"}},
					{ "sh",			0,COMMAND_HAS_ARGS 	,COMMAND_TYPE_SHELL,{}},
					{ "s",			0,COMMAND_HAS_ARGS 	,COMMAND_TYPE_SHELL,{}},
					{ "ll",			2,COMMAND_HAS_ARGS	,COMMAND_TYPE_SHELL,{"ls","-l"}},
					{ "l",			2,COMMAND_HAS_ARGS 	,COMMAND_TYPE_SHELL,{"ls","-la"}},
					{ "lha",		2,COMMAND_HAS_ARGS 	,COMMAND_TYPE_SHELL,{"ls","-lha"}},
					// mount helper
					{ "mount",		1,COMMAND_HAS_ARGS 	,COMMAND_TYPE_SHELL,{"mount"}},
					{ "umount",		1,COMMAND_HAS_ARGS 	,COMMAND_TYPE_SHELL,{"umount"}},
					{ "lspart",		1,COMMAND_HAS_ARGS 	,COMMAND_TYPE_SHELL,{"mount | grep"}},
					{ "remount",	2,COMMAND_HAS_ARGS 	,COMMAND_TYPE_SHELL,{"mount","-orw,remount"}},
					// native service stop and starters 
					{ "st",			1,COMMAND_HAS_ARGS 	,COMMAND_TYPE_SHELL,{"start"}},
					{ "ks",			1,COMMAND_HAS_ARGS 	,COMMAND_TYPE_SHELL,{"stop"}},
					{ "dmesg",		1,COMMAND_NO_ARGS 	,COMMAND_TYPE_SHELL,{"dmesg"}},
					{ "uname",		2,COMMAND_NO_ARGS 	,COMMAND_TYPE_SHELL,{"cat","proc/version"}},
					// property management helpers
					{ "gp",			1,COMMAND_HAS_ARGS 	,COMMAND_TYPE_SHELL,{"getprop"}},
					{ "grp",		1,COMMAND_HAS_ARGS 	,COMMAND_TYPE_SHELL,{"getprop | grep"}},
					{ "wp",			1,COMMAND_NO_ARGS 	,COMMAND_TYPE_SHELL,{"watchprops"}},
					{ "sp",			1,COMMAND_HAS_ARGS 	,COMMAND_TYPE_SHELL,{"setprop"}},
					{ "du",			1,COMMAND_HAS_ARGS 	,COMMAND_TYPE_SHELL,{"du"}},
					{ "df",			1,COMMAND_HAS_ARGS 	,COMMAND_TYPE_SHELL,{"df"}},
					{ "dfh",		2,COMMAND_HAS_ARGS 	,COMMAND_TYPE_SHELL,{"df","-h"}},
					{ "cat",		1,COMMAND_HAS_ARGS 	,COMMAND_TYPE_SHELL,{"cat"}},
					{ "c",			1,COMMAND_HAS_ARGS 	,COMMAND_TYPE_SHELL,{"cat"}},
					{ "echo",		1,COMMAND_HAS_ARGS 	,COMMAND_TYPE_SHELL,{"echo"}},
					{ "ps",			1,COMMAND_NO_ARGS	,COMMAND_TYPE_SHELL,{"ps"}},
					{ "mv",			1,COMMAND_HAS_ARGS 	,COMMAND_TYPE_SHELL,{"mv"}},
					{ "cp",			1,COMMAND_HAS_ARGS 	,COMMAND_TYPE_SHELL,{"cp"}},
					{ "rm",			1,COMMAND_HAS_ARGS 	,COMMAND_TYPE_SHELL,{"rm"}},
					{ "rmf",		2,COMMAND_HAS_ARGS 	,COMMAND_TYPE_SHELL,{"rm","-f"}},
					{ "symlink",	2,COMMAND_HAS_ARGS 	,COMMAND_TYPE_SHELL,{"ln","-s"}},
					{ "netcfg",		1,COMMAND_HAS_ARGS 	,COMMAND_TYPE_SHELL,{"netcfg"}},
					{ "touch",		1,COMMAND_HAS_ARGS 	,COMMAND_TYPE_SHELL,{"touch"}},
					{ "lsusb",		1,COMMAND_HAS_ARGS 	,COMMAND_TYPE_SHELL,{"lsusb"}},
					{ "mkdir",		1,COMMAND_HAS_ARGS 	,COMMAND_TYPE_SHELL,{"mkdir"}},
					{ "chmod",		1,COMMAND_HAS_ARGS 	,COMMAND_TYPE_SHELL,{"chmod"}},
					// kernel module commands
					{ "lsmod",		1,COMMAND_NO_ARGS 	,COMMAND_TYPE_SHELL,{"lsmod"}},
					{ "insmod",		1,COMMAND_HAS_ARGS 	,COMMAND_TYPE_SHELL,{"insmod"}},
					{ "rmmod",		1,COMMAND_HAS_ARGS 	,COMMAND_TYPE_SHELL,{"rmmod"}},
					// input events
					{ "gevt",		1,COMMAND_HAS_ARGS 	,COMMAND_TYPE_SHELL,{"getevent"}},
					{ "sevt",		1,COMMAND_HAS_ARGS 	,COMMAND_TYPE_SHELL,{"sendevent"}},
					
					// archos specific device management.... because they're special
					{ "kdf",		1,COMMAND_HAS_ARGS 	,COMMAND_TYPE_SHELL,{"kd_flasher"}},
					{ "updaterd",	1,COMMAND_NO_ARGS 	,COMMAND_TYPE_SHELL,{"updaterd"}},
					{ "sde",		2,COMMAND_NO_ARGS 	,COMMAND_TYPE_SHELL,{"reboot_into","sde"}},
					{ "into",		2,COMMAND_HAS_ARGS 	,COMMAND_TYPE_SHELL,{"reboot_into","-s"}},
					{ "arec",		2,COMMAND_NO_ARGS 	,COMMAND_TYPE_SHELL,{"reboot_into","recovery"}},
					{ "android",	2,COMMAND_NO_ARGS 	,COMMAND_TYPE_SHELL,{"reboot_into","android"}},	
					

					// write a local.prop --- handy temp rooting
					{ "temp-root",	1,COMMAND_NO_ARGS	,COMMAND_TYPE_SHELL,{"echo 'ro.kernel.qemu=1'>/data/local.prop"}},
					{ "un-root",	1,COMMAND_NO_ARGS	,COMMAND_TYPE_SHELL,{"echo '#'>/data/local.prop"}},
					// Input				
					{ "key",		2,COMMAND_HAS_ARGS	,COMMAND_TYPE_SHELL,{"input","keyevent"}},
					{ "tw",			2,COMMAND_HAS_ARGS 	,COMMAND_TYPE_SHELL,{"input","text"}},
					{ "tap",		2,COMMAND_HAS_ARGS 	,COMMAND_TYPE_SHELL,{"input","tap"}},
					{ "swipe",		2,COMMAND_HAS_ARGS 	,COMMAND_TYPE_SHELL,{"input","swipe"}},
					// Activity Manager Startups
					{ "vending",	6,COMMAND_NO_ARGS 	,COMMAND_TYPE_SHELL,{"am", "start","-a" ,"android.intent.action.MAIN","-n","com.android.vending/.AssetBrowserActivity"}},
					{ "settings",	6,COMMAND_NO_ARGS 	,COMMAND_TYPE_SHELL,{"am", "start","-a" ,"android.intent.action.MAIN","-n","com.android.settings/.Settings"}}
} ;
// Keyevents List  frameworks/base/core/java/android/view/KeyEvent.java
static struct input_keyevents{
	char * name;
	char * alt_name;
	char * keycode;
} keyevents[] = { 
		{ "HOME","home","3"},
		{ "BACK","bk","4"},		
		{ "CALL","call","5"},
		{ "ENDCALL","hangup","6"},
		{ "DPAD_UP","up","19"},
		{ "DPAD_DOWN","down","20"},
		{ "DPAD_LEFT","left","21"},
		{ "DPAD_RIGHT","right","22"},
		{ "POWER","pwr","26"},
		{ "CAMERA","cam","27"},
		{ "TAB","tab","61"},
		{ "EXPLORER", "browser","64"},
		{ "ENTER","ent","66"},
		{ "MENU","menu","82"},
		{ "UNLOCK","unlock","82"},
		{ "NOTIFICATION","systray","83"},
		{ "SEARCH","search","84"}
};

static const int shortcut_total = ARRAYSIZE(shortcuts);
static const int keyevents_total = ARRAYSIZE(keyevents);
int process_shortcut(int argc, char **argv,int *new_argc ,char ***new_argv);
int process_keyevent_chain(int argc, char **argv,int *new_argc ,char ***new_argv);
int process_ipaddress(int argc, char **argv,int *new_argc ,char ***new_argv);
int is_keyevent(char* test_string);
int is_shortcut(char* test_string);
int is_ipaddress(char *ipaddress);

int adb_extended_commandline(int argc , char** argv);
#endif
