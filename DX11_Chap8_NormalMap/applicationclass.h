////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _APPLICATIONCLASS_H_
#define _APPLICATIONCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "inputclass.h"
#include "cameraclass.h"
#include "normalmapshaderclass.h"
#include "modelclass.h"
#include "lightclass.h"
#include "rendertextureclass.h"
#include "glassshaderclass.h"
#include "fireshaderclass.h"
#include "displayplaneclass.h"
#include "textureshaderclass.h"
#include "originnormalshaderclass.h"



/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.3f;


////////////////////////////////////////////////////////////////////////////////
// Class name: ApplicationClass
////////////////////////////////////////////////////////////////////////////////
class ApplicationClass
{
public:
	ApplicationClass();
	ApplicationClass(const ApplicationClass&);
	~ApplicationClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(InputClass*);

	// 방향키 좌우 입력 -> 큐브 모델 이동
	void InputMove(InputClass*, float);

	// 방향키 상하 입력 -> 필터의 scale 조절
	void InputFilterScale(InputClass*, float, float);



private:
	bool Render(float);
	bool RenderSceneToTextureIce(float cubePosX);
	bool RenderSceneToTextureOrigin(float rotation);

private:
	int m_filterMode;	// 필터 모드 정보를 저장하는 변수

	float m_rotation;	// 모델 회전 값
	float m_cubePosX;	// 모델 위치 x값

	float m_refractionScale;	// Ice 효과 refraction값
	float m_fireBright;		// Fire 효과 밝기 값
	XMFLOAT4 m_shiftColor;	// Filter에 따른 RTT 전달 값
	float m_shiftValue;		// Shift 증가 값

	D3DClass* m_Direct3D;
	CameraClass* m_Camera;

	NormalMapShaderClass* m_NormalMapShader;
	OriginNormalShaderClass* m_OriginNormalShader;
	GlassShaderClass* m_GlassShader;
	FireShaderClass* m_FireShader;
	TextureShaderClass* m_TextureShader;

	ModelClass* m_Model;
	ModelClass* m_IceModel;
	ModelClass* m_FireModel;
	DisplayPlaneClass* m_DisplayPlane;

	RenderTextureClass* m_RenderTextureIce;
	RenderTextureClass* m_RenderTextureOrigin;

	LightClass* m_Light1;
	LightClass* m_Light2;
	LightClass* m_LightPhong;
};

#endif