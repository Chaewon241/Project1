#include "pch.h"
#include "D3DBasic.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	D3DBasic App(hInstance);  // �����ڿ��� ������,������ �̸��� �ٲ۴�
	if (!App.Initialize(1024, 768))
		return -1;

	return App.Run();
}