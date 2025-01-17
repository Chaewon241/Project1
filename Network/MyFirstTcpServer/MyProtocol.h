// 컴퓨터, 프로그램마다 구조체 정렬 규칙이 다를 수 있다.
// 구조체를 정의하고 #pragma pack을 통해 원하는대로 정렬할 수 있다.

// 패킷이 크지 않고 고정 크기로도 충분할 때가 많아서
// 패킷 정의를 구조체로 하고 1바이트 패킹을 하는 경우가 많다.
// 장점은 구현이 쉽다.
// 단점은 멤버 데이터 타입이 자유롭지 않다.
// 좀 더 자유롭게 사용하기 위해 시리얼라이즈 패킷 모듈을 만들거나 기존 라이브러리를 사용한다.
// 유명한 라이브러리로는 FlatBuffer가 있다.
#pragma once
#include <cstddef>
#include <string>

#pragma pack (push, 1)

enum EPacketId	: short
{
	PACKETID_START,

	C2S_ACCESS,
	S2C_ACCESS,
	C2S_BROADCAST_MSG,
	S2C_BROADCAST_MSG,

	PACKETID_END,
};

struct PacketHeader
{
	short size;
	EPacketId id;
};

struct PacketC2S_Access : PacketHeader
{
	char* nickname;
};

struct PacketS2C_Access : PacketHeader
{
	std::byte result;
};

struct PacketC2S_BroadcastMsg : PacketHeader
{
	char* clientMessage;
};

struct PacketS2C_BroadcastMsg : PacketHeader
{
	char* serverMessage;
};

#pragma pack (pop)