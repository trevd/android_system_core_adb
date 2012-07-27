#!/bin/bash
mm && pkill -9 adb && rm /media/android/sdk/platform-tools/adb && cp /media/android/build/android-4.1.1/out/host/linux-x86/bin/adb /media/android/sdk/platform-tools/  
adb kill-server
adb start-server

