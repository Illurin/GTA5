#pragma once
#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

class PlayerInput {
public:
	//��ʼ������
	bool Init(HINSTANCE hInstance, HWND hwnd);
	bool Update();
	
	//��ü�����Ϣ����
	bool GetKey(int dikValue);
	bool GetKeyDown(int dikValue);

private:
	IDirectInput8* inputSystem;
	IDirectInputDevice8* keyboardDevice;

	char currentKeyBuffer[256] = {};
	char lastKeyBuffer[256] = {};
};