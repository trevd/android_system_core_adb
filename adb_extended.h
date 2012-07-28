#ifndef _ADB_EXTENDED_H_
#define _ADB_EXTENDED_H_
#define ADB_EXTENDED
#include "adb.h"

int process_input_command(int argc,char **argv);
int is_shell_executable(char* line) ;
int strtolist(char* input, char*** output_ptr);
//static char* read_shellcommand(transport_type transport, char* serial, char* buf);

//static int send_command(transport_type transport, char* serial,int argc, char** argv);
#endif 

