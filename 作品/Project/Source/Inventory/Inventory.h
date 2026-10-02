#pragma once
#include <memory>
#include <vector>
#include <unordered_set>
#include "../Item/ItemBase.h"

class Player;
class ToolItem;

class Inventory
{
public:
	// 定数
	static constexpr int MAX_SLOTS = 5; // インベントリの最大スロット数
	static constexpr int MAX_TOOL_SLOTS = 1; // ツールインベントリの最大スロット数

	Inventory();
	~Inventory();

	void Init();	// 初期化
	void Load();	// ロード
	void Start();	// 開始
	void Step();	// ステップ
	void Update();	// 更新
	void Draw();	// 描画
	void Fin();		// 終了

	static void CreateInstance() { if (!m_Instance) m_Instance = new Inventory; }
	// マネージャーの関数が呼びたいときに使用する、マネージャー取得関数
	static Inventory* GetInstance() { return m_Instance; }
	// 使わなくなったら削除する際の削除関数
	static void DeleteInstance() { if (m_Instance) delete m_Instance; m_Instance = nullptr; }

	void AddItem(std::unique_ptr<ItemBase> item); // アイテムを追加する
	void AddTool(std::unique_ptr<ItemBase> tool); // 道具を追加する
	bool HasTool(ToolType type) const; // 所持している道具の種類チェック
	std::unique_ptr<ItemBase> ReplaceTool(std::unique_ptr<ItemBase> newTool);// 道具を入れ替える

	bool RemoveTreasureItem(); // 宝アイテムを削除する
	bool RemoveToolItem(); // 道具アイテムを削除する
	void UseItem(int index, Player* player);// アイテムを使用する
	const std::vector<std::unique_ptr<ItemBase>>& GetItems(); // アイテムのリストを取得する
	const std::vector<std::unique_ptr<ItemBase>>& GetTool(); // 道具のリストを取得する

	void OpenInventory() { m_IsOpenInventory = true; } // インベントリを開く
	void CloseInventory() { m_IsOpenInventory = false; } // インベントリを閉じる
	bool IsOpenInventory() const { return m_IsOpenInventory; } // インベントリが開いているかどうかを返す
	bool IsInventoryMax()const { return m_Items.size() >= MAX_SLOTS; }// インベントリがいっぱいかどうかを返す
	bool IsToolInventoryMax()const { return m_Tool.size() >= MAX_TOOL_SLOTS; }// インベントリがいっぱいかどうかを返す
	// 現在装備中のツール（スロット0）を取得する
	ToolItem* GetCurrentToolItem();
	const ToolItem* GetCurrentToolItem() const;
private:
	int m_SelectedIndex; // 選択中のスロット

	static Inventory* m_Instance;
	bool m_IsOpenInventory; // インベントリが開いているかどうか
	bool m_InventoryMax; // インベントリいっぱいどうか
	std::vector<std::unique_ptr<ItemBase>> m_Items;
	std::vector<std::unique_ptr<ItemBase>> m_Tool;
	//std::unordered_map<ItemModel, int> m_UIModelHandles;

	// 既に表示済みの道具説明を記録
	std::unordered_set<int> m_ToolHintShown;
};
