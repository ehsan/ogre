/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2009 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

#include "OgreException.h"
#include "OgreLogManager.h"
#include "OgreStringConverter.h"
#include "OgreRoot.h"

#include "OgreGLES2Prerequisites.h"
#include "OgreGLES2RenderSystem.h"

#include "OgreSDLGLSupport.h"
#include "OgreSDLWindow.h"
#include "OgreSDLGLContext.h"

namespace Ogre {

    SDLGLSupport::SDLGLSupport()
    {
    }

    SDLGLSupport::~SDLGLSupport()
    {
    }

    String SDLGLSupport::getDisplayName(void)
    {
        return "SDL GLES2 Support";
    }


    void SDLGLSupport::switchMode(uint& width, uint& height, short& frequency)
    {
    }

	RenderWindow* SDLGLSupport::createWindow(bool autoCreateWindow,
											GLES2RenderSystem *renderSystem,
											const String& windowTitle)
	{
		LogManager::getSingleton().logMessage("\tGLSupport createWindow called");
		
		RenderWindow *window = 0;

        if (autoCreateWindow)
        {
            ConfigOptionMap::iterator opt;
            ConfigOptionMap::iterator end = mOptions.end();
            NameValuePairList miscParams;

            bool fullscreen = true;
			unsigned int w = 1, h = 1;

            if ((opt = mOptions.find("Display Frequency")) != end)
            {
                miscParams["displayFrequency"] = opt->second.currentValue;
            }

            window = renderSystem->_createRenderWindow(windowTitle, w, h, fullscreen, &miscParams);
        }

        return window;
	}

    RenderWindow* SDLGLSupport::newWindow(const String &name,
                                        unsigned int width, unsigned int height,
                                        bool fullScreen,
                                        const NameValuePairList *miscParams)
    {
		LogManager::getSingleton().logMessage("\tGLSupport newWindow called");
		
		SDLWindow* window = new SDLWindow(this);
        window->create(name, width, height, fullScreen, miscParams);
        String targetName =  window->getName();

        return window;
    }
	
	void SDLGLSupport::start(void)
	{
		LogManager::getSingleton().logMessage("\tGLSupport start called");
	}
    
	void SDLGLSupport::stop(void)
	{
		LogManager::getSingleton().logMessage("\tGLSupport stop called");
	}
    
	void SDLGLSupport::addConfig(void)
	{
		LogManager::getSingleton().logMessage("\tGLSupport addConfig called");
		
		// Currently no config options supported
		refreshConfig();
	}
	
	void SDLGLSupport::refreshConfig(void)
	{
	}
    
	String SDLGLSupport::validateConfig(void)
	{
		return StringUtil::BLANK;
	}
    
	void SDLGLSupport::setConfigOption(const String &name, const String &value)
	{
		GLES2Support::setConfigOption(name, value);
	}
    
	void* SDLGLSupport::getProcAddress(const Ogre::String& name)
	{
        //return (void*)pglGetProcAddress((const char*) name.c_str()); - doesn't link
        return 0;
	}
	
}
