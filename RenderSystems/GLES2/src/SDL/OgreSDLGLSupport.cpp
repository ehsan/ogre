#include "OgreException.h"
#include "OgreLogManager.h"
#include "OgreStringConverter.h"
#include "OgreGLES2RenderSystem.h"

#include "OgreSDLGLSupport.h"

#include "OgreSDLWindow.h"

using namespace Ogre;

SDLGLSupport::SDLGLSupport()
{

  LogManager::getSingleton().logMessage("\tGLSupport ctor called");
    SDL_Init(SDL_INIT_VIDEO);
}

SDLGLSupport::~SDLGLSupport()
{
}

void SDLGLSupport::addConfig(void)
{
    SDL_Rect** videoModes = SDL_ListModes(NULL, SDL_FULLSCREEN | SDL_OPENGL);
    
    if (videoModes == (SDL_Rect **)0)
    {
        OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, "Unable to load video modes",
                "SDLRenderSystem::initConfigOptions");
    }

    ConfigOption optFullScreen;
    ConfigOption optVideoMode;
    ConfigOption optFSAA;
	ConfigOption optRTTMode;
#ifdef RTSHADER_SYSTEM_BUILD_CORE_SHADERS
	ConfigOption optEnableFixedPipeline;
#endif

    // FS setting possibilities
    optFullScreen.name = "Full Screen";
    optFullScreen.possibleValues.push_back("Yes");
    optFullScreen.possibleValues.push_back("No");
    optFullScreen.currentValue = "Yes";
    optFullScreen.immutable = false;

    // Video mode possibilities
    optVideoMode.name = "Video Mode";
    optVideoMode.immutable = false;
    if (videoModes != (SDL_Rect**)-1) { // All resolutions
      for (size_t i = 0; videoModes[i]; i++)
      {
          char szBuf[16];
                  snprintf(szBuf, 16, "%d x %d", videoModes[i]->w, videoModes[i]->h);
          optVideoMode.possibleValues.push_back(szBuf);
          // Make the first one default
          if (i == 0)
          {
              optVideoMode.currentValue = szBuf;
          }
      }
    } else {
      optVideoMode.possibleValues.push_back("640 x 480");
      optVideoMode.currentValue = "640 x 480";
    }
    
    //FSAA possibilities
    optFSAA.name = "FSAA";
    optFSAA.possibleValues.push_back("0");
    optFSAA.possibleValues.push_back("2");
    optFSAA.possibleValues.push_back("4");
    optFSAA.possibleValues.push_back("6");
    optFSAA.currentValue = "0";
    optFSAA.immutable = false;

	optRTTMode.name = "RTT Preferred Mode";
	optRTTMode.possibleValues.push_back("FBO");
	optRTTMode.possibleValues.push_back("PBuffer");
	optRTTMode.possibleValues.push_back("Copy");
	optRTTMode.currentValue = "FBO";
	optRTTMode.immutable = false;

#ifdef RTSHADER_SYSTEM_BUILD_CORE_SHADERS
		optEnableFixedPipeline.name = "Fixed Pipeline Enabled";
		optEnableFixedPipeline.possibleValues.push_back( "Yes" );
		optEnableFixedPipeline.possibleValues.push_back( "No" );
		optEnableFixedPipeline.currentValue = "Yes";
		optEnableFixedPipeline.immutable = false;
#endif

    mOptions[optFullScreen.name] = optFullScreen;
    mOptions[optVideoMode.name] = optVideoMode;
    mOptions[optFSAA.name] = optFSAA;
	mOptions[optRTTMode.name] = optRTTMode;
#ifdef RTSHADER_SYSTEM_BUILD_CORE_SHADERS
		mOptions[optEnableFixedPipeline.name] = optEnableFixedPipeline;
#endif

}

String SDLGLSupport::validateConfig(void)
{
    return String("");
}

RenderWindow* SDLGLSupport::createWindow(bool autoCreateWindow, GLES2RenderSystem* renderSystem, const String& windowTitle)
{
  LogManager::getSingleton().logMessage("\tGLSupport createWindow called");
	if (autoCreateWindow)
    {
        ConfigOptionMap::iterator opt = mOptions.find("Full Screen");
        if (opt == mOptions.end())
            OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, "Can't find full screen options!", "SDLGLSupport::createWindow");
        bool fullscreen = (opt->second.currentValue == "Yes");

        opt = mOptions.find("Video Mode");
        if (opt == mOptions.end())
            OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, "Can't find video mode options!", "SDLGLSupport::createWindow");
        String val = opt->second.currentValue;
        String::size_type pos = val.find('x');
        if (pos == String::npos)
            OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, "Invalid Video Mode provided", "SDLGLSupport::createWindow");

		// Parse FSAA config
		NameValuePairList winOptions;
		winOptions["title"] = windowTitle;
        int fsaa_x_samples = 0;
        opt = mOptions.find("FSAA");
        if(opt != mOptions.end())
        {
			winOptions["FSAA"] = opt->second.currentValue;
        }

#ifdef RTSHADER_SYSTEM_BUILD_CORE_SHADERS
			opt = mOptions.find("Fixed Pipeline Enabled");
			if (opt == mOptions.end())
				OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Can't find Fixed Pipeline enabled options!", "Win32GLSupport::createWindow");
			bool enableFixedPipeline = (opt->second.currentValue == "Yes");
			renderSystem->setFixedPipelineEnabled(enableFixedPipeline);
#endif

        unsigned int w = StringConverter::parseUnsignedInt(val.substr(0, pos));
        unsigned int h = StringConverter::parseUnsignedInt(val.substr(pos + 1));

        const SDL_VideoInfo* videoInfo = SDL_GetVideoInfo();
        return renderSystem->_createRenderWindow(windowTitle, w, h, fullscreen, &winOptions);
    }
    else
    {
        // XXX What is the else?
		return NULL;
    }

}


RenderWindow* SDLGLSupport::newWindow(const String &name, unsigned int width, unsigned int height, 
	bool fullScreen, const NameValuePairList *miscParams)
{
  LogManager::getSingleton().logMessage("\tGLSupport newWindow called");
	SDLWindow* window = new SDLWindow(this);
	window->create(name, width, height, fullScreen, miscParams);
	return window;
}

void SDLGLSupport::start()
{
    LogManager::getSingleton().logMessage(
        "******************************\n"
        "*** Starting SDL Subsystem ***\n"
        "******************************");

    SDL_Init(SDL_INIT_VIDEO);
}

void SDLGLSupport::stop()
{
    LogManager::getSingleton().logMessage(
        "******************************\n"
        "*** Stopping SDL Subsystem ***\n"
        "******************************");

    SDL_Quit();
}

void* SDLGLSupport::getProcAddress(const String& procname)
{
    return SDL_GL_GetProcAddress(procname.c_str());
}
