#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <math.h>
#include <ctype.h>
#include <assert.h>
#include "sysdeps.h"
#define TRACE_TAG TRACE_EXTENDED
#include "adb_extended.h" 
#include "input_keyevent_codes.h"
#include "file_sync_service.h"
#define DELIMITERS "\n"
#define TAB_DELIMITERS "\t"

#define MAX_STRING_LENGTH 512
#define MAX_TOKS 512
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
		if (newline) printf("\n");
	return 0;
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
// Split a input string into a list return total
int strtolist(char* input, char*** outputlist)
{
        char* lineptr;
        int argc=0;
        char* full_input= (char*) malloc(strlen(input) * sizeof(char));
        strcpy(full_input,input);
        (*outputlist) = (char**) malloc(MAX_TOKS * sizeof(char**));
        do{
                lineptr = strsep(&full_input,DELIMITERS);
                (*outputlist)[argc]=lineptr;
	        printf("argc:%d %s\n",argc,lineptr);
       	        argc++;
                if(strlen(full_input) == 0) break;
        }while((lineptr != NULL) || (strlen(full_input) != 0));  
        return argc;
        
}
int parse_string(char* line, char*** argv) {

  char* lineptr;  char* tabptr;
  int argc=0;
  char* full_input= (char*) malloc(strlen(line) * sizeof(char));
  strcpy(full_input,line);
  (*argv) = (char**) malloc(MAX_TOKS * sizeof(char**));
  do{
	  lineptr = strsep(&full_input,DELIMITERS);
	  tabptr = strsep(&lineptr,TAB_DELIMITERS);
	  (*argv)[argc]=tabptr;
	  argc++;
	  if(strlen(full_input) == 0) break;
	}while((lineptr != NULL) || (strlen(full_input) != 0));  
  return argc;
}

/*

int get_device_count(int argc, char **argv){
 	char **device_list=NULL;
	char *tmp = adb_query("host:devices");
       	int device_count = 0;
        if(tmp) device_count =parse_string(tmp,&device_list);
        return device_count;
}
char *select_device(int argc, char **argv){
	print_args("Starting select_device Command",argc,argv,0);
	int i =0 ; char **device_list=NULL;
	char *tmp = adb_query("host:devices");
       	int device_count = 0;
        if(tmp) device_count = parse_string(tmp,&device_list);
        int input_count =  device_count > 0 ? (int) log10 ((double) device_count) + 1 : 1;        
        printf("Select Device:[1-%d]\n",device_count);
	for (i = 0; i < device_count; ++i)
		printf("%d: %s\n",i+1,device_list[i]);
	int index=get_user_entry(input_count,device_count,argc,argv);
	char * serial = get_serial_from_index(index);
	//printf("Attempting to connect to device: %s\n",serial);
	print_args("finishing select_device Command",argc,argv,1);	
	return  serial;
	
}
char *get_serial_from_index(int index){
	char **device_list=NULL;
	char *tmp = adb_query("host:devices");
       	int device_count = 0;
        if(tmp) device_count =parse_string(tmp,&device_list);
       	if(index > device_count){
       		fprintf(stderr,"error: index out of range");
       		return NULL;
       		}
	if(index>0 && index<=device_count ) return device_list[index-1];
	return NULL;
}
*/
