#define  TRACE_TAG  TRACE_ADB
#include <stdio.h>
#include <string.h>

#define ARRAYSIZE(a) ((int)(sizeof(a) / sizeof(*(a))))

struct command_shortcut {
		char * short_version;
		char * full_version;
} shortcuts[]  = { { "dev","devices"},
					{ "sh","shell"} } ;
static const int shortcut_total = ARRAYSIZE(shortcuts);
int print_args(int argc, char **argv); 
int is_shortcut(int argc, char **argv,int *new_argc ,char ***new_argv);

// Cheeky Debug function 
int print_args(int argc, char **argv)
{
	int counter = 0;
	printf("argc:%d ",argc);	
	for(counter = 0 ; counter < argc;counter++){
		printf("argv[%d]:%s ",counter,argv[counter]);	
	}
	printf("\n");
	return 0;
}
//
int is_shortcut(int argc, char **argv,int *new_argc ,char ***new_argv){
	// 
	int counter = 0 ; int shortcut_index = -1 ;
	for(counter = 0 ; counter < shortcut_total ; counter++){
		if(!strncmp(argv[0],shortcuts[counter].short_version,strlen(argv[0]))){
			shortcut_index = counter;
			break;				
		}
	}
	if(shortcut_index != -1)
	{
		printf("Shortcut Found %d %s\n",shortcut_index,shortcuts[shortcut_index].full_version);
		// build our argc and argv 		
		*new_argc = argc;
		(*new_argv) = (char**) malloc(argc * sizeof(char**));
		(*new_argv) [0] = shortcuts[shortcut_index].full_version;
		for(counter = 1 ; counter < argc;counter++){
			(*new_argv)[counter] = argv[counter];
		}
		print_args(*new_argc,(*new_argv));
		return 1;
	}
	return 0;
}


int adb_extended_commandline(int argc, char **argv,int *new_argc ,char ***new_argv){
	
	print_args(argc,argv);
	if(is_shortcut(argc,argv,new_argc,new_argv))
		return 0;	

	if(!strncmp(argv[0],"dmesg",strlen(argv[0]))){
		(*new_argv) = (char**) malloc(2 * sizeof(char**));
		(*new_argv) [0] = "shell";
		printf("dmesg\n");	
		(*new_argv) [1] = "dmesg";
		*new_argc = 2;
		//printf("argc:%d argv[0]:%s\n",argc,(*new_argv)[1]);
	}else{
		*new_argc = argc;
		*new_argv = argv;
	}
	print_args(*new_argc,(*new_argv));
	
	return 0;
}


