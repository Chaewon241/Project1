#pragma once

#define MAXTHREADS 128

typedef struct _OPTIONS
{
	char szHostname[64];
	const char* port;
	int nTotalThreads;
	int nBufSize;
	BOOL bVerbose;
} OPTIONS;

typedef struct THREADINFO
{
	HANDLE hThread[MAXTHREADS];
	SOCKET sd[MAXTHREADS];
} THREADINFO;

class Client
{
public:
	
private:
	OPTIONS default_options = { "localhost", "5001", 1, 4096, FALSE };
	OPTIONS m_Options;
	THREADINFO m_ThreadInfo;
	BOOL m_bEndClient = FALSE;
	WSAEVENT m_hCleanupEvent[1];
};