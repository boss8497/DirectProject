#pragma once
#include"UpdateItem.h"

namespace UpdateItemSubProc
{
	static UpdateItem* m_CurrItem = nullptr;
	static WNDPROC m_oldProc;
	static LRESULT CALLBACK SubMessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
	{
		switch (umsg)
		{
		case WM_KEYDOWN:
		{
			switch (wparam)
			{
			case VK_RETURN:
				if (m_CurrItem != nullptr)
				{
					m_CurrItem->InspecterEditBoxUpdate();
				}
				break;
			}

		}
		break;


		default:
			break;
		}
		return CallWindowProc(m_oldProc, hwnd, umsg, wparam, lparam);
	}

	static void ResetSubProc()
	{
		if (m_CurrItem != nullptr)
		{
			(WNDPROC)SetWindowLong(m_CurrItem->m_Currhwnd, GWL_WNDPROC, (LONG)UpdateItemSubProc::m_oldProc);
			UpdateItemSubProc::m_CurrItem->SetNowText();
			SetWindowText(UpdateItemSubProc::m_CurrItem->m_Currhwnd, UpdateItemSubProc::m_CurrItem->m_NowText);
			m_CurrItem = nullptr;
		}
	}

	static void SetSubProc(UpdateItem* uitem)
	{
		if(UpdateItemSubProc::m_CurrItem == nullptr)
		{
			UpdateItemSubProc::m_CurrItem = uitem;
			UpdateItemSubProc::m_oldProc = (WNDPROC)SetWindowLong(m_CurrItem->m_Currhwnd, GWL_WNDPROC, (LONG)UpdateItemSubProc::SubMessageHandler);
		}
		else
		{
			UpdateItemSubProc::ResetSubProc();


			UpdateItemSubProc::m_CurrItem = uitem;
			UpdateItemSubProc::m_oldProc = (WNDPROC)SetWindowLong(m_CurrItem->m_Currhwnd, GWL_WNDPROC, (LONG)UpdateItemSubProc::SubMessageHandler);
		}
	}


	
};