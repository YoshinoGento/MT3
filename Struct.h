#pragma once  

struct Vector4 {  
float x;  
float y;  
float z;  
float w;  
};  


// -- Vector3の定義を追加 (演算子オーバーロード）-- //
struct Vector3 {
    float x, y, z;
    Vector3(float _x = 0, float _y = 0, float _z = 0) : x(_x), y(_y), z(_z) {}
    Vector3 operator+(const Vector3& rhs) const { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
    Vector3 operator-(const Vector3& rhs) const { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
    Vector3 operator*(float s) const { return Vector3(x * s, y * s, z * s); }
};

//struct Vector3 {  
//float x;  
//float y;  
//float z;  
//};  

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
struct segment {  
Vector3 start; //!< 始点  
Vector3 end;   //!< 終点 
};


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