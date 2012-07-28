#!/bin/bash
echo "Building"
mm
ADB_PATH=`which adb`
if [  -z "$ADB_PATH" ] ; then
        ADB_PATH="/media/android/sdk/platform-tools/adb"
fi
if [ ! -z "$?" ] ; then
   echo "Killing"
   
   pkill -9 adb
   echo "Removing $ADB_PATH"
   rm $ADB_PATH
   echo "Copying  $ADB_PATH"
   cp /media/android/build/android-4.1.1/out/host/linux-x86/bin/adb $ADB_PATH
   adb kill-server
adb start-server

fi 


