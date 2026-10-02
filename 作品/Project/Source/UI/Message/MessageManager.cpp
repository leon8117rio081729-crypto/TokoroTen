#include "MessageManager.h"
#include "../../Color/Color.h"
#include <DxLib.h>
#include <cstdarg>
#include <cstdio>

MessageManager* MessageManager::m_Instance = nullptr;

MessageManager::MessageManager()
{
    m_DefaultFontId = FontId::UI_NORMAL;
}

MessageManager::~MessageManager()
{
}

void MessageManager::Load()
{
    // フォント作成（日本語フォントやサイズを指定）
    m_FontHandles[FontId::UI_SMALL] = CreateFontToHandle("ＭＳ ゴシック", 28, -1, DX_FONTTYPE_ANTIALIASING);
    m_FontHandles[FontId::UI_NORMAL] = CreateFontToHandle("ＭＳ ゴシック", 42, -1, DX_FONTTYPE_ANTIALIASING);
    m_FontHandles[FontId::UI_LARGE] = CreateFontToHandle("ＭＳ ゴシック", 72, -1, DX_FONTTYPE_ANTIALIASING);
    m_FontHandles[FontId::UI_TIP] = CreateFontToHandle("MS ゴシック", 32, 2, DX_FONTTYPE_ANTIALIASING);
    m_FontHandles[FontId::UI_PRESS] = CreateFontToHandle("Arial", 48, 3, DX_FONTTYPE_ANTIALIASING);
    m_FontHandles[FontId::UI_PLAYER_STATS] = CreateFontToHandle("ＭＳ ゴシック", 30, 5, DX_FONTTYPE_ANTIALIASING_EDGE);
    m_FontHandles[FontId::UI_INK] = CreateFontToHandle("Ink Journal", 80, -1, DX_FONTTYPE_ANTIALIASING_EDGE);

    m_DefaultFontId = FontId::UI_NORMAL;
}

void MessageManager::Start()
{
}

void MessageManager::Update()
{
    long long now = GetNowCount();// 現在のミリ秒を取得

    // 期限切れのメッセージを削除
    for (auto it = m_Messages.begin(); it != m_Messages.end(); )
    {
        if (it->endTimeMs != -1 && now >= it->endTimeMs)
        {
            it = m_Messages.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void MessageManager::Draw()
{
    // メッセージを描画
    for (const auto& msg : m_Messages)
    {
        DrawStringToHandle(msg.x, msg.y, msg.text.c_str(), msg.color, msg.fontHandle);
    }
}

void MessageManager::Fin()
{
    // フォント削除
    for (int i = 0; i < UI_FONT_ID_MAX; ++i)
    {
        if (m_FontHandles[i] >= 0)
        {
            DeleteFontToHandle(m_FontHandles[i]);
            m_FontHandles[i] = -1;
        }
    }
}

void MessageManager::ShowMessage(int x, int y, unsigned int color, FontId fontId, const std::string& text, int ms)
{
    if (text.empty() || ms == 0) return;
    Message msg;
    msg.text = text;
    msg.x = x;
    msg.y = y;
    msg.color = color;
    msg.fontHandle = GetFontHandle(fontId);// GetNowCount はミリ秒
    msg.active = true;
    if (ms < 0)
    {
        // 無期限表示
        msg.endTimeMs = -1;
    }
    else
    {
        msg.endTimeMs = GetNowCount() + ms;
    }
    m_Messages.push_back(msg);
}

void MessageManager::ClearMessage()
{
    m_Messages.clear();
}

void MessageManager::SetMessage(int x, int y, unsigned int color, FontId fontId, const char* text, ...)
{
    // 可変引数を使って文字列をフォーマット
    char buffer[256];
    va_list args;
    va_start(args, text);
    vsnprintf(buffer, sizeof(buffer), text, args);
    va_end(args);

    // 同じ位置のメッセージがあれば更新
    for (auto& msg : m_Messages)
    {
        if (msg.x == x && msg.y == y)
        {
            msg.text = buffer;
            msg.color = color;
            msg.fontHandle = GetFontHandle(fontId);
            msg.endTimeMs = -1;
            return;
        }
    }

    // 無ければ新しく追加
    ShowMessage(x, y, color, fontId, buffer, -1);
}

int MessageManager::GetFontHandle(FontId fontId) const
{
    return m_FontHandles[fontId];
}