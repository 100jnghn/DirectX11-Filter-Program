////////////////////////////////////////////////////////////////////////////////
// Filename: textureclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "textureclass.h"
#include "WICTextureLoader.h"


TextureClass::TextureClass()
{
	m_targaData = 0;
	m_texture = 0;
	m_textureView = 0;
}


TextureClass::TextureClass(const TextureClass& other)
{
}


TextureClass::~TextureClass()
{
}


bool TextureClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* filename)
{
	DirectX::CreateWICTextureFromFile(device, filename, NULL, &m_textureView);

	return true;
}


void TextureClass::Shutdown()
{
	// Release the texture view resource.
	if(m_textureView)
	{
		m_textureView->Release();
		m_textureView = 0;
	}

	// Release the texture.
	if(m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}

	// Release the targa data.
	if(m_targaData)
	{
		delete [] m_targaData;
		m_targaData = 0;
	}

	return;
}


ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_textureView;
}

int TextureClass::GetWidth()
{
    return m_width;
}


int TextureClass::GetHeight()
{
    return m_height;
}