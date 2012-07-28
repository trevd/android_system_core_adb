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
// Split a input string into a list return total
int strtolist(char* input, char*** outputlist)
{
        char* lineptr;
        if(strlen(input) == 0)
                return 0;
        int argc=0;
        char* full_input= (char*) malloc(strlen(input) * sizeof(char));
        //printf("input length:%d\n",strlen(input));
        strcpy(full_input,input);
        (*outputlist) = (char**) malloc(MAX_TOKS * sizeof(char**));
        do{
                // Sometimes the last remaining chars are 2 - tab and bkspace for some reason
                // a bit of an hack to handle to 
                if((strlen(full_input) ==2) && (get_ascii(full_input[0]) == 9) && (get_ascii(full_input[1]) == 8))
                        break;
                
                lineptr = strsep(&full_input,DELIMITERS);
                if(lineptr != NULL){
                        (*outputlist)[argc]=lineptr;
                        //print_ascii(full_input);
	                //printf("argc:%d %s Remaining:%d\n Full Input:%s:",argc,lineptr,strlen(full_input) ,full_input);
	        }       
	        argc++;
                if(strlen(full_input) == 0) break;
        }while((lineptr != NULL) || (strlen(full_input) != 0));  
        return argc;
        
}

