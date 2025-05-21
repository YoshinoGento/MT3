#pragma once
#include "Struct.h"

class Player {
public:
    Vector3 position;


    void Move(char* keys);

    float speed_;
    float radius_ = 20.0f;
};

