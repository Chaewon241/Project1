// eventSelect.cpp WSAEventSelect �� ����Ͽ� �񵿱� ���� ���α׷����� �غ��ô�
// https://learn.microsoft.com/ko-KR/windows/win32/api/winsock2/nf-winsock2-wsaeventselect
#include <winsock2.h>

#include "LoopThread.h"

#include <ws2tcpip.h>
#include <iostream>
#include <vector>
// ���� �߿��Դ�.

// link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

using namespace std;

constexpr int BUFSIZE = 1024;

//void OnError(errorType, WSAGetLastError())

class CNetClient : public CLoopThreadBase
{
public:
    CNetClient() {}
    virtual ~CNetClient() {}

    bool Connect();

private:
    virtual void DoUpdate() override;

    SOCKET    m_clientSocket = INVALID_SOCKET;

    WSAEVENT m_clientEvent = WSA_INVALID_EVENT;

    bool m_isConnected = false;

    char m_sendBuffer[BUFSIZE] = "�˴���~";
};

struct Session
{
    SOCKET socket = INVALID_SOCKET; // ���� �ڵ�

    char recvBuffer[BUFSIZE] = {};

    int recvBytes = 0;
    int sendBytes = 0; // ������ŭ ���� �ִ� �ΰ����� ���� ����
};

int main()
{
    WSAData wsaData;

    if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        return 0;

    // ��α׿� �ϳ�
    // 
    SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket == INVALID_SOCKET)
        return 0;

    SOCKADDR_IN serverAddr;
    ::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
    serverAddr.sin_port = ::htons(7777);

    if (::bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
        return 0;

    // SOMAXCONN���� 1�� �ٲ��ֱ�
    if (::listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
        return 0;

    cout << "Listen" << endl;

    // createevent
    vector<WSAEVENT> wsaEvents;
    // Session ����ü
    vector<Session> sessions;
    sessions.reserve(100);

    WSAEVENT listenEvent = ::WSACreateEvent();
    wsaEvents.push_back(listenEvent);               //[listenEvent][][][][]
    sessions.push_back(Session{ listenSocket });    //[listenSession][][][][]

    // �̺�Ʈ�� ����ϰ� �ش� �̺�Ʈ�� �߻��ϸ� �ε����� �˾ƿ� �� �ִ�.
    // WSAEventSelect �Լ��� listenSocket�� �ͺ���ŷ �������� �����.
    if (::WSAEventSelect(listenSocket, listenEvent, FD_ACCEPT) == SOCKET_ERROR) return 0;

    //���� Ŭ���̾�Ʈ ��ü�� ���� ������� �����ϰ� �غ��ϴ�.
    CNetClient client;
    client.Start(true);

    if (false == client.Connect())
    {
        cout << "Connect Error " << WSAGetLastError() << endl;
    }

    while (true)
    {
        /*Sleep(1000);*/
        // ���⼭ cmdâ�� telnet ip 7777 �ϰ� �� cmd â���� �����Ҷ��ϸ� �ȴ�
        // ��α�.. accept�� �� ���༭..?

        // �ε����� ������ � �̺�Ʈ���� ��ġ����. �ٵ� �̷��� �ϸ� ��� �̺�Ʈ�� ���� ���� ��� false
        // ������ ��ٸ����� WSA_INFINITE
        // ��ϵ� �̺�Ʈ�� ���ؼ� �뺸�� �ޱ� ���� �Լ�
        int index = ::WSAWaitForMultipleEvents(wsaEvents.size(), &wsaEvents[0], FALSE, WSA_INFINITE, FALSE);
        //int index = ::WSAWaitForMultipleEvents(wsaEvents.size(), &wsaEvents[0], FALSE, 1, FALSE);

        if (index == WSA_WAIT_FAILED)
            continue;
        // �̰� �Ŵ��� �Դϴ� ���� �׻� WSA_WAIT_EVENT_0 �����༭ �ϳ� �������.
        index -= WSA_WAIT_EVENT_0;

        // WSAEventSelect�� ���Ͽ� ��ϵ� �̺�Ʈ�� �߻��ϸ� � �̺�Ʈ���� Ȯ��
        // �۾��� ó���ϴµ� �̺�Ʈ�� ��Ʈ��ũ ������ Ȯ���ؾ���.
        // FD_ACCEPT

        // ��Ʈ��ũ �̺�Ʈ�� �޾ƿ��ʹ�!
        WSANETWORKEVENTS networkEvents;

        if (::WSAEnumNetworkEvents(sessions[index].socket, wsaEvents[index], &networkEvents) == SOCKET_ERROR)
            return 0;

        // ��Ʈ�������� �ش� ��Ʈ�� ACCEPT�̸�(�̺�Ʈ�� �߻�������!)
        if (networkEvents.lNetworkEvents & FD_ACCEPT)   
        {
            // ���� ����.
            // �ٵ� �̺�Ʈ�� ������ ���� �־ üũ �������.
            if (networkEvents.iErrorCode[FD_ACCEPT_BIT] != 0) // FD_ACCEPT�� �´� ����
                continue;

            // ������ �ƴϸ�?
            // ACCEPT�� �ϰڴ�.

            // Ŭ���̾�Ʈ �����ϳ� �ּ� �޾ƿͼ�
            SOCKADDR_IN clientAddr;
            int addrLen = sizeof(clientAddr);
            // Ŭ���̾�Ʈ �ּ� ���� ����
            SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);

            if (clientSocket != INVALID_SOCKET)
            {
                cout << "Client Connected" << endl;

                WSAEVENT clientEvent = ::WSACreateEvent();
                // �ι�°�� �̺�Ʈ Ǫ����
                wsaEvents.push_back(clientEvent);
                sessions.push_back(Session{ clientSocket });

                // Ŭ��� ����ȰŴϱ� READ����.
                if (::WSAEventSelect(clientSocket, clientEvent, FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR)
                    return 0;

                if (networkEvents.lNetworkEvents & FD_ACCEPT)
                {
                    if (networkEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
                    {
                        // FD_ACCEPT�� �´� ����
                        OnError(errorType, WSAGetLastError()); // �տ��� enum���� Avvept�� connect �����ؼ� �����ϰ�
                        // ���� ������ ���� �� �Լ����� �Ͽ�ȭ
                        OnAccept(WSAGetLastError());
                        continue;
                    }
                }
            }   
        }// FD_ACCEPT

        // ���� ���Ϲ��ۿ� �о�� �����Ͱ� �ִٸ�(���̻� ������ ���� ����) || 
        if (networkEvents.lNetworkEvents & FD_READ || networkEvents.lNetworkEvents & FD_WRITE)
        {

            if ((networkEvents.lNetworkEvents & FD_READ) && networkEvents.iErrorCode[FD_READ_BIT] != 0)
                continue;


            if ((networkEvents.lNetworkEvents & FD_WRITE) && networkEvents.iErrorCode[FD_WRITE_BIT] != 0)
                continue;


            Session& s = sessions[index];

            // �� �о���.
            if (s.recvBytes == 0)
            {
                // receive ���� �� ����
                int recvLen = ::recv(s.socket, s.recvBuffer, BUFSIZE, 0);
                if (recvLen == SOCKET_ERROR && ::WSAGetLastError() != WSAEWOULDBLOCK) {
                    continue;
                }

                if (recvLen > 0)
                    s.recvBytes = recvLen;

                cout << "Recv Data= " << recvLen << " : " << s.recvBuffer << endl;
            }

            // Send
            if (s.recvBytes > s.sendBytes) {
                int sendLen = ::send(s.socket, &s.recvBuffer[s.sendBytes], s.recvBytes - s.sendBytes, 0);
                if (sendLen == SOCKET_ERROR && ::WSAGetLastError() != WSAEWOULDBLOCK) {
                    continue;
                }

                s.sendBytes += sendLen;
                if (s.recvBytes == s.sendBytes) {
                    s.recvBytes = 0;
                    s.sendBytes = 0;
                }

                cout << "Send Data= " << sendLen << endl;
            }
        }
    }

    WSACleanup();
    
    // ���α׷��� ����Ǹ� ������ ������ ���������� ������ ���߱� ����~
    client.Stop();

    return 0;
}

bool CNetClient::Connect()
{
    m_clientSocket = ::socket(AF_INET, SOCK_STREAM, 0);

    if (m_clientSocket == INVALID_SOCKET) return false;

    m_clientEvent = ::WSACreateEvent();

    // �����

    // �ش� �Լ��� Ŭ���̾�Ʈ ���� �ڵ鿡 FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE 4������ ��Ʈ��ũ ��ȣ�� ������
    // m_clientEvent �̺�Ʈ ��ü�� ���ؼ� �� �� �ֵ��� ���� �մϴ�.
    // m_clientSocket�� �ͺ���ŷ ���� �ٲ㵵 ��.
    if (::WSAEventSelect(m_clientSocket, m_clientEvent, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR) return 0;

    SOCKADDR_IN serverAddr;
    ::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = ::htons(7777);
    inet_pton(AF_INET, "172.21.1.68", &(serverAddr.sin_addr)); // �ٸ� �ּҷ� �ٲ㼭 �׽�Ʈ �غ�����.

    if (::connect(m_clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) return false;

    cout << "Connecting" << endl;

    return true;
}

// ������, ���� �Լ�
void CNetClient::DoUpdate()
{
    // ���Ͽ� ����� �̺�Ʈ ��ü�� �뺸�� ��� �մϴ�..
    // �̺�Ʈ�� �ϳ�? 1000�� 1�ʸ��� ��ٸ�.
    int index = ::WSAWaitForMultipleEvents(1, &m_clientEvent, FALSE, 1000, FALSE);

    if (index == WSA_WAIT_FAILED)
        return;

    index -= WSA_WAIT_EVENT_0;

    WSANETWORKEVENTS networkEvents;

    if (::WSAEnumNetworkEvents(m_clientSocket, m_clientEvent, &networkEvents) == SOCKET_ERROR)
    {
        cout << "EnumNetworkEvents Error" << WSAGetLastError() << endl;
        return;
    }

    // ���࿡ Ŀ��Ʈ���
    if (networkEvents.lNetworkEvents & FD_CONNECT)
    {
        if (networkEvents.iErrorCode[FD_CONNECT_BIT] != 0)
        {
            cout << "Connect Error " << networkEvents.iErrorCode[FD_CONNECT_BIT] << endl;
            return;
        }

        cout << "Connected" << endl;

        m_isConnected = true;
    }

    // receive ���� �о�ð� �ִٸ�
    if(networkEvents.lNetworkEvents & FD_READ)
    {
        if (networkEvents.iErrorCode[FD_READ_BIT] != 0)
        {
            cout << "Read Error" << endl;
            return;
        }
        char buffer[BUFSIZE] = {};
        int recvBytes = ::recv(m_clientSocket, buffer, BUFSIZE, 0);
        if (recvBytes == SOCKET_ERROR)
        {
            cout << "Read Error" << endl;
            return;
        }

        // ������ ���� ��
        if (recvBytes == 0)
        {
            cout << "Disconnected" << endl;
            return;
        }
        cout << "Recv: " << buffer << endl;
    }

    if (m_isConnected)
    {
        Sleep(1000);

        int sendLen = ::send(m_clientSocket, m_sendBuffer, strlen(m_sendBuffer) + 1, 0);
        if (sendLen == SOCKET_ERROR && ::WSAGetLastError() != WSAEWOULDBLOCK) {
            cout << "send Error " << ::WSAGetLastError() << endl;
        }
        else
        {
            cout << "send " << sendLen << " " << endl;
        }
    }
}