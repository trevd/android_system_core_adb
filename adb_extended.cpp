#define  TRACE_TAG  TRACE_EXT

#include "sysdeps.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>



#include "adb.h"
#include "adb_client.h"

#define ADB 0x01
#define SHELL 0x02
#define COMPOUND  0x04


#define PP_NARG(...) \
    PP_NARG_(__VA_ARGS__,PP_RSEQ_N())
#define PP_NARG_(...) \
    PP_ARG_N(__VA_ARGS__)
#define PP_ARG_N( \
     _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
    _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
    _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
    _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
    _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
    _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
    _61,_62,_63,  N, ...) N
#define PP_RSEQ_N() \
    63,62,61,60,                   \
    59,58,57,56,55,54,53,52,51,50, \
    49,48,47,46,45,44,43,42,41,40, \
    39,38,37,36,35,34,33,32,31,30, \
    29,28,27,26,25,24,23,22,21,20, \
    19,18,17,16,15,14,13,12,11,10, \
     9, 8, 7, 6, 5, 4, 3, 2, 1, 0


#include "adb_extended.h"

#define KEYWORD(symbol, flags,func,nargs, ...) \
	[ K_##symbol ] = { #symbol, flags,do_##func, nargs ,{ __VA_ARGS__ } },


#define KEYWORD_SHELL(symbol, ...) \
    [ KS_##symbol ] = { #symbol, PP_NARG(__VA_ARGS__)+1 ,{ "shell", __VA_ARGS__ } },

#define KEYWORD_ADB(symbol, ...) \
    [ KS_##symbol ] = { #symbol, PP_NARG(__VA_ARGS__) ,{ __VA_ARGS__ } },

#define KEYWORD_COMPOUND(symbol, ...) \
    [ K_##symbol ] = { #symbol, COMPOUND,do_##symbol,PP_NARG(__VA_ARGS__) ,{ __VA_ARGS__ } },


#define MAX_ARGS 255

static struct keyword_shell_info_t {
    const char *name;
    int nargs;
    const char* command[MAX_ARGS] ;
} keyword_shell_info[KEYWORD_SHELL_COUNT] = {
    [ KS_UNKNOWN ] = { "unknown", 0, { NULL }},
    [ KS_S ] = { "s", 1, { "shell" }},
#include "adb_extended.h"
};

#define SHELL_COMMAND_FOUND 1
#define SHELL_COMMAND_NOT_FOUND KS_UNKNOWN

/*
static struct keyword_info_t {
    const char *name;
    unsigned char flags;
    int (*func)(int nargs, const char **args);
    int nargs;
    const char* command[MAX_ARGS] ;
} keyword_info[KEYWORD_COUNT] = {
    [ K_UNKNOWN ] = { "unknown", 0, 0, 0 },
#include "adb_extended.h"
};
#undef KEYWORD
#undef KEYWORD_SHELL
#undef KEYWORD_SIMPLE
#undef KEYWORD_COMPOUND
*/
#define ks_is(kw, type) (keyword_shell_info[kw].flags & (type))
#define ks_name(kw) (keyword_shell_info[kw].name)

#define ks_nargs(kw) (keyword_shell_info[kw].nargs)
#define ks_command_index(kw,index) (keyword_shell_info[kw].command[index])
#define ks_command(kw) (keyword_shell_info[kw].command)

/*
static int do_adb(int argc , const char **argv)
{
	return 0;
}

static int do_shell(int argc , const char **argv)
{
	printf("do_shell argc=%d\n",argc);

	return 0;
}
*/

static const char** create_new_shell_commands_args(int argc_a,const char **argv_a, int argc_b , const char **argv_b)
{

	int nargc = argc_a +argc_b ;
	//D("ks_nargs(%d)=%d argc=%d nargc=%d\n",keyword_index,ks_nargs(keyword_index),argc,nargc );
	char** newargv = (char**) calloc(nargc,sizeof(char*));
	size_t args_a_size = argc_a*sizeof(char*) ;
	size_t args_b_size = argc_b*sizeof(char*) ;
	char* p = (char*)memcpy(newargv,argv_a,args_a_size);
	//D("newargv=%p p=%p ks_args_size=%zu args_size=%zu\n",newargv , p, ks_args_size,args_size );
	p+= args_a_size;
	memcpy(p,argv_b,args_b_size);

	return const_cast<const char**> (newargv);



}
static int handle_adb_known_shell_commandline(int keyword_index,int argc , const char **argv)
{

	for(int a = 0 ; a < argc ; a++){
		D("argv[%d]=%s\n",a,argv[a]);

	}

	int newargc =  argc + ks_nargs(keyword_index);
	const char** newargv = create_new_shell_commands_args(ks_nargs(keyword_index),ks_command(keyword_index),argc,argv);
	D("newargc=%d\n",newargc);
	for(int a = 0 ; a < newargc ; a++){
		D("newargv[%d]=%s\n",a,newargv[a]);

	}
	int ret = adb_commandline(newargc,newargv);
	D("ret=%d\n",ret);
	free(newargv);
	return 1 ;

}

static int is_known_shell_command(int argc , const char **argv)
{

	for ( int kw = KS_UNKNOWN ; kw < KEYWORD_SHELL_COUNT ; kw++){
		if(!strncmp(argv[0],ks_name(kw),PATH_MAX)){
			D("ks_name=%s ks_nargs=%d\n",ks_name(kw),ks_nargs(kw));
			return kw;

		}
	}


	return KS_UNKNOWN;


}
static int is_shell_command(int argc , const char **argv)
{
	int stdout_save;
	char buffer[PATH_MAX];

	//fflush(stdout); //clean everything first
	stdout_save = dup(STDOUT_FILENO); //save the stdout state
	freopen("/dev/null", "a", stdout); //redirect stdout to null pointer
	setvbuf(stdout, buffer, _IOFBF, 1024); //set buffer to stdout
	const char* newargv[3] = { "shell","which",argv[0] };
		for(int a = 0 ; a < 3 ; a++){
		D("newargv[%d]=%s\n",a,newargv[a]);

	}
	adb_commandline(3,newargv);
	freopen("/dev/null", "a", stdout); //redirect stdout to null again
	dup2(stdout_save, STDOUT_FILENO); //restore the previous state of stdout
	setbuf(stdout, NULL); //disable buffer to print to screen instantly
	//fprintf(stderr,"buf=%s %zu\n",buffer,strlen(buffer));
	if ( strlen(buffer) > 0 ){
		return SHELL_COMMAND_FOUND;
	}
	return SHELL_COMMAND_NOT_FOUND;

}


int adb_extended_commandline(int argc , const char **argv)
{
	D("argc=%d argv=%s\n",argc,argv[0]);

	if ( argc == 0 ){
		return 0;
	}
	if ( argv[0] == NULL ){
		return 0;
	}

	// check for known shell command shortcuts
	int ks =  is_known_shell_command(argc,argv) ;
	if (ks != KS_UNKNOWN ){
		argv++; argc--;
		return handle_adb_known_shell_commandline(ks,argc,argv);
	}
	// check if the command is in the PATH
	ks = is_shell_command(argc,argv);
	if ( ks != SHELL_COMMAND_NOT_FOUND ){

		return handle_adb_known_shell_commandline(KS_S,argc,argv);
	}

	D("KEYWORD_COUNT %d\n",KEYWORD_SHELL_COUNT);

	return 0;

}
