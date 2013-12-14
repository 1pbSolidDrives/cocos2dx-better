#ifndef __CCTCPSocketManager_h__
#define __CCTCPSocketManager_h__

#include "cocos2d.h"
#include "ExtensionMacros.h"
#include "CCTCPSocket.h"
#include "CCByteBuffer.h"

NS_CC_BEGIN

typedef bool (*ProAllFunc)(int, int, CCByteBuffer&);    // 接收所有协议，自行处理，@socket标识,@协议头，@数据包,返回是否分发
typedef void (*ProFunc)(int, CCByteBuffer&);    // 接收单个协议，@socket标识,@数据包
typedef void (*sckFunc)(int);    // 连接成功/断开事件

// 创建SOCKET管理器
#define CREATE_CCTCPSocketMGR(pNode)    pNode->addChild(new CCTCPSocketManager(), 0)

class CC_DLL CCTCPSocketManager : public CCNode
{
public:
    CCTCPSocketManager()
    {
        assert(!mSingleton);
        this->mSingleton = this;
        // 开启update
        scheduleUpdate();
    };
    ~CCTCPSocketManager(){};
    // 创建socket并添加到管理器
    CCTCPSocket *createSocket(const char* pszServerIP,    // IP地址
							  int nServerPort,            // 端口
							  int _tag,                    // 标识ID
							  int nBlockSec = BLOCKSECONDS, // 阻塞时间ms
							  bool bKeepAlive = false);
    // 注册协议包
    void    register_process(const uint16 &entry, ProFunc callback);
    // 注册接收所有协议
    void    register_all_process(ProAllFunc callback){ _pProcess = callback; }
    // 注册socket连接成功事件
    void    register_connect(sckFunc callback){ _pOnConnect = callback; }
    // 注册socket断线事件
    void    register_disconnect(sckFunc callback){ _OnDisconnect = callback; }
	
    // 单独添加socket到管理器
    bool    addSocket(CCTCPSocket *pSocket);
    // 删除socket
    bool    removeSocket(int _tag);
    // 断开socket
    void    disconnect(int _tag);
    // 获取socket
    CCTCPSocket    *GetSocket(int _tag);
    // 发送消息
    bool    SendPacket(int _tag, CCByteBuffer *packet);
	
    virtual void    update(float delta);
	
    static CCTCPSocketManager &getSingleton(){ assert(mSingleton); return *mSingleton;}
	
private:
    ProAllFunc _pProcess;
    sckFunc _pOnConnect;
    sckFunc _OnDisconnect;
    std::list<CCTCPSocket*> m_lstSocket;
    std::map<uint16, ProFunc> _mapProcess;
    static CCTCPSocketManager * mSingleton;
};

#define sSocketMgr    CCTCPSocketManager::getSingleton()

NS_CC_END

#endif //__CCTCPSocketManager_h__