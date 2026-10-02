#pragma once
class Goal;

// プレイヤーオブジェクト管理クラス
class GoalManager
{
public:
	GoalManager();	// コンストラクタ
	~GoalManager();	// デストラクタ

public:
	// プレイヤーマネージャーを生成する
	static void CreateInstance() { if (!m_Instance) m_Instance = new GoalManager; }
	// マネージャーの関数が呼びたいときに使用する、マネージャー取得関数
	static GoalManager* GetInstance() { return m_Instance; }
	// 使わなくなったら削除する際の削除関数
	static void DeleteInstance() { if (m_Instance) delete m_Instance; m_Instance = nullptr; }

public:
	void Init();	// 初期化
	void Load();	// ロード
	void Start();	// 開始
	void Step();	// ステップ
	void Update();	// 更新
	void Draw();	// 描画
	void Fin();		// 終了

public:
	// プレイヤーを生成する
	void CreateGoal();

	// 管理中のプレイヤーを取得する
	Goal* GetGoal() { return m_Goal; }

private:
	// 生成されたGoalManager自身を格納する変数
	// GoalManagerはゲーム上に１つのみなのでstaticにしている
	static GoalManager* m_Instance;
	// 生成されたプレイヤーの参照
	Goal* m_Goal;
};
