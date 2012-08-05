#ifndef _ADB_EXTENDED_H_
#define _ADB_EXTENDED_H_
#define ADB_EXTENDED
#include "adb.h"
int run_play(transport_type transport, char* serial);
int run_settings(transport_type transport, char* serial);
char *get_serial_from_index(int index);
int process_input_command(int argc,char **argv);
int is_shell_executable(char* line) ;
int do_cmd(transport_type ttype, char* serial, char *cmd, ...); 
int send_shellcommand(transport_type transport, char* serial, char* buf);
int do_shellcommand(transport_type transport, char* serial,char* shellcommand,char* argument);
int strtolist(char* input, char*** output_ptr);
int is_ipaddress(char* ipaddress);
int is_adb_command(int argc,char* argv[]);
void read_and_dump(int fd);
int kill_server();
int is_server_control_command(char* argv);
int get_device_list(char*** outputlist);
int is_adb_device_command(int argc,char* argv[]);
char *gProductOutPath;
#endif 

