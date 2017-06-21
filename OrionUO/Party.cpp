﻿/***********************************************************************************
**
** Party.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "Party.h"
#include "OrionUO.h"
#include "Gumps/GumpStatusbar.h"
#include "Managers/MouseManager.h"
#include "Managers/GumpManager.h"
#include "Managers/ConfigManager.h"
#include "Game objects/GameWorld.h"
#include "Game objects/GamePlayer.h"
//----------------------------------------------------------------------------------
CParty g_Party;
//----------------------------------------------------------------------------------
CParty::CParty()
: m_Leader(0), m_Inviter(0), m_CanLoot(false)
{
}
//----------------------------------------------------------------------------------
CParty::~CParty()
{
}
//----------------------------------------------------------------------------------
bool CParty::Contains(const uint &serial)
{
	WISPFUN_DEBUG("c196_f1");
	bool result = false;

	if (m_Leader != 0)
	{
		IFOR(i, 0, 10)
		{
			if (Member[i].Serial == serial)
			{
				result = true;

				break;
			}
		}
	}

	return result;
}
//----------------------------------------------------------------------------------
void CParty::Clear()
{
	WISPFUN_DEBUG("c196_f2");
	IFOR(i, 0, 10)
		Member[i].Serial = 0;;
}
//----------------------------------------------------------------------------------
void CParty::ParsePacketData(WISP_DATASTREAM::CDataReader &reader)
{
	WISPFUN_DEBUG("c196_f3");
	uchar code = reader.ReadUInt8();

	switch (code)
	{
		case 1: //Add member
		{
		}
		case 2: //Remove member
		{
			Clear();

			uchar count = reader.ReadUInt8();

			if (code == 1)
			{
				if (m_Leader == 0)
				{
					m_Leader = g_PlayerSerial;
					m_Inviter = 0;
				}
			}
			else if (count <= 1)
			{
				m_Leader = 0;
				m_Inviter = 0;
				IFOR(i, 0, 10)
				{
					Member[i].Serial = 0;
					Member[i].Character = NULL;
				}
				break;
			}

			WISP_GEOMETRY::CPoint2Di oldPos = g_MouseManager.Position;
			WISP_GEOMETRY::CPoint2Di mousePos(76, 30);
			g_MouseManager.Position = mousePos;

			CGumpStatusbar *prevGump = NULL;

			IFOR(i, 0, count)
			{
				uint serial = reader.ReadUInt32BE();
				if (serial != g_PlayerSerial)
				{
					Member[i].Character = g_World->FindWorldCharacter(serial);
					g_Orion.StatusReq(serial);
				}
				Member[i].Serial = serial;

				CGumpStatusbar *gump = (CGumpStatusbar*)g_GumpManager.UpdateContent(serial, 0, GT_STATUSBAR);

				if (gump == NULL)
				{
					g_Orion.OpenStatus(serial);
					
					gump = (CGumpStatusbar*)g_GumpManager.UpdateContent(serial, 0, GT_STATUSBAR);

					if (serial == g_PlayerSerial)
					{
						gump->Minimized = false;
					}						

					

					if (prevGump != NULL)
						prevGump->AddStatusbar(gump);

					prevGump = gump;

					mousePos.Y += 59;
					g_MouseManager.Position = mousePos;
				}
				else
				{
					g_Orion.StatusReq(serial);
					gump->WantRedraw = true;
				}
			}
			
			g_MouseManager.Position = oldPos;
			
			g_GumpManager.UpdateContent(0, 0, GT_PARTY_MANIFEST);

			break;
		}
		case 3: //Private party message
		case 4: //Party message
		{
			uint serial = reader.ReadUInt32BE();
			wstring name = reader.ReadWString(0, true);

			IFOR(i, 0, 10)
			{
				if (Member[i].Serial == serial)
				{
					string str = "[" + Member[i].GetName(i) + "]: " + ToString(name);

					g_Orion.CreateTextMessage(TT_SYSTEM, serial, 3, g_ConfigManager.PartyMessageColor, str);

					break;
				}
			}

			break;
		}
		case 7: //Party invition
		{
			g_Party.Inviter = reader.ReadUInt32BE();

			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------------
