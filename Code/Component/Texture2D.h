#pragma once
#include"Component.h"
#include"Transform.h"
#include"TextureShader.h"
#include"Texture.h"

class Texture2D : public Component<BaseObject>
{
private:
	struct VertexType2D
	{
		Vector::Vector3 position;
		Vector::Vector2 texture;
	};

public:
	Texture2D();
	~Texture2D();

public:
	/*파일 지정 하기 위해서는 초기화 이전에 파일경로를 설정 해줘야 된다*/
	bool Inite(ID3D11Device* device, ID3D11DeviceContext * deviceContext, HWND hwnd, Transform* tr, Vector::Vector2 screensize);
	void Destroy();
	void Update(UpdateQueue* uQueue);

	bool Render(ID3D11DeviceContext * deviceContext, Vector::Vector2 screensize, Transform* tr);
	bool Render_Mesh(ID3D11DeviceContext * deviceContext, Vector::Vector2 screensize, Transform* tr);
	bool Render_Shader(ID3D11DeviceContext * deviceContext, Transform* tr);


	bool UpdateBuffer(ID3D11DeviceContext * deviceContext, Vector::Vector2 screensize, Transform* tr);

	void SetTextureFilePath(char* name);
	void SetVSFilePath(char* name);
	void SetPSFilePath(char* name);

	//인스팩터 맵핑
	InspacterItem* CreateInspecter(std::list<UpdateItem*>* upList, HWND phwnd, HINSTANCE phins, POINT& NextPos, float compFrameXsize);
private:
	bool TextureLoad(ID3D11Device* device);
	bool ShaderLoad(ID3D11Device* device, HWND hwnd);
	bool IniteBuffer(ID3D11Device* device);
	bool UpdateVertex(ID3D11DeviceContext * deviceContext, Vector::Vector2 screensize);

private:
	char m_Textrue_FilePath[FILENAME_MAX];
	char m_VS_FilePath[FILENAME_MAX];
	char m_PS_FilePath[FILENAME_MAX];

	Texture * m_texture = nullptr;
	TextureShader* m_shader = nullptr;
	TextureShader::TextureBufferType m_textureBuffer;

	Vector::Vector2 m_previous_position;
	Vector::Vector2 m_size;
	int m_indexCount = 0;
	int m_vertexCount = 0;

	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;

public:
	int Depth = 0;


private:
	/*파일 교체 플레그*/
	bool m_ChangeFlag = false;
};