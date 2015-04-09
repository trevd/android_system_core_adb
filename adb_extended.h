/*
 *
 *
 */

#ifndef __ADB_EXTENDED_H__
#define __ADB_EXTENDED_H__ 1

#ifndef __MAKE_KEYWORD_ENUM__
#define __MAKE_KEYWORD_ENUM__ 1
#define KEYWORD_SHELL(symbol,  ... ) KS_##symbol,
#define KEYWORD_ADB(symbol, ... ) KS_##symbol,
#define KEYWORD_COMPOUND(symbol,  ... ) K_##symbol,
enum {
    KS_UNKNOWN,
    KS_S,
#else // __MAKE_KEYWORD_ENUM defined

/*
 * PP_NARG a non-iterative macro to figure out the number of __VA_ARGS__
 * Borrowed from Laurent Deniau post to comp.std.c ( 2006-01-16 )
 * https://groups.google.com/forum/#!topic/comp.std.c/d-6Mj5Lko_s
*/
#define PP_NARG(...) PP_NARG_(__VA_ARGS__,PP_RSEQ_N())
#define PP_NARG_(...) PP_ARG_N(__VA_ARGS__)
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


#define KEYWORD_SHELL(symbol, ...) \
    [ KS_##symbol ] = { #symbol, PP_NARG(__VA_ARGS__)+1 ,{ "shell", __VA_ARGS__ } },

#define KEYWORD_ADB(symbol, ...) \
    [ KS_##symbol ] = { #symbol, PP_NARG(__VA_ARGS__) ,{ __VA_ARGS__ } },

#define MAX_ARGS 255

static struct keyword_shell_info_t {
    const char *name;
    int nargs;
    const char* command[MAX_ARGS] ;
} keyword_shell_info[KEYWORD_SHELL_COUNT] = {
    [ KS_UNKNOWN ] = { "unknown", 0, { NULL }},
    [ KS_S ] = { "s", 1, { "shell" }},
#endif // ifndef __MAKE_KEYWORD_ENUM__
    KEYWORD_SHELL(l,"ls","-Zl")
    KEYWORD_SHELL(lz,"ls","-Z")
    KEYWORD_SHELL(pz,"ps","-Z")
    KEYWORD_SHELL(mnt,"mount")
    KEYWORD_SHELL(gp,"getprop")
    KEYWORD_SHELL(grp,"getprop | grep")
    KEYWORD_SHELL(sp,"setprop")
    KEYWORD_SHELL(c,"cat")
    KEYWORD_ADB(pl,"pull")
    KEYWORD_ADB(pu,"push")
    KEYWORD_ADB(d,"devices","-l")
    KEYWORD_ADB(dl,"devices")
    KEYWORD_ADB(lca,"logcat","-b","system","-b","main","-b","events","-b","radio")
    KEYWORD_ADB(lc,"logcat")
    KEYWORD_ADB(lcs,"logcat","-b","system")
    KEYWORD_ADB(lcm,"logcat","-b","main")
    KEYWORD_ADB(lce,"logcat","-b","events")
    KEYWORD_ADB(lcr,"logcat","-b","radio")
    KEYWORD_ADB(recovery,"reboot","recovery")
    KEYWORD_ADB(download,"reboot","download")
    KEYWORD_ADB(rbtbl,"reboot-bootloader")
    KEYWORD_ADB(rbtbootloader,"reboot","bootloader")
    KEYWORD_ADB(rbtsl,"reboot","sideload")
    KEYWORD_ADB(rbtslauto,"reboot","sideload-auto-reboot")
    KEYWORD_ADB(fb,"reboot","bootloader")
    KEYWORD_ADB(rbt,"reboot")
    KEYWORD_ADB(bl,"reboot","bootloader")
    KEYWORD_ADB(rec,"reboot","recovery")
    KEYWORD_ADB(sl,"reboot","sideload")
    KEYWORD_ADB(slauto,"reboot","sideload-auto-reboot")
#ifndef __MAKE_KEYWORD_STRUCT__
    KEYWORD_SHELL_COUNT
#endif
};
#ifndef __MAKE_KEYWORD_STRUCT__
#undef KEYWORD_SHELL
#undef KEYWORD_ADB
#undef __ADB_EXTENDED_H__
#define __MAKE_KEYWORD_STRUCT__ 1
#include "adb_extended.h"
#include <limits.h>
#include <string.h>
int adb_extended_maybe_expand_args(int *argc ,const char ***argv);
#define KW_MATCH 0
#define kw_is(kw, type) (keyword_shell_info[kw].flags & (type))
#define kw_name(kw) (keyword_shell_info[kw].name)

#define kw_argc(kw) (keyword_shell_info[kw].nargs)
#define kw_command_index(kw,index) (keyword_shell_info[kw].command[index])
#define kw_command(kw) (keyword_shell_info[kw].command)

#endif

#endif
