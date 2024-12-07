////////////////////////////////////////////////////////////////////////////////
// Filename: applicationclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "applicationclass.h"


ApplicationClass::ApplicationClass()
{
	// ���� �� ���� ��� 0���� �ʱ�ȭ
	m_filterMode = 0;

	m_refractionScale = 0.02f;



	m_Direct3D = 0;
	m_Camera = 0;

	m_NormalMapShader = 0;
	m_GlassShader = 0;

	m_Model = 0;
	m_IceModel = 0;
	m_FireModel = 0;

	m_RenderTextureIce = 0;
	
	m_Light = 0;
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
    m_Light = new LightClass;

    m_Light->SetDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);	// ������
	m_Light->SetPosition(1.0f, 100.0f, 0.0f);
    m_Light->SetDirection(-2.0f, -1.0f, 3.0f);
	// ----------------- //



	// ----- Phong ���� �𵨷� ��� ----- //
	m_LightPhong = new LightClass;

	m_LightPhong->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);	// �Ͼ��
	m_LightPhong->SetPosition(-1.0f, 100.0f, 0.0f);
	m_LightPhong->SetDirection(2.0f, -1.0f, 3.0f);
	// -------------------------------- //



	

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

	result = m_FireModel->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFilename, (WCHAR*)L"data/flame01.jpg", (WCHAR*)L"data/flame02.jpg", NULL);
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

	return true;
}


void ApplicationClass::Shutdown()
{
	if (m_RenderTextureIce)	{
		m_RenderTextureIce->Shutdown();
		delete m_RenderTextureIce;
		m_RenderTextureIce = 0;
	}

	// Release the light object.
    if(m_Light)
    {
        delete m_Light;
        m_Light = 0;
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
	if (m_GlassShader) {
		m_GlassShader->Shutdown();
		delete m_GlassShader;
		m_GlassShader = 0;
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
		if (m_filterMode == 1) {
			m_refractionScale += 0.01f;

			// max refraction scale: 0.3f
			if (m_refractionScale >= 0.3f) {
				m_refractionScale = 0.3f;
			}
		}
		
	}

	if (Input->IsDownArrowPressed()) {
		if (m_filterMode == 1) {
			m_refractionScale -= 0.01f;

			// min refraction scale: 0.0f
			if (m_refractionScale <= 0.0f) {
				m_refractionScale = 0.0f;
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



	// Update the rotation variable each frame.
    rotation -= 0.0174532925f * 0.25f;
    if(rotation <= 0.0f)
    {
        rotation += 360.0f;
    }

	// ----- ���� Rotation Model RTT ----- //



	// ----- Filter Mode�� ���� RTT ��ȭ ----- //
	// mode1 -> Ice Filter
	if (m_filterMode == 1) {
		result = RenderSceneToTextureIce(cubePosX);

		if (!result)
		{
			return false;
		}
	}
	// mode2 -> Fire Filter
	else if (m_filterMode == 2) {
		
	}
	// -------------------------------------- //
	
	

	
	// Render the graphics scene.
	// � filter mode�̴� ��� Render()�� �����ؾ� �ϹǷ�(Cube Model�� ��Ÿ���� �ϱ� ����?) ���ǹ� ���� �� Render() �����ϴ� �ڵ�� �ۼ�
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
										m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_LightPhong->GetDirection(), m_LightPhong->GetDiffuseColor());
	if (!result)
	{
		return false;
	}

	// Reset the render target back to the original back buffer and not the render to texture anymore.  And reset the viewport back to the original.
	m_Direct3D->SetBackBufferRenderTarget();
	m_Direct3D->ResetViewport();

	



	return true;
}

bool ApplicationClass::Render(float cubePosX)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	//float refractionScale = 0.02f;


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
									   m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_LightPhong->GetDirection(), m_LightPhong->GetDiffuseColor());
	if(!result)
	{
		return false;
	}



	// ----- Filter Mode�� ���� Render ��ȭ ----- //

	// Ice �� ����
	if (m_filterMode == 1) {
		// ----- Ice Model Render ----- //
		worldMatrix = XMMatrixScaling(3.0f, 3.0f, 3.0f);
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
		worldMatrix = XMMatrixScaling(3.0f, 3.0f, 3.0f);
		worldMatrix *= XMMatrixTranslation(0.0f, 0.0f, -1.1f);

		m_FireModel->Render(m_Direct3D->GetDeviceContext());

		static float f = 0.0f;
		f += 0.001f;

		result = m_FireShader->Render(m_Direct3D->GetDeviceContext(), m_FireModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
								m_FireModel->GetTexture(0), m_FireModel->GetTexture(1), f);
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