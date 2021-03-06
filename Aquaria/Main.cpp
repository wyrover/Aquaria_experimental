/*
Copyright (C) 2007, 2010 - Bit-Blot

This file is part of Aquaria.

Aquaria is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "DSQ.h"
#include "LVPAFile.h"
#include "DemoContainerKeygen.h"


#ifdef BBGE_BUILD_WINDOWS
	#include <shellapi.h>
#endif


	void enumerateTest()
	{
#ifdef BBGE_BUILD_SDL
		SDL_Rect **modes;
		/* Get available fullscreen/hardware modes */
		modes=SDL_ListModes(NULL, SDL_FULLSCREEN|SDL_HWSURFACE);

#ifdef BBGE_BUILD_WINDOWS
		/* Check is there are any modes available */
		if(modes == (SDL_Rect **)0){
			MessageBox(0, "No modes available!\n", "", MB_OK);
			return;
		}

		/* Check if or resolution is restricted */
		if(modes == (SDL_Rect **)-1){
			MessageBox(0, "All resolutions available.\n", "", MB_OK);
		}
		else{
			/* Print valid modes */
			printf("Available Modes\n");
			for(int i=0;modes[i];++i){
				std::ostringstream os;
				os << "[" << modes[i]->w << "x" << modes[i]->h << "]";
				MessageBox(0, os.str().c_str(), "", MB_OK);
				//printf("  %d x %d\n", modes[i]->w, modes[i]->h);
			}
		}
#endif
#endif
	}

// may be used externally
const char *main_procname = NULL;

static void MakeRan(void)
{
#ifdef BBGE_BUILD_WINDOWS
    std::ofstream out("ran");
    for (int i = 0; i < 1; i++)
        out << rand()%1000;
    out.close();
#endif
}

static void StartAQConfig()
{
#if defined(BBGE_BUILD_WINDOWS) && defined(AQUARIA_WIN32_AQCONFIG)
#if defined(AQUARIA_DEMO) || defined(AQUARIA_FULL)
    if (!exists("ran", false, true))
    {
        MakeRan();
        if(exists("aqconfig.exe", false, true))
        {
            ShellExecute(NULL, "open", "aqconfig.exe", NULL, NULL, SW_SHOWNORMAL);
            exit(0);
        }
    }
#endif
    remove("ran");
#endif
}

static void CheckConfig(void)
{
#ifdef BBGE_BUILD_WINDOWS
    bool hasCfg = exists("usersettings.xml", false, true);
    bool hasContainer = exists("data.lvpa", false, true);
    // - if the config file is there, start aqconfig IF NECESSARY
    // - if its not there, we need to restore it, otherwise aqconfig will mess up the controls.
    //   since the file can be loaded from the container, the default settings will be restored
    //   automatically, and be back on next config save. No need to start aqconfig in that case.
    if(hasCfg || (!hasCfg && !hasContainer))
        StartAQConfig();
    
#endif
}

static bool InitContainerFile(void)
{
    debugLog("Init container file...");
    lvpa::LVPAFile *container = NULL;
    if(exists("data.lvpa", false, true))
        container = new lvpa::LVPAFile;
#ifdef AQUARIA_DEMO
    if(!container)
        return false;
    DemoKeygen_MakeKey(container);
#endif
    if(container && container->LoadFrom("data.lvpa")) // try to load the file
    {
        debugLog("OK, pre-init VFS...");
        dsq->vfs.LoadBaseContainer(container, true); // success, link into VFS. will auto-delete when VFS is deleted
    }
    else
    {
#ifdef AQUARIA_DEMO
        dsq->errorLog("Something is wrong with the data container, can't start.");
        return false;
#else
        if(container) // its there, but can't be loaded, weird
            dsq->errorLog("Something is wrong with the data container, trying to start anyways");
#endif
    }
    return true;
}


#if defined(BBGE_BUILD_WINDOWS) && defined(AQUARIA_WIN32_NO_SDLMAIN)
	int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
						HINSTANCE	hPrevInstance,		// Previous Instance
						LPSTR		lpCmdLine,			// Command Line Parameters
						int			nCmdShow)			// Window Show State
	{
		#ifdef _DEBUG
			_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); 
			_CrtSetReportMode ( _CRT_ERROR, _CRTDBG_MODE_DEBUG);
		#endif
        char *argv2 = "";
        char **argv = &argv2;
        std::string dsqParam = GetCommandLine();

#elif defined(BBGE_BUILD_SDL)

	extern "C" int main(int argc,char *argv[])
	{
        main_procname = argv[0];

		std::string dsqParam = ""; // fileSystem

#ifdef BBGE_BUILD_UNIX
		const char *envPath = getenv("AQUARIA_DATA_PATH");
		if (envPath != NULL)
			dsqParam = envPath;
#endif
	
#endif

        CheckConfig();

        {
            DSQ dsql(dsqParam);

            bool ok = InitContainerFile();

            #if !defined(_DEBUG) && defined(AQUARIA_DEMO)
            if(!ok)
                return 9;
            #endif

		    dsql.init();
		    //enumerateTest();
		    dsql.main();
		    dsql.shutdown();
        }

        MakeRan();

		return (0);
	}

