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
		ī�޶� �佺���̽� ��ȯ : ������� ��� ��ü�� ī�޶� 0, 0, 0���� �̵��ϴ� ��ŭ
		�̵���Ű�� ī�޶� �ٶ󺸴� ����(Z��)�� Z + ������ �ٶ� �� �ְ� ȸ����Ű�� ��ŭ �̵���
		��� ��ü���� ���� �������� �׸�ŭ ���������ش�.

		�׷��� ī�޶�� ��������� ��ȯ�� �Ͼ ��� ����������� ī�޶��� x, y, z ����
		X : 1, 0, 0
		Y : 0, 1, 0
		Z : 0, 0, 1
		�� �ɰ��̴�.

		ī�޶��� ������ : WX, WY, WZ ���̶�� �Ѵ�.
		WX* ����� = 1, 0, 0
		WY * ����� = 0, 1, 0
		WZ * ����� = 0, 0, 1

		WXX WXY WXZ   11 12 13   1 0 0
		WYX WYY WYZ * 21 22 23 = 0 1 0
		WZX WZY WZZ   31 32 33   0 0 1

		�������� �̿��Ͽ� ������ ����� ������� �����ָ� �׵������ ���´�.
		������� �������� �̿��Ͽ� ������ ����� ������� �ȴٴ� ���̴�.

		�������� �̿��Ͽ� ������ ����� �������̱� ������ ���� �������Ѵ�.
		�̷��� ���� �������ϴ� ���� �̿��Ͽ� ������ ����� ��������̶�� �Ѵ�.

		��������� Ư¡ : ����İ� ��ġ����� ����.
		������� �������� �̿��Ͽ� ������ ����� ��ġ��İ� ���ٴ� �ǹ��̴�.
		WXX WXY WXZ    WXX WYX WZX
		WYX WYY WYZ = > WXY WYY WZY
		WZX WZY WZZ    WXZ WYZ WZZ

		ī�޶��� ��ġ : PX, PY, PZ

		�̵����
		1   0   0	0
		0   1   0	0
		0   0   1	0
		-PX -PY -PZ 1

		ȸ�����
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
		m_matView.Identity();	//�׵�
		for (int i = 0; i < 3; ++i)
		{
			Vector3	axis = GetWorldAxis((AXIS)i);
			memcpy(&m_matView[i][0], &axis, sizeof(Vector3));
		}
		m_matView.Transpose();	//��ġ
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
