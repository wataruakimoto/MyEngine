#include "PauseUI.h"
#include "Input.h"

PauseUI::PauseUI() {
}

PauseUI::~PauseUI() {
}

void PauseUI::Initialize() {

	/// ===== 黒フィルターの初期化 ===== ///

	backFilter_ = std::make_unique<Sprite>();
	backFilter_->Initialize("White1x1.png");
	backFilter_->SetSize({ 1280.0f,720.0f }); // 画面全体サイズ
	backFilter_->SetColor({ 0.0f,0.0f,0.0f,0.75f }); // 半透明黒

	/// ===== ボタンの初期化 ===== ///

	CreateButton(PauseSelect::Resume, "PauseUI/ResumeButton.png", { centerX_, buttonStartY_ });
	CreateButton(PauseSelect::Restart, "PauseUI/RestartButton.png", { centerX_, buttonStartY_ + buttonGapY_ });
	CreateButton(PauseSelect::Quit, "PauseUI/QuitButton.png", { centerX_, buttonStartY_ + buttonGapY_ * 2 });

	/// ===== 選択枠の初期化 ===== ///
	selectFrame_ = std::make_unique<Sprite>();
	selectFrame_->Initialize("PauseUI/Frame.png");
	selectFrame_->SetAnchorPoint({ 0.5f,0.5f });
}

void PauseUI::Update() {
	
	/// ===== マウスの処理 ===== ///

	Vector2 mousePos = Input::GetInstance()->GetMousePosition();

	for (int i = 0; i < buttons_.size(); ++i) {

		if (CheckButtonSelect(buttons_[i], mousePos)) {

			selectedIndex_ = i;

			// 左クリックで決定
			if (Input::GetInstance()->TriggerMouseButton(MouseButton::Left)) {

				currentSelect_ = buttons_[i].select;
			}
			break; // 重なりはない前提でbreak
		}
	}

	/// ===== 選択枠の位置更新 ===== ///
	
	// 選択中インデックスの範囲チェック
	if(selectedIndex_ >= 0 && selectedIndex_ < buttons_.size()) {

		Vector2 framePos = buttons_[selectedIndex_].position;

		selectFrame_->SetPosition(framePos);

		selectFrame_->SetColor({ 1.0f,1.0f,1.0f,1.0f }); // 表示
	}

	backFilter_->Update();

	for (const auto& button : buttons_) {
		button.sprite->Update();
	}

	selectFrame_->Update();
}

void PauseUI::Draw() {

	// 背景フィルターの描画
	backFilter_->Draw();

	// ボタンの描画
	for (const auto& button : buttons_) {
		button.sprite->Draw();
	}

	// 選択枠の描画
	selectFrame_->Draw();
}

void PauseUI::CreateButton(PauseSelect select, const std::string& relativePath, const Vector2& position) {

	Button button = {};
	button.sprite = std::make_unique<Sprite>();
	button.sprite->Initialize(relativePath);
	button.sprite->SetAnchorPoint({ 0.5f,0.5f }); // 中央
	button.sprite->SetPosition(position);
	button.position = position;
	button.size = button.sprite->GetSize();
	button.select = select;
	buttons_.emplace_back(std::move(button));
}

bool PauseUI::CheckButtonSelect(const Button& button, const Vector2& mousePosition) {

	// 中心基準の矩形判定
	float halfW = button.size.x / 2.0f;
	float halfH = button.size.y / 2.0f;

	return (mousePosition.x >= button.position.x - halfW &&
		mousePosition.x <= button.position.x + halfW &&
		mousePosition.y >= button.position.y - halfH &&
		mousePosition.y <= button.position.y + halfH);
}

