#include "pch.h"
#include "Texture2D.h"
#include "UpdateQueue.h"

Texture2D::Texture2D()
{
	ZeroMemory(m_Textrue_FilePath, FILE_NAME_MAX);
	strcpy_s(m_Textrue_FilePath, FILE_NAME_MAX, Default_Texture2D);
	ZeroMemory(m_VS_FilePath, FILE_NAME_MAX);
	strcpy_s(m_VS_FilePath, FILE_NAME_MAX, Default_2D_VS);
	ZeroMemory(m_PS_FilePath, FILE_NAME_MAX);
	strcpy_s(m_PS_FilePath, FILE_NAME_MAX, Default_2D_PS);
	this->m_CID = COMPONENT_ID::TEXTURE2D;
	m_textureBuffer.color.Set(1, 1, 1, 1);
}

Texture2D::~Texture2D()
{
}

bool Texture2D::Inite(ID3D11Device * device, ID3D11DeviceContext * deviceContext, HWND hwnd, Transform* tr
	, Vector::Vector2 screensize)
{
	//��ġ, ũ�� ����
	m_previous_position = tr->Position;
	m_size = tr->Size;

	//���ۻ���
	if (!this->IniteBuffer(device))
	{
		printf("Texture2D : IniteBuffer Error\n");
		return false;
	}


	//�ؽ��� �ε�
	if (!this->TextureLoad(device))
	{
		printf("Texture2D : TextureLoad Error\n");
		return false;
	}

	//���̴� �ε�
	if (!this->ShaderLoad(device, hwnd))
	{
		printf("Texture2D : ShaderLoad Error\n");
		return false;
	}


	this->UpdateVertex(deviceContext, screensize);

	return true;
}

void Texture2D::Destroy()
{
	if (m_texture)
	{
		m_texture->Destroy();
		delete m_texture;
		m_texture = nullptr;
	}

	if (m_shader)
	{
		m_shader->Destroy();
		delete m_shader;
		m_shader = nullptr;
	}


	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = nullptr;
	}

	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = nullptr;
	}

}

void Texture2D::Update(UpdateQueue* uQueue)
{
	if (this->m_ChangeFlag)
	{
		/*1. ���� � ���ϸ��� �ٲ������ ã�´�*/
		/*���ϸ��� ������ ��ٸ� �ٽ� �ʱ�ȭ �Ѵ�*/
		if (std::strcmp(this->m_texture->GetFilePath(), m_Textrue_FilePath) != 0)
		{
			m_texture->SetFileName(m_Textrue_FilePath);
			uQueue->Push(m_texture);
		}

		if (std::strcmp(this->m_shader->GetVSFilePath(), m_VS_FilePath) != 0)
		{

		}

		if (std::strcmp(this->m_shader->GetVSFilePath(), m_PS_FilePath) != 0)
		{

		}

		this->m_ChangeFlag = false;
	}
}

bool Texture2D::Render(ID3D11DeviceContext * deviceContext, Vector::Vector2 screensize, Transform* tr)
{
	UINT stride = sizeof(VertexType2D);
	UINT offset = 0;

	//���۾�����Ʈ
	if (!this->UpdateBuffer(deviceContext, screensize, tr))
	{
		printf("Texture2D : UpdateBuffer Error\n");
		return false;
	}


	//������ �� �� �ֵ��� �Է� ��������� ���� ���۸� Ȱ�� ��Ų��
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	//������ �� �� �ֵ��� �Է� ��������� �ε����� ���۸� Ȱ��
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//���� ���۷� �׸� �⺻���� �����մϴ�. �ﰢ������
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	if (!m_shader->SetShaderParameters(deviceContext, tr, &m_textureBuffer, m_texture->GetTexture()))
	{
		printf("Texture2D : Shader - SetShaderParameters Error");
		return false;
	}
	
	m_shader->RenderShader(deviceContext, m_indexCount);

	return true;
}

bool Texture2D::Render_Mesh(ID3D11DeviceContext * deviceContext, Vector::Vector2 screensize, Transform * tr)
{
	UINT stride = sizeof(VertexType2D);
	UINT offset = 0;

	//���۾�����Ʈ
	if (!this->UpdateBuffer(deviceContext, screensize, tr))
	{
		printf("Texture2D : Render_Mesh Error\n");
		return false;
	}


	//������ �� �� �ֵ��� �Է� ��������� ���� ���۸� Ȱ�� ��Ų��
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	
	//������ �� �� �ֵ��� �Է� ��������� �ε����� ���۸� Ȱ��
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//���� ���۷� �׸� �⺻���� �����մϴ�. �ﰢ������
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	return true;
}

bool Texture2D::Render_Shader(ID3D11DeviceContext * deviceContext, Transform * tr)
{
	if (!m_shader->SetShaderParameters(deviceContext, tr, &m_textureBuffer, m_texture->GetTexture()))
	{
		printf("Texture2D : Shader - SetShaderParameters Error");
		return false;
	}

	m_shader->RenderShader(deviceContext, m_indexCount);
	return true;
}

bool Texture2D::TextureLoad(ID3D11Device * device)
{
	m_texture = new Texture();
	if (m_texture)
	{
		if (!m_texture->Inite(device, m_Textrue_FilePath))
		{
			printf("Texture2D : TextureLoad Error \n");
			return false;
		}
	}
	else
	{
		return false;
	}
	return true;
}

bool Texture2D::ShaderLoad(ID3D11Device * device, HWND hwnd)
{
	m_shader = new TextureShader();
	if (m_shader)
	{
		if (!m_shader->Inite(device, hwnd, m_VS_FilePath, m_PS_FilePath))
		{
			printf("Texture2D : ShaderLoad Error \n");
			return false;
		}
	}
	else
	{
		printf("Texture2D : ShaderLoad - Shader Dynamic cast Error \n");
		return false;
	}

	return true;
}

bool Texture2D::IniteBuffer(ID3D11Device * device)
{

	// ���� �迭�� ���� ���� �ε��� �迭�� �ε��� ���� �����մϴ�.
	m_indexCount = 6;
	m_vertexCount = 6;

	// ���� �迭�� ����ϴ�.
	VertexType2D* vertices = new VertexType2D[m_vertexCount];
	if (!vertices)
	{
		return false;
	}
	ZeroMemory(vertices, sizeof(VertexType2D)*m_vertexCount);

	// ���� �迭�� 0���� �ʱ�ȭ�մϴ�.
	memset(vertices, 0, (sizeof(VertexType2D) * m_vertexCount));

	// �ε��� �迭�� ����ϴ�.
	unsigned long* indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// �����ͷ� �ε��� �迭�� �ε��մϴ�.
	for (int i = 0; i < m_indexCount; i++)
	{
		indices[i] = i;
	}

	// ���� ���� ������ ����ü�� �����մϴ�.
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType2D) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;


	// subresource ������ ���� �����Ϳ� ���� �����͸� �����մϴ�.
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// ���� ���� ���۸� ����ϴ�.
	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
	{
		return false;
	}

	// ���� �ε��� ������ ����ü�� �����մϴ�.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// �ε��� �����͸� ����Ű�� ���� ���ҽ� ����ü�� �ۼ��մϴ�.
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// �ε��� ���۸� �����մϴ�.
	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer)))
	{
		return false;
	}

	// �����ǰ� ���� �Ҵ�� ���� ���ۿ� �ε��� ���۸� �����մϴ�.
	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return true;
}

bool Texture2D::UpdateVertex(ID3D11DeviceContext * deviceContext, Vector::Vector2 screensize)
{
	float left, right, top, bottom;
	VertexType2D* vertices = nullptr;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	std::string me;



	//���� ��ǥ ���
	left = ((screensize.x / 2) * -1) + m_previous_position.x;

	//������
	right = left + m_size.x;

	//��
	top = (screensize.y / 2) - m_previous_position.y;

	//��
	bottom = top - m_size.y;


	vertices = new VertexType2D[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// ���� �迭�� �����͸��ε��մϴ�.
	// ù ��° �ﰢ��
	vertices[0].position = XMFLOAT3(left, top, 0.0f);  // Top left.
	vertices[0].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[1].position = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
	vertices[1].texture = XMFLOAT2(1.0f, 1.0f);

	vertices[2].position = XMFLOAT3(left, bottom, 0.0f);  // Bottom left.
	vertices[2].texture = XMFLOAT2(0.0f, 1.0f);

	// �� ��° �ﰢ��
	vertices[3].position = XMFLOAT3(left, top, 0.0f);  // Top left.
	vertices[3].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[4].position = XMFLOAT3(right, top, 0.0f);  // Top right.
	vertices[4].texture = XMFLOAT2(1.0f, 0.0f);

	vertices[5].position = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
	vertices[5].texture = XMFLOAT2(1.0f, 1.0f);

	// ���ؽ� ���۸� �� �� �ֵ��� ��޴ϴ�.
	result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		me = std::system_category().message(result);
		return false;
	}


	// �����͸� ���� ���ۿ� �����մϴ�.
	memcpy(mappedResource.pData, vertices, sizeof(VertexType2D) * m_vertexCount);


	// ���� ������ ����� �����մϴ�.
	deviceContext->Unmap(m_vertexBuffer, 0);

	// �� �̻� �ʿ����� ���� ������ �迭�� �����մϴ�.
	delete[] vertices;
	vertices = 0;

	return true;
}

bool Texture2D::UpdateBuffer(ID3D11DeviceContext * deviceContext, Vector::Vector2 screensize, Transform* tr)
{
	Vector::Vector2 pos = tr->Position;
	Vector::Vector2 size = tr->Size;
	//��ġ�� ������ ������Ʈ�� ���� ����
	if (Vector::Vector2::NearEqual(m_previous_position, pos) && 
		Vector::Vector2::NearEqual(m_size, size))
	{
		return true;
	}

	m_previous_position = pos;
	//m_size = size;

	if (!this->UpdateVertex(deviceContext, screensize))
	{
		printf("Texture2D : UpdateBuffer-UpdateVertex Error\n");
		return false;
	}


	return true;
}

void Texture2D::SetTextureFilePath(char * name)
{
	if (name == nullptr)
	{
		ZeroMemory(m_Textrue_FilePath, FILE_NAME_MAX);
		strcpy_s(m_Textrue_FilePath, FILE_NAME_MAX, Default_Texture2D);
	}
	else
	{
		ZeroMemory(m_Textrue_FilePath, FILE_NAME_MAX);
		strcpy_s(m_Textrue_FilePath, FILE_NAME_MAX, name);
	}
	this->m_ChangeFlag = true;
}

void Texture2D::SetVSFilePath(char * name)
{
	if (name == nullptr)
	{
		ZeroMemory(m_VS_FilePath, FILE_NAME_MAX);
		strcpy_s(m_VS_FilePath, FILE_NAME_MAX, Default_2D_VS);
	}
	else
	{
		ZeroMemory(m_VS_FilePath, FILE_NAME_MAX);
		strcpy_s(m_VS_FilePath, FILE_NAME_MAX, name);
	}
	this->m_ChangeFlag = true;
}

void Texture2D::SetPSFilePath(char * name)
{
	if (name == nullptr)
	{
		ZeroMemory(m_PS_FilePath, FILE_NAME_MAX);
		strcpy_s(m_PS_FilePath, FILE_NAME_MAX, Default_2D_PS);
	}
	else
	{
		ZeroMemory(m_PS_FilePath, FILE_NAME_MAX);
		strcpy_s(m_PS_FilePath, FILE_NAME_MAX, name);
	}
	this->m_ChangeFlag = true;
}


InspacterItem* Texture2D::CreateInspecter(std::list<UpdateItem*>* upList, HWND phwnd, HINSTANCE phins, POINT& NextPos, float compFrameXsize)
{
	InspacterItem* FrameItem = nullptr;
	POINT FrameSize = { (LONG)compFrameXsize, 150 };
	POINT nPos = NextPos;


	//������ ����
	FrameItem = new InspacterItem();
	FrameItem->CreateComponentFrame(phwnd, phins, nPos, FrameSize, "Texture");
	


	POINT FilePathPos = nPos;
	POINT FilePathSize = { 0, };
	InspacterItem* FilePathItem = nullptr;

	FilePathPos.x += INFO_PADDING;
	FilePathPos.y += INFO_PADDING * 2;

	FilePathSize.x = (LONG)compFrameXsize - INFO_PADDING * 2;
	FilePathSize.y = INFO_PADDING * 2;

	FilePathItem = new InspacterItem();
	FilePathItem->CreateInspecterEditBox(phwnd, phins, FilePathPos, FilePathSize
		, IDC_ITEM::EDITTMP, 0, "Path", this->m_Textrue_FilePath, true);

	FrameItem->m_SubItemList.push_back(FilePathItem);


	//���� ��ġ
	FilePathPos.y += FilePathSize.y;

	POINT BtnPos = FilePathPos;
	POINT BtnSize = { (LONG)compFrameXsize - INFO_PADDING * 2  , INFO_PADDING * 2 };
	InspacterItem* Btn = nullptr;
	UpdateItem * BtnUpdate = nullptr;

	Btn = new InspacterItem();
	Btn->CreateInspecterButton(phwnd, phins, BtnPos, BtnSize, IDC_ITEM::BUTTONETMP1, COMPONENT_ID::TEXTURE2D, "Find File");
	FrameItem->m_SubItemList.push_back(Btn);


	//������Ʈ �̺�Ʈ ���
	BtnUpdate = new UpdateItem();
	BtnUpdate->Inite(Btn->m_hwnd, (void*)FilePathItem->m_hwnd,
		UpdateType::UPDATE_BUTTON_OPEN_FILEPATH_DIALOG, UpdateValueType::HWND_WINDOWGETTEXT_YTYPE,
		IDC_ITEM::BUTTONETMP1);

	upList->push_back(BtnUpdate);

	//���� ��ġ
	BtnPos.y += BtnSize.y;



	//������ ����Ʈ �ؽ�Ʈ
	POINT EditSizeTextPos = BtnPos;
	POINT EditSizeTextSize = { ((LONG)compFrameXsize - INFO_PADDING * 2) / 2,INFO_PADDING * 2 };
	InspacterItem* EditSizeTex = nullptr;
	RECT EditSizeTextRect;

	EditSizeTextRect.left = EditSizeTextPos.x;
	EditSizeTextRect.top = EditSizeTextPos.y;
	EditSizeTextRect.right = EditSizeTextSize.x;
	EditSizeTextRect.bottom = EditSizeTextSize.y;
	

	EditSizeTex = new InspacterItem();
	EditSizeTex->CreateInspecterStaticText(phwnd, phins, EditSizeTextRect, "ImageSize");
	FrameItem->m_SubItemList.push_back(EditSizeTex);

	//������ġ
	EditSizeTextPos.y += EditSizeTextSize.y;


	//������ ����Ʈ
	POINT EditSizePos = EditSizeTextPos;
	POINT EditSizeSize = { ((LONG)compFrameXsize - INFO_PADDING * 2) / 2,INFO_PADDING * 2 };
	InspacterItem* EditSize = nullptr;
	UpdateItem * EditSizeUpdate = nullptr;


	EditSize = new InspacterItem();
	EditSize->CreateInspecterEditBox(phwnd, phins, EditSizePos, EditSizeSize,
		IDC_ITEM::VECTERTMP2_X, 0, "X", MethodHelper::FloatToCstr(this->m_size.x), false);
	FrameItem->m_SubItemList.push_back(EditSize);


	EditSizeUpdate = new UpdateItem();
	EditSizeUpdate->Inite(EditSize->m_hwnd, (void*)&this->m_size.x, UpdateType::UPDATE_TEXT_EDIT,
		UpdateValueType::FLOAT_YTYPE, IDC_ITEM::VECTERTMP2_X);
	upList->push_back(EditSizeUpdate);



	EditSizePos.x += EditSizeSize.x;

	EditSize = new InspacterItem();
	EditSize->CreateInspecterEditBox(phwnd, phins, EditSizePos, EditSizeSize,
		IDC_ITEM::VECTERTMP2_Y, 0, "Y", MethodHelper::FloatToCstr(this->m_size.y), false);
	FrameItem->m_SubItemList.push_back(EditSize);


	EditSizeUpdate = new UpdateItem();
	EditSizeUpdate->Inite(EditSize->m_hwnd, (void*)&this->m_size.y, UpdateType::UPDATE_TEXT_EDIT,
		UpdateValueType::FLOAT_YTYPE, IDC_ITEM::VECTERTMP2_Y);
	upList->push_back(EditSizeUpdate);



	EditSizePos.y += EditSizeSize.y;


	//����
	POINT DepthPos = { EditSizeTextPos.x, EditSizePos.y };
	POINT DepthSize = { ((LONG)compFrameXsize - INFO_PADDING * 2),INFO_PADDING * 2 };
	InspacterItem* Depth = nullptr;
	UpdateItem* DepthUpdate = nullptr;
	char DepthChar[255];

	ZeroMemory(DepthChar, sizeof(DepthChar));
	sprintf_s(DepthChar, "%d", this->Depth);


	Depth = new InspacterItem();
	Depth->CreateInspecterEditBox(phwnd, phins, DepthPos, DepthSize,
		IDC_ITEM::VECTERTMP2_Z, 0, "Depth", DepthChar, false);
	FrameItem->m_SubItemList.push_back(Depth);


	DepthUpdate = new UpdateItem();
	DepthUpdate->Inite(Depth->m_hwnd, (void*)&this->Depth, UpdateType::UPDATE_TEXT_EDIT,
		UpdateValueType::INT_YTYPE, IDC_ITEM::VECTERTMP2_Z);
	upList->push_back(DepthUpdate);



	DepthPos.y += DepthSize.y;


	POINT ColorPos = DepthPos;
	POINT ColorSize = { ((LONG)compFrameXsize - INFO_PADDING * 2),INFO_PADDING * 2 };
	InspacterItem* ColorItem = nullptr;
	UpdateItem* ColorUpdate = nullptr;


	ColorItem = new InspacterItem();
	ColorItem->CreateInspecterButton(phwnd, phins, ColorPos, ColorSize, IDC_ITEM::BUTTONETMP2,
		0, "SelectColor");
	FrameItem->m_SubItemList.push_back(ColorItem);

	ColorUpdate = new UpdateItem();
	ColorUpdate->Inite(ColorItem->m_hwnd, (void*)&this->m_textureBuffer.color , UpdateType::UPDATE_BUTTON_OPEN_COLOR_DIALOG,
		UpdateValueType::VECTER3_YTYPE, IDC_ITEM::BUTTONETMP2);
	upList->push_back(ColorUpdate);




	return FrameItem;
}