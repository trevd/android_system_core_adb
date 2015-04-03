#ifndef KEYWORD
#define __MAKE_KEYWORD_ENUM__

/*static int do_shell(int argc , const char **argv);
static int do_adb(int argc ,  const char **argv);*/
#define KEYWORD(symbol, flags,func,nargs, ... ) K_##symbol,
#define KEYWORD_SHELL(symbol,  ... ) KS_##symbol,
#define KEYWORD_ADB(symbol, ... ) KS_##symbol,
#define KEYWORD_COMPOUND(symbol,  ... ) K_##symbol,
enum {
    KS_UNKNOWN,
    KS_S,
#endif


    KEYWORD_SHELL(lz,"ls","-Z")
    KEYWORD_SHELL(pz,"ps","-Z")
    KEYWORD_SHELL(mnt,"mount")
    KEYWORD_SHELL(gp,"getprop")
    KEYWORD_SHELL(sp,"getprop")
    KEYWORD_ADB(k,"devices","-l")
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




#ifdef __MAKE_KEYWORD_ENUM__
    KEYWORD_SHELL_COUNT,
};
#undef __MAKE_KEYWORD_ENUM__
#undef KEYWORD
#undef KEYWORD_SHELL
#undef KEYWORD_ADB
#undef KEYWORD_COMPOUND
#endif
//KEYWORD_SIMPLE(ver ,"version")
//    KEYWORD_SIMPLE(rec, "reboot","recovery")
