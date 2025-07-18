#include "Basecharacter.h"

void Basecharacter::Initialize() {
}

void Basecharacter::Update() {
}

void Basecharacter::Draw() {
}

void Basecharacter::OnCollision(Collider* other) {
}

const Vector3 Basecharacter::GetCenterPosition() const {
  
	return worldTransform_.GetTranslate();
}
