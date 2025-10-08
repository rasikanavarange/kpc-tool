// =============================================================================
// XNU kperf/kpc demo
// Available for 64-bit Intel/Apple Silicon, macOS/iOS, with root privileges
//
//
// Demo 1 (profile a function in current thread):
// 1. Open directory '/usr/share/kpep/', find your CPU PMC database.
//    M1 (Pro/Max/Ultra): /usr/share/kpep/a14.plist
//    M2 (Pro/Max):       /usr/share/kpep/a15.plist
//    M3:                 /usr/share/kpep/as1.plist
//    M3 (Pro/Max):       /usr/share/kpep/as3.plist
//    M4:                 /usr/share/kpep/as4.plist
// 2. Select a few events that you are interested in,
//    add their names to the `profile_events` array below.
// 3. Put your code in `profile_func` function below.
// 4. Compile and run with root (sudo).
//
//
// Demo 2 (profile a select process):
// Replace step 3 with: set `target_pid` and `total_profile_time`.
// Use main2() as the entry function.
//
//
// References:
//
// XNU source (since xnu 2422.1.72):
// https://github.com/apple/darwin-xnu/blob/main/osfmk/kern/kpc.h
// https://github.com/apple/darwin-xnu/blob/main/bsd/kern/kern_kpc.c
//
// Lightweight PET (Profile Every Thread, since xnu 3789.1.32):
// https://github.com/apple/darwin-xnu/blob/main/osfmk/kperf/pet.c
// https://github.com/apple/darwin-xnu/blob/main/osfmk/kperf/kperf_kpc.c
//
// System Private frameworks (since macOS 10.11, iOS 8.0):
// /System/Library/PrivateFrameworks/kperf.framework
// /System/Library/PrivateFrameworks/kperfdata.framework
//
// Xcode framework (since Xcode 7.0):
// /Applications/Xcode.app/Contents/SharedFrameworks/DVTInstrumentsFoundation.framework
//
// CPU database (plist files)
// macOS (since macOS 10.11):
//     /usr/share/kpep/<name>.plist
// iOS (copied from Xcode, since iOS 10.0, Xcode 8.0):
//     /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform
//     /DeviceSupport/<version>/DeveloperDiskImage.dmg/usr/share/kpep/<name>.plist
//
// Use this shell command to get plist file name for the current host:
// printf "cpu_%s_%s_%s.plist\n" $(sysctl -nx hw.cputype hw.cpusubtype hw.cpufamily) | sed -E 's/0x0*//g'
//
// Created by YaoYuan <ibireme@gmail.com> on 2021.
// Released into the public domain (unlicense.org).
// =============================================================================


int profile_process(const int target_pid, const double total_profile_time, const double sample_period);
