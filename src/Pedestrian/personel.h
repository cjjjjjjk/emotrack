#pragma once
#include "Pendestrian.h"

class Personel : public Pendestrian
{
private:
    /* data */
public:
    Personel(/* args */);
    Personel(int ID, int age) : Pendestrian(ID, age) {};
    Personel(int ID, int age, int velocity, int walkingTime, int distace, std::shared_ptr<Ward> start, std::shared_ptr<Ward> end) 
    : Pendestrian(ID, age, velocity,walkingTime,distace, start, end) {};
    ~Personel();
};

Personel::Personel(/* args */)
{
}

Personel::~Personel()
{
}
