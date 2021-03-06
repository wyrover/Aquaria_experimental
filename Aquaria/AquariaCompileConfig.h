#ifndef __AQUARIA_COMPILE_CONFIG_H__
#define __AQUARIA_COMPILE_CONFIG_H__

//#define AQUARIA_FULL 1
//#define AQUARIA_DEMO 1
#define AQUARIA_BUILD_CONSOLE 1
#define AQUARIA_BUILD_SCENEEDITOR 1

#define AQUARIA_CUSTOM_BUILD_ID "-experimental-[08Jan2012] [fgenesis]"

// no console window in release mode (note: use together with linker SubSystem setting)
// (FG: This setting is deprecated. However, it should work if linking SDLmain causes trouble for some reason)
//#ifdef NDEBUG
//#  define AQUARIA_WIN32_NO_SDLMAIN
//#endif

// start Aqconfig.exe if unclean shutdown or first start detected
#define AQUARIA_WIN32_AQCONFIG

#define AQUARIA_BUILD_MAPVIS

// Define this to save map visited data in a base64-encoded raw format.
// This can take much less space than the standard text format (as little
// as 10%), but WILL BE INCOMPATIBLE with previous builds of Aquaria --
// the visited data will be lost if the file is loaded into such a build.
// (Current builds will load either format regardless of whether or not
// this is defined.)
//#define AQUARIA_SAVE_MAPVIS_RAW

// Interesting, old test stuff
//#define AQ_TEST_QUADTRAIL

#endif
