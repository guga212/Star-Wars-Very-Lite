#pragma once
#include "App.h"

class camera {

	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	std::unique_ptr<DirectX::Mouse> m_mouse;
	HWND hwnd;

	XMMATRIX camView;
	XMVECTOR camPosition;
	XMVECTOR camTarget;
	XMVECTOR camUp;

	XMVECTOR DefaultForward;
	XMVECTOR DefaultRight;
	XMVECTOR camForward;
	XMVECTOR camRight;

	
	XMMATRIX camRotationMatrix;
	XMMATRIX groundWorld;

	float inc;

	float moveLeftRight;
	float moveBackForward;

	float camYaw;
	float camPitch;

	void input();
public:

	camera(HWND _hwnd);
	XMMATRIX get_view();
	XMVECTOR get_cam_pos();

	void Updt_pos(XMVECTOR _pos) {
		camPosition = _pos;

	}


	void Updt();
	
};