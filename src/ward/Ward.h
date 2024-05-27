#pragma once
#include "lib/vecmath/vecmath.h"
class Ward
{
private:
    char ID;
    int G1_x, G1_y, G2_x, G2_y;
    Point2<int> InPosition;
    Point2<int> OutPosition;
    int length;
public:
    Ward();
    Ward(int G1_x, int G1_y, int G2_x, int G2_y, int length, char ID);
    ~Ward();

    char GetID() {return this->ID;};

    int GetGate1_x() {return this->G1_x;};
    int GetGate1_y() {return this->G1_y;};
    Point2<int> GetInposition();

    Point2<int> GetOutposition();
    int GetGate2_x() {return this->G2_x;} 
    int GetGate2_y() {return this->G2_y;}

    int GetLength()  {return this->length;}
    void Draw(){};
};


