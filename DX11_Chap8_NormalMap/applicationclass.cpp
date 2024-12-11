////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "applicationclass.h"
#include "iostream"
using namespace std;



ApplicationClass::ApplicationClass()
{
	// 시작 시 필터 모드 0으로 초기화
	m_filterMode = 0;

	m_rotation = 360.0f;
	m_cubePosX = 0.0f;

	m_refractionScale = 0.02f;
	m_fireBright = 1.3f;
	m_shiftColor = { 1.0f, 1.0f, 1.0f, 1.0f };	// 하얀색으로 초기화
	m_shiftValue = 0.0005f;						// ex) 빨간색을 원하면: shiftColor의 g,b -= shiftValue

	m_Direct3D = 0;
	m_Camera = 0;

	m_NormalMapShader = 0;
	m_OriginNormalShader = 0;
	m_GlassShader = 0;
	m_TextureShader = 0;

	m_Model = 0;
	m_IceModel = 0;
	m_FireModel = 0;
	m_DisplayPlane = 0;

	m_RenderTextureIce = 0;
	m_RenderTextureOrigin = 0;
	
	m_Light1 = 0;
	m_Light2 = 0;
	m_LightPhong = 0;
}


ApplicationClass::ApplicationClass(const ApplicationClass& other)
{
}


ApplicationClass::~ApplicationClass()
{
}


bool ApplicationClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	char modelFilename[128], textureFilename1[128], textureFilename2[128];
	bool result;


	// Create and initialize the Direct3D object.
	m_Direct3D = new D3DClass;

	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// Create and initialize the camera object.
	m_Camera = new CameraClass;

	m_Camera->SetPosition(0.0f, 1.0f, -15.0f);
	m_Camera->Render();

	// Create and initialize the normal map shader object.
	m_NormalMapShader = new NormalMapShaderClass;

	result = m_NormalMapShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the normal map shader object.", L"Error", MB_OK);
		return false;
	}

	// Set the file name of the model.
    strcpy_s(modelFilename, "data/cube.txt");

    // Set the file name of the textures.
    strcpy_s(textureFilename1, "data/block.tga");
    strcpy_s(textureFilename2, "data/blockNormal.tga");

    // Create and initialize the model object.
    m_Model = new ModelClass;

    result = m_Model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFilename, (WCHAR*)L"data/block.jpg", (WCHAR*)L"data/blockNormal.jpg", NULL);
    if(!result)
    {
        return false;
    }

	// Create and initialize the light object.
	// ----- 조명1 ----- //
    m_Light1 = new LightClass;

    m_Light1->SetDiffuseColor(1.0f, 0.2f, 0.2f, 1.0f);	// 붉은색
	//m_Light1->SetPosition(1.0f, 100.0f, 0.0f);
    m_Light1->SetDirection(-2.0f, -1.0f, 3.0f);
	// ----------------- //



	// ----- 조명2 ----- //
	m_Light2 = new LightClass;

	m_Light2->SetDiffuseColor(0.2f, 0.2f, 1.0f, 1.0f);	// 푸른색
	//m_Light2->SetPosition(-1.0f, 100.0f, 0.0f);
	m_Light2->SetDirection(2.0f, -1.0f, 3.0f);
	// -------------------------------- //



	// ----- Original Model 조명 (Phong) ----- // 
	m_LightPhong = new LightClass;

	m_LightPhong->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);	// 하얀색
	//m_LightPhong->SetPosition(0.0f, 100.0f, 0.0f);
	m_LightPhong->SetDirection(0.0f, -3.0f, 0.0f);
	// --------------------------------------- //


	

	// ----- Ice RTT 초기화 ----- //
	strcpy_s(modelFilename, "data/square.txt");			// Ice를 입힐 모델 (네모)
	//strcpy_s(textureFilename1, "data/ice01.tga");		// IceImg
	//strcpy_s(textureFilename2, "data/icebump01.tga");	// IceBump

	// 모델 생성
	m_IceModel = new ModelClass;

	result = m_IceModel->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFilename, (WCHAR*)L"data/ice01.jpg", (WCHAR*)L"data/icebump01.jpg", NULL);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the window model object.", L"Error", MB_OK);
		return false;
	}



	// Render Texture 생성
	m_RenderTextureIce = new RenderTextureClass;

	result = m_RenderTextureIce->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR, 1);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the render texture object.", L"Error", MB_OK);
		return false;
	}



	// Ice Shader 생성
	m_GlassShader = new GlassShaderClass;

	result = m_GlassShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the glass shader object.", L"Error", MB_OK);
		return false;
	}
	// ------------ Ice RTT 초기화 끝 -------------- //
	




	// ----- Fire RTT 초기화 ----- //
	// 모델 생성
	m_FireModel = new ModelClass;

	result = m_FireModel->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFilename, (WCHAR*)L"data/flame01.jpg", (WCHAR*)L"data/flame02.jpg", (WCHAR*)L"data/flameAlpha.png");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the window model object.", L"Error", MB_OK);
		return false;
	}



	// shader 생성
	m_FireShader = new FireShaderClass;

	result = m_FireShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the fire shader object.", L"Error", MB_OK);
		return false;
	}

	// ------------ Fire RTT 초기화 끝 -------------- //



	// ---------- Original Renter Texture 생성 ---------- //
	// 우상단에 필터 효과를 뺀 Original Model Render 
	m_DisplayPlane = new DisplayPlaneClass;

	result = m_DisplayPlane->Initialize(m_Direct3D->GetDevice(), 1.0f, 0.75f);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the window model object.", L"Error", MB_OK);
		return false;
	}



	m_RenderTextureOrigin = new RenderTextureClass;
	
	result = m_RenderTextureOrigin->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR, 1);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the render texture object.", L"Error", MB_OK);
		return false;
	}



	m_TextureShader = new TextureShaderClass;

	result = m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		return false;
	}



	m_OriginNormalShader = new OriginNormalShaderClass;

	result = m_OriginNormalShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the origin normal map shader object.", L"Error", MB_OK);
		return false;
	}





	return true;
}


void ApplicationClass::Shutdown()
{
	if (m_RenderTextureIce)	{
		m_RenderTextureIce->Shutdown();
		delete m_RenderTextureIce;
		m_RenderTextureIce = 0;
	}

	if (m_RenderTextureOrigin) {
		m_RenderTextureOrigin->Shutdown();
		delete m_RenderTextureOrigin;
		m_RenderTextureOrigin = 0;
	}

	// Release the light object.
    if(m_Light1)
    {
        delete m_Light1;
        m_Light1 = 0;
    }
	if (m_Light2)
	{
		delete m_Light2;
		m_Light2 = 0;
	}
	if (m_LightPhong)
	{
		delete m_LightPhong;
		m_LightPhong = 0;
	}

	// Release the model object.
    if(m_Model)
    {
        m_Model->Shutdown();
        delete m_Model;
        m_Model = 0;
    }
	if (m_IceModel)
	{
		m_IceModel->Shutdown();
		delete m_IceModel;
		m_IceModel = 0;
	}
	if (m_FireModel)
	{
		m_FireModel->Shutdown();
		delete m_FireModel;
		m_FireModel = 0;
	}

	// Release the normal map shader object.
	if(m_NormalMapShader)
	{
		m_NormalMapShader->Shutdown();
		delete m_NormalMapShader;
		m_NormalMapShader = 0;
	}
	if (m_OriginNormalShader)
	{
		m_OriginNormalShader->Shutdown();
		delete m_OriginNormalShader;
		m_OriginNormalShader = 0;
	}
	if (m_GlassShader) {
		m_GlassShader->Shutdown();
		delete m_GlassShader;
		m_GlassShader = 0;
	}
	if (m_FireShader) {
		m_FireShader->Shutdown();
		delete m_FireShader;
		m_FireShader = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the Direct3D object.
	if(m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	return;
}

void ApplicationClass::InputMove(InputClass* Input, float value) {

	// cube 모델 좌우 이동
	if (Input->IsLeftArrowPressed()) {
		m_cubePosX -= value;
	}

	if (Input->IsRightArrowPressed()) {
		m_cubePosX += value;
	}
}

void ApplicationClass::InputFilterScale(InputClass* Input, float value, float shiftValue) {

	if (Input->IsUpArrowPressed()) {

		switch (m_filterMode) {
			case 1:
				m_refractionScale += value;

				// max refraction scale: 0.3f
				if (m_refractionScale >= 0.3f) {
					m_refractionScale = 0.3f;
				}

				// shift color 증가 값
				m_shiftValue += shiftValue;
				if (m_shiftValue >= 0.001f) {
					m_shiftValue = 0.001f;
				}

				break;

			case 2:
				m_fireBright -= value;

				// min bright value: 0.8f;
				if (m_fireBright <= 0.8f) {
					m_fireBright = 0.8f;
				}

				// shift color 증가 값
				m_shiftValue += shiftValue;
				if (m_shiftValue >= 0.001f) {
					m_shiftValue = 0.001f;
				}

				break;

			default:
				break;
		}
	}

	if (Input->IsDownArrowPressed()) {

		switch (m_filterMode) {
			case 1:
				m_refractionScale -= value;

				// min refraction scale: 0.0f
				if (m_refractionScale <= 0.0f) {
					m_refractionScale = 0.0f;
				}

				// shift color 감소 값
				m_shiftValue -= shiftValue;
				if (m_shiftValue <= 0.0005f) {
					m_shiftValue = 0.0005f;
				}

				break;
				
			case 2:
				m_fireBright += value;

				// max bright value: 1.7f
				if (m_fireBright >= 1.7f) {
					m_fireBright = 1.7f;
				}

				// shift color 감소 값
				m_shiftValue -= shiftValue;
				if (m_shiftValue <= 0.0005f) {
					m_shiftValue = 0.0005f;
				}

				break;

			default:
				break;
		}
	}
}

void ApplicationClass::InputFilterMode(InputClass* Input, int filterMode, float shiftValue) {
	
	m_filterMode = filterMode;

	m_shiftColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_shiftValue = shiftValue;	// (0.005f)
}

//void ApplicationClass::ChangeFilter() {
//
//}

bool ApplicationClass::Frame(InputClass* Input)
{
	bool result;

	// ---------- 사용자 Input 관리 ---------- //

	if(Input->IsEscapePressed())
	{
		return false;
	}

	// 큐브 움직임 (방향키 좌우)
	InputMove(Input, 0.1f);

	// FilterMode에 따른 Refraction scale, Fire bright 조정
	InputFilterScale(Input, 0.01f, 0.001f);

	


	// 숫자 눌러서 filter mode 변경
	if (Input->IsNum1Pressed()) {	// mode1 -> Ice Filter
		InputFilterMode(Input, 1, 0.005f);
	}

	if (Input->IsNum2Pressed()) {	// mode2 -> Fire Filter
		InputFilterMode(Input, 2, 0.005f);
	}

	if (Input->IsNum0Pressed()) {	// mode0 -> basic
		InputFilterMode(Input, 0, 0.005f);
	}

	// ----------- 사용자 Input 관리 끝 ----------- //



	// ----- Filter Mode에 따른 RTT 변화 ----- //
	// mode1 -> Ice Filter
	if (m_filterMode == 1) {
		result = RenderSceneToTextureIce(m_cubePosX);

		if (!result)
		{
			return false;
		}


		
		// 모델의 x좌표 값이 범위 안에 있다면 파랗게
		if (m_cubePosX >= -1.0f && m_cubePosX <= 1.0f) {

			// shift color 연산 수행 -> 파랗게
			m_shiftColor.x -= m_shiftValue;
			m_shiftColor.y -= m_shiftValue;
			m_shiftColor.z = 1.0f;
		}

		
	}
	// mode2 -> Fire Filter
	else if (m_filterMode == 2) {
		
		// 모델의 x좌표 값이 범위 안에 있다면 빨갛게
		if (m_cubePosX >= -1.0f && m_cubePosX <= 1.0f) {

			// shift color 연산 수행 -> 빨갛게
			m_shiftColor.x = 1.0f;
			m_shiftColor.y -= m_shiftValue;
			m_shiftColor.z -= m_shiftValue;
		}
		
		
	}
	else if (m_filterMode == 0) {
		
	}
	// -------------------------------------- //
	
	

	
	// Render the graphics scene.
	// 어떤 filter mode이던 모두 Render()를 수행해야 하므로(Cube Model을 나타내야 하기 때문?) 조건문 수행 후 Render() 수행하는 코드로 작성
	//result = Render(cubePosX);
	//if (!result)
	//{
	//	return false;
	//}



	// Update the rotation variable each frame.
	m_rotation -= 0.0174532925f * 0.25f;
	if (m_rotation <= 0.0f)
	{
		m_rotation += 360.0f;
	}

	

	// ----- 우상단 Rotation Model RTT ----- //
	result = RenderSceneToTextureOrigin(m_rotation);

	if (!result) {
		return false;
	}

	result = Render(m_cubePosX);
	if (!result)
	{
		return false;
	}

	return true;
}

// ----- Ice RTT 함수 정의 ----- //
bool ApplicationClass::RenderSceneToTextureIce(float cubePosX) {
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;


	// Set the render target to be the render to texture and clear it.
	m_RenderTextureIce->SetRenderTarget(m_Direct3D->GetDeviceContext());
	m_RenderTextureIce->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Rotate the world matrix by the rotation value so that the cube will spin.
	//worldMatrix = XMMatrixRotationY(rotation);


	// ----- 방향키 Input에 따른 worldMatrix 변경 ----- //
	worldMatrix = XMMatrixTranslation(cubePosX, 0.0f, 0.0f);
	// ----------------------------------------------- //
	
	// Render the cube model using the texture shader.
	m_Model->Render(m_Direct3D->GetDeviceContext());
	result = m_NormalMapShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
										m_Model->GetTexture(0), m_Model->GetTexture(1),
										m_Light1->GetDirection(), m_Light1->GetDiffuseColor(), m_Light2->GetDirection(), m_Light2->GetDiffuseColor());
	if (!result)
	{
		return false;
	}

	// Reset the render target back to the original back buffer and not the render to texture anymore.  And reset the viewport back to the original.
	m_Direct3D->SetBackBufferRenderTarget();
	m_Direct3D->ResetViewport();

	return true;
}

bool ApplicationClass::RenderSceneToTextureOrigin(float rotation) {
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	bool result;

	m_RenderTextureOrigin->SetRenderTarget(m_Direct3D->GetDeviceContext());
	m_RenderTextureOrigin->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.5f, 0.5f, 0.5f, 1.0f);	// 배경 색 지정

	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);
	m_Camera->Render();

	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_RenderTextureOrigin->GetProjectionMatrix(projectionMatrix);



	worldMatrix = XMMatrixRotationRollPitchYaw(0.0f, -1.3f + rotation, 0.0f);


	m_Model->Render(m_Direct3D->GetDeviceContext());

	result = m_OriginNormalShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
										m_Model->GetTexture(0), m_Model->GetTexture(1),
										m_LightPhong->GetDirection(), m_LightPhong->GetDiffuseColor(),
										m_shiftColor);

	if (!result)
	{
		return false;
	}

	m_Direct3D->SetBackBufferRenderTarget();
	m_Direct3D->ResetViewport();

	

	return true;

}

bool ApplicationClass::Render(float cubePosX)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	m_Camera->Render();

	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	
	
	//// 조명 회전
	//static float yRot = 0.0f;
	//yRot += 0.02f;
	//m_Light->SetDirection(0.0f, sinf(yRot), 1.0f);	// sinf, cosf
	

	// WorldMatrix 값으로 모델 회전
    //worldMatrix = XMMatrixRotationY(rotation);



	// ----- 방향키 Input에 따른 worldMatrix 변경 ----- //
	worldMatrix = XMMatrixTranslation(cubePosX, 0.0f, 0.0f);
	// ----------------------------------------------- //



	// Render the model using the normal map shader.
	m_Model->Render(m_Direct3D->GetDeviceContext());

	result = m_NormalMapShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
									   m_Model->GetTexture(0), m_Model->GetTexture(1), 
									   m_Light1->GetDirection(), m_Light1->GetDiffuseColor(), m_Light2->GetDirection(), m_Light2->GetDiffuseColor());
	if(!result)
	{
		return false;
	}


	// Setup matrices - Top display plane.
	worldMatrix = XMMatrixTranslation(4.0f, 2.5f, 0.0f);

	// Render the display plane using the texture shader and the render texture resource.
	m_DisplayPlane->Render(m_Direct3D->GetDeviceContext());
	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_DisplayPlane->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_RenderTextureOrigin->GetShaderResourceView());
	if (!result)
	{
		return false;
	}





	// ----- Filter Mode에 따른 Render 변화 ----- //

	// Ice 모델 렌더
	if (m_filterMode == 1) {
		// ----- Ice Model Render ----- //
		worldMatrix = XMMatrixScaling(2.0f, 2.0f, 2.0f);
		worldMatrix *= XMMatrixTranslation(0.0f, 0.0f, -1.1f);

		m_IceModel->Render(m_Direct3D->GetDeviceContext());

		result = m_GlassShader->Render(m_Direct3D->GetDeviceContext(), m_IceModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_IceModel->GetTexture(0),
			m_IceModel->GetTexture(1), m_RenderTextureIce->GetShaderResourceView(), m_refractionScale);
		if (!result)
		{
			return false;
		}
	}
	// Fire 모델 렌더
	else if (m_filterMode == 2) {
		m_Direct3D->EnableAlphaBlending();

		// ----- Fire Model Render ----- //
		worldMatrix = XMMatrixScaling(2.0f, 2.0f, 2.0f);
		worldMatrix *= XMMatrixTranslation(0.0f, 0.0f, -1.1f);

		m_FireModel->Render(m_Direct3D->GetDeviceContext());

		// ----- Fire Filter의 Texture들 이동 값 ----- //
		static float trans = 0.0f;
		trans += 0.001f;

		if (trans >= 1.0f) {
			trans = 0.0f;
		}
		// ------------------------------------------ //

		result = m_FireShader->Render(m_Direct3D->GetDeviceContext(), m_FireModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
								m_FireModel->GetTexture(0), m_FireModel->GetTexture(1), m_FireModel->GetTexture(2), trans, m_fireBright);
		if (!result)
		{
			return false;
		}

		m_Direct3D->DisableAlphaBlending();
	}



	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}