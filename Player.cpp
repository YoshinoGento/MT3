#include "Player.h"
#include "Novice.h"

//
//Player::Player(float x, float y) : position_(x, y) {}


//Player::Player(float x, float y) {
//    position_ = Vector2(x, y);
//}

void Player::Move(char* keys) {
	if (keys[DIK_W]) {
		position.y -= speed_;
	}
	if (keys[DIK_S]) {
		position.y += speed_;
	}
	if (keys[DIK_A]) {
		position.x -= speed_;
	}
	if (keys[DIK_D]) {
		position.x += speed_;
	}