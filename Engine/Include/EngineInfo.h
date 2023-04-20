#pragma once
#define	DIRECTINPUT_VERSION	0x0800 //include전에 써야 dxinput이 작동한다 
#include <Windows.h>
#include <vector>
#include <list>
#include <unordered_map>
#include <crtdbg.h>
#include <typeinfo>
#include <string>
#include <functional>
#include <algorithm>
#include <stack>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dinput.h>
#include <filesystem>
#include <dwrite_3.h>
#include <d2d1.h>
#include <TCHAR.H>
#include <process.h>
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix.h"
#include "SharedPtr.h"
#include "Resource/Texture/DirectXTex.h"
#include "fmod.hpp"

#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "../Bin/fmodL_vc.lib")

extern float g_DeltaTime;	//extern을 쓰면 다른 외부파일에서 사용할수 있다

#define CAN_MOVE_TIME 1.5f
#define	GRAVITY	9.8f
#define MAX_DISTANCE 50.f
#define MAX_FEVER 1500.f
#define	ROOT_PATH		"Root"
#define	SHADER_PATH		"Shader"
#define	TEXTURE_PATH	"Texture"
#define	SOUND_PATH		"Sound"
#define	FONT_PATH		"Font"
#define	ANIMATION_PATH	"Animation"
#define	SCENE_PATH		"Scene"
#define	SAVE_PATH		"Save"
#define	DEFINITION_SINGLE(type)	type* type::m_Inst = nullptr;
#define	SAFE_DELETE(p)	if(p)	{ delete p; p = nullptr; }
#define	SAFE_DELETE_ARRAY(p)	if(p)	{ delete[] p; p = nullptr; }
#define	SAFE_RELEASE(p)	if(p)	{ p->Release(); p = nullptr;}

#define	DECLARE_SINGLE(type)	\
private:\
	static type*	m_Inst;\
public:\
	static type* GetInst()\
	{\
		if (!m_Inst)\
			m_Inst = new type;\
		return m_Inst;\
	}\
	static void DestroyInst()\
	{\
		if(m_Inst)\
		{\
			delete m_Inst;\
			m_Inst = nullptr;\
		}\
	}\
private:\
	type();\
	~type();

struct Resolution
{
	unsigned int	width;
	unsigned int	height;
};

struct VertexColor	// 위치, 색상 정보를 가지고 있는 정점.
{
	Vector3	pos;
	Vector4	color;

	VertexColor()
	{
	}

	VertexColor(const Vector3& _pos, const Vector4& _color)
		: pos(_pos)
		, color(_color)
	{
	}
};

struct VertexBuffer
{
	ID3D11Buffer* buffer;
	int		size;	// 정점 1개의 크기
	int		count;	// 정점 개수
	void* data;		// 정점 정보

	VertexBuffer() 
		: buffer(nullptr)
		, size(0)
		, count(0)
		, data(nullptr)
	{
	}

	~VertexBuffer()
	{
		SAFE_DELETE_ARRAY(data);
		SAFE_RELEASE(buffer);
	}
};

struct IndexBuffer
{
	ID3D11Buffer* buffer;
	DXGI_FORMAT	fmt;	// 인덱스 포멧
	int		size;		// 인덱스 1개의 크기
	int		count;		// 인덱스 개수
	void* data;			// 인덱스 정보

	IndexBuffer() 
		: buffer(nullptr)
		, size(0)
		, count(0)
		, fmt(DXGI_FORMAT_UNKNOWN)
		, data(nullptr)
	{
	}

	~IndexBuffer()
	{
		SAFE_DELETE_ARRAY(data);
		SAFE_RELEASE(buffer);
	}
};
//struct가 matrix크기에 맞춰서 생성되서 Vector3 pivot을 생성할때 남는 메모리때문에 pivot이 이상해짐 
// 그래서 float 1개 넣어서 크기 맞춰줌
struct TransformCBuffer
{
	Matrix  matWorld;		//월드
	Matrix  matView;		//뷰
	Matrix  matProj;		//투영
	Matrix  matWVP;
	Vector3 pivot;
	float	empty1;
	Vector3 meshSize;
	float	empty2;
};

struct VertexUV
{
	Vector3	pos;
	Vector2	UV;

	VertexUV()
	{
	}

	VertexUV(const Vector3& _Pos, const Vector2& _UV) 
		: pos(_Pos)
		, UV(_UV)
	{
	}
};

struct MaterialCBuffer
{
	Vector4 baseColor;
	Vector4 ambientColor;
	Vector4 specularColor;
	Vector4 emissiveColor;
	float opacity;
	int	textureType;
	float textureWidth;
	float textureHeight;
};

struct HierarchyName
{
	class CComponent* component;
	class CComponent* parent;
	std::string	name;
	std::string	parentName;
	std::string	className;
	std::string	parentClassName;
	HierarchyName()
		: component(nullptr)
		, parent(nullptr)
	{
	}
};

struct HierarchyObjectName
{
	class CGameObject* obj;
	class CGameObject* parent;
	std::string	name;
	std::string	parentName;
	std::string	className;
	std::string	parentClassName;
	HierarchyObjectName()
		: obj(nullptr)
		, parent(nullptr)
	{
	}
};

struct HierarchyWindowName
{
	class CUIWindow* window;
	class CUIWindow* parent;
	std::string	name;
	std::string	parentName;
	std::string	className;
	std::string	parentClassName;
	HierarchyWindowName()
		: window(nullptr)
		, parent(nullptr)
	{
	}
};

struct HierarchyWidgetName
{
	class CUIWidget* widget;
	class CUIWidget* parent;
	std::string	name;
	std::string	parentName;
	std::string	className;
	std::string	parentClassName;
	HierarchyWidgetName()
		: widget(nullptr)
		, parent(nullptr)
	{
	}
};

struct Animation2DFrameData
{
	Vector2	start;
	Vector2	end;
};

struct Animation2DCBuffer
{
	float anim2DImageWidth;
	float anim2DImageHeight;
	Vector2 anim2DFrameStart;
	Vector2 anim2DFrameEnd;
	int anim2DType;
	int anim2DEnable;
	int	anim2DFrame;
	Vector3 anim2DEmpty;
};

struct CollisionChannel
{
	std::string			name;
	ECollision_Channel	channel;
	ECollision_Interaction	interaction;
};

struct CollisionProfile
{
	std::string							name;
	CollisionChannel* channel;
	bool								enable;
	std::vector<ECollision_Interaction>	vecCollisionInteraction;
	CollisionProfile() 
		: enable(true)
		, channel(nullptr)
	{
	}
};

struct CollisionResult
{
	class CCollider* src;
	class CCollider* dest;
	Vector3			hitPoint;
	CollisionResult() 
		: src(nullptr)
		, dest(nullptr)
	{
	}
};

struct ColliderCBuffer
{
	Vector4	color;
	Matrix	matWVP;
};

struct Box2DInfo
{
	float	left;
	float	bottom;
	float	right;
	float	top;
};

struct Sphere2DInfo
{
	Vector2	center;
	float	radius;
};

struct OBB2DInfo
{
	Vector2	center;
	Vector2	axis[(int)AXIS2D::AXIS2D_MAX];
	float	length[(int)AXIS2D::AXIS2D_MAX];
};

struct PixelInfo
{
	ID3D11ShaderResourceView* SRV;
	Box2DInfo	box2D;
	EPixelCollision_Type	pixelColorCollisionType;
	EPixelCollision_Type	pixelAlphaCollisionType;
	std::string	name;
	TCHAR		fileName[MAX_PATH];
	char		pathName[MAX_PATH];
	unsigned char* pixel;
	unsigned int	width;
	unsigned int	height;
	unsigned char	typeColor[4];
	int			refCount;
	PixelInfo() 
		: fileName{}
		, pathName{}
		, refCount(0)
		, pixel(nullptr)
		, box2D{}
		, width(0)
		, height(0)
		, pixelColorCollisionType(EPixelCollision_Type::Alpha_Confirm)
		, pixelAlphaCollisionType(EPixelCollision_Type::None)
		, typeColor{}
	{
	}
};

struct UICBuffer
{
	Vector4 UITint;
	Matrix	UIWVP;
	Vector2 UIMeshSize;
	Vector2 UIPivot;
	int		UITextureEnable;
	float	UIOpacity;
	Vector2	UIEmpty;
	UICBuffer()
		: UITextureEnable(0)
		, UIOpacity(0)
	{
	}
};

struct UIProgressBarCBuffer
{
	int		barDir;
	float	percent;
	Vector2	empty;
};

struct ThreadSyncData
{
	int	header;
	int	size;
	unsigned char data[1024];
};

struct TileMapCBuffer
{
	Vector2	imageSize;
	Vector2	start;
	Vector2	end;
	Vector2	tileSize;
	Matrix	matWVP;
	int		frame;
	Vector3	empty;
};

struct TileInfo
{
	Matrix	matWVP;
	Vector2	start;
	Vector2	end;
	Vector4	typeColor;
	float	opacity;
	int		animationType;
	int		frame;
	float	empty;
	TileInfo()
		: opacity(0.f)
		, animationType(0)
		, frame(0)
		, empty(0.f)
	{
	}
};

struct UserData
{
	int money;
	int cash;
	int mana;
	int score;
	int highScore;
	float time;
	float fastTime;
	int partyIndex;
	std::string charName[36];
	int charCount;
	std::string holdCharNames[30];
	UserData()
		: money(0)
		, cash(0)
		, mana(0)
		, score(0)
		, highScore(0)
		, partyIndex(0)
		, charCount(0)
		, time(0.f)
		, fastTime(0.f)
	{
	}
	UserData& operator = (const UserData& v)
	{
		money = v.money;
		cash = v.cash;
		mana = v.mana;
		score = v.score;
		highScore = v.highScore;
		time = v.time;
		fastTime = v.fastTime;
		partyIndex = v.partyIndex;
		charCount = v.charCount;
		size_t size = std::size(charName);
		for (size_t i = 0; i < size; ++i)
		{
			charName[i] = v.charName[i];
		}
		for (int i = 0; i < charCount; ++i)
		{
			holdCharNames[i] = v.holdCharNames[i];
		}
		return *this;
	}
	bool operator == (const UserData& v)	const
	{
		if (money != v.money || cash != v.cash || mana != v.mana || partyIndex != v.partyIndex || charCount != v.charCount
			|| score != v.score || highScore != v.highScore || time != v.time || fastTime != v.fastTime)
		{
			return false;
		}
		size_t size = std::size(charName);
		for (size_t i = 0; i < size; ++i)
		{
			if (charName[i] != v.charName[i])
			{
				return false;
			}
		}
		for (int i = 0; i < charCount; ++i)
		{
			if (holdCharNames[i] != v.holdCharNames[i])
			{
				return false;
			}
		}
		return true;
	}
	bool operator != (const UserData& v)	const
	{
		if (money != v.money || cash != v.cash || mana != v.mana || partyIndex != v.partyIndex || charCount != v.charCount
			|| score != v.score || highScore != v.highScore || time != v.time || fastTime != v.fastTime)
		{
			return true;
		}
		size_t size = std::size(charName);
		for (size_t i = 0; i < size; ++i)
		{
			if (charName[i] != v.charName[i])
			{
				return true;
			}
		}
		for (int i = 0; i < charCount; ++i)
		{
			if (holdCharNames[i] != v.holdCharNames[i])
			{
				return true;
			}
		}
		return false;
	}
};

struct ObjectData
{
	int hp;
	int maxHp;
	int attack;
	int defense;
	EElement element;
	ETribe tribe;
	float speed;
	float maxSpeed;
	ObjectData()
		: hp(0)
		, attack(0)
		, defense(0)
		, element(EElement::None)
		, tribe(ETribe::None)
		, speed(0.f)
	{
		maxHp = hp;
		maxSpeed = speed;
	}
	void operator += (const ObjectData& v)
	{
		hp += v.hp;
		attack += v.attack;
		defense += v.defense;
	}
};

struct StatusData
{
	bool isBuff[(int)EBuff::End];
	float buffTime[(int)EBuff::End];
	float buffMaxTime[(int)EBuff::End];
	float buffQuantity[(int)EBuff::End];

	bool isDeBuff[(int)EDeBuff::End];
	float deBuffTime[(int)EDeBuff::End];
	float deBuffMaxTime[(int)EDeBuff::End];
	float deBuffQuantity[(int)EDeBuff::End];
	StatusData()
		: isBuff{}
		, buffTime{}
		, buffMaxTime{}
		, buffQuantity{}
		, isDeBuff{}
		, deBuffTime{}
		, deBuffMaxTime{}
		, deBuffQuantity{}
	{
	}
};