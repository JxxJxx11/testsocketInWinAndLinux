#ifndef _X_SOCKET_H_
#define _X_SCOKET_H_

class WSAinit {
public:
	WSAinit();
	~WSAinit();
};
extern WSAinit wsinit;

class Xsocket{
public:
	Xsocket();
	virtual ~Xsocket();
};

class XTcp :public Xsocket {
public:
	XTcp();
	~XTcp();
	int CreateSocket();
	bool Bind(unsigned short port);
	XTcp Accept();
	void Close();
	int Recv(char* buf, int bufSize);
	int Send(const char* buf, int sendSize);
	bool Connect(const char* ip, unsigned short port);
public:
	int sock = 0;
	unsigned short port = 0;
	char ip[16];
};

#endif // !_XSOCKET_H_