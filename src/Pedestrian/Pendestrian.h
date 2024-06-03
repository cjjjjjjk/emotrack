#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <vector>
#include "src/ward/Ward.h"
#include "src/wall/Wall.h"
#include "src/object/MovingObject.h"
// Author: HAI =========================================
enum Walkability {
    noDisability, 
    crutches, 
    sticks, 
    wheelchairs, 
    blind
};
enum PedesType{
    patient,
    visitor,
    personel
};
class Pendestrian : public MovingObject
{
private:
    int ID; 
    double age;
    PedesType type;
    double v;
    double walkingTime;
    double distance;
    // Ward start;
    // Ward end;
    std::shared_ptr<Ward> start;
    std::shared_ptr<Ward> end;

    std::vector<std::shared_ptr<Ward>> journey; 
    
    // for rendering
    float radius = 0.2;
    bool stopAtCorridor;

public:
    Pendestrian() : start(nullptr), end(nullptr), journey({}) {
        std::cout<<" Create a pedestrian ! \n";
    };
    Pendestrian(int ID, double age) : ID(ID), age(age) {};
    Pendestrian(int ID, int age, float velocity, int walkingTime, int distace, std::shared_ptr<Ward> start, std::shared_ptr<Ward> end) : 
    ID(ID), age(age), walkingTime(walkingTime), distance(distace), start(start), end(end)
    {
        this->setDesiredSpeed(velocity);
    };

    virtual ~Pendestrian() = default;

    void SetType(PedesType type) {this->type = type;};
    void SetID(int id){ID = id;};
    void SetAge(double ag) {age = ag;};
    void SetVelocity(float v) {this->v = v; this->setDesiredSpeed(v);};
    void SetDistance(double dis) {distance = dis;};
    void SetWalkingTime(double time) {walkingTime = time;};
    void SetStartWard(std::shared_ptr<Ward> ward) {start = ward;};
    void SetEndWard(std::shared_ptr<Ward> ward) {end = ward;} 
    void addWard(std::shared_ptr<Ward> ward) {journey.push_back(ward);};

    float getStopAtCorridor() const { return stopAtCorridor; }
    int GetID() {return this->ID;};
    double GetAge() { return this->age;};
    PedesType GetPedesType(){return type;};
    double GetVelo() {return this->v;};
    std::shared_ptr<Ward> getStartWard() {return start;};
    std::shared_ptr<Ward> getEndWard() {return end;};
    std::vector<std::shared_ptr<Ward>> getJourney() {return journey;};

    // Radius
    void setRadius(float radius);
    float getRadius() const { return radius; }

    float getMinDistanceToWalls(std::vector<Wall *> walls, Point3f position, float radius);
    // Vector3f
    Vector3f getDrivingForce(const Point3f destination);
    Vector3f getAgentInteractForce(std::vector<std::shared_ptr<Pendestrian>> pedesntrian_list);
    Vector3f getWallInteractForce(std::vector<Wall *> walls);
    // Vector3f getAgvInteractForce(std::vector<AGV *> agvs);

    void move(std::vector<std::shared_ptr<Pendestrian>> pedes_list, std::vector<Wall *> walls, float stepTime); // add AGVs
};
