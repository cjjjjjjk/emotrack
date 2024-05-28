#pragma once
#include <iostream>
#include <vector>
#include <random>
#include <utility>
#include <lib/nlohmann/json.hpp>
#include <src/utility/Utility.h>
#include "src/Pedestrian/Pendestrian.h"
#include "src/Pedestrian/personel.h"
#include "src/Pedestrian/patient.h"
#include "src/Pedestrian/visitor.h"
#include "src/ward/Ward.h"


json inputData = Utility::readInputData("data/input.json");
int numberOfPersonel = 0;
int numberOfVisitor = 0;
int numberOfPatient = 0;
int numberOfAgent = (int)inputData["numOfAgents"]["value"];
int checkType(double age, double v) // 1-Personel  2-Patient 3-Visitor
{
    if(age < 61 && age > 23 && v > 1.2) return 1;
    if(age > 80 || v < 0.7) return 2;
    return 3;
}


void CreatePedestrian_list(std::vector<std::shared_ptr<Pendestrian>> &pedestrian_list, int M = 50)
{
    int IDcount = 0;
    // Mang do tuoi cua 50 nguoi di bo
    double Age_arr[] = {79.4, 76.1, 53.3, 66.8, 49.9, 37., 53.7, 60.2, 74.2, 53.7, 27.9, 62.2, 64.2, 27.2, 50., 67.2, 50.3, 62.3, 14.8, 79.9, 62.3, 47.9, 85.8, 50.7, 68.9, 79.5, 45.9, 52.1, 50., 38.8, 77.2, 83.5, 35.3, 64.1, 42.3, 38.8, 54.5, 93.9, 92., 56.1, 97.8, 67.1, 38.1, 47.5, 38.5, 48.2, 56., 53.6, 71.8, 73.0 };
    // So luong nguoi di bo = 50
    int numberOfwalkability[] = {13, 9 ,10, 8,4,6};
    /*  noDisabilityNoOvertaking    : 13
        “noDisabilityOvertaking”    : 9
        “crutches”                  : 10
        sticks                      : 8
        wheelchair                  : 4
        blind                       : 6
    */ 

    // Vector van toc ======
    float deviationParam = randomFloat(1 - (float)inputData["experimentalDeviation"]["value"] / 100, 1 + (float)inputData["experimentalDeviation"]["value"] / 100);
    // cout << "Deviation: "<< deviationParam <<" - Num agents: "<< int(int(inputData["numOfAgents"]["value"]) * deviationParam) << endl;
    vector<double> velocityList = Utility::getPedesVelocity(0, inputData, deviationParam);
    std::cout<<deviationParam<<" "<<velocityList.size()<<"\n";
    auto rng = std::default_random_engine{};
    std::shuffle(velocityList.begin(), velocityList.end(), rng);    
    //=================

    Walkability walkability;
    
    for(int i = 0; i< M; i++)
    {
        IDcount++;
        double age = Age_arr[i];
        double v = velocityList[i];
        // Tao doi tuong la nhan vien y te Personel
        if(checkType(age, v) == 1 )
        {
                if(numberOfPersonel < (numberOfwalkability[0]+numberOfwalkability[1])){

                    std::shared_ptr<Personel> personel = std::make_shared<Personel>(IDcount, age);
                    numberOfPersonel++;
                    // Xay dung cac gia tri con lai

                    personel->SetVelocity(v);
                    personel->SetType(PedesType::personel);
                    // Them doi tuong vao mang
                    pedestrian_list.push_back(personel);
                } else{
                    std::shared_ptr<Visitor> visitor = std::make_shared<Visitor>(IDcount, age);
                    visitor->SetWalkability(Walkability::noDisability);
                    numberOfVisitor++;
                    visitor->SetType(PedesType::visitor);
                    visitor->SetVelocity(v);
                    pedestrian_list.push_back(visitor);
                }
        } else if(checkType(age, v) == 2) // patient
        {
            std::shared_ptr<Patient> patient = std::make_shared<Patient>(IDcount, age);
            numberOfPatient++;
            patient->SetWalkability(Walkability::crutches);
            patient->SetType(PedesType::patient);
            patient->SetVelocity(v);
            pedestrian_list.push_back(patient);
        } else
        {
            std::shared_ptr<Visitor> visitor = std::make_shared<Visitor> (IDcount, age);
            visitor->SetWalkability(Walkability::noDisability);
            numberOfVisitor++;
            visitor->SetType(PedesType::visitor);
            visitor->SetVelocity(v);
            pedestrian_list.push_back(visitor);
        }
    }
    std::cout<<"Pendestrian: "<<pedestrian_list.size()<<"\n";
    std::cout<<"Personel: "<<numberOfPersonel<<"\n";
    std::cout<<"Visitor : "<<numberOfVisitor<<"\n";
    std::cout<<"Patient : "<<numberOfPatient<<"\n";
}


// Bai 4: So lan xuat hien cac phong theo luat phan phoi chuan
std::vector<std::pair<std::shared_ptr<Ward>, int>>  Create_pairWard_list(std::vector<std::shared_ptr<Ward>> Ward_list, int triple, int single)
{
    std::vector<std::pair<std::shared_ptr<Ward>, int>> rs ;
    int total = triple*3 + single;
    if((triple+single) != numberOfAgent ) 
    {
        std::cout<<"Gia tri triple va single khong thoa man  ="<<(int)inputData["numOfAgents"]["value"]<<"!  ";
        return rs ;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> d(0, 1);
    std::vector<int> intValues;
    int currentSum = 0;
    for (size_t i = 0; i < Ward_list.size(); ++i) {
        int value = std::round(d(gen));
        intValues.push_back(value);
        currentSum += value;
    }

    int diff = total - currentSum;
    while (diff != 0) {
        for (size_t i = 0; i < intValues.size() && diff != 0; ++i) {
            if (diff > 0) {
                ++intValues[i];
                --diff;
            } else if (diff < 0) {
                --intValues[i];
                ++diff;
            }
        }
    }

    for (size_t i = 0; i < Ward_list.size(); ++i) {
        rs.push_back(std::make_pair(Ward_list[i], intValues[i]));
    }

    return rs;
}


// Bài 5: Xây dựng lộ trình (journey) cho 50 người đi bộ.
void SetPedesJourney(std::vector<std::shared_ptr<Pendestrian>> &pedestrian_list, std::vector<std::shared_ptr<Ward>> &Ward_list)
{
    int single = numberOfVisitor;
    int triple = numberOfAgent - single;
    std::cout<<"triple : "<<triple<<"  single:  "<<single<<"\n";
    std::vector<std::pair<std::shared_ptr<Ward>, int>> pair_list = Create_pairWard_list(Ward_list, triple, single); 

    // Hiện để kiểm tra thông tin ----------
    std::shared_ptr<Ward> ward;
    for(int i = 0;  i < pair_list.size(); i++ )
    {
        ward = pair_list[i].first;
        std::cout<<"Room: "<<ward->GetID()<<" repeat: "<<pair_list[i].second<<"\n";
    }
    //--------------------------------------

    

}