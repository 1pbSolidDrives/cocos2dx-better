/****************************************************************************
 Author: Luma (stubma@gmail.com)
 
 https://github.com/stubma/cocos2dx-common
 
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
 ****************************************************************************/
#include "CCUtils.h"
#include "CCMoreMacros.h"

NS_CC_BEGIN

unsigned char CCUtils::UnitScalarToByte(float x) {
    if (x < 0) {
        return 0;
    }
    if (x >= 1) {
        return 255;
    }
    return (int)(x * (1 << 16)) >> 8;
}

void CCUtils::toLowercase(string& s) {
	if(s.empty())
		return;
	
	char* buf = new char[s.length() + 1];
	strcpy(buf, s.c_str());
	size_t len = s.length();
    for(int i = 0; i < len; i++)
		if(buf[i] >= 0x41 && buf[i] <= 0x5A)
			buf[i] += 0x20;
	
	s.copy(buf, len);
	delete buf;
}

bool CCUtils::startsWith(const string& s, const string& sub) {
    return s.find(sub) == 0;
}

bool CCUtils::endsWith(const string& s, const string& sub) {
    return s.rfind(sub) == s.length() - sub.length();
}

bool CCUtils::deleteFile(string path) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
	string mappedPath = mapLocalPath(path);
	NSString* p = [NSString stringWithFormat:@"%s", mappedPath.c_str()];
	NSFileManager* fm = [NSFileManager defaultManager];
	NSError* error = nil;
	[fm removeItemAtPath:p error:&error];
	return error == nil;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	return unlink(path.c_str()) == 0;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	string mappedPath = mapLocalPath(path);
	return DeleteFile(mappedPath.c_str()) != 0;
#else
	CCLOGERROR("CCUtils::mapLocalPath is not implemented for this platform, please finish it");
	return false;
#endif
}

string CCUtils::mapLocalPath(string path) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
	if(CCFileUtils::sharedFileUtils()->isAbsolutePath(path)) {
		NSString* nsPath = [NSString stringWithFormat:@"~/Documents/%s", path.c_str()];
		nsPath = [nsPath stringByExpandingTildeInPath];
		return [nsPath cStringUsingEncoding:NSUTF8StringEncoding];
	} else {
		NSBundle* bundle = [NSBundle mainBundle];
		NSString* relativePath = [NSString stringWithFormat:@"%s", path.c_str()];
		NSString* ext = [relativePath pathExtension];
		NSString* filename = [relativePath lastPathComponent];
		NSString* filenameWithoutExt = [filename stringByDeletingPathExtension];
		NSString* dir = [relativePath stringByDeletingLastPathComponent];
		NSString* path = [bundle pathForResource:filenameWithoutExt ofType:ext inDirectory:dir];
		return [path cStringUsingEncoding:NSUTF8StringEncoding];
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	return path;
#else
	CCLOGERROR("CCUtils::mapLocalPath is not implemented for this platform, please finish it");
#endif
}

string CCUtils::getParentPath(string path) {
	if(path.empty())
		return "";
	
	size_t slash = path.rfind('/');
	if(slash == string::npos)
		return "";
	else if(slash == 0)
		return path.substr(0, 1);
	else
		return path.substr(0, slash);
}

bool CCUtils::createIntermediateFolders(string path) {
	string parent = getParentPath(path);
	bool exist = isPathExistent(parent);
	bool success = true;
	if(!exist) {
		createIntermediateFolders(parent);
		success = createFolder(parent);
	}
	
	// return success flag
	return success;
}

bool CCUtils::createFolder(string path) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
	string mappedPath = mapLocalPath(path);
	NSString* nsPath = [NSString stringWithFormat:@"%s", mappedPath.c_str()];
	NSFileManager* fm = [NSFileManager defaultManager];
	return [fm createDirectoryAtPath:nsPath withIntermediateDirectories:YES attributes:NULL error:NULL];
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	return mkdir(path.c_str(), S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) == 0;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	string mappedPath = mapLocalPath(path);
	return CreateDirectory(mappedPath.c_str(), NULL) != 0;
#else
	CCLOGERROR("CCUtils::mapLocalPath is not implemented for this platform, please finish it");
	return false;
#endif
}

bool CCUtils::isPathExistent(string path) {
	// if path is empty, directly return
	if(path.empty())
		return true;
	
	// get mapped path
	string mappedPath = mapLocalPath(path);
	
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
	NSString* nsPath = [NSString stringWithFormat:@"%s", mappedPath.c_str()];
	return [[NSFileManager defaultManager] fileExistsAtPath:nsPath];
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	return access(mappedPath.c_str(), 0) == 0;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	DWORD dwAttrib = GetFileAttributes((LPCTSTR)mappedPath.c_str());
	return dwAttrib != INVALID_FILE_ATTRIBUTES;
#else
	CCLOGERROR("CCUtils::mapLocalPath is not implemented for this platform, please finish it");
	return false;
#endif
}

ccColorHSV CCUtils::ccc32hsv(ccColor3B c) {
	unsigned char min = MIN(c.r, MIN(c.g, c.b));
    unsigned char max = MAX(c.r, MAX(c.g, c.b));
    unsigned char delta = max - min;
	
    // get v
    float v = max / 255.f;
	
    // if input color a gray color?
    if(delta == 0) {
    	return cchsv(0, 0, v);
    }
	
    // get s
    float s = (float)delta / max;
	
    // get h
    float h;
    if(c.r == max) {
        h = (float)(c.g - c.b) / delta;
    } else if (c.g == max) {
        h = 2 + (float)(c.b - c.r) / delta;
    } else { // b == max
        h = 4 + (float)(c.r - c.g) / delta;
    }
	
    // adjust h
    h *= 60;
    if (h < 0) {
        h += 360;
    }
	
    return cchsv(h, s, v);
}

ccColor3B CCUtils::hsv2ccc3(ccColorHSV c) {
	unsigned char s = UnitScalarToByte(c.s);
    unsigned char v = UnitScalarToByte(c.v);
	
    // if s is zero then rgb color is gray
    if (0 == s) {
    	return ccc3(v, v, v);
    }
	
    int hx = (c.h < 0 || c.h >= 360.f) ? 0 : (int)((c.h / 60) * (1 << 16));
    int f = hx & 0xFFFF;
	
    unsigned char v_scale = v + 1;
    unsigned char p = ((255 - s) * v_scale) >> 8;
    unsigned char q = ((255 - (s * f >> 16)) * v_scale) >> 8;
    unsigned char t = ((255 - (s * ((1 << 16) - f) >> 16)) * v_scale) >> 8;
	
    unsigned char r, g, b;
    switch (hx >> 16) {
        case 0:
        	r = v;
        	g = t;
        	b = p;
        	break;
        case 1:
        	r = q;
        	g = v;
        	b = p;
        	break;
        case 2:
        	r = p;
        	g = v;
        	b = t;
        	break;
        case 3:
        	r = p;
        	g = q;
        	b = v;
        	break;
        case 4:
        	r = t;
        	g = p;
        	b = v;
        	break;
        default:
        	r = v;
        	g = p;
        	b = q;
        	break;
    }
	
    return ccc3(r, g, b);
}

CCPoint CCUtils::getOrigin(CCNode* node) {
	if(node->isIgnoreAnchorPointForPosition()) {
		return node->getPosition();
	} else {
		return ccp(node->getPositionX() - node->getAnchorPointInPoints().x,
				   node->getPositionY() - node->getAnchorPointInPoints().y);
	}
}

CCPoint CCUtils::getCenter(CCNode* node) {
	if(node->isIgnoreAnchorPointForPosition()) {
		return ccpAdd(node->getPosition(), ccp(node->getContentSize().width / 2, node->getContentSize().height / 2));
	} else {
		return ccpAdd(ccp(node->getPositionX() - node->getAnchorPointInPoints().x,
						  node->getPositionY() - node->getAnchorPointInPoints().y),
					  ccp(node->getContentSize().width / 2, node->getContentSize().height / 2));
	}
}

bool CCUtils::testSegmentAABB(CCPoint p0, CCPoint p1, ccAABB b) {
	CCPoint c = ccpMult(ccpAdd(b.min, b.max), 0.5f);
	CCPoint e = ccpSub(b.max, c);
	CCPoint m = ccpMult(ccpAdd(p0, p1), 0.5f);
	CCPoint d = ccpSub(p1, m);
	m = ccpSub(m, c);
	
	// Box center-point
	// Box halflength extents
	// Segment midpoint
	// Segment halflength vector
	// Translate box and segment to origin
	// Try world coordinate axes as separating axes
	float adx = fabs(d.x);
	if (fabs(m.x) > e.x + adx)
		return false;
	float ady = fabs(d.y);
	if (fabs(m.y) > e.y + ady)
		return false;
	
	// Add in an epsilon term to counteract arithmetic errors when segment is
	// (near) parallel to a coordinate axis (see text for detail)
	adx += FLT_EPSILON;
	ady += FLT_EPSILON;
	
	// Try cross products of segment direction vector with coordinate axes
	if (fabs(m.x * d.y - m.y * d.x) > e.x * ady + e.y * adx)
		return false;
	
	// No separating axis found; segment must be overlapping AABB
	return true;
}

NS_CC_END