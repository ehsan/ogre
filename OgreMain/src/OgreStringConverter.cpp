/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2011 Torus Knot Software Ltd

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
#include "OgreStableHeaders.h"
#include "OgreStringConverter.h"

namespace Ogre {

    //-----------------------------------------------------------------------
    String StringConverter::toString(Real val, unsigned short precision, 
        unsigned short width, char fill, std::ios::fmtflags flags)
    {
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
		static char buffer[128] = "";
		int n = snprintf(buffer, 128, "%f", val);
		return String(buffer, n);
#else
        stringstream stream;
        stream.precision(precision);
        stream.width(width);
        stream.fill(fill);
        if (flags)
            stream.setf(flags);
        stream << val;
        return stream.str();
#endif
    }
    //-----------------------------------------------------------------------
    String StringConverter::toString(int val, 
        unsigned short width, char fill, std::ios::fmtflags flags)
    {
        stringstream stream;
		stream.width(width);
        stream.fill(fill);
        if (flags)
            stream.setf(flags);
        stream << val;
        return stream.str();
    }
    //-----------------------------------------------------------------------
#if OGRE_PLATFORM != OGRE_PLATFORM_NACL && OGRE_PLATFORM != OGRE_PLATFORM_EMSCRIPTEN && \
  ( OGRE_ARCH_TYPE == OGRE_ARCHITECTURE_64 || OGRE_PLATFORM == OGRE_PLATFORM_APPLE || OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS )
    String StringConverter::toString(unsigned int val, 
        unsigned short width, char fill, std::ios::fmtflags flags)
    {
        stringstream stream;
        stream.width(width);
        stream.fill(fill);
        if (flags)
            stream.setf(flags);
        stream << val;
        return stream.str();
    }
    //-----------------------------------------------------------------------
    String StringConverter::toString(size_t val, 
        unsigned short width, char fill, std::ios::fmtflags flags)
    {
        stringstream stream;
        stream.width(width);
        stream.fill(fill);
        if (flags)
            stream.setf(flags);
        stream << val;
        return stream.str();
    }
#if OGRE_COMPILER == OGRE_COMPILER_MSVC
    //-----------------------------------------------------------------------
    String StringConverter::toString(unsigned long val, 
        unsigned short width, char fill, std::ios::fmtflags flags)
    {
        stringstream stream;
        stream.width(width);
        stream.fill(fill);
        if (flags)
            stream.setf(flags);
        stream << val;
        return stream.str();
    }

#endif
    //-----------------------------------------------------------------------
#else
    String StringConverter::toString(size_t val, 
        unsigned short width, char fill, std::ios::fmtflags flags)
    {
        stringstream stream;
		stream.width(width);
        stream.fill(fill);
        if (flags)
            stream.setf(flags);
        stream << val;
        return stream.str();
    }
    //-----------------------------------------------------------------------
    String StringConverter::toString(unsigned long val, 
        unsigned short width, char fill, std::ios::fmtflags flags)
    {
        stringstream stream;
		stream.width(width);
        stream.fill(fill);
        if (flags)
            stream.setf(flags);
        stream << val;
        return stream.str();
    }
    //-----------------------------------------------------------------------
#endif
    String StringConverter::toString(long val, 
        unsigned short width, char fill, std::ios::fmtflags flags)
    {
        stringstream stream;
		stream.width(width);
        stream.fill(fill);
        if (flags)
            stream.setf(flags);
        stream << val;
        return stream.str();
    }
	//-----------------------------------------------------------------------
    String StringConverter::toString(const Vector2& val)
    {
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
		static char buffer[128] = "";
		int n = snprintf(buffer, 128, "%f %f", val.x, val.y);
		return String(buffer, n);
#else
        stringstream stream;
		stream << val.x << " " << val.y;
        return stream.str();
#endif
    }
    //-----------------------------------------------------------------------
    String StringConverter::toString(const Vector3& val)
    {
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
		static char buffer[128] = "";
		int n = snprintf(buffer, 128, "%f %f %f", val.x, val.y, val.z);
		return String(buffer, n);
#else
        stringstream stream;
		stream << val.x << " " << val.y << " " << val.z;
        return stream.str();
#endif
    }
	//-----------------------------------------------------------------------
    String StringConverter::toString(const Vector4& val)
    {
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
		static char buffer[128] = "";
		int n = snprintf(buffer, 128, "%f %f %f %f", val.x, val.y, val.z, val.w);
		return String(buffer, n);
#else
        stringstream stream;
		stream << val.x << " " << val.y << " " << val.z << " " << val.w;
        return stream.str();
#endif
    }
    //-----------------------------------------------------------------------
    String StringConverter::toString(const Matrix3& val)
    {
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
		static char buffer[128] = "";
		int n = snprintf(buffer, 128, "%f %f %f %f %f %f %f %f %f", val[0][0], 
			val[0][1], val[0][2], val[1][0], val[1][1], val[1][2],
			val[2][0], val[2][1], val[2][2]);
		return String(buffer, n);
#else
		stringstream stream;
        stream << val[0][0] << " " 
            << val[0][1] << " "             
            << val[0][2] << " "             
            << val[1][0] << " "             
            << val[1][1] << " "             
            << val[1][2] << " "             
            << val[2][0] << " "             
            << val[2][1] << " "             
            << val[2][2];
        return stream.str();
#endif
    }
    //-----------------------------------------------------------------------
    String StringConverter::toString(bool val, bool yesNo)
    {
        if (val)
        {
            if (yesNo)
            {
                return "yes";
            }
            else
            {
                return "true";
            }
        }
        else
            if (yesNo)
            {
                return "no";
            }
            else
            {
                return "false";
            }
    }
    //-----------------------------------------------------------------------
    String StringConverter::toString(const Matrix4& val)
    {
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
		static char buffer[128] = "";
		int n = snprintf(buffer, 128, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f", 
			val[0][0], val[0][1], val[0][2], val[0][3], 
			val[1][0], val[1][1], val[1][2], val[1][3], 
			val[2][0], val[2][1], val[2][2], val[2][3],
			val[3][0], val[3][1], val[3][2], val[3][3]);
		return String(buffer, n);
#else
		stringstream stream;
        stream << val[0][0] << " " 
            << val[0][1] << " "             
            << val[0][2] << " "             
            << val[0][3] << " "             
            << val[1][0] << " "             
            << val[1][1] << " "             
            << val[1][2] << " "             
            << val[1][3] << " "             
            << val[2][0] << " "             
            << val[2][1] << " "             
            << val[2][2] << " "             
            << val[2][3] << " "             
            << val[3][0] << " "             
            << val[3][1] << " "             
            << val[3][2] << " "             
            << val[3][3];
        return stream.str();
#endif
    }
    //-----------------------------------------------------------------------
    String StringConverter::toString(const Quaternion& val)
    {
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
		static char buffer[128] = "";
		int n = snprintf(buffer, 128, "%f %f %f %f", val.w, val.x, val.y, val.z);
		return String(buffer, n);
#else
		stringstream stream;
        stream  << val.w << " " << val.x << " " << val.y << " " << val.z;
        return stream.str();
#endif
    }
    //-----------------------------------------------------------------------
    String StringConverter::toString(const ColourValue& val)
    {
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
		static char buffer[128] = "";
		int n = snprintf(buffer, 128, "%f %f %f %f", val.r, val.g, val.b, val.a);
		return String(buffer, n);
#else
		stringstream stream;
        stream << val.r << " " << val.g << " " << val.b << " " << val.a;
        return stream.str();
#endif
    }
    //-----------------------------------------------------------------------
    String StringConverter::toString(const StringVector& val)
    {
		stringstream stream;
        StringVector::const_iterator i, iend, ibegin;
        ibegin = val.begin();
        iend = val.end();
        for (i = ibegin; i != iend; ++i)
        {
            if (i != ibegin)
                stream << " ";

            stream << *i; 
        }
        return stream.str();
    }
    //-----------------------------------------------------------------------
    Real StringConverter::parseReal(const String& val, Real defaultValue)
    {
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
		Real ret = 0;
		int n = sscanf(val.c_str(), "%f", &ret);
		
		if(n == 0){
			// Nothing read, so try integer format
			int ret2 = 0;
			n = sscanf(val.c_str(), "%d", &ret2);
			if(n == 1)
				ret = (Real)ret2;
		}
#else
		// Use istringstream for direct correspondence with toString
		StringStream str(val);
		Real ret = defaultValue;
		str >> ret;
#endif
        return ret;
    }
    //-----------------------------------------------------------------------
    int StringConverter::parseInt(const String& val, int defaultValue)
    {
		// Use istringstream for direct correspondence with toString
		StringStream str(val);
		int ret = defaultValue;
		str >> ret;

        return ret;
    }
    //-----------------------------------------------------------------------
    unsigned int StringConverter::parseUnsignedInt(const String& val, unsigned int defaultValue)
    {
		// Use istringstream for direct correspondence with toString
		StringStream str(val);
		unsigned int ret = defaultValue;
		str >> ret;

		return ret;
    }
    //-----------------------------------------------------------------------
    long StringConverter::parseLong(const String& val, long defaultValue)
    {
		// Use istringstream for direct correspondence with toString
		StringStream str(val);
		long ret = defaultValue;
		str >> ret;

		return ret;
    }
    //-----------------------------------------------------------------------
    unsigned long StringConverter::parseUnsignedLong(const String& val, unsigned long defaultValue)
    {
		// Use istringstream for direct correspondence with toString
		StringStream str(val);
		unsigned long ret = defaultValue;
		str >> ret;

		return ret;
    }
    //-----------------------------------------------------------------------
    bool StringConverter::parseBool(const String& val, bool defaultValue)
    {
		if ((StringUtil::startsWith(val, "true") || StringUtil::startsWith(val, "yes")
			|| StringUtil::startsWith(val, "1")))
			return true;
		else if ((StringUtil::startsWith(val, "false") || StringUtil::startsWith(val, "no")
			|| StringUtil::startsWith(val, "0")))
			return false;
		else
			return defaultValue;
    }
    //-----------------------------------------------------------------------
    Vector2 StringConverter::parseVector2(const String& val, const Vector2& defaultValue)
    {
        // Split on space
        vector<String>::type vec = StringUtil::split(val);

        if (vec.size() != 2)
        {
            return defaultValue;
        }
        else
        {
            return Vector2(parseReal(vec[0]),parseReal(vec[1]));
        }
    }
	//-----------------------------------------------------------------------
    Vector3 StringConverter::parseVector3(const String& val, const Vector3& defaultValue)
    {
        // Split on space
        vector<String>::type vec = StringUtil::split(val);

        if (vec.size() != 3)
        {
            return defaultValue;
        }
        else
        {
            return Vector3(parseReal(vec[0]),parseReal(vec[1]),parseReal(vec[2]));
        }
    }
	//-----------------------------------------------------------------------
    Vector4 StringConverter::parseVector4(const String& val, const Vector4& defaultValue)
    {
        // Split on space
        vector<String>::type vec = StringUtil::split(val);

        if (vec.size() != 4)
        {
            return defaultValue;
        }
        else
        {
            return Vector4(parseReal(vec[0]),parseReal(vec[1]),parseReal(vec[2]),parseReal(vec[3]));
        }
    }
    //-----------------------------------------------------------------------
    Matrix3 StringConverter::parseMatrix3(const String& val, const Matrix3& defaultValue)
    {
        // Split on space
        vector<String>::type vec = StringUtil::split(val);

        if (vec.size() != 9)
        {
            return defaultValue;
        }
        else
        {
            return Matrix3(parseReal(vec[0]),parseReal(vec[1]),parseReal(vec[2]),
                parseReal(vec[3]),parseReal(vec[4]),parseReal(vec[5]),
                parseReal(vec[6]),parseReal(vec[7]),parseReal(vec[8]));
        }
    }
    //-----------------------------------------------------------------------
    Matrix4 StringConverter::parseMatrix4(const String& val, const Matrix4& defaultValue)
    {
        // Split on space
        vector<String>::type vec = StringUtil::split(val);

        if (vec.size() != 16)
        {
            return defaultValue;
        }
        else
        {
            return Matrix4(parseReal(vec[0]),parseReal(vec[1]),parseReal(vec[2]), parseReal(vec[3]),
                parseReal(vec[4]),parseReal(vec[5]), parseReal(vec[6]), parseReal(vec[7]),
                parseReal(vec[8]),parseReal(vec[9]), parseReal(vec[10]), parseReal(vec[11]),
                parseReal(vec[12]),parseReal(vec[13]), parseReal(vec[14]), parseReal(vec[15]));
        }
    }
    //-----------------------------------------------------------------------
    Quaternion StringConverter::parseQuaternion(const String& val, const Quaternion& defaultValue)
    {
        // Split on space
        vector<String>::type vec = StringUtil::split(val);

        if (vec.size() != 4)
        {
            return defaultValue;
        }
        else
        {
            return Quaternion(parseReal(vec[0]),parseReal(vec[1]),parseReal(vec[2]), parseReal(vec[3]));
        }
    }
    //-----------------------------------------------------------------------
    ColourValue StringConverter::parseColourValue(const String& val, const ColourValue& defaultValue)
    {
        // Split on space
        vector<String>::type vec = StringUtil::split(val);

        if (vec.size() == 4)
        {
            return ColourValue(parseReal(vec[0]),parseReal(vec[1]),parseReal(vec[2]), parseReal(vec[3]));
        }
        else if (vec.size() == 3)
        {
            return ColourValue(parseReal(vec[0]),parseReal(vec[1]),parseReal(vec[2]), 1.0f);
        }
        else
        {
            return defaultValue;
        }
    }
    //-----------------------------------------------------------------------
    StringVector StringConverter::parseStringVector(const String& val)
    {
        return StringUtil::split(val);
    }
	//-----------------------------------------------------------------------
	bool StringConverter::isNumber(const String& val)
	{
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
		float test;
		int n = sscanf(val.c_str(), "%f", &test);
		return n == 1;
#else
		StringStream str(val);
		float tst;
		str >> tst;
		return !str.fail() && str.eof();
#endif
	}
}


