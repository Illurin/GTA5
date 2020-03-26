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

	//��������Ϣ����
	bool GetMouse(int button);
	bool GetMouseDown(int button);
	void GetCursorPosition(long& cursorPositionX, long& cursorPositionY);

private:
	IDirectInput8* inputSystem;
	IDirectInputDevice8* keyboardDevice;
	IDirectInputDevice8* mouseDevice;

	char currentKeyBuffer[256] = {};
	char lastKeyBuffer[256] = {};

	DIMOUSESTATE currentMouseState;
	DIMOUSESTATE lastMouseState;

	HWND hwnd;
};