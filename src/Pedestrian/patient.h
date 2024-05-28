#pragma once
#include "Pendestrian.h"

class Patient : public Pendestrian
{
private:
    Walkability walkability;
public:
    Patient() {};
    Patient(int ID, int age) : Pendestrian(ID, age) {};
    Patient(int ID, int age, int velocity, int walkingTime, int distace, std::shared_ptr<Ward> start, std::shared_ptr<Ward> end, Walkability wal) 
    : Pendestrian(ID, age, velocity,walkingTime,distace, start, end), walkability(wal) {};
    ~Patient() {};

    void SetWalkability(Walkability wal) {this->walkability = wal;};
    Walkability GetWalkability(){return this->walkability;};

    void GetType() override {std::cout<<"A Patient ! ";};
};
