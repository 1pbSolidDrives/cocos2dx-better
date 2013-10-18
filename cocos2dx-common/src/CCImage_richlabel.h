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
#ifndef __CCImage_richlabel__
#define __CCImage_richlabel__

#include "cocos2d.h"

/**
 * Meta info of a link area, only used for link tag
 */
typedef struct LinkMeta {
    int normalBgColor;
    int selectedBgColor;
	
	// the tag of link, multiple link can have same tag (in line break situation)
	int tag;
    
    // link rect area
    float x;
    float y;
    float width;
    float height;
} LinkMeta;
typedef std::vector<LinkMeta> LinkMetaList;

NS_CC_BEGIN

/**
 * Customized CCImage for rich label implementation
 */
class CCImage_richlabel : public CCImage {	
public:
    CCImage_richlabel();
    virtual ~CCImage_richlabel();
	
	bool initWithRichStringShadowStroke(const char *    pText,
										int             nWidth      = 0,
										int             nHeight     = 0,
										ETextAlign      eAlignMask  = kAlignCenter,
										const char *    pFontName   = 0,
										int             nSize       = 0,
										float           textTintR   = 1,
										float           textTintG   = 1,
										float           textTintB   = 1,
										bool shadow                 = false,
										float shadowOffsetX         = 0.0,
										float shadowOffsetY         = 0.0,
                                        int   shadowColor           = 0,
										float shadowBlur            = 0.0,
										bool  stroke                = false,
										float strokeR               = 1,
										float strokeG               = 1,
										float strokeB               = 1,
										float strokeSize            = 1);
	
	/// shadow and stroke padding value
	CC_SYNTHESIZE_READONLY_PASS_BY_REF(CCPoint, m_shadowStrokePadding, ShadowStrokePadding);
	
	/// link meta list
	CC_SYNTHESIZE_READONLY_PASS_BY_REF(LinkMetaList, m_linkMetas, LinkMetas);
};

NS_CC_END

#endif /* defined(__CCImage_richlabel__) */
