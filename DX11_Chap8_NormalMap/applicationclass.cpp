////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "applicationclass.h"



ApplicationClass::ApplicationClass()
{
	// ���� �� ���� ��� 0���� �ʱ�ȭ
	m_filterMode = 0;

	m_refractionScale = 0.02f;
	m_fireBright = 1.3f;

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
	// ----- ����1 ----- //
    m_Light1 = new LightClass;

    m_Light1->SetDiffuseColor(0.5f, 0.0f, 0.0f, 1.0f);	// ������
	//m_Light1->SetPosition(1.0f, 100.0f, 0.0f);
    m_Light1->SetDirection(-2.0f, -1.0f, 3.0f);
	// ----------------- //



	// ----- Phong ���� �𵨷� ��� ----- //
	m_Light2 = new LightClass;

	m_Light2->SetDiffuseColor(0.0f, 0.0f, 0.5f, 1.0f);	// Ǫ����
	//m_Light2->SetPosition(-1.0f, 100.0f, 0.0f);
	m_Light2->SetDirection(2.0f, -1.0f, 3.0f);
	// -------------------------------- //



	// ----- Original Model ���� (Phong) ----- // 
	m_LightPhong = new LightClass;

	m_LightPhong->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);	// �Ͼ��
	//m_LightPhong->SetPosition(0.0f, 100.0f, 0.0f);
	m_LightPhong->SetDirection(0.0f, -3.0f, 0.0f);
	// --------------------------------------- //


	

	// ----- Ice RTT �ʱ�ȭ ----- //
	strcpy_s(modelFilename, "data/square.txt");			// Ice�� ���� �� (�׸�)
	//strcpy_s(textureFilename1, "data/ice01.tga");		// IceImg
	//strcpy_s(textureFilename2, "data/icebump01.tga");	// IceBump

	// �� ����
	m_IceModel = new ModelClass;

	result = m_IceModel->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFilename, (WCHAR*)L"data/ice01.jpg", (WCHAR*)L"data/icebump01.jpg", NULL);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the window model object.", L"Error", MB_OK);
		return false;
	}



	// Render Texture ����
	m_RenderTextureIce = new RenderTextureClass;

	result = m_RenderTextureIce->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR, 1);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the render texture object.", L"Error", MB_OK);
		return false;
	}



	// Ice Shader ����
	m_GlassShader = new GlassShaderClass;

	result = m_GlassShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the glass shader object.", L"Error", MB_OK);
		return false;
	}
	// ------------ Ice RTT �ʱ�ȭ �� -------------- //
	




	// ----- Fire RTT �ʱ�ȭ ----- //
	// �� ����
	m_FireModel = new ModelClass;

	result = m_FireModel->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFilename, (WCHAR*)L"data/flame01.jpg", (WCHAR*)L"data/flame02.jpg", (WCHAR*)L"data/flameAlpha.png");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the window model object.", L"Error", MB_OK);
		return false;
	}



	// shader ����
	m_FireShader = new FireShaderClass;

	result = m_FireShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the fire shader object.", L"Error", MB_OK);
		return false;
	}

	// ------------ Fire RTT �ʱ�ȭ �� -------------- //



	// ---------- Original Renter Texture ���� ---------- //
	// ���ܿ� ���� ȿ���� �� Original Model Render 
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


bool ApplicationClass::Frame(InputClass* Input)
{
	bool result;

	// �� ȸ�� ��
	static float rotation = 360.0f;

	// cube ���� x��ǥ ���� �����ϴ� ����
	static float cubePosX = 0.0f;



	// ---------- ����� Input ���� ---------- //

	if(Input->IsEscapePressed())
	{
		return false;
	}

	// ----- cube �� �¿� �̵� ----- //
	if (Input->IsLeftArrowPressed()) {
		cubePosX -= 0.1f;
	}

	if (Input->IsRightArrowPressed()) {
		cubePosX += 0.1f;
	}
	// ------------------------------ //

	// ----- refractionScale ���� ----- //
	if (Input->IsUpArrowPressed()) {

		// Ice Refraction ����
		if (m_filterMode == 1) {
			m_refractionScale += 0.01f;

			// max refraction scale: 0.3f
			if (m_refractionScale >= 0.3f) {
				m_refractionScale = 0.3f;
			}
		}
		// Fire Bright ����
		else if (m_filterMode == 2) {
			m_fireBright -= 0.01f;

			// min bright value: 0.8f;
			if (m_fireBright <= 0.8f) {
				m_fireBright = 0.8f;
			}
		}
	}

	if (Input->IsDownArrowPressed()) {

		// Ice Refraction ����
		if (m_filterMode == 1) {
			m_refractionScale -= 0.01f;

			// min refraction scale: 0.0f
			if (m_refractionScale <= 0.0f) {
				m_refractionScale = 0.0f;
			}
		}
		// Fire Bright ����
		else if (m_filterMode == 2) {
			m_fireBright += 0.01f;

			// max bright value: 1.7f
			if (m_fireBright >= 1.7f) {
				m_fireBright = 1.7f;
			}
		}
	}
	// ----------------------------------- //


	// ----- ���� ������ filter mode ���� ----- //
	// mode1 (Ice)
	if (Input->IsNum1Pressed()) {
		m_filterMode = 1;
	}

	// mode2 (Fire)
	if (Input->IsNum2Pressed()) {
		m_filterMode = 2;
	}

	// mode0 (Nothing) - base
	if (Input->IsNum0Pressed()) {
		m_filterMode = 0;
	}

	// ----------- ����� Input ���� �� ----------- //



	// ----- Filter Mode�� ���� RTT ��ȭ ----- //
	// mode1 -> Ice Filter
	if (m_filterMode == 1) {
		result = RenderSceneToTextureIce(cubePosX);

		if (!result)
		{
			return false;
		}
	}
	// -------------------------------------- //
	
	

	
	// Render the graphics scene.
	// � filter mode�̴� ��� Render()�� �����ؾ� �ϹǷ�(Cube Model�� ��Ÿ���� �ϱ� ����?) ���ǹ� ���� �� Render() �����ϴ� �ڵ�� �ۼ�
	//result = Render(cubePosX);
	//if (!result)
	//{
	//	return false;
	//}



	// Update the rotation variable each frame.
	rotation -= 0.0174532925f * 0.25f;
	if (rotation <= 0.0f)
	{
		rotation += 360.0f;
	}

	

	// ----- ���� Rotation Model RTT ----- //
	result = RenderSceneToTextureOrigin(rotation);

	if (!result) {
		return false;
	}

	result = Render(cubePosX);
	if (!result)
	{
		return false;
	}

	return true;
}

// ----- Ice RTT �Լ� ���� ----- //
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


	// ----- ����Ű Input�� ���� worldMatrix ���� ----- //
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
	m_RenderTextureOrigin->ClearRenderTarget(m_Direct3D->GetDeviceContext(), 0.5f, 0.5f, 0.5f, 1.0f);	// ��� �� ����

	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);
	m_Camera->Render();

	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_RenderTextureOrigin->GetProjectionMatrix(projectionMatrix);



	worldMatrix = XMMatrixRotationRollPitchYaw(0.0f, -1.3f + rotation, 0.0f);


	m_Model->Render(m_Direct3D->GetDeviceContext());

	result = m_OriginNormalShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
										m_Model->GetTexture(0), m_Model->GetTexture(1),
										m_LightPhong->GetDirection(), m_LightPhong->GetDiffuseColor());

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

	
	
	//// ���� ȸ��
	//static float yRot = 0.0f;
	//yRot += 0.02f;
	//m_Light->SetDirection(0.0f, sinf(yRot), 1.0f);	// sinf, cosf
	

	// WorldMatrix ������ �� ȸ��
    //worldMatrix = XMMatrixRotationY(rotation);



	// ----- ����Ű Input�� ���� worldMatrix ���� ----- //
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





	// ----- Filter Mode�� ���� Render ��ȭ ----- //

	// Ice �� ����
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
	// Fire �� ����
	else if (m_filterMode == 2) {
		m_Direct3D->EnableAlphaBlending();

		// ----- Fire Model Render ----- //
		worldMatrix = XMMatrixScaling(2.0f, 2.0f, 2.0f);
		worldMatrix *= XMMatrixTranslation(0.0f, 0.0f, -1.1f);

		m_FireModel->Render(m_Direct3D->GetDeviceContext());

		// ----- Fire Filter�� Texture�� �̵� �� ----- //
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