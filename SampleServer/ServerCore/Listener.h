#pragma once
#include "IocpCore.h"
#include "NetAddress.h"

class AcceptEvent;
class ServerService;

/// <summary>
/// ������, �ȳ���
/// �����ʸ� �����ϰ� ������ �����ʸ� ���� ������� ���캸�� ��
/// IOCP�� ����� ���
/// </summary>

class Listener : public IocpObject
{
public:
	Listener() = default;
	~Listener();

public:
	/* �ܺο��� ��� */
	bool StartAccept(NetAddress netAddress);
	void CloseSocket();

public:
	/* �������̽� ���� */
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

private:
	/* ���� ���� */
	void RegisterAccept(AcceptEvent* acceptEvent);
	void ProcessAccept(AcceptEvent* acceptEvent);

protected:
	SOCKET _socket = INVALID_SOCKET;
	Vector<AcceptEvent*> _acceptEvents;
};
