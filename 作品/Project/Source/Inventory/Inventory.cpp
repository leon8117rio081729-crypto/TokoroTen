#include "Inventory.h"
#include "../Input/Input.h"
#include <DxLib.h>
#include "../Scene/SceneManager.h"
#include "../Player/PlayerManager.h"
#include "../Item/Treasure/TreasureItem.h"
#include "../Sound/SoundManager.h"
#include "../Item/Medicine/MedicineItem.h"
#include "../Color/Color.h"
#include "../Item/Tool/ToolItem.h"
#include "../UI/Message/MessageManager.h"
#include "../Item/ItemManager.h"
#include "../Player/Player.h"

 
Inventory* Inventory::m_Instance = nullptr; // インスタンスの初期化

Inventory::Inventory()
	: m_IsOpenInventory (false) // インベントリは初期状態では閉じている
	, m_InventoryMax (false)    // インベントリは初期状態ではいっぱいではない
	, m_SelectedIndex (0)       // 選択中のスロットは初期状態では0
{
}

Inventory::~Inventory()
{
	m_Items.clear();
}

void Inventory::Init()
{
	if (!m_Instance)
	{
		m_Instance = new Inventory();
	}
	
}

void Inventory::Load()
{

}

void Inventory::Start()
{
}

void Inventory::Step()
{
	if (Input::IsTriggerPadButton(PAD_INPUT_X)) // Yボタンでインベントリを開く
	{
		m_IsOpenInventory = !m_IsOpenInventory; // インベントリの開閉を切り替える
		SoundManager::GetInstance()->PlaySE(SE_TYPE_UI_INVENTORY_OPEN); // インベントリ開閉音
	}
	if (m_IsOpenInventory)
	{
		if (Input::IsTriggerPadButton(PAD_INPUT_Y)) // Lボタンで左に移動
		{
			m_SelectedIndex--;
			SoundManager::GetInstance()->PlaySE(SE_TYPE_UI_INVENTORY_SELECT); // スロット移動音
		}
		if (Input::IsTriggerPadButton(PAD_INPUT_Z)) // Rボタンで右に移動
		{
			m_SelectedIndex++;
			SoundManager::GetInstance()->PlaySE(SE_TYPE_UI_INVENTORY_SELECT); // スロット移動音
		}
		if (m_SelectedIndex < 0)
		{
			m_SelectedIndex = MAX_SLOTS - 1;
		}
		if (m_SelectedIndex >= MAX_SLOTS)
		{
			m_SelectedIndex = 0;
		}

		// Xボタンで使用
		if (Input::IsTriggerPadButton(PAD_INPUT_C) && m_SelectedIndex < m_Items.size())
		{
			// プレイヤー取得
			Player* player = PlayerManager::GetInstance()->GetPlayer();
			if (player && m_Items[m_SelectedIndex])
			{
				// 薬アイテムの場合のみ使用可能
				if (auto medicine = dynamic_cast<MedicineItem*>(m_Items[m_SelectedIndex].get()))
				{
					m_Items[m_SelectedIndex]->Use(player); // 効果を発動
					SoundManager::GetInstance()->PlaySE(SE_TYPE_ITEM_MEDICINE_USE); // 使用音
					m_Items.erase(m_Items.begin() + m_SelectedIndex); // 使用したアイテムを削除

					// インデックスずれ防止
					if (m_SelectedIndex >= m_Items.size())
					{
						m_SelectedIndex = (int)m_Items.size() - 1;
					}
					if (m_SelectedIndex < 0)
					{
						m_SelectedIndex = 0; // 最小値を0に
					}
				}				
			}
		}
	}	
}

void Inventory::Update()
{
}

void Inventory::Draw()
{
	if (m_IsOpenInventory)
	{
		// インベントリが開いているときの描画処理
		// 半透明の背景
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);// 透過
		DrawBox(400, 700, 1200, 900, BLACK, TRUE); // インベントリの背景
		DrawBox(450, 650, 600, 700, BLACK, TRUE); // インベントリ文字
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // 透過解除
		DrawString(460, 670, "インベントリ(Y)", WHITE); // インベントリのタイトル
		
		// スロット枠表示 & アイテム名描画
		for (int i = 0; i < MAX_SLOTS; ++i)
		{
			int x = 450 + i * 150; // スロットのX座標
			int y = 750; // スロットのY座標

			// スロットのどこを選択しているか
			if (i == m_SelectedIndex)
			{
				DrawBox(x, y, x + 100, y + 100, YELLOW, TRUE); // 選択中は黄色背景
			}
			else
			{
				DrawBox(x, y, x + 100, y + 100, BLUE, FALSE); // 枠だけ
			}

			// スロット枠を描画
			DrawBox(x, y, x + 100, y + 100, BLUE, FALSE);

			// アイテムがある場合はアイテム名を描画
			if (i < m_Items.size())
			{
				ItemKey key = m_Items[i]->GetKey();

				int graph =
					ItemManager::GetInstance()->GetItemGraph(key);

				if (graph != -1)
				{
					DrawGraph(x + 10, y + 10, graph, TRUE);
				}

				const std::string& itemName = m_Items[i]->GetName();
				DrawString(x + 15, y - 20, itemName.c_str(), WHITE);
			}

			DrawString(1050, 875, "使用する:(X)", WHITE);
			DrawString(900, 875, "切り替え:(L/R)", WHITE);
		}

		// 道具欄を下に追加
		for (int i = 0; i < MAX_TOOL_SLOTS; ++i)
		{
			int x = 1350; // スロットのX座標
			int y = 750; // スロットのY座標

			// スロットのどこを選択しているか
			if (i == m_SelectedIndex)
			{
				//DrawBox(x, y, x + 100, y + 100, YELLOW, TRUE); // 選択中は黄色背景
			}
			else
			{
				DrawBox(x, y, x + 100, y + 100, BLUE, FALSE); // 枠だけ
			}

			// スロット枠を描画
			DrawBox(x, y, x + 100, y + 100, BLUE, FALSE);

			// アイテムがある場合はアイテム名を描画
			if (i < m_Tool.size())
			{
				ItemKey key = m_Tool[i]->GetKey();

				int graph =
					ItemManager::GetInstance()->GetItemGraph(key);

				if (graph != -1)
				{
					DrawGraph(x + 10, y + 10, graph, TRUE);
				}

				const std::string& itemName = m_Tool[i]->GetName();
				DrawString(x + 15, y - 20, itemName.c_str(), WHITE);
			}
		}


	}
	else if(!m_IsOpenInventory&&!SceneManager::GetInstance()->m_IsPaused)
	{
		// 半透明の背景
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);// 透過
		DrawBox(450, 850, 600, 900, BLACK, TRUE); // インベントリ文字背景
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // 透過解除
		DrawString(460, 870, "インベントリ(Y)", WHITE); // インベントリのタイトル
	}
}

void Inventory::Fin()
{
}

// アイテムを追加する
void Inventory::AddItem(std::unique_ptr<ItemBase> item)
{
	m_Items.push_back(std::move(item));
}

// 道具を追加する
void Inventory::AddTool(std::unique_ptr<ItemBase> tool)
{
	// 追加前にツールの種類を取得
	int toolTypeKey = -1;
	ToolItem* t = nullptr;
	if (tool)
	{
		t = dynamic_cast<ToolItem*>(tool.get());
		if (t)
		{
			toolTypeKey = static_cast<int>(t->GetToolType());
		}
	}

	// 実際にコンテナへ追加
	m_Tool.push_back(std::move(tool));

	// 追加したツールが ToolItem なら、初回取得かを判定してメッセージ表示
	if (t)
	{
		// まだ表示していない場合
		if (toolTypeKey >= 0 && m_ToolHintShown.find(toolTypeKey) == m_ToolHintShown.end())
		{
			// 初回取得：道具の説明を表示
			std::string msg = t->GetName() + std::string(" : ") + t->GetDescription();
			if (MessageManager::GetInstance())
			{
				// 位置・色・フォントサイズ・表示時間
				MessageManager::GetInstance()->ShowMessage(600, 650, WHITE, UI_NORMAL, msg, 4000);
			}
			// 再表示しないよう記録
			m_ToolHintShown.insert(toolTypeKey);
		}
	}
}

bool Inventory::HasTool(ToolType type) const
{
	for (const auto& up : m_Tool)
	{
		if (!up) continue;
		if (auto t = dynamic_cast<ToolItem*>(up.get()))
		{
			if (t->GetToolType() == type) return true;
		}
	}
	return false;
}

std::unique_ptr<ItemBase> Inventory::ReplaceTool(std::unique_ptr<ItemBase> newTool)
{
	// 空ならそのまま追加して古いのは返さない
	if (m_Tool.empty())
	{
		m_Tool.push_back(std::move(newTool));
		return nullptr;
	}
	
	// 先頭スロットを置き換えて古いものを返す
	std::unique_ptr<ItemBase> old = std::move(m_Tool[0]);
	m_Tool[0] = std::move(newTool);
	return old;
}

// 宝アイテムを削除する(納品時)
bool Inventory::RemoveTreasureItem()
{
	// 宝アイテムを探して削除
	auto it = std::find_if(m_Items.begin(), m_Items.end(),
		[](const std::unique_ptr<ItemBase>& item) {
			return dynamic_cast<TreasureItem*>(item.get()) != nullptr;
		});

	if (it != m_Items.end())
	{
		m_Items.erase(it);
		return true;
	}

	return false;
}

// 道具アイテムを削除する(使用時)
bool Inventory::RemoveToolItem()
{
	// 道具アイテムを探して削除
	auto it = std::find_if(m_Tool.begin(), m_Tool.end(),
		[](const std::unique_ptr<ItemBase>& item) {
			return dynamic_cast<ToolItem*>(item.get()) != nullptr;
		});
	if (it != m_Tool.end())
	{
		m_Tool.erase(it);
		return true;
	}
	return false;
}

void Inventory::UseItem(int index, Player* player)
{
	if (!player) return;
	if (index < 0 || index >= m_Items.size()) return;

	m_Items[index]->Use(player);
	m_Items.erase(m_Items.begin() + index);
}

const std::vector<std::unique_ptr<ItemBase>>& Inventory::GetItems()
{
	return m_Items; // アイテムのリストを返す
}

const std::vector<std::unique_ptr<ItemBase>>& Inventory::GetTool()
{
	return m_Tool; // アイテムのリストを返す
}

// 現在装備中のツールを取得（nullptrなら装備なし）
ToolItem* Inventory::GetCurrentToolItem()
{
	if (m_Tool.empty()) return nullptr;

	auto& item = m_Tool[0];
	return dynamic_cast<ToolItem*>(item.get());
}

const ToolItem* Inventory::GetCurrentToolItem() const
{
	if (m_Tool.empty()) return nullptr;

	auto& item = m_Tool[0];
	return dynamic_cast<const ToolItem*>(item.get());
}
