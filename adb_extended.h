#ifndef __ADB_EXTENDED_H
#define __ADB_EXTENDED_H	

#define ARRAYSIZE(a) ((int)(sizeof(a) / sizeof(*(a))))
#define TRACE_INLINE_ARGS 1=1; //D("argc:%d\n",argc);int counter = 0 ;for(counter = 0 ; counter < argc;counter++){D("argv[%d]:%s\n",counter,argv[counter]);}	
#define TRACE_NEW_INLINE_ARGS 1=1; // D("argc:%d\n",new_argc);int new_counter = 0 ;for(new_counter = 0 ; new_counter < new_argc;new_counter++){D("argv[%d]:%s\n",new_counter,new_argv[new_counter]);}	

#define COMMAND_MAX_TOKENS 255
#define COMMAND_ARGS_CONCAT 2
#define COMMAND_ARGS_APPEND 1
#define COMMAND_ARGS_NONE 0
#define COMMAND_TYPE_SHELL 1
#define COMMAND_TYPE_ADB 0

#define COMMAND_SHORTCUT_NOT_FOUND -1
#define COMMAND_LINE_PROCESS_DONE 0


static struct command_shortcut {
		char * short_version;
		int token_total;
		int process_args_type; 
		int command_type;
		void *full_version[COMMAND_MAX_TOKENS];
} shortcuts[]  = { 
					
					{ "ver",		1,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_ADB,{"version"}},
					{ "dev",		1,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_ADB,{"devices"}},
					{ "d",			1,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_ADB,{"devices"}},
					{ "dl",			2,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_ADB,{"devices","-l"}},
					{ "kill",		1,COMMAND_ARGS_NONE 			,COMMAND_TYPE_ADB,{"kill-server"}},
					{ "pl",			1,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_ADB,{"pull"}},
					{ "pu",			1,COMMAND_ARGS_CONCAT 			,COMMAND_TYPE_ADB,{"push"}},
					{ "wfd",		1,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_ADB,{"wait-for-device"}},
					// reboot commands, some device specific
					{ "kick",		1,COMMAND_ARGS_NONE 			,COMMAND_TYPE_ADB,{"reboot"}},
					{ "rbt",		1,COMMAND_ARGS_NONE 			,COMMAND_TYPE_ADB,{"reboot"}},
					{ "recovery",	2,COMMAND_ARGS_NONE 			,COMMAND_TYPE_ADB,{"reboot","recovery"}},
					{ "rec",		2,COMMAND_ARGS_NONE 			,COMMAND_TYPE_ADB,{"reboot","recovery"}},
					{ "fastboot",	2,COMMAND_ARGS_NONE 			,COMMAND_TYPE_ADB,{"reboot","bootloader"}},
					{ "fb",			2,COMMAND_ARGS_NONE 			,COMMAND_TYPE_ADB,{"reboot","bootloader"}},
					// samsung download mode
					{ "download",	2,COMMAND_ARGS_NONE				,COMMAND_TYPE_ADB,{"reboot","download"}}, 

								
					// logcat
					{ "lca",		9,COMMAND_ARGS_NONE 			,COMMAND_TYPE_ADB,{"logcat","-b","system","-b","radio","-b","events","-b","main" }},
					{ "lc",			1,COMMAND_ARGS_APPEND			,COMMAND_TYPE_ADB,{"logcat"}},
					{ "sh",			0,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{}},
					{ "s",			0,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{}},
					{ "ll",			2,COMMAND_ARGS_APPEND			,COMMAND_TYPE_SHELL,{"ls","-l"}},
					{ "l",			2,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"ls","-la"}},
					{ "lha",		2,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"ls","-lha"}},
					// mount helper
					{ "adb-remount",1,COMMAND_ARGS_NONE 			,COMMAND_TYPE_ADB,{"remount"}},
					{ "mount",		1,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"mount"}},
					{ "umount",		1,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"umount"}},
					{ "lspart",		1,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"mount | grep"}},
					{ "remount",	2,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"mount","-orw,remount"}},
					// native service stop and starters 
					{ "st",			1,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"start"}},
					{ "ks",			1,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"stop"}},
					{ "dmesg",		1,COMMAND_ARGS_NONE 			,COMMAND_TYPE_SHELL,{"dmesg"}},
					{ "uname",		2,COMMAND_ARGS_NONE 			,COMMAND_TYPE_SHELL,{"cat","proc/version"}},
					// property management helpers
					{ "gp",			1,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"getprop"}},
					{ "grp",		1,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"getprop | grep"}},
					{ "wp",			1,COMMAND_ARGS_NONE 			,COMMAND_TYPE_SHELL,{"watchprops"}},
					{ "sp",			1,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"setprop"}},
					{ "up",			7,COMMAND_ARGS_CONCAT			,COMMAND_TYPE_SHELL,{"sed -i -e 's/","1","=\\(.*\\)$/","1","=","2","/g' /system/build.prop"}},			
					{ "updateprop",	7,COMMAND_ARGS_CONCAT			,COMMAND_TYPE_SHELL,{"sed -i -e 's/","1","=\\(.*\\)$/","1","=","2","/g' /system/build.prop"}},			
					{ "du",			1,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"du"}},
					{ "df",			1,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"df"}},
					{ "dfh",		2,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"df","-h"}},
					{ "cat",		1,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"cat"}},
					{ "c",			1,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"cat"}},
					{ "echo",		1,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"echo"}},
					{ "ps",			1,COMMAND_ARGS_NONE				,COMMAND_TYPE_SHELL,{"ps"}},
					{ "mv",			1,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"mv"}},
					{ "cp",			1,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"cp"}},
					{ "rm",			1,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"rm"}},
					{ "id",			1,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"id"}},
					{ "wipe",		1,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"wipe"}},
					{ "svc",		1,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"svc"}},
					{ "service",	1,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"service"}},
					{ "rmf",		2,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"rm","-f"}},
					{ "symlink",	2,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"ln","-s"}},
					{ "netcfg",		1,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"netcfg"}},
					{ "touch",		1,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"touch"}},
					{ "lsusb",		1,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"lsusb"}},
					{ "mkdir",		2,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"mkdir","-p"}},
					{ "chmod",		1,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"chmod"}},
					{ "chown",		1,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"chown"}},
					{ "755",		2,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"chmod","755"}},
					// kernel module commands
					{ "lsmod",		1,COMMAND_ARGS_NONE 			,COMMAND_TYPE_SHELL,{"lsmod"}},
					{ "insmod",		1,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"insmod"}},
					{ "rmmod",		1,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"rmmod"}},
					// input events
					{ "gevt",		1,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"getevent"}},
					{ "sevt",		1,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"sendevent"}},
					
					// busybox
					{ "busybox",	1,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"busybox"}},
					{ "bb",			1,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"busybox"}},
					{ "bootimg-tools",	1,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"bootimg-tools"}},
					{ "bits",			1,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"bootimg-tools"}},
					{ "bitsup",			5,COMMAND_ARGS_CONCAT 			,COMMAND_TYPE_SHELL,{"bootimg-tools"," u ","1"," -f ","2"}},
					// archos specific device management.... because they're special
					{ "kdf",		1,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"kd_flasher"}},
					{ "updaterd",	1,COMMAND_ARGS_NONE 			,COMMAND_TYPE_SHELL,{"updaterd"}},
					{ "sde",		2,COMMAND_ARGS_NONE 			,COMMAND_TYPE_SHELL,{"reboot_into","sde"}},
					{ "into",		2,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"reboot_into","-s"}},
					{ "arec",		2,COMMAND_ARGS_NONE 			,COMMAND_TYPE_SHELL,{"reboot_into","recovery"}},
					{ "android",	2,COMMAND_ARGS_NONE 			,COMMAND_TYPE_SHELL,{"reboot_into","android"}},	
					{ "remountall",	9,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"mount","-o","rw,remount","/","&&","mount","-o","rw,remount","/system"}},
					{ "mnt",	1,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"mount"}},

					// write a local.prop --- handy temp rooting
					{ "temp-root",	1,COMMAND_ARGS_NONE				,COMMAND_TYPE_SHELL,{"echo 'ro.kernel.qemu=1'>/data/local.prop"}},
					{ "un-root",	1,COMMAND_ARGS_NONE				,COMMAND_TYPE_SHELL,{"echo '#'>/data/local.prop"}},
					// Input				
					{ "key",		2,COMMAND_ARGS_APPEND			,COMMAND_TYPE_SHELL,{"input","keyevent"}},
					{ "tw",			1,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"CLASSPATH=/system/framework/input.jar app_process /system/bin com.android.commands.input.Input text"}},
					{ "tap",		2,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"input","tap"}},
					{ "swipe",		2,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"input","swipe"}},
					{ "input",		1,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"input"}},
					
					// easy filesystem list directorys
					{ "lsapp",		3,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"ls","-l","/system/app"}}, 
					{ "lsbin",		3,COMMAND_ARGS_APPEND 			,COMMAND_TYPE_SHELL,{"ls","-l","/system/bin"}},
					
					// Activity Manager Startups
					{ "vending",	6,COMMAND_ARGS_NONE 			,COMMAND_TYPE_SHELL,{"am", "start","-a" ,"android.intent.action.MAIN","-n","com.android.vending/.AssetBrowserActivity"}},
					{ "settings",	6,COMMAND_ARGS_NONE 			,COMMAND_TYPE_SHELL,{"am", "start","-a" ,"android.intent.action.MAIN","-n","com.android.settings/.Settings"}}
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
		{ "SPACE","space","62"},
		{ "SPACE","spc","62"},
		{ "EXPLORER", "browser","64"},
		{ "ENTER","ent","66"},
		{ "DELETE","del","67"},
		{ "MENU","menu","82"},
		{ "UNLOCK","unlock","82"},
		{ "NOTIFICATION","systray","83"},
		{ "SEARCH","search","84"},
		{ "MEDIA_PLAY_PAUSE","mplay","85"},
		{ "MEDIA_STOP","mstop","86"},
		{ "MEDIA_NEXT","mnext","87"},
		{ "MEDIA_PREVIOUS","mprev","88"},
		{ "MEDIA_REWIND","mrw","89"},
		{ "MEDIA_FAST_FORWARD","mffw","90"},
		{ "MUTE","mute","91"},
		{ "PAGE_UP","pgup","92"},
		{ "PAGE_DOWN","pgdwm","93"},
		{ "APP_SWITCH","appsw","187"},
		{ "MANNER_MODE","manner","205"},
		{ "3D_MODE","3dmode", "206"},
		{ "CONTACTS","contacts","207"},
		{ "CALENDAR","calendar","208"},
		{ "MUSIC","music", "209"},
		{ "CALCULATOR","calc","210"}
};

static const int shortcut_total = ARRAYSIZE(shortcuts);
static const int keyevents_total = ARRAYSIZE(keyevents);
char *format_shortcut(int shortcut_index,int argc, char **argv);
int process_shortcut(int argc, char **argv,int *new_argc ,char ***new_argv);
int process_keyevent_chain(int argc, char **argv);
int process_ipaddress(int argc, char **argv,int *new_argc ,char ***new_argv);
int is_keyevent(char* test_string);
int is_shortcut(char* test_string);
int is_ipaddress(char *ipaddress);

int adb_extended_commandline(int argc , char** argv);
#endif
