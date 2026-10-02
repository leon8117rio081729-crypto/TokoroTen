#pragma once
#include <string>
#include <array>
#include <vector>
#include <cstdarg>

// フォントID
enum FontId
{
    UI_SMALL,
    UI_NORMAL,
    UI_LARGE,
    UI_TIP,
    UI_PRESS,
    UI_PLAYER_STATS,
    UI_INK,
    UI_FONT_ID_MAX,
};

// メッセージ構造体
struct Message
{
    std::string text{};
    int x{ 0 };
    int y{ 0 };
    unsigned int color{ 0 };
    int fontHandle{ 0 };
    long long endTimeMs{ 0 };
    bool active{ false };
};

class MessageManager
{
private:
    MessageManager();
    ~MessageManager();

public:
    static void CreateInstance() { if (!m_Instance) m_Instance = new MessageManager; }
    static MessageManager* GetInstance() { return m_Instance; }
    static void DeleteInstance() { delete m_Instance; m_Instance = nullptr; }

    void Load();
    void Start();
    void Update();
    void Draw();
    void Fin();

    // メッセージを表示（ms = 表示時間ミリ秒）
    void ShowMessage(int x, int y, unsigned int color, FontId fontId, const std::string& text, int ms);
    // メッセージを消す
    void ClearMessage();
    // メッセージを設定
    void SetMessage(int x, int y, unsigned int color, FontId fontId, const char* text, ...);


    // 設定
    void SetFontHandle(FontId fontId) { m_DefaultFontId = fontId; }
    // 取得
    int GetFontHandle(FontId fontId) const;

private:
    std::vector<Message> m_Messages;
    static MessageManager* m_Instance;

    // フォントハンドル管理用マップ
    std::array<int, UI_FONT_ID_MAX> m_FontHandles{};
    FontId m_DefaultFontId;
};