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

#include <iostream>
#include <algorithm>
#include <climits>

namespace Ogre {
	SDLWindow::SDLWindow(SDLGLSupport *glsupport)
		: mGLSupport(glsupport), mClosed(false), mContext(0)
	{
	}

	SDLWindow::~SDLWindow()
	{
		if(mContext)
			delete mContext;
	}

	void SDLWindow::getCustomAttribute( const String& name, void* pData )
	{
        if(name == "GLCONTEXT")
        {
            *static_cast<GLES2Context **>(pData) = mContext;
            return;
        }
	}

	void SDLWindow::getLeftAndTopFromNativeWindow( int & left, int & top, uint width, uint height )
	{
	}

	void SDLWindow::initNativeCreatedWindow(const NameValuePairList *miscParams)
	{
		mContext = new SDLGLContext(this, mGLSupport);
	}

	void SDLWindow::createNativeWindow( int &left, int &top, uint &width, uint &height, String &title )
	{
	}

	void SDLWindow::reposition( int left, int top )
	{
	}

	void SDLWindow::resize(uint width, uint height)
	{
        mWidth  = width;
        mHeight  = height;

        mContext->resize();
	}

	void SDLWindow::windowMovedOrResized()
	{
	}

	void SDLWindow::copyContentsToMemory(const PixelBox &dst, FrameBuffer buffer)
	{
	
	}

	bool SDLWindow::requiresTextureFlipping() const
	{
		return false;
	}

	void SDLWindow::destroy(void)
	{
	}

	bool SDLWindow::isClosed(void) const
	{
		return mClosed;
	}

    void SDLWindow::create(const String& name, uint width, uint height,
                           bool fullScreen, const NameValuePairList *miscParams)
    {
		initNativeCreatedWindow(miscParams);

		mName = name;
        mWidth = width;
        mHeight = height;
        mLeft = 0;
        mTop = 0;
        mActive = true;
		//mVisible = true;

        mClosed = false;
	}

    void SDLWindow::swapBuffers(bool waitForVSync)
    {
        if (mClosed)
        {
            return;
        }

        mContext->swapBuffers(waitForVSync);

    }


}
