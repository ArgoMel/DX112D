#include "CameraComponent.h"
#include "../Device.h"
#include "../Input.h"
#include "../Scene/SceneManager.h"

CCameraComponent::CCameraComponent()
	: m_CameraViewDistance(1000.f)
	, m_CameraType(ECameraType::Camera2D)
{
	SetTypeID<CCameraComponent>();
	m_ComponentTypeName = "CameraComponent";
}

CCameraComponent::CCameraComponent(const CCameraComponent& component)
	: CSceneComponent(component)
{
	m_CameraViewDistance = component.m_CameraViewDistance;
	m_matView = component.m_matView;
	m_matProj = component.m_matProj;
	m_CameraType = component.m_CameraType;
	ComputeProjectionMatrix();
}

CCameraComponent::~CCameraComponent()
{
}

void CCameraComponent::ComputeProjectionMatrix()
{
	Resolution	RS = CDevice::GetInst()->GetResolution();
	switch (m_CameraType)
	{
	case ECameraType::Camera2D:
		m_matProj = DirectX::XMMatrixOrthographicOffCenterLH(0.f, (float)RS.width, 0.f,	(float)RS.height, 0.f,
			m_CameraViewDistance);
		break;
	case ECameraType::Camera3D:
		//m_matProj = DirectX::XMMatrixPerspectiveFovLH()
		break;
	case ECameraType::CameraUI:
		m_matProj = DirectX::XMMatrixOrthographicOffCenterLH(0.f, (float)RS.width, 0.f,	(float)RS.height, 0.f, 
			m_CameraViewDistance);
		break;
	}
}

void CCameraComponent::Destroy()
{
	CSceneComponent::Destroy();
}

void CCameraComponent::Start()
{
	CSceneComponent::Start();
	ComputeProjectionMatrix();
}

bool CCameraComponent::Init()
{
	CSceneComponent::Init();
	return true;
}

void CCameraComponent::Update(float deltaTime)
{
	CSceneComponent::Update(deltaTime);
}

void CCameraComponent::PostUpdate(float deltaTime)
{
	CSceneComponent::PostUpdate(deltaTime);
	/*
		카메라 뷰스페이스 변환 : 월드상의 모든 물체를 카메라가 0, 0, 0으로 이동하는 만큼
		이동시키고 카메라가 바라보는 방향(Z축)이 Z + 방향을 바라볼 수 있게 회전시키는 만큼 이동된
		모든 물체들을 원점 기준으로 그만큼 공전시켜준다.

		그러면 카메라는 뷰공간으로 변환이 일어날 경우 뷰공간에서의 카메라의 x, y, z 축은
		X : 1, 0, 0
		Y : 0, 1, 0
		Z : 0, 0, 1
		이 될것이다.

		카메라의 월드축 : WX, WY, WZ 축이라고 한다.
		WX* 뷰행렬 = 1, 0, 0
		WY * 뷰행렬 = 0, 1, 0
		WZ * 뷰행렬 = 0, 0, 1

		WXX WXY WXZ   11 12 13   1 0 0
		WYX WYY WYZ * 21 22 23 = 0 1 0
		WZX WZY WZZ   31 32 33   0 0 1

		월드축을 이용하여 구성한 행렬을 뷰행렬을 곱해주면 항등행렬이 나온다.
		뷰행렬은 월드축을 이용하여 구성한 행렬의 역행렬이 된다는 것이다.

		월드축을 이용하여 구성한 행렬은 월드축이기 때문에 서로 정직교한다.
		이렇게 서로 정직교하는 축을 이용하여 구성한 행렬을 직교행렬이라고 한다.

		직교행렬의 특징 : 역행렬과 전치행렬이 같다.
		뷰행렬은 월드축을 이용하여 구성한 행렬의 전치행렬과 같다는 의미이다.
		WXX WXY WXZ    WXX WYX WZX
		WYX WYY WYZ = > WXY WYY WZY
		WZX WZY WZZ    WXZ WYZ WZZ

		카메라의 위치 : PX, PY, PZ

		이동행렬
		1   0   0	0
		0   1   0	0
		0   0   1	0
		-PX -PY -PZ 1

		회전행렬
		WXX WYX WZX 0
		WXY WYY WZY 0
		WXZ WYZ WZZ 0
		0   0   0   1

		1    0   0	 0   WXX WYX WZX 0   WXX   WYX    WZX  0
		0    1   0	 0 * WXY WYY WZY 0 = WXY   WYY    WZY  0
		0    0   1	 0	 WXZ WYZ WZZ 0   WXZ   WYZ    WZZ  0
		-PX  -PY -PZ 1	 0   0   0   1  -P.WX -P.WY  -P.WZ 1
		*/
	if (m_CameraType != ECameraType::CameraUI)
	{
		m_matView.Identity();	//항등
		for (int i = 0; i < 3; ++i)
		{
			Vector3	axis = GetWorldAxis((AXIS)i);
			memcpy(&m_matView[i][0], &axis, sizeof(Vector3));
		}
		m_matView.Transpose();	//전치
		Vector3	pos = GetWorldPos();
		for (int i = 0; i < 3; ++i)
		{
			m_matView[3][i] = -pos.Dot(GetWorldAxis((AXIS)i));
		}
		CInput::GetInst()->ComputeWorldMousePos(Vector2(pos.x, pos.y));
	}
}

void CCameraComponent::Render()
{
	CSceneComponent::Render();
}

CCameraComponent* CCameraComponent::Clone() const
{
	return new CCameraComponent(*this);
}

void CCameraComponent::Save(FILE* file)
{
	CSceneComponent::Save(file);
	fwrite(&m_CameraType, sizeof(ECameraType), 1,file);
	fwrite(&m_CameraViewDistance, sizeof(float), 1, file);
	fwrite(&m_matView, sizeof(Matrix), 1, file);
	fwrite(&m_matProj, sizeof(Matrix), 1, file);
}

void CCameraComponent::Load(FILE* file)
{
	CSceneComponent::Load(file);
	fread(&m_CameraType, sizeof(ECameraType), 1, file);
	fread(&m_CameraViewDistance, sizeof(float), 1, file);
	fread(&m_matView, sizeof(Matrix), 1, file);
	fread(&m_matProj, sizeof(Matrix), 1, file);
}
