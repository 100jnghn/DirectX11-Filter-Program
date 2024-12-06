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


private:
	bool Render(float);
	bool RenderSceneToTextureIce(float cubePosX);

private:
	int m_filterMode;	// 필터 모드 정보를 저장하는 변수
	float m_refractionScale;	// Ice 효과 refraction값

	D3DClass* m_Direct3D;
	CameraClass* m_Camera;
	NormalMapShaderClass* m_NormalMapShader;
	
	ModelClass* m_Model;
	ModelClass* m_IceModel;

	RenderTextureClass* m_RenderTextureIce;

	GlassShaderClass* m_GlassShader;

	LightClass* m_Light;
	LightClass* m_LightPhong;
};

#endif