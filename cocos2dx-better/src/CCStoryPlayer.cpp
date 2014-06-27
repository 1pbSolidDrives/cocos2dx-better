/****************************************************************************
 Author: Luma (stubma@gmail.com)
 
 https://github.com/stubma/cocos2dx-better
 
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
#include "CCStoryPlayer.h"
#include "CCStoryLayer.h"
#include "CCStoryCommandSet.h"
#include "CCRichLabelTTF.h"
#include "CCStoryMessageLayer.h"
#include "CBArmature.h"
#include "CCFlash.h"
#include "CCShake.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    #import <AppKit/AppKit.h>
#endif

// tag of node
#define TAG_MESSAGE_LAYER 10000
#define TAG_BG_COLOR_LAYER 10001
#define TAG_BG 10002

// z order
#define Z_MESSAGE_LAYER MAX_INT
#define Z_BG -MAX_INT
#define Z_BG_COLOR_LAYER (Z_BG + 1)

NS_CC_BEGIN

CCStoryPlayer::CCStoryPlayer() :
m_owner(NULL),
m_curCmd(NULL),
m_done(false),
m_curCmdIndex(-1),
m_msgSize(20),
m_nameSize(20),
m_msgColor(0xffffffff),
m_nameColor(0xffffffff),
m_msgPos(CCPointZero),
m_namePos(CCPointZero),
m_msgAnchor(ccp(0.5f, 0.5f)),
m_nameAnchor(ccp(0.5f, 0.5f)) {
}

CCStoryPlayer::~CCStoryPlayer() {
}

CCStoryPlayer* CCStoryPlayer::create(CCStoryLayer* owner) {
    CCStoryPlayer* p = new CCStoryPlayer();
    if(p->initWithOwner(owner)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_RELEASE(p);
    return NULL;
}

bool CCStoryPlayer::initWithOwner(CCStoryLayer* owner) {
    // init
    m_owner = owner;
    m_msgPos = CCUtils::getLocalCenter(m_owner);
    m_namePos = CCUtils::getLocalCenter(m_owner);
    m_bgPos = CCUtils::getLocalCenter(m_owner);
    
    return true;
}

void CCStoryPlayer::start() {
    // if has error, stop
    if(hasError())
        return;
    
    // first command
    fetchNextCommand();
    
    // is fetched?
    if(m_curCmd) {
        executeCurrentCommand();
    } else {
        m_done = true;
    }
}

void CCStoryPlayer::onMessageDone() {
    // remove message layer
    m_owner->removeChildByTag(TAG_MESSAGE_LAYER);
    
    // next
    start();
}

void CCStoryPlayer::executeCurrentCommand() {
    switch (m_curCmd->getType()) {
        case CCStoryCommand::MSG:
        {
            // remove old dialog and create new dialog layer
            CCStoryMessageLayer* dl = CCStoryMessageLayer::create(this);
            m_owner->addChild(dl, Z_MESSAGE_LAYER, TAG_MESSAGE_LAYER);
            
            // show dialog
            dl->showMessage(m_curCmd);
            break;
        }
        case CCStoryCommand::MSG_ANCHOR:
        {
            m_msgAnchor.x = m_curCmd->m_param.msganchor.x;
            m_msgAnchor.y = m_curCmd->m_param.msganchor.y;
            
            // next
            start();
            break;
        }
        case CCStoryCommand::MSG_SIZE:
        {
            m_msgSize = m_curCmd->m_param.msgsize.size;
            
            // next
            start();
            break;
        }
        case CCStoryCommand::MSG_COLOR:
        {
            m_msgColor = m_curCmd->m_param.msgcolor.c;
            
            // next
            start();
            break;
        }
        case CCStoryCommand::MSG_POS:
        {
            m_msgPos.x = m_curCmd->m_param.msgpos.x;
            m_msgPos.y = m_curCmd->m_param.msgpos.y;
            
            // next
            start();
            break;
        }
        case CCStoryCommand::NAME_SIZE:
        {
            m_nameSize = m_curCmd->m_param.namesize.size;
            
            // next
            start();
            break;
        }
        case CCStoryCommand::NAME_COLOR:
        {
            m_nameColor = m_curCmd->m_param.namecolor.c;
            
            // next
            start();
            break;
        }
        case CCStoryCommand::NAME_POS:
        {
            m_namePos.x = m_curCmd->m_param.namepos.x;
            m_namePos.y = m_curCmd->m_param.namepos.y;
            
            // next
            start();
            break;
        }
        case CCStoryCommand::NAME_ANCHOR:
        {
            m_nameAnchor.x = m_curCmd->m_param.nameanchor.x;
            m_nameAnchor.y = m_curCmd->m_param.nameanchor.y;
            
            // next
            start();
            break;
        }
        case CCStoryCommand::WAIT:
        {
            m_owner->runAction(CCSequence::create(CCDelayTime::create(m_curCmd->m_param.wait.time),
                                                  CCCallFunc::create(this, callfunc_selector(CCStoryPlayer::start)),
                                                  NULL));
            break;
        }
        case CCStoryCommand::WAIT_ARM:
        {
            if(m_curCmdIndex > 0) {
                CCStoryCommand* prevCmd = (CCStoryCommand*)gStoryCommandSet.objectAtIndex(m_curCmdIndex - 1);
                if(prevCmd->getType() == CCStoryCommand::ARM_PLAY) {
                    CBArmature* arm = getArmatureRole(prevCmd->m_param.armplay.name);
                    arm->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(CCStoryPlayer::onArmatureAnimationDone));
                } else {
                    setError("waitarm must follow an armplay command");
                }
            } else {
                setError("waitarm must follow an armplay command");
            }
            
            break;
        }
        case CCStoryCommand::IMG:
        {
            CCSpriteFrame* pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(m_curCmd->m_param.img.frameName);
            if(pFrame) {
                if(!m_roleMap.objectForKey(m_curCmd->m_param.img.name)) {
                    // create role
                    CCSprite* role = CCSprite::createWithSpriteFrame(pFrame);
                    role->setPosition(ccp(m_curCmd->m_param.img.x,
                                          m_curCmd->m_param.img.y));
                    m_owner->addChild(role);
                    
                    // save role
                    m_roleMap.setObject(role, m_curCmd->m_param.img.name);
                } else {
                    char buf[512];
                    sprintf(buf, "image role %s is already existent", m_curCmd->m_param.img.name);
                    setError(buf);
                }
            } else {
                char buf[512];
                sprintf(buf, "image frame: %s is not existent", m_curCmd->m_param.img.frameName);
                setError(buf);
            }
            
            // next
            start();
            break;
        }
        case CCStoryCommand::ARM:
        {
            CCArmatureDataManager* adm = CCArmatureDataManager::sharedArmatureDataManager();
            if(adm->getAnimationData(m_curCmd->m_param.arm.armName)) {
                if(!m_roleMap.objectForKey(m_curCmd->m_param.arm.name)) {
                    CBArmature* arm = CBArmature::create(m_curCmd->m_param.arm.armName);
                    arm->setPosition(ccp(m_curCmd->m_param.arm.x,
                                         m_curCmd->m_param.arm.y));
                    m_owner->addChild(arm);
                    
                    // save role
                    m_roleMap.setObject(arm, m_curCmd->m_param.arm.name);
                } else {
                    char buf[512];
                    sprintf(buf, "armature role %s is already existent", m_curCmd->m_param.arm.name);
                    setError(buf);
                }
            } else {
                char buf[512];
                sprintf(buf, "armature %s is not existent", m_curCmd->m_param.arm.armName);
                setError(buf);
            }
            
            // next
            start();
            break;
        }
        case CCStoryCommand::ARM_PLAY:
        {
            CBArmature* arm = getArmatureRole(m_curCmd->m_param.armplay.name);
            if(arm) {
                if(arm->getAnimation()->getAnimationData()->getMovement(m_curCmd->m_param.armplay.animName)) {
                    arm->getAnimation()->play(m_curCmd->m_param.armplay.animName);
                    arm->updateOffsetPoint();
                } else {
                    char buf[512];
                    sprintf(buf, "animation %s is not existent", m_curCmd->m_param.armplay.animName);
                    setError(buf);
                }
            }
            
            // next
            start();
            break;
        }
        case CCStoryCommand::FLASH:
        {
            // get role and shake
            CCNode* role = getRole(m_curCmd->m_param.flash.name);
            if(role) {
                role->runAction(CCRepeat::create(CCFlash::create(m_curCmd->m_param.flash.time / m_curCmd->m_param.flash.times, ccc3FromInt(m_curCmd->m_param.flash.c)),
                                                 m_curCmd->m_param.flash.times));
            }
            
            // next
            start();
            break;
        }
        case CCStoryCommand::SHAKE:
        {
            // get role and shake
            CCNode* role = getRole(m_curCmd->m_param.shake.name);
            if(role) {
                role->runAction(CCShake::create(m_curCmd->m_param.shake.time,
                                                m_curCmd->m_param.shake.amplitude));
            }
            
            // next
            start();
            break;
        }
        case CCStoryCommand::MOVE:
        {
            // get role and move
            CCNode* role = getRole(m_curCmd->m_param.move.name);
            if(role) {
                role->runAction(CCMoveTo::create(m_curCmd->m_param.move.time,
                                                 ccp(m_curCmd->m_param.move.x, m_curCmd->m_param.move.y)));
            }
            
            // next
            start();
            break;
        }
        case CCStoryCommand::ROTATE:
        {
            // get role and rotate
            CCNode* role = getRole(m_curCmd->m_param.rotate.name);
            if(role) {
                role->runAction(CCRotateBy::create(m_curCmd->m_param.rotate.time,
                                                   m_curCmd->m_param.rotate.delta));
            }
            
            // next
            start();
            break;
        }
        case CCStoryCommand::SCALE:
        {
            // get role and scale
            CCNode* role = getRole(m_curCmd->m_param.scale.name);
            if(role) {
                role->runAction(CCScaleTo::create(m_curCmd->m_param.scale.time,
                                                  m_curCmd->m_param.scale.to));
            }
            
            // next
            start();
            break;
        }
        case CCStoryCommand::FADE_IN:
        {
            // get role and fade in
            CCNode* role = getRole(m_curCmd->m_param.fadein.name);
            if(role) {
                role->runAction(CCFadeIn::create(m_curCmd->m_param.fadein.time));
            }
            
            // next
            start();
            break;
        }
        case CCStoryCommand::FADE_OUT:
        {
            // get role and fade out
            CCNode* role = getRole(m_curCmd->m_param.fadeout.name);
            if(role) {
                role->runAction(CCFadeOut::create(m_curCmd->m_param.fadeout.time));
            }
            
            // next
            start();
            break;
        }
        case CCStoryCommand::ANGLE:
        {
            // get role and set rotation
            CCNode* role = getRole(m_curCmd->m_param.angle.name);
            if(role) {
                role->setRotation(m_curCmd->m_param.angle.degree);
            }
            
            // next
            start();
            break;
        }
        case CCStoryCommand::BG_POS:
        {
            m_bgPos.x = m_curCmd->m_param.bgpos.x;
            m_bgPos.y = m_curCmd->m_param.bgpos.y;
            
            // next
            start();
            break;
        }
        case CCStoryCommand::BG:
        {
            CCSprite* bg = (CCSprite*)m_owner->getChildByTag(TAG_BG);
            if(!bg) {
                CCSpriteFrame* pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(m_curCmd->m_param.bg.frameName);
                if(!pFrame) {
                    char buf[512];
                    sprintf(buf, "bg frame: %s is not existent", m_curCmd->m_param.bg.frameName);
                    setError(buf);
                } else {
                    bg = CCSprite::createWithSpriteFrame(pFrame);
                    bg->setPosition(m_bgPos);
                    m_owner->addChild(bg, Z_BG, TAG_BG);
                }
            }
            
            // next
            start();
            break;
        }
        case CCStoryCommand::BG_COLOR:
        {
            // create bg color layer and set color
            CCLayerColor* bg = (CCLayerColor*)m_owner->getChildByTag(TAG_BG_COLOR_LAYER);
            if(!bg) {
                bg = CCLayerColor::create();
                m_owner->addChild(bg, Z_BG_COLOR_LAYER);
            }
            ccColor4B c = ccc4FromInt(m_curCmd->m_param.bgcolor.c);
            bg->setColor(ccc3FromCCC4(c));
            bg->setOpacity(c.a);
            
            // next
            start();
            break;
        }
        case CCStoryCommand::Z:
        {
            // get role and set z order
            CCNode* role = getRole(m_curCmd->m_param.z.name);
            if(role) {
                role->setZOrder(m_curCmd->m_param.z.z);
            }
            
            // next
            start();
            break;
        }
        case CCStoryCommand::REMOVE:
        {
            // remove role
            CCNode* role = getRole(m_curCmd->m_param.remove.name);
            if(role) {
                m_roleMap.removeObjectForKey(m_curCmd->m_param.remove.name);
                role->removeFromParent();
            }
            
            // next
            start();
            break;
        }
        case CCStoryCommand::SHOW:
        {
            // show role
            CCNode* role = getRole(m_curCmd->m_param.remove.name);
            if(role) {
                role->setVisible(true);
            }
            
            // next
            start();
            break;
        }
        case CCStoryCommand::HIDE:
        {
            // hide role
            CCNode* role = getRole(m_curCmd->m_param.remove.name);
            if(role) {
                role->setVisible(false);
            }
            
            // next
            start();
            break;
        }
        default:
            break;
    }
}

CBArmature* CCStoryPlayer::getArmatureRole(const char* name) {
    CCObject* obj = m_roleMap.objectForKey(name);
    if(obj) {
        CBArmature* arm = dynamic_cast<CBArmature*>(obj);
        if(arm) {
            return arm;
        } else {
            char buf[512];
            sprintf(buf, "role %s is not an armature!", name);
            setError(buf);
            return NULL;
        }
    } else {
        char buf[512];
        sprintf(buf, "role %s is not found", name);
        setError(buf);
        return NULL;
    }
}

CCNode* CCStoryPlayer::getRole(const char* name) {
    // get role
    CCObject* obj = m_roleMap.objectForKey(name);
    if(obj) {
        CCNode* role = dynamic_cast<CCNode*>(obj);
        if(role) {
            return role;
        } else {
            char buf[512];
            sprintf(buf, "role %s is not valid!", name);
            setError(buf);
            return NULL;
        }
    } else {
        char buf[512];
        sprintf(buf, "role %s is not found", name);
        setError(buf);
        return NULL;
    }
}

void CCStoryPlayer::setError(const string& e) {
    m_error = e;
    
    // for story designer debug
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    [[NSNotificationCenter defaultCenter] postNotificationName:@"NSNotificationStoryPlayerError"
                                                        object:[NSString stringWithCString:m_error.c_str() encoding:NSUTF8StringEncoding]];
#endif
}

void CCStoryPlayer::fetchNextCommand() {
    if(m_curCmdIndex < (int)gStoryCommandSet.count() - 1) {
        m_curCmdIndex++;
        m_curCmd = (CCStoryCommand*)gStoryCommandSet.objectAtIndex(m_curCmdIndex);
    } else {
        m_curCmd = NULL;
    }
}

void CCStoryPlayer::onArmatureAnimationDone(CBArmature* armature, MovementEventType e, const char* name) {
    if(e >= COMPLETE) {
        // remove callback
        armature->getAnimation()->setMovementEventCallFunc(NULL, NULL);
        
        // next command
        start();
    }
}

NS_CC_END