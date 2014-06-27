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
#include "CCStoryCommandSet.h"
#include "CCUtils.h"

CCArray gStoryCommandSet;

void msg(string name, string s) {
    CCStoryCommand* c = CCStoryCommand::create(CCStoryCommand::MSG);
    c->m_param.msg.name = CCUtils::copy(name.c_str());
    c->m_param.msg.s = CCUtils::copy(s.c_str());
    gStoryCommandSet.addObject(c);
}

void msgsize(float size) {
    CCStoryCommand* c = CCStoryCommand::create(CCStoryCommand::MSG_SIZE);
    c->m_param.msgsize.size = size;
    gStoryCommandSet.addObject(c);
}

void msgcolor(unsigned int c) {
    CCStoryCommand* cmd = CCStoryCommand::create(CCStoryCommand::MSG_COLOR);
    cmd->m_param.msgcolor.c = c;
    gStoryCommandSet.addObject(cmd);
}

void msgpos(float x, float y) {
    CCStoryCommand* c = CCStoryCommand::create(CCStoryCommand::MSG_POS);
    c->m_param.msgpos.x = x;
    c->m_param.msgpos.y = y;
    gStoryCommandSet.addObject(c);
}

void msganchor(float x, float y) {
    CCStoryCommand* c = CCStoryCommand::create(CCStoryCommand::MSG_ANCHOR);
    c->m_param.msganchor.x = x;
    c->m_param.msganchor.y = y;
    gStoryCommandSet.addObject(c);
}

void namepos(float x, float y) {
    CCStoryCommand* c = CCStoryCommand::create(CCStoryCommand::NAME_POS);
    c->m_param.namepos.x = x;
    c->m_param.namepos.y = y;
    gStoryCommandSet.addObject(c);
}

void namecolor(unsigned int c) {
    CCStoryCommand* cmd = CCStoryCommand::create(CCStoryCommand::NAME_COLOR);
    cmd->m_param.namecolor.c = c;
    gStoryCommandSet.addObject(cmd);
}

void namesize(float size) {
    CCStoryCommand* c = CCStoryCommand::create(CCStoryCommand::NAME_SIZE);
    c->m_param.namesize.size = size;
    gStoryCommandSet.addObject(c);
}

void nameanchor(float x, float y) {
    CCStoryCommand* c = CCStoryCommand::create(CCStoryCommand::NAME_ANCHOR);
    c->m_param.nameanchor.x = x;
    c->m_param.nameanchor.y = y;
    gStoryCommandSet.addObject(c);
}

void move(string name, float x, float y, float time) {
    CCStoryCommand* c = CCStoryCommand::create(CCStoryCommand::MOVE);
    c->m_param.move.name = CCUtils::copy(name.c_str());
    c->m_param.move.x = x;
    c->m_param.move.y = y;
    c->m_param.move.time = time;
    gStoryCommandSet.addObject(c);
}

void wait(float time) {
    CCStoryCommand* c = CCStoryCommand::create(CCStoryCommand::WAIT);
    c->m_param.wait.time = time;
    gStoryCommandSet.addObject(c);
}

void waitarm() {
    CCStoryCommand* c = CCStoryCommand::create(CCStoryCommand::WAIT_ARM);
    gStoryCommandSet.addObject(c);
}

void fadein(string name, float time) {
    CCStoryCommand* cmd = CCStoryCommand::create(CCStoryCommand::FADE_IN);
    cmd->m_param.fadein.name = CCUtils::copy(name.c_str());
    cmd->m_param.fadein.time = time;
    gStoryCommandSet.addObject(cmd);
}

void fadeout(string name, float time) {
    CCStoryCommand* cmd = CCStoryCommand::create(CCStoryCommand::FADE_OUT);
    cmd->m_param.fadeout.name = CCUtils::copy(name.c_str());
    cmd->m_param.fadeout.time = time;
    gStoryCommandSet.addObject(cmd);
}

void dialog() {
    
}

void flash(string name, unsigned int color, float times, float time) {
    CCStoryCommand* cmd = CCStoryCommand::create(CCStoryCommand::FLASH);
    cmd->m_param.flash.name = CCUtils::copy(name.c_str());
    cmd->m_param.flash.c = color;
    cmd->m_param.flash.times = times;
    cmd->m_param.flash.time = time;
    gStoryCommandSet.addObject(cmd);
}

void shake(string name, float amplitude, float time) {
    CCStoryCommand* cmd = CCStoryCommand::create(CCStoryCommand::SHAKE);
    cmd->m_param.shake.name = CCUtils::copy(name.c_str());
    cmd->m_param.shake.amplitude = amplitude;
    cmd->m_param.shake.time = time;
    gStoryCommandSet.addObject(cmd);
}

void scale(string name, float to, float time) {
    CCStoryCommand* cmd = CCStoryCommand::create(CCStoryCommand::SCALE);
    cmd->m_param.scale.name = CCUtils::copy(name.c_str());
    cmd->m_param.scale.to = to;
    cmd->m_param.scale.time = time;
    gStoryCommandSet.addObject(cmd);
}

void img(string name, string frameName, float x, float y) {
    CCStoryCommand* cmd = CCStoryCommand::create(CCStoryCommand::IMG);
    cmd->m_param.img.name = CCUtils::copy(name.c_str());
    cmd->m_param.img.frameName = CCUtils::copy(frameName.c_str());
    cmd->m_param.img.x = x;
    cmd->m_param.img.y = y;
    gStoryCommandSet.addObject(cmd);
}

void arm(string name, string armName, float x, float y) {
    CCStoryCommand* cmd = CCStoryCommand::create(CCStoryCommand::ARM);
    cmd->m_param.arm.name = CCUtils::copy(name.c_str());
    cmd->m_param.arm.armName = CCUtils::copy(armName.c_str());
    cmd->m_param.arm.x = x;
    cmd->m_param.arm.y = y;
    gStoryCommandSet.addObject(cmd);
}

void armplay(string name, string animName) {
    CCStoryCommand* cmd = CCStoryCommand::create(CCStoryCommand::ARM_PLAY);
    cmd->m_param.armplay.name = CCUtils::copy(name.c_str());
    cmd->m_param.armplay.animName = CCUtils::copy(animName.c_str());
    gStoryCommandSet.addObject(cmd);
}

void angle(string name, float degree) {
    CCStoryCommand* cmd = CCStoryCommand::create(CCStoryCommand::ANGLE);
    cmd->m_param.angle.name = CCUtils::copy(name.c_str());
    cmd->m_param.angle.degree = degree;
    gStoryCommandSet.addObject(cmd);
}

void rotate(string name, float delta, float time) {
    CCStoryCommand* cmd = CCStoryCommand::create(CCStoryCommand::ROTATE);
    cmd->m_param.rotate.name = CCUtils::copy(name.c_str());
    cmd->m_param.rotate.delta = delta;
    cmd->m_param.rotate.time = time;
    gStoryCommandSet.addObject(cmd);
}

void bgpos(float x, float y) {
    CCStoryCommand* cmd = CCStoryCommand::create(CCStoryCommand::BG_POS);
    cmd->m_param.bgpos.x = x;
    cmd->m_param.bgpos.y = y;
    gStoryCommandSet.addObject(cmd);
}

void bg(string frameName) {
    CCStoryCommand* cmd = CCStoryCommand::create(CCStoryCommand::BG);
    cmd->m_param.bg.frameName = CCUtils::copy(frameName.c_str());
    gStoryCommandSet.addObject(cmd);
}

void bgcolor(unsigned int c) {
    CCStoryCommand* cmd = CCStoryCommand::create(CCStoryCommand::BG_COLOR);
    cmd->m_param.bgcolor.c = c;
    gStoryCommandSet.addObject(cmd);
}

void z(string name, int zOrder) {
    CCStoryCommand* cmd = CCStoryCommand::create(CCStoryCommand::Z);
    cmd->m_param.z.name = CCUtils::copy(name.c_str());
    cmd->m_param.z.z = zOrder;
    gStoryCommandSet.addObject(cmd);
}

void bgm(string musicFile) {
    CCStoryCommand* cmd = CCStoryCommand::create(CCStoryCommand::BGM);
    cmd->m_param.bgm.musicFile = CCUtils::copy(musicFile.c_str());
    gStoryCommandSet.addObject(cmd);
}

void sound(string soundFile) {
    CCStoryCommand* cmd = CCStoryCommand::create(CCStoryCommand::SOUND);
    cmd->m_param.sound.soundFile = CCUtils::copy(soundFile.c_str());
    gStoryCommandSet.addObject(cmd);
}

void remove(string name) {
    CCStoryCommand* cmd = CCStoryCommand::create(CCStoryCommand::REMOVE);
    cmd->m_param.remove.name = CCUtils::copy(name.c_str());
    gStoryCommandSet.addObject(cmd);
}

void show(string name) {
    CCStoryCommand* cmd = CCStoryCommand::create(CCStoryCommand::SHOW);
    cmd->m_param.show.name = CCUtils::copy(name.c_str());
    gStoryCommandSet.addObject(cmd);
}

void hide(string name) {
    CCStoryCommand* cmd = CCStoryCommand::create(CCStoryCommand::HIDE);
    cmd->m_param.hide.name = CCUtils::copy(name.c_str());
    gStoryCommandSet.addObject(cmd);
}

NS_CC_BEGIN

CCStoryCommand::CCStoryCommand() :
m_type(UNKNOWN) {
    
}

CCStoryCommand::~CCStoryCommand() {
    switch(m_type) {
        case MSG:
            free((void*)m_param.msg.name);
            free((void*)m_param.msg.s);
            break;
        case IMG:
            free((void*)m_param.img.name);
            free((void*)m_param.img.frameName);
            break;
        case ARM:
            free((void*)m_param.arm.name);
            free((void*)m_param.arm.armName);
            break;
        case ARM_PLAY:
            free((void*)m_param.armplay.name);
            free((void*)m_param.armplay.animName);
            break;
        case ANGLE:
            free((void*)m_param.angle.name);
            break;
        case MOVE:
            free((void*)m_param.move.name);
            break;
        case FLASH:
            free((void*)m_param.flash.name);
            break;
        case SHAKE:
            free((void*)m_param.shake.name);
            break;
        case ROTATE:
            free((void*)m_param.rotate.name);
            break;
        case SCALE:
            free((void*)m_param.scale.name);
            break;
        case FADE_IN:
            free((void*)m_param.fadein.name);
            break;
        case FADE_OUT:
            free((void*)m_param.fadeout.name);
            break;
        case BG:
            free((void*)m_param.bg.frameName);
            break;
        case BGM:
            free((void*)m_param.bgm.musicFile);
            break;
        case SOUND:
            free((void*)m_param.sound.soundFile);
            break;
        case Z:
            free((void*)m_param.z.name);
            break;
        case REMOVE:
            free((void*)m_param.remove.name);
            break;
        case SHOW:
            free((void*)m_param.show.name);
            break;
        case HIDE:
            free((void*)m_param.hide.name);
            break;
        default:
            break;
    }
}

CCStoryCommand* CCStoryCommand::create(Type type) {
    CCStoryCommand* c = new CCStoryCommand();
    if(c->initWithCommand(type)) {
        c->autorelease();
        return c;
    }
    CC_SAFE_RELEASE(c);
    return NULL;
}

bool CCStoryCommand::initWithCommand(Type type) {
    m_type = type;
    memset(&m_param, 0, sizeof(m_param));
    
    return true;
}

NS_CC_END