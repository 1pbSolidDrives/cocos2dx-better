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
#ifndef __CCTCPSocket_h__
#define __CCTCPSocket_h__

#include "cocos2d.h"
#include "ExtensionMacros.h"
#include "CCByteBuffer.h"
#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>

NS_CC_BEGIN

#define kCCSocketMaxPacketSize (16 * 1024)
#define kCCSocketDefaultTimeout 30
#define kCCSocketInputBufferDefaultSize (64 * 1024)
#define kCCSocketOutputBufferDefaultSize (8 * 1024)

/**
 * TCP socket client. Data in read buffer has a length header, i.e., first two
 * bytes is data packet length
 */
class CC_DLL CCTCPSocket : public CCObject {
private:
	/// socket handle
    int m_sockClient;
	
    /// write buffer
    char m_outBuf[kCCSocketOutputBufferDefaultSize];
	
	/// writable data in write buffer
    int m_outBufLen;
	
    /// read buffer, it is a loop buffer
    char m_inBuf[kCCSocketInputBufferDefaultSize];
	
	/// available data in read buffer
    int m_inBufLen;
	
	/// start reading pos of read buffer, between 0 and (size - 1)
    int m_inBufStart;
	
	/// tag of this socket
    int m_tag;
	
private:
	/// receive data from socket until no more data or buffer full, or error
	bool recvFromSock();
	
	/// has error
    bool hasError();
	
	/// close socket
    void closeSocket();
	
protected:
	/**
	 * init socket
	 *
	 * @param hostname host name or ip address, ipv4 only
	 * @param port port
	 * @param tag tag of socket
	 * @param blockSec block time when create this socket, 0 means not block
	 * @param keepAlive true means keep socket alive
	 * @return true means initialization successful
	 */
    bool init(const string& hostname, int port, int tag = -1, int blockSec = kCCSocketDefaultTimeout, bool keepAlive = false);
	
public:
    CCTCPSocket();
	virtual ~CCTCPSocket();
	
	/**
	 * create socket instance
	 *
	 * @param hostname host name or ip address, ipv4 only
	 * @param port port
	 * @param tag tag of socket
	 * @param blockSec block time when create this socket, 0 means not block
	 * @param keepAlive true means keep socket alive
	 * @return instance or NULL if failed
	 */
	static CCTCPSocket* create(const string& hostname, int port, int tag = -1, int blockSec = kCCSocketDefaultTimeout, bool keepAlive = false);
	
	/**
	 * send data in a buffer
	 *
	 * @param buf buffer
	 * @param size data to be sent
	 * @return operation success or failed
	 */
    bool sendData(void* buf, int size);
	
	/**
	 * receive data and put into a buffer
	 * 
	 * @param buf buffer large enough to hold data
	 * @param size wanted data length
	 * @return actual read data size, or -1 if fail to read a complete packet
	 */
    int receiveData(void* buf, int size);
	
	/// flush write buffer, send them now
    bool flush();
	
	/// check is there any data can be read
    bool hasAvailable();
	
	/// destroy socket
    void destroy();
	
	/// get socket handle
    int getSocket() const { return m_sockClient; }
    
	/// get tag
    int getTag() { return m_tag; }
};

NS_CC_END

#endif //__CCTCPSocket_h__