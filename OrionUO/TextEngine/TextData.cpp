

#include "stdafx.h"

CTextData::CTextData()
    : CRenderTextObject()
{
    WISPFUN_DEBUG("c174_f1");
}

CTextData::CTextData(CTextData *obj)
    : Unicode(obj->Unicode)
    , Type(obj->Type)
    , Font(obj->Font)
    , Timer(obj->Timer)
    , Alpha(obj->Alpha)
{
    WISPFUN_DEBUG("c174_f2");
    RealDrawX = obj->RealDrawX;
    RealDrawY = obj->RealDrawY;
    Color = obj->Color;
    Text = obj->Text;
    UnicodeText = obj->UnicodeText;
}

CTextData::~CTextData()
{
    WISPFUN_DEBUG("c174_f3");
    m_Texture.Clear();
    Owner = NULL;
}

bool CTextData::CanBeDrawedInJournalGump()
{
    WISPFUN_DEBUG("c174_f4");
    bool result = true;

    switch (Type)
    {
        case TT_SYSTEM:
            result = g_JournalShowSystem;
            break;
        case TT_OBJECT:
            result = g_JournalShowObjects;
            break;
        case TT_CLIENT:
            result = g_JournalShowClient;
            break;
        default:
            break;
    }

    return result;
}

void CTextData::GenerateTexture(
    int maxWidth, ushort flags, TEXT_ALIGN_TYPE align, uchar cell, int font)
{
    WISPFUN_DEBUG("c174_f5");
    if (Unicode)
    {
        if (font == -1)
            font = Font;

        g_FontManager.GenerateW(
            (uchar)font, m_Texture, UnicodeText, Color, cell, maxWidth, align, flags);
    }
    else
        g_FontManager.GenerateA((uchar)Font, m_Texture, Text, Color, maxWidth, align, flags);

    if (!m_Texture.Empty())
    {
        if (g_ConfigManager.ScaleSpeechDelay)
        {
            Timer += (g_ConfigManager.SpeechDelay * m_Texture.LinesCount) * 100;
        }
        else
        {
            Timer += g_ConfigManager.SpeechDelay * 100;
        }
    }
}
