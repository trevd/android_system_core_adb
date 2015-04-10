#define TRACE_TAG TRACE_ADB

#include "sysdeps.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "adb_extended.h"

static int adb_extended_maybe_expand_shell_command(int *argc ,const char ***argv)
{
	return 0;
}
static int adb_extended_maybe_expand_keyword(int *argc ,const char ***argv)
{
	for ( int kw = KS_UNKNOWN ; kw < KEYWORD_SHELL_COUNT ; kw++){
		if(strncmp(kw_name(kw),(*argv)[0],PATH_MAX) == KW_MATCH ){
			int keyword_argc = kw_argc(kw);
			/* new_argc is the argc total related to the matched keyword ( keyword_argc)
			   plus the current argc as we append any additional args to the end of the line
			   we disgread the matched argument which is substituded out ( -1 )
			*/
			int new_argc = keyword_argc + (*argc)-1;
			/* allocate the memory required to hold the new argv list */
			char** new_argv = (char**)calloc(new_argc,sizeof(char*));
			if ( new_argv == NULL  && ( errno != 0 ) ){
				/* Memory allocation failed */
				return -1 ;
			}

			for (int c = 0; c < keyword_argc ; c++ ){
				// allocate the memory to hold the command string
				size_t arg_len = strnlen(kw_command_index(kw,c),PATH_MAX);
				new_argv[c] = (char*)calloc(arg_len,sizeof(char*));
				if ( ( new_argv[c] == NULL ) && ( errno != 0 )){
					/* Memory allocation failed */
					free(new_argv);
					return -1 ;
				}
				// copy to command into the
				if ( memcpy(new_argv[c],kw_command_index(kw,c),arg_len) != new_argv[c] ){
					free(new_argv[c]);
					free(new_argv);
					return -1 ;
				}
			}

			if ( (*argc) > 1 ) {
				/* append any additional shell variables */
				int argv_index = 1 ;
				for ( int c= keyword_argc ; c < new_argc ; c++ ){
					size_t arg_len = strnlen((*argv)[argv_index],PATH_MAX);
					new_argv[c] = (char*)calloc(arg_len,sizeof(char*));
					if ( ( new_argv[c] == NULL ) && ( errno != 0 )){
						/* Memory allocation failed */
						free(new_argv);
						return -1 ;
					}
					if ( memcpy(new_argv[c],(*argv)[argv_index],arg_len)!= new_argv[c] ){
						free(new_argv[c]);
						free(new_argv);
						return -1 ;
					}
					argv_index++ ;
				}
			}
			(*argv)= (const char**)new_argv;
			(*argc)= new_argc;
			return 1;

		}
	}
    return 0 ;
}
int adb_extended_maybe_expand_args(int *argc ,const char ***argv)
{
	if ( ( argc == 0 ) || ( argv == NULL ) || ( (*argv) == NULL ) || ( (*argv)[0] == NULL ) ){
		return -1;
	}
	int expanded = adb_extended_maybe_expand_keyword(argc,argv) ;
	if ( expanded == 1 ){
		return expanded;
	}
	expanded = adb_extended_maybe_expand_shell_command(argc,argv) ;
	if ( expanded == 1 ){
		return expanded;
	}
	return expanded;
}
