#include "SlideTransition.h"
#include "Sprite/SpriteRenderer.h"
#include "Easing.h"

using namespace Engine;
using namespace Easing;

SlideTransition::SlideTransition( Vector2 endPosition ) {

	endPosition_ = endPosition;

	// スプライトレンダラーのインスタンス取得
	spriteRenderer_ = SpriteRenderer::GetInstance();

	// スプライトの初期化
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize( "White1x1.png" );
	sprite_->SetSize( { 1280.0f, 720.0f } ); // 画面全体を覆うサイズに設定
	sprite_->SetColor( { 1.0f, 1.0f, 1.0f, 1.0f } ); // 黒色で初期化
}

void SlideTransition::Update( float progress ) {

	// 進行度に応じてスプライトの位置を更新
	Vector2 currentPosition = Lerp( startPosition_, endPosition_, progress );

	// スプライトの位置を更新
	sprite_->SetPosition( currentPosition );

	// スプライトの更新
	sprite_->Update();
}

void SlideTransition::Draw() {

	// スプライトの描画設定
	spriteRenderer_->SettingDrawing();

	// スプライトの描画
	sprite_->Draw();
}

void SlideTransition::ShowImGui() {


}