#include"pch.h"


Matrix::Matrix Transform::World_Matrix = Matrix::Matrix::Identity();
Matrix::Matrix Transform::Projection_Matrix = Matrix::Matrix::Identity();
Matrix::Matrix Transform::View_Matrix = Matrix::Matrix::Identity();
Matrix::Matrix Transform::ViewUI_Matrix = Matrix::Matrix::Identity();

std::map<std::string, Font*>* Text::m_FontMap = nullptr;


//test
std::queue<BaseObject*> BaseObject::DestroyQ;
std::queue<BaseObject*> BaseObject::NewQ;
std::queue<BaseObject*> BaseObject::NewUIQ;

int BaseObject::ObjectCount = 0;