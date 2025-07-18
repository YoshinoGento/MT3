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
struct Vector2 {  
float x;  
float y;  
};  

// 球  
struct Sphere {  
Vector3 center; //!< 中心点  
float radius;   //!< 半径  
};  

// 線分  
//struct Segment {  
//Vector3 start; //!< 始点  
//Vector3 end;   //!< 終点 
//};


/// <summary>
/// 直線
/// </summary>
struct Line {
	Vector3 origin;//!< 始点
	Vector3 diff;  //!< 終点への差分ベクトル

};


/// <summary>
/// 半直線
/// </summary>
struct Ray {
	Vector3 origin;//!< 始点
	Vector3 diff;  //!< 終点への差分ベクトル
};

/// <summary>
/// 線分
/// </summary>
struct Segment {
    Vector3 origin;
    Vector3 diff;

    Vector3 End() const {
        return {
            origin.x + diff.x,
            origin.y + diff.y,
            origin.z + diff.z
        };
    }
};

struct Plane {
    Vector3 normal;//!<法線
    float distance;//!<距離
};

struct Triangle {
	Vector3 vertex[3]; //!< 頂点の配列
};

struct AABB{ 
    Vector3 min; //!< 最小点
	Vector3 max; //!< 最大点
};