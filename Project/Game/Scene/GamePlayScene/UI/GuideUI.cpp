#include "GuideUI.h"
#include "Input.h"

void GuideUI::Initialize() {

	// マウススプライト生成
	mouseSprite = std::make_unique<Sprite>();
	mouseSprite->Initialize("Guide/Mouse.png");
	mouseSprite->SetPosition({ 940.0f, 600.0f }); // 画面中央に設定
	mouseSprite->SetAnchorPoint({ 0.5f, 0.5f }); // 中心に設定
	mouseSprite->SetSize({ 96.0f, 96.0f });

	// ボタンAスプライト生成
	buttonASprite = std::make_unique<Sprite>();
	buttonASprite->Initialize("Guide/ButtonA.png");
	buttonASprite->SetPosition({ 1040.0f, 600.0f });
	buttonASprite->SetAnchorPoint({ 0.5f, 0.5f }); // 中心に設定
	buttonASprite->SetSize({ 96.0f, 96.0f });

	// ボタンDスプライト生成
	buttonDSprite = std::make_unique<Sprite>();
	buttonDSprite->Initialize("Guide/ButtonD.png");
	buttonDSprite->SetPosition({ 1140.0f, 600.0f });
	buttonDSprite->SetAnchorPoint({ 0.5f, 0.5f }); // 中心に設定
	buttonDSprite->SetSize({ 96.0f, 96.0f });

	// ボタンESCスプライト生成
	buttonESCSprite = std::make_unique<Sprite>();
	buttonESCSprite->Initialize("Guide/Pause.png");
	buttonESCSprite->SetPosition({ 64.0f, 64.0f });

	isShooting = false;
	isRollLeft = false;
	isRollRight = false;
	isPause = false;

	input = Input::GetInstance();
}

void GuideUI::Update() {

	if (input->PushMouseButton(MouseButton::Left)) {

		// 「まだ射撃フラグが立っていない」時だけ切り替える
		if (isShooting == false) {
			mouseSprite->SetTexture("Resources/Textures/Guide/MouseClick.png"); // 射撃時の画像(黄色)へ
			isShooting = true; // 状態を「射撃中」にする
		}
	}
	// 押していない時
	else {

		// 「射撃フラグが立っていた」時だけ元に戻す
		if (isShooting == true) {
			mouseSprite->SetTexture("Resources/Textures/Guide/Mouse.png"); // 通常時の画像(水色)へ
			isShooting = false; // 状態を「通常」にする
		}
	}

	if (input->PushKey('A')) {

		// 「まだ左ロールフラグが立っていない」時だけ切り替える
		if (isRollLeft == false) {
			buttonASprite->SetTexture("Resources/Textures/Guide/PushA.png"); // 押下時の画像(黄色)へ
			isRollLeft = true; // 状態を「押下中」にする
		}
	}
	// 押していない時
	else {
		// 「左ロールフラグが立っていた」時だけ元に戻す
		if (isRollLeft == true) {
			buttonASprite->SetTexture("Resources/Textures/Guide/ButtonA.png"); // 通常時の画像(水色)へ
			isRollLeft = false; // 状態を「通常」にする
		}
	}

	if (input->PushKey('D')) {
		// 「まだ右ロールフラグが立っていない」時だけ切り替える
		if (isRollRight == false) {
			buttonDSprite->SetTexture("Resources/Textures/Guide/PushD.png"); // 押下時の画像(黄色)へ
			isRollRight = true; // 状態を「押下中」にする
		}
	}
	// 押していない時
	else {
		// 「右ロールフラグが立っていた」時だけ元に戻す
		if (isRollRight == true) {
			buttonDSprite->SetTexture("Resources/Textures/Guide/ButtonD.png"); // 通常時の画像(水色)へ
			isRollRight = false; // 状態を「通常」にする
		}
	}

	//if (input->TriggerKey(VK_ESCAPE)) {
	//	if (isPause == false) {
	//		buttonESCSprite->SetTexture("Resources/Textures/Guide/Back.png"); // 戻るの画像へ
	//		isPause = true; // 状態を「ポーズ中」にする
	//	}
	//}
	//else {
	//	if (isPause == true) {
	//		buttonESCSprite->SetTexture("Resources/Textures/Guide/Pause.png"); // ポーズの画像へ
	//		isPause = false; // 状態を「通常」にする
	//	}
	//}

	// マウススプライト更新
	mouseSprite->Update();

	// ボタンAスプライト更新
	buttonASprite->Update();

	// ボタンDスプライト更新
	buttonDSprite->Update();

	// ボタンESCスプライト更新
	buttonESCSprite->Update();
}

void GuideUI::Draw() {

	// マウススプライト描画
	mouseSprite->Draw();

	// ボタンAスプライト描画
	buttonASprite->Draw();

	// ボタンDスプライト描画
	buttonDSprite->Draw();

	// ボタンESCスプライト描画
	buttonESCSprite->Draw();
}

void GuideUI::ShowImGui() {
}