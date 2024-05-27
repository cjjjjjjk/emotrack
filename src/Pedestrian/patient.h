#pragma once
#include "Pendestrian.h"

class Patient : public Pendestrian
{
private:
    Walkability walkability;
public:
    Patient();
    Patient(int ID, int age, int velocity, int walkingTime, int distace, std::shared_ptr<Ward> start, std::shared_ptr<Ward> end, Walkability wal) 
    : Pendestrian(ID, age, velocity,walkingTime,distace, start, end), walkability(wal) {};
    ~Patient();

};

Patient::Patient(/* args */)
{
}

Patient::~Patient()
{
}
