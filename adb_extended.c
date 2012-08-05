#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <math.h>
#include <ctype.h>
#include <assert.h>
#include "sysdeps.h"
#define TRACE_TAG TRACE_EXTENDED
#include "adb_extended.h" 
#include "input_keyevent_codes.h"
#include "file_sync_service.h"
#include "adb_client.h"
#define DELIMITERS "\n"
#define TAB_DELIMITERS "\t"

#define MAX_STRING_LENGTH 512
#define MAX_TOKS 512
int run_play(transport_type transport, char* serial)
{
   char buf[256];
   snprintf(buf, sizeof(buf), "shell:am start -a android.intent.action.MAIN -n com.android.vending/.AssetBrowserActivity");
   send_shellcommand(transport, serial, buf);    
   return 0;
}
int run_settings(transport_type transport, char* serial)
{
    char buf[256];
        snprintf(buf, sizeof(buf), "shell:am start -a android.intent.action.MAIN -n com.android.settings/.Settings");
    send_shellcommand(transport, serial, buf);    
    return 0;
    
}
int is_server_control_command(char* argv)
{
    D("is_server_control_command: %s",argv);
    if(!strcmp(argv, "kill-server") || !strcmp(argv, "kill")) {
        return kill_server();
    }
    if (!strcmp(argv, "start-server") || !strcmp(argv, "start")) {
        return adb_connect("host:start-server");
    }
    if (!strcmp(argv, "restart-server") || !strcmp(argv, "restart")) {
        kill_server();
        sleep(2);
        return adb_connect("host:start-server");
    }
    return -1;
}

int get_device_list(char*** outputlist)
{
	char *tmp ;
	tmp = adb_query("host:devices");
       	int device_count = 0;
        device_count = strtolist(tmp,outputlist);
        return device_count;
}
char *get_serial_from_index(int index)
{
	char **device_list=NULL;
	char *tmp ;
	tmp = adb_query("host:devices");
       	int device_count = 0;
        if(tmp) device_count = get_device_list(&device_list);//strtolist(tmp,&device_list);
       	if(index > device_count){
       		fprintf(stderr,"error: index out of range\n");
       		return NULL;
       		}
	if(index>0 && index<=device_count ) {
	        char * lineptr = NULL;
        	lineptr = strsep(&device_list[index-1],"\t");       		
                return lineptr;
        }
	return NULL;
}
int is_adb_device_command(int argc,char* argv[])
{
        return -1;
}
int kill_server()
{
        int fd;
        fd = _adb_connect("host:kill");
        if(fd == -1) {
            fprintf(stderr,"* server not running *\n");
            return 1;
        }
        return 0;
}
int is_string_in_command_list(char *argv,char **cmd_list,int array_length,int step ){
	int return_value = -1;int counter = 0;
	array_length = array_length /sizeof(char *);

	for(counter=0;counter<array_length;counter+=step){
		D("Comparing=%s to %s\n",argv,cmd_list[counter]);
		if(!strcmp(argv,cmd_list[counter])) {		
			return_value= counter; 
			break;
			}
			
	}
	return return_value;
}
int process_input_command(int argc,char **argv){
	int command_string_index = is_string_in_command_list(argv[0],adb_input_keyevent_command_pairs,sizeof(adb_input_keyevent_command_pairs),2);
        D("argc:%d\tcommand_string_index:%d\n",argc,command_string_index);	
	if(command_string_index == -1) return command_string_index;
	if(command_string_index >= 0)  {
	int command_code_index =command_string_index+1;
        D("command_string_index:%d\n",command_string_index);	
	char* real_keycode=adb_input_keyevent_command_pairs[command_code_index];
        D("real_keycode:%s\n",real_keycode);	
		return atol(real_keycode) ;
	} return -1;
}
int print_args(char* function,int argc, char **argv,int newline)
{
	if (newline <2) return 0;
	int i;
	printf("%s argc:%d",function,argc);
	for (i = 0; i < argc; i++)
                printf(" argv[%d] = \"%s\" ", i, argv[i]);
		
		//do_if (newline) printf("\n");
	return 0;
}
void read_and_dump(int fd)
{
    char buf[4096];
    int len;

    while(fd >= 0) {
        D("read_and_dump(): pre adb_read(fd=%d)\n", fd);
        len = adb_read(fd, buf, 4096);
        D("read_and_dump(): post adb_read(fd=%d): len=%d\n", fd, len);
        if(len == 0) {
            break;
        }

        if(len < 0) {
            if(errno == EINTR) continue;
            break;
        }
        fwrite(buf, 1, len, stdout);
        fflush(stdout);
    }
}
int get_ascii(char single_char) {
  int ret =  (int)single_char; 
	  //printf("get_ascii:%c(%d)",single_char,ret);  
  return ret;
}
void print_ascii(char* buf) {
  int i=0 , len = strlen(buf);
  for(i=0;i<len;i++)
	  printf("ch:%c(%d)",(char)buf[i],(int)buf[i]);
  printf("\n");
}
int is_shell_executable(char* line) {

        char* path = getenv("PATH");
        D("Got PATH=%s\n",path);
        return 0;    
}

int send_shellcommand(transport_type transport, char* serial, char* buf)
{
    int fd, ret; 

    for(;;) {
        fd = adb_connect(buf);
        if(fd >= 0)
            break;
        fprintf(stderr,"- waiting for device -\n");
        adb_sleep_ms(1000);
        do_cmd(transport, serial, "wait-for-device", 0);
    }

    read_and_dump(fd);
    ret = adb_close(fd);
    if (ret)
        perror("close");

    return ret;
}
int do_shellcommand(transport_type transport, char* serial,char* shellcommand,char* argument)
{
    char buf[256];
    if ( argument == NULL )
            snprintf(buf, sizeof(buf), "shell:%s ",shellcommand);
    else
            snprintf(buf, sizeof(buf), "shell:%s%s ",shellcommand,argument);
    send_shellcommand(transport, serial, buf);    
    return 0;
}

int do_cmd(transport_type ttype, char* serial, char *cmd, ...)
{
    char *argv[16];
    int argc;
    va_list ap;

    va_start(ap, cmd);
    argc = 0;

    if (serial) {
        argv[argc++] = "-s";
        argv[argc++] = serial;
    } else if (ttype == kTransportUsb) {
        argv[argc++] = "-d";
    } else if (ttype == kTransportLocal) {
        argv[argc++] = "-e";
    }

    argv[argc++] = cmd;
    while((argv[argc] = va_arg(ap, char*)) != 0) argc++;
    va_end(ap);

#if 0
    int n;
    fprintf(stderr,"argc = %d\n",argc);
    for(n = 0; n < argc; n++) {
        fprintf(stderr,"argv[%d] = \"%s\"\n", n, argv[n]);
    }
#endif

    return adb_commandline(argc, argv);
}

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
// Split a input string into a list return total
int strtolist(char* input, char*** outputlist)
{
        char* lineptr;
        if(strlen(input) == 0)
                return 0;
        int argc=0;
        char* full_input= (char*) malloc(strlen(input) * sizeof(char));
        strcpy(full_input,input);
        (*outputlist) = (char**) malloc(MAX_TOKS * sizeof(char**));
        do{
                // Sometimes the last remaining chars are 2 - tab and bkspace for some reason ; a bit of an hack to handle it
                if((strlen(full_input) ==2) && (get_ascii(full_input[0]) == 9) && (get_ascii(full_input[1]) == 8))
                        break;
                
                lineptr = strsep(&full_input,DELIMITERS);
                if(lineptr != NULL)
                        (*outputlist)[argc]=lineptr;
	        argc++;
                if(strlen(full_input) == 0) break;
        }while((lineptr != NULL) || (strlen(full_input) != 0));  
        return argc;
        
}
int is_adb_command(int argc,char* argv[])
{
    /* "adb /?" is a common idiom under Windows */
    if(!strcmp(argv[0], "help") || !strcmp(argv[0], "/?")) {
        if(argc == 1)
                help();
        else {
                if(!strcmp(argv[1], "all"))
                        help_all();
                if(!strcmp(argv[1], "logcat"))
                        help_logcat();
               if(!strcmp(argv[1], "keycodes"))
                        help_keycodes();                        
        }
        
        return 0;
    }
    if(!strcmp(argv[0], "version") || !strcmp(argv[0], "ver")) {
        
        if(argc==1)
                version(stdout);
        else if(!strcmp(argv[1],"full"))
                fullversion(stdout);
                       
        return 0;
    }
    return -1;

}

