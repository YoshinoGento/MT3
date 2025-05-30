#pragma once

struct Vector4 {
	float x;
	float y;
	float z;
	float w;
};
struct Vector3 {
	float x;
	float y;
	float z;
};
struct  Vector2 {
	float x;
	float y;
};

//球
struct Sphere {
	Vector3 center; //!< 中心点
	float radius;   //!< 半径
};