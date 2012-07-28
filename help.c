#include "help.h"
void help_switches(int full);
void version(FILE * out) {
    fprintf(out, "Android Debug Bridge version %d.%d.%d\n",
         ADB_VERSION_MAJOR, ADB_VERSION_MINOR, ADB_SERVER_VERSION);
}
void fullversion(FILE * out) {
    fprintf(out, "Android Debug Bridge Extended version %d.%d.%d.%d\n",
         ADB_VERSION_MAJOR, ADB_VERSION_MINOR, ADB_SERVER_VERSION,ADB_EXTENDED_VERSION);

}
void help_reboot(int full)
{
       if(full) 
                fprintf(stderr,

                        "  adb [reboot] <[bootloader|recovery]>         - reboots the device, optionally into the bootloader or recovery program\n"
                        "                                                 the reboot keyword is optional\n"
                        "  adb reboot-bootloader                        - reboots the device into the bootloader\n");
        else
                fprintf(stderr,
                        "  adb reboot [bootloader|recovery]            - reboots the device, optionally into the bootloader or recovery program\n"
                        "  adb reboot-bootloader                       - reboots the device into the bootloader\n");
}
void help_switches(int full)
{
        fprintf(stderr,
                        " -d                                            - directs command to the only connected USB device\n"
                        "                                                 returns an error if more than one USB device is present.\n"
                        " -e                                            - directs command to the only running emulator.\n"
                        "                                                 returns an error if more than one emulator is running.\n"
                        " -s <serial number>                            - directs command to the USB device or emulator with\n"
                        "                                                 the given serial number. Overrides ANDROID_SERIAL\n"
                        "                                                 environment variable.\n"
                        " -p <product name or path>                     - simple product name like 'sooner', or\n"
                        "                                                 a relative/absolute path to a product\n"
                        "                                                 out directory like 'out/target/product/sooner'.\n"
                        "                                                 If -p is not specified, the ANDROID_PRODUCT_OUT\n"
                        "                                                 environment variable is used, which must\n"
                        "                                                 be an absolute path.\n");
        if(full) 
                fprintf(stderr,
                        " -i <device index>                             - directs command to the device or emulator with\n"
                        "                                                 the given index number. The index number is defined\n"
                        "                                                 by the order in which the devices were connected.\n"
                        "                                                 Overrides ANDROID_INDEX environment variable\n");

        
}
void help_adb_commands(int full)
{
        
        if(full) 
                fprintf(stderr,
                        " devices | dev                                 - list all connected devices. using dev will\n"
                        "                                                 list all connected devices as an numbered list\n"
                        "                                                 The device index can be used with the -i option\n"
                        " connect | conn  <host>[:<port>]               - connect to a device via TCP/IP\n"
                        "                                                 Port 5555 is used by default if no port number is specified.\n"
                        " disconnect | discon [<host>[:<port>]]         - disconnect from a TCP/IP device.\n"
                        "                                                 Port 5555 is used by default if no port number is specified.\n"
                        "                                                 Using this command with no additional arguments\n"
                        "                                                 will disconnect from all connected TCP/IP devices.\n"
                        "\n" );
        else
                fprintf(stderr,
                        " devices                                       - list all connected devices\n"
                        " connect <host>[:<port>]                       - connect to a device via TCP/IP\n"
                        "                                                 Port 5555 is used by default if no port number is specified.\n"
                        " disconnect [<host>[:<port>]]                  - disconnect from a TCP/IP device.\n"
                        "                                                 Port 5555 is used by default if no port number is specified.\n"
                        "                                                 Using this command with no additional arguments\n"
                        "                                                 will disconnect from all connected TCP/IP devices.\n"
                        "\n" );

}
void help_sync_commands(int full)
{
        if(full) 
                fprintf(stderr,
                        "  adb push | pu <local> <remote>               - copy file/dir to device. If no remote\n"
                        "                                                 directory is specified then the root (\"/\")\n"
                        "                                                 direcory is used\n"                                       
                        "  adb pull | pl <remote> [<local>]             - copy file/dir from device. If no local\n"
                        "                                                 file is specified then the current\n"
                        "                                                 working direcory is used\n"        
                        "  adb sync [ <directory> ]                     - copy host->device only if changed\n"
                        "                                                 (-l means list but don't copy)\n"
                        "                                                 (see 'sync notes')\n"
                        "  adb shell | sh                               - run remote shell interactively\n"
                        "  adb shell | sh <command>                     - run remote shell command\n"
                        "  adb emu <command>                            - run emulator console command\n");
        else
                fprintf(stderr,
                        "  adb push <local> <remote>                    - copy file/dir to device\n"
                        "  adb pull <remote> [<local>]                  - copy file/dir from device\n"
                        "  adb sync [ <directory> ]                     - copy host->device only if changed\n"
                        "                                                 (-l means list but don't copy)\n"
                        "                                                 (see 'adb help all')\n"
                        "  adb shell                                    - run remote shell interactively\n"
                        "  adb shell <command>                          - run remote shell command\n"
                        "  adb emu <command>                            - run emulator console command\n");
        
}
void help_keycodes()
{
}
void help_input(int full)
{
        if(full)
                fprintf(stderr,"input commands:\n"
                "  - You can send keyevents by name to a connected device simply by running\n"
                "       adb <keycode_name>\n"
                "  - This is the equivalent of adb shell input keyevent <keyevent code>\n"
                "    All keyevents begin with the KEYCODE prefix. Some select keyevents have\n"
                "    short versions, for a full list enter\n"
                "       adb help keycodes\n" ) ;
  
}
void help_logcat(int full)
{
        if(full) 
                fprintf(stderr,
                        "  adb logcat | lc [ <filter-spec> ]            - View device log\n"
                        "  adb longcat [ <filter-spec> ]                - View verbose device log. Events are tagged\n"
                        "                                                 date and time\n"
                        "  adb logcat-all | lca [ <filter-spec> ]       - View all device logs. This is the equivalent\n"
                        "                                                 adb logcat -b radio -b system -b events -b main\n"
                        "  adb logcat-main | lcm [ <filter-spec> ]      - View device main log only\n"
                        "  adb logcat-radio | lcr [ <filter-spec> ]     - View device radio log only\n"
                        "  adb logcat-events | lce [ <filter-spec> ]    - View device events log\n"
                        "  adb logcat-system | lcs [ <filter-spec> ]    - View device syetem log only\n"
                        "  adb logcat-help | lch                        - View help information for logcat command\n"
                        "  adb dmesg                                    - Print the kernel ring buffer. This is the equivalent\n"
                        "                                                 adb shell dmesg\n"
                );
        else
                fprintf(stderr,
                        "  adb logcat [ <filter-spec> ]                 - View device log\n"
                );
}
void help_enviroment_variables(int full)
{
        fprintf(stderr,"\n"
                "environmental variables:\n" 
                "  ADB_TRACE                                    - Print debug information. A comma separated list of the following values\n"
                "                                                 1 or all, adb, sockets, packets, rwx, usb, sync, sysdeps, transport, jdwp\n"
                "  ANDROID_SERIAL                               - The serial number to connect to. -s takes priority over this if given.\n"
                "  ANDROID_LOG_TAGS                             - When used with the logcat option, only these debug tags are printed.\n");

        if(full) 
                fprintf(stderr,
                        "  ANDROID_INDEX                                - The index number to connect to. -i takes priority over this if given.\n");
                
                        
}
void help_help(int full)
{
        if(full) 
                fprintf(stderr,
                        "  adb help                                     - show short help message\n"
                        "  adb help all                                 - show this help message\n"
                        "  adb version | ver [ full ]                   - show version number. version full will print the\n"
                        "                                                 extended version number\n"
                );
        else                  
                fprintf(stderr,      
                        "  adb help                                     - show this help message\n"
                        "  adb help all                                 - show full extended version help message\n"
                        "  adb version                                  - show version num\n");


}
void help_main(int full)
{
    fprintf(stderr,"\n");
    help_switches(full);
    help_adb_commands(full);
    fprintf(stderr,"device commands:\n");
    help_sync_commands(full);
    help_logcat(full);
    fprintf(stderr,
                        "  adb forward <local> <remote>                 - forward socket connections\n"
                        "                                                 forward specs are one of: \n"
                        "                                                 tcp:<port>\n"
                        "                                                 localabstract:<unix domain socket name>\n"
                        "                                                 localreserved:<unix domain socket name>\n"
                        "                                                 localfilesystem:<unix domain socket name>\n"
                        "                                                 dev:<character device name>\n"
                        "                                                 jdwp:<process pid> (remote only)\n"
                        "  adb jdwp                                     - list PIDs of processes hosting a JDWP transport\n"
                        "  adb install [-l] [-r] [-s] [--algo <algorithm name> --key <hex-encoded key> --iv <hex-encoded iv>] <file>\n"
                        "                                               - push this package file to the device and install it\n"
                        "                                                 ('-l' means forward-lock the app)\n"
                        "                                                 ('-r' means reinstall the app, keeping its data)\n"
                        "                                                 ('-s' means install on SD card instead of internal storage)\n"
                        "                                                 ('--algo', '--key', and '--iv' mean the file is encrypted already)\n"
                        "  adb uninstall [-k] <package> - remove this app package from the device\n"
                        "                                                 ('-k' means keep the data and cache directories)\n"
                        "  adb bugreport                                - return all information from the device\n"
                        "                                                 that should be included in a bug report.\n"
                        "\n"
                        "  adb backup [-f <file>] [-apk|-noapk] [-shared|-noshared] [-all] [-system|-nosystem] [<packages...>]\n"
                        "                                               - write an archive of the device's data to <file>.\n"
                        "                                                 If no -f option is supplied then the data is written\n"
                        "                                                 to \"backup.ab\" in the current directory.\n"
                        "                                                 (-apk|-noapk enable/disable backup of the .apks themselves\n"
                        "                                                 in the archive; the default is noapk.)\n"
                        "                                                 (-shared|-noshared enable/disable backup of the device's\n"
                        "                                                 shared storage / SD card contents; the default is noshared.)\n"
                        "                                                 (-all means to back up all installed applications)\n"
                        "                                                 (-system|-nosystem toggles whether -all automatically includes\n"
                        "                                                   system applications; the default is to include system apps)\n"
                        "                                                 (<packages...> is the list of applications to be backed up.  If\n"
                        "                                                  the -all or -shared flags are passed, then the package\n"
                        "                                                  list is optional.  Applications explicitly given on the\n"
                        "                                                  command line will be included even if -nosystem would\n"
                        "                                                  ordinarily cause them to be omitted.)\n"
                        "\n"
                        "  adb restore <file>                           - restore device contents from the <file> backup archive\n"
                        "\n");
                        help_help(full);
                        fprintf(stderr,"\n"
                        "scripting:\n"
                        "  adb wait-for-device                          - block until device is online\n"
                        "  adb start-server                             - ensure that there is a server running\n"
                        "  adb kill-server                              - kill the server if it is running\n"
                        "  adb get-state                                - prints: offline | bootloader | device\n"
                        "  adb get-serialno                             - prints: <serial-number>\n"
                        "  adb status-window                            - continuously print device status for a specified device\n"
                        "  adb remount                                  - remounts the /system partition on the device read-write\n");
                        help_reboot(full);
                        fprintf(stderr,
                        "  adb root                                     - restarts the adbd daemon with root permissions\n"
                        "  adb usb                                      - restarts the adbd daemon listening on USB\n"
                        "  adb tcpip <port>                             - restarts the adbd daemon listening on TCP on the specified port\n");
                        if(full)fprintf(stderr,"\n");                      
                        fprintf(stderr,"networking:\n"
                        "  adb ppp <tty> [parameters]   - Run PPP over USB.\n"
                        " Note: you should not automatically start a PPP connection.\n"
                        " <tty> refers to the tty for PPP stream. Eg. dev:/dev/omap_csmi_tty1\n"
                        " [parameters] - Eg. defaultroute debug dump local notty usepeerdns\n"
                        "\n"
                        "adb sync notes: adb sync [ <directory> ]\n"
                        "  <localdir> can be interpreted in several ways:\n"
                        "\n"
                        "  - If <directory> is not specified, both /system and /data partitions will be updated.\n"
                        "\n"
                        "  - If it is \"system\" or \"data\", only the corresponding partition\n"
                        "    is updated.\n"
                        "\n"
                        );
                        help_input(full);
                        help_enviroment_variables(full);
}
void help_all()
{
   fullversion(stderr);
   help_main(1);

}
void help()
{
    version(stderr);
    help_main(0);
}
int usage()
{
    help();
    return 1;
}

