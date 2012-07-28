#include <stdio.h>
#define ADB_VERSION_MAJOR 1         // Used for help/version information
#define ADB_VERSION_MINOR 0         // Used for help/version information
#define ADB_SERVER_VERSION    29    // Increment this when we want to force users to start a new adb server
#define ADB_EXTENDED_VERSION 0
void version(FILE * out);
void fullversion(FILE * out);
void help_input();
void help_logcat();
void help_all();
void help();
int usage();

