#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <vector>
#include "src/ward/Ward.h"
// Author: HAI =========================================
enum Walkability {
    noDisability, 
    crutches, 
    sticks, 
    wheelchairs, 
    blind
};
class Pendestrian
{
private:
    int ID; 
    double velocity;
    double walkingTime;
    double distance;
    double age;
    // Ward start;
    // Ward end;
    std::shared_ptr<Ward> start;
    std::shared_ptr<Ward> end;

    std::vector<std::shared_ptr<Ward>> journey; 
    

public:
    Pendestrian() : start(nullptr), end(nullptr), journey({}) {
        std::cout<<" Create a pedestrian ! \n";
    };
    Pendestrian(int ID, double age) : ID(ID), age(age) {};
    Pendestrian(int ID, int age, int velocity, int walkingTime, int distace, std::shared_ptr<Ward> start, std::shared_ptr<Ward> end) : 
    ID(ID), age(age), velocity(velocity), walkingTime(walkingTime), distance(distace), start(start), end(end)
    {};

    virtual ~Pendestrian() = default;

    void SetID(int id){ID = id;};
    void SetAge(double ag) {age = ag;};
    void SetDistance(double dis) {distance = dis;};
    void SetWalkingTime(double time) {walkingTime = time;};
    void SetStartWard(std::shared_ptr<Ward> ward) {start = ward;};
    void SetEndWard(std::shared_ptr<Ward> ward) {end = ward;} 
    void addWard(std::shared_ptr<Ward> ward) {journey.push_back(ward);};


    int GetID() {return this->ID;};
    double GetAge() { return this->age;};
    std::shared_ptr<Ward> getStartWard() {return start;};
    std::shared_ptr<Ward> getEndWard() {return end;};
    std::vector<std::shared_ptr<Ward>> getJourney() {return journey;};

    virtual void GetType(){};
};
