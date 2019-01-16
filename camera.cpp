#include"camera.h"
camera::camera(HWND _hwnd) {
	
	hwnd = _hwnd;
	////////////////////////////////////////////////
	m_keyboard = std::make_unique<Keyboard>();
	m_mouse = std::make_unique<Mouse>();
	m_mouse->SetWindow(hwnd);
	m_mouse->SetMode(Mouse::MODE_RELATIVE);

	//////////////////////////////////////////////////////////////
	camPosition = XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f);
	camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);



	DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	DefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	camForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	camRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	inc = 0.03f;
	moveLeftRight = 0.0f;
	moveBackForward = 0.0f;

	camYaw = 0.0f;
	camPitch = 0.0f;
}

void camera::Updt() {
	input();

	camRotationMatrix = XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0);
	camTarget = XMVector3TransformCoord(DefaultForward, camRotationMatrix);
	camTarget = XMVector3Normalize(camTarget);

	/* //FP camera
	XMMATRIX RotateYTempMatrix;
	RotateYTempMatrix = XMMatrixRotationY(camPitch);

	camRight = XMVector3TransformCoord(DefaultRight, RotateYTempMatrix);
	camUp = XMVector3TransformCoord(camUp, RotateYTempMatrix);
	camForward = XMVector3TransformCoord(DefaultForward, RotateYTempMatrix);
	*/
	// Free-Look Camera
	camRight = XMVector3TransformCoord(DefaultRight, camRotationMatrix);
	camForward = XMVector3TransformCoord(DefaultForward, camRotationMatrix);
	camUp = XMVector3Cross(camForward, camRight);
    /////////////////////////////////////////////////////////


	camPosition += moveLeftRight*camRight;
	camPosition += moveBackForward*camForward;

	moveLeftRight = 0.0f;
	moveBackForward = 0.0f;

	camTarget = camPosition + camTarget;
	camView = XMMatrixLookAtLH(camPosition, camTarget, camUp);

}
XMMATRIX camera::get_view() {
	return camView;
}
XMVECTOR camera::get_cam_pos() {
	return camPosition;
}


void camera::input() {
	/////////sterowanie
	/////keyboard

	auto kb = m_keyboard->GetState();
	if (kb.Escape)
		PostQuitMessage(0);
	if (kb.W)
		moveBackForward += inc *2;
	if (kb.S)
		moveBackForward -= inc * 2;
	if (kb.D)
		moveLeftRight += inc * 2;
	if (kb.A)
		moveLeftRight -= inc * 2;

	if (kb.Up)
		camPitch -= (inc) /10;
	if (kb.Down)
		camPitch += (inc) / 10;
	if (kb.Right)
		camYaw += (inc) / 10;
	if (kb.Left)
		camYaw -= (inc) / 10;
	
	
	//// mysz

	auto mouse = m_mouse->GetState();
	camYaw += (mouse.x)*0.003f;
	camPitch += (mouse.y)*0.003f;
	
}