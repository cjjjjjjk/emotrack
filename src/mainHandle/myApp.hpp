#pragma once
#include <iostream>
#include <vector>
#include <lib/nlohmann/json.hpp>
#include <src/utility/Utility.h>
#include "src/Pedestrian/Pendestrian.h"
#include "src/Pedestrian/personel.h"
#include "src/Pedestrian/patient.h"
#include "src/Pedestrian/visitor.h"



int numberOfPersonel = 0;
int numberOfVisitor = 0;
int numberOfPatient = 0;
int type; // 1 2 3
bool check_age(int type, int age)
{
    if(type == 1 ) // personel
    {
        if(age > 61) return false;
        if(age < 23) return false;
        return true;
    }

}


void CreatePedestrian_list(std::vector<std::shared_ptr<Pendestrian>> &pedestrian_list, int M = 50)
{
    int IDcount = 0;
    // Mang do tuoi cua 50 nguoi di bo
    double Age_arr[51] = {79.4, 76.1, 53.3, 66.8, 49.9, 37., 53.7, 60.2, 74.2, 53.7, 27.9, 62.2, 64.2, 27.2, 50., 67.2, 50.3, 62.3, 14.8, 79.9, 62.3, 47.9, 85.8, 50.7, 68.9, 79.5, 45.9, 52.1, 50., 38.8, 77.2, 83.5, 35.3, 64.1, 42.3, 38.8, 54.5, 93.9, 92., 56.1, 97.8, 67.1, 38.1, 47.5, 38.5, 48.2, 56., 53.6, 71.8, 73.0 };
    // So luong nguoi di bo = 50
    int numberOfwalkability[] = {13, 9 ,10, 8,4,6};
    /*  noDisabilityNoOvertaking    : 13
        “noDisabilityOvertaking”    : 9
        “crutches”                  : 10
        sticks                      : 8
        wheelchair                  : 4
        blind                       : 6
    */ 
    Walkability walkability;
    
    for(int i = 1; i<= M; i++)
    {
        IDcount++;
        double age = Age_arr[i];
        
        // Tao doi tuong la nhan vien y te Personel
        if(check_age(1, age) && (numberOfPersonel < numberOfwalkability[0]))
        {
                std::shared_ptr<Personel> personel = std::make_shared<Personel>(IDcount, age);
                numberOfPersonel++;
                // Xay dung cac gia tri con lai



                // Them doi tuong vao mang
                pedestrian_list.push_back(personel);
        }


    }

}