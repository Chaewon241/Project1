#pragma once

/*
TLS(Thread Local Storage) : thread ���� ������ ��������� ���� �� �ִ� ����̴�.
������ thread�� ������ Stack�� ������ ������ Stack ����(���� ����)�� thread���� �����ϴ�.
�׷��� ������ thread�� ���� �Լ��� �����Ѵٰ� �ص� �� �Լ����� ���ǵ� ���� ������
������ ���� �ٸ� �޸� ������ ��ġ�Ѵٴ� �ǹ̴�.
�׷��� ���� ������ ���� ������ ��쿡�� ��� thread�� �����ȴ�.
*/

/// <summary>///
/// �����忡�� ����� TLS�� �����ϴ� Ŭ����
/// </summary>

// ���� ������� ������ID �ο����ֱ�(������ �߱������̶� ���� �ο��Ѵ�.)
extern thread_local uint32 LThreadId;