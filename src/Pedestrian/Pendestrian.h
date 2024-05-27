#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include "../ward/Ward.h"
// Author: HAI =========================================
class Pendestrian
{
private:
    int ID; 
    double velocity;
    double workingTime;
    double distance;
    double age;
    // Ward start;
    // Ward end;
    std::shared_ptr<Ward> start;
    std::shared_ptr<Ward> end;

    std::vector<std::shared_ptr<Ward>> journey; 
    

public:
    Pendestrian() {};
    ~Pendestrian() {};
};
