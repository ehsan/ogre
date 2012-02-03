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

#include "OgreSDLGLContext.h"
#include "OgreLog.h"
#include "OgreException.h"
#include "OgreRoot.h"
#include "OgreException.h"
#include "OgreLogManager.h"
#include "OgreStringConverter.h"
#include "OgreWindowEventUtilities.h"

#include "OgreGLES2Prerequisites.h"
#include "OgreGLES2RenderSystem.h"

#include "OgreSDLGLSupport.h"
#include "OgreSDLWindow.h"
#include "OgreSDLGLContext.h"

#include <stdint.h>

namespace Ogre {
    SDLGLContext::SDLGLContext(const SDLWindow * window, const SDLGLSupport *glsupport)
        : mGLSupport(glsupport)
        , mWindow(window)
        , mWidth(mWindow->getWidth())
        , mHeight(mWindow->getHeight())
    {
    }

    SDLGLContext::~SDLGLContext()
    {
    }

    void SDLGLContext::setCurrent()
    {
        unsigned int flags = SDL_OPENGL | SDL_RESIZABLE;
        mContext = SDL_SetVideoMode(mWidth, mHeight, 0, flags);
    }

    void SDLGLContext::endCurrent()
    {
    }

    GLES2Context* SDLGLContext::clone() const
    {
        SDLGLContext* res = new SDLGLContext(mWindow, mGLSupport);
        return res;
    }

    void SDLGLContext::swapBuffers( bool waitForVSync )
    {
        SDL_GL_SwapBuffers();
    }

    void SDLGLContext::resize()
    {
        if(mWidth != mWindow->getWidth() || mHeight != mWindow->getHeight() )
        {
            LogManager::getSingleton().logMessage("\tresizing");
            mWidth = mWindow->getWidth();
            mHeight = mWindow->getHeight();
            setCurrent(); // call SDL_SetVideoMode
            LogManager::getSingleton().logMessage("\tdone resizing");
        }

    }




} // namespace
