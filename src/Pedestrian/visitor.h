#pragma once
#include "Pendestrian.h"


class Visitor : public Pendestrian
{
private:
    Walkability walkability;
public:
    Visitor();
    Visitor(int ID, double age) : Pendestrian(ID, age) {};
    Visitor(int ID, int age, int velocity, int walkingTime, int distace, std::shared_ptr<Ward> start, std::shared_ptr<Ward> end, Walkability wal) 
    : Pendestrian(ID, age, velocity,walkingTime,distace, start, end), walkability(wal)  {};
    ~Visitor();


    void SetWalkability(Walkability wal) {this->walkability = wal;};
    Walkability GetWalkability(){return this->walkability;};


};

Visitor ::Visitor ()
{
}
Visitor ::~Visitor ()
{
}
