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

	// ����Ű �¿� �Է� -> ť�� �� �̵�
	void InputMove(InputClass*, float);

	// ����Ű ���� �Է� -> ������ scale ����
	void InputFilterScale(InputClass*, float, float);



private:
	bool Render(float);
	bool RenderSceneToTextureIce(float cubePosX);
	bool RenderSceneToTextureOrigin(float rotation);

private:
	int m_filterMode;	// ���� ��� ������ �����ϴ� ����

	float m_rotation;	// �� ȸ�� ��
	float m_cubePosX;	// �� ��ġ x��

	float m_refractionScale;	// Ice ȿ�� refraction��
	float m_fireBright;		// Fire ȿ�� ��� ��
	XMFLOAT4 m_shiftColor;	// Filter�� ���� RTT ���� ��
	float m_shiftValue;		// Shift ���� ��

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