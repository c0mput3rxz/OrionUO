﻿// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUILine.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUILine::CGUILine(const int &startX, const int &startY, const int &targetX, const int &targetY, const uint &polygoneColor)
: CBaseGUI(GOT_LINE, 0, 0, 0, startX, startY), m_TargetX(targetX), m_TargetY(targetY)
{
	WISPFUN_DEBUG("c65_f1");
	m_ColorR = GetRValue(polygoneColor);
	m_ColorG = GetGValue(polygoneColor);
	m_ColorB = GetBValue(polygoneColor);
	m_ColorA = polygoneColor >> 24;

	if (!m_ColorA)
		m_ColorA = 0xFF;
}
//----------------------------------------------------------------------------------
CGUILine::~CGUILine()
{
}
//----------------------------------------------------------------------------------
void CGUILine::Draw(const bool &checktrans)
{
	WISPFUN_DEBUG("c65_f2");
	glColor4ub(m_ColorR, m_ColorG, m_ColorB, m_ColorA);

	if (m_ColorA < 0xFF)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		g_GL.DrawLine(m_X, m_Y, m_TargetX, m_TargetY);

		glDisable(GL_BLEND);
	}
	else
		g_GL.DrawLine(m_X, m_Y, m_TargetX, m_TargetY);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}
//----------------------------------------------------------------------------------
