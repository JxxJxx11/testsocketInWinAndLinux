#ifdef WIN32
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")
#define socklen_t int
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#define closesocket close
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Xsocket.h"

WSAinit::WSAinit() {
#ifdef WIN32
	WSADATA ws;
	WSAStartup(MAKEWORD(2, 2), &ws);
#endif
}

WSAinit::~WSAinit() {
#ifdef WIN32
	WSACleanup();
#endif
}

WSAinit wainit;

Xsocket::Xsocket() {

}

Xsocket::~Xsocket() {

}


XTcp::XTcp() {

}

XTcp::~XTcp() {

}

int XTcp::CreateSocket() {
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		printf("create socket failed!\n");
		return sock;
	}
	printf("sock: %d create success!\n", sock);
	return sock;
}

bool XTcp::Bind(unsigned short port) {
	if (sock <= 0) {
		CreateSocket();
	}
	sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = htonl(0);
	if (::bind(sock, (sockaddr*)&saddr, sizeof(saddr)) != 0) {
		printf("bind port: %d failed!\n", port);
		return false;
	}
	printf("bind port: %d success!\n", port);
	listen(sock, 10);
	return true;
}

XTcp XTcp::Accept() {
	XTcp tcp;
	sockaddr_in caddr;
	socklen_t clen = sizeof(caddr);
	tcp.sock = accept(sock, (sockaddr*)&caddr, &clen);
	if (tcp.sock <= 0) return tcp;
	char* ip = inet_ntoa(caddr.sin_addr);
	strcpy(tcp.ip, ip);
	tcp.port = ntohs(caddr.sin_port);
	printf("accept client: %d success! ip: %s port: %d\n", tcp.sock, tcp.ip, tcp.port);
	return tcp;
}

void XTcp::Close() {
	if (sock <= 0) return;
	closesocket(sock);
}

int XTcp::Recv(char* buf, int bufSize) {
	return recv(sock, buf, bufSize, 0);
}

int XTcp::Send(const char* buf, int sendSize) {
	int s = 0;
	while(s != sendSize) {
		int len = send(sock, buf + s, sendSize - s, 0);
		if (len <= 0) break;
		s += len;
	}
	return s;
}

bool XTcp::Connect(const char* ip, unsigned short port) {
	if (sock <= 0) CreateSocket();
	sockaddr_in  saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = inet_addr(ip);
	if (connect(sock, (sockaddr*)&saddr, sizeof(saddr)) != 0) {
		printf("connect %s: %d failed! : %s\n", ip, port, strerror(errno));
		return false;
	}
	printf("connect %s: %d success!\n", ip, port);
}