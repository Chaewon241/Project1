#include "pch.h"
#include "CameraComponent.h"
#include "CommonApp.h"

CameraComponent::CameraComponent()
	: m_AspectRatio(static_cast<float>(ScreenWidth) / static_cast<float>(ScreenHeight))
	, m_FOV(90.f)
	, m_NearPlane(0.3f)
	, m_FarPlane(1000.f)
{
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::Initialize()
{
}

void CameraComponent::Update(float deltaTime)
{
	__super::Update(deltaTime);

	//ToDo 우선 주석처리 해놨음.
	CommonApp::m_pInstance->GetRenderer()->SetViewMatrix(m_worldMatrix.Invert());
}

Math::Matrix CameraComponent::GetViewMatrix() const
{
	return (m_worldMatrix.Invert());

	//or

	//Math::Vector3 eye = m_worldMatrix.Translation();
	//Math::Vector3 target = m_worldMatrix.Translation() + -m_worldMatrix.Forward();
	//Math::Vector3 up = m_worldMatrix.Up();
	//Math::Matrix viewMatrix = XMMatrixLookAtLH(eye, target, up);
	//return viewMatrix.Transpose();
}

Math::Matrix CameraComponent::GetProjectionMatrix() const
{
	return XMMatrixPerspectiveFovLH(XMConvertToRadians(m_FOV), m_AspectRatio, m_NearPlane, m_FarPlane);
}

