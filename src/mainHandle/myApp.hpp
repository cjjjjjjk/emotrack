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
float walkwayWidth = (float)inputData["hallwayWidth"]["value"];
int checkType(double age, double v) // 1-Personel  2-Patient 3-Visitor
{
    if(age < 61 && age > 23 && v > 1.2) return 1;
    if(age > 80 || v < 0.7) return 2;
    return 3;
}
bool checkInsideWardP(Point3f position)
{
    

}
// Tạo tọa độ bất kỳ cho một pedés trong bệnh viện.
std::vector<Point3f> CreateRandomPosition(int M, std::vector<std::shared_ptr<Pendestrian>> &pede_lis);
// Lấy một tọa độ bất kỳ trong 1 Ward (Xây dụng điểm đến cho một pedestrian)
Point3f GetWardRandomPosition(std::shared_ptr<Ward> ward)
{
    double x_min = ward->GetGate1_x() - ward->GetLength()/2 + 1.0 ;
    double x_max = x_min + ward->GetLength() - 2.0;
    double y_min = ward->GetGate2_y() + 1.0;
    double y_max = ward->GetGate1_y() - 1.0;

    // Khởi tạo seed cho hàm rand()
    static bool initialized = false;
    if (!initialized) {
        std::srand(std::time(0));
        initialized = true;
    }
        Point3f p;
        // Tạo giá trị ngẫu nhiên cho x trong khoảng [x_min, x_max]
        p.x = x_min + static_cast<double>(rand()) / (static_cast<double>(RAND_MAX / (x_max - x_min)));
        // Tạo giá trị ngẫu nhiên cho y trong khoảng [y_min, y_max]
        p.y = y_min + static_cast<double>(rand()) / (static_cast<double>(RAND_MAX / (y_max - y_min)));
        p.z = 0.0;
        
    return p;
}

// Ham xay dung path
void setPedesFlow(std::shared_ptr<Pendestrian> &pedes, int caseJump, std::vector<float> juncData)
{
    // if (socialForce->getCrowdSize() < threshold)
    // {
    //     agent->setStopAtCorridor(true);
    // }
    float desiredSpeed = pedes->getDesiredSpeed();
    int codeSrc = 0;
    int codeDes = 0;

    int juncType = juncData.size();

    if (juncType == 4)
    {
        if (caseJump < 3)
        {
            codeSrc = 0; // Go from Left to Right
        }
        else if (caseJump < 6)
        {
            codeSrc = 1; // Go from Right to Left
        }
        else if (caseJump < 9)
        {
            codeSrc = 2; // Go from Top to Bottom
        }
        else
        {
            codeSrc = 3; // Go from Bottom to Top
        }
    }
    else if (juncType == 3)
    {
        if (caseJump < 6)
        {
            codeSrc = 0;
            if (caseJump % 2 == 0)
            {
                codeDes = 0;
            }
            else
            {
                codeDes = 2;
            }
        }
        else if (caseJump < 12)
        {
            codeSrc = 1;
            if (caseJump % 2 == 0)
            {
                codeDes = 1;
            }
            else
            {
                codeDes = 2;
            }
        }
        else if (caseJump < 18)
        {
            codeSrc = 3;
            if (caseJump % 2 == 0)
            {
                codeDes = 0;
            }
            else
            {
                codeDes = 1;
            }
        }
    }
    else if (juncType == 2)
    {
        if (caseJump < 3)
        {
            codeSrc = 0;
        }
        else
        {
            codeSrc = 1;
        }
    }

    // vector<float> position = Utility::getPedesSource(
    //     codeSrc,
    //     (float)inputData["totalCrowdLength"]["value"],
    //     (float)inputData["headCrowdLength"]["value"],
    //     (float)inputData["crowdWidth"]["value"],
    //     walkwayWidth,
    //     juncData);
    vector<float> desList;

    if (juncType == 4 || juncType == 2)
    {
        desList = Utility::getPedesDestination(codeSrc, caseJump % 3, walkwayWidth, juncData, pedes->getStopAtCorridor());
    }
    else if (juncType == 3)
    {
        desList = Utility::getPedesDestination(codeDes, caseJump % 3, walkwayWidth, juncData, pedes->getStopAtCorridor());
    }

    // pedes->setPosition(position[0], position[1]);
    if (juncType == 3 && codeSrc != codeDes)
    {
        pedes->setPath(randomFloat(-walkwayWidth / 2, walkwayWidth / 2), randomFloat(-walkwayWidth / 2, walkwayWidth / 2), 2.0);
    }
    pedes->setPath(desList[0], desList[1], desList[2]);
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
    // Vecto toa do
    
    // cout << "Deviation: "<< deviationParam <<" - Num agents: "<< int(int(inputData["numOfAgents"]["value"]) * deviationParam) << endl;
    vector<double> velocityList = Utility::getPedesVelocity(0, inputData, deviationParam);
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
                    personel->setColor(133.0, 22.0, 255.0);
                    personel->SetType(PedesType::personel);
                    // Them doi tuong vao mang
                    pedestrian_list.push_back(personel);
                } else{
                    std::shared_ptr<Visitor> visitor = std::make_shared<Visitor>(IDcount, age);
                    visitor->SetWalkability(Walkability::noDisability);
                    numberOfVisitor++;
                    visitor->SetType(PedesType::visitor);
                    visitor->SetVelocity(v);
                    visitor->setColor(0.0, 128.0, 0.0);
                    pedestrian_list.push_back(visitor);
                }
        } else if(checkType(age, v) == 2) // patient
        {
            std::shared_ptr<Patient> patient = std::make_shared<Patient>(IDcount, age);
            numberOfPatient++;
            patient->SetWalkability(Walkability::crutches);
            patient->SetType(PedesType::patient);
            patient->SetVelocity(v);
            patient->setColor(216.0, 32.0, 42.0);
            pedestrian_list.push_back(patient);
        } else
        {
            std::shared_ptr<Visitor> visitor = std::make_shared<Visitor> (IDcount, age);
            visitor->SetWalkability(Walkability::noDisability);
            numberOfVisitor++;
            visitor->SetType(PedesType::visitor);
            visitor->SetVelocity(v);
            visitor->setColor(0.0, 128.0, 0.0);
            pedestrian_list.push_back(visitor);
        }
    }
    std::cout<<"mainHandle line 128 ------\n";
    std::cout<<"Pendestrian: "<<pedestrian_list.size()<<"\n";
    std::cout<<"Personel: "<<numberOfPersonel<<"\n";
    std::cout<<"Visitor : "<<numberOfVisitor<<"\n";
    std::cout<<"Patient : "<<numberOfPatient<<"\n--------------------------\n";
}

// Hàm sinh ra mảng tọa độ bất kỳ
std::vector<Point3f> CreateRandomPosition(int M, std::vector<std::shared_ptr<Pendestrian>> &pedes_list) {

    std::cout<<"lin251: "<<pedes_list.size()<<" \n";
    // Khởi tạo seed cho hàm rand()
    std::srand(std::time(0));
    double x_min, x_max, y_min, y_max;
    std::vector<Point3f> points;
    for (int i = 0; i < M; ++i) {
    // Đặt giới hạn cho tọa độ x và y dựa trên tọa độ các góc của căn phòng
    std::shared_ptr<Ward> startWard = pedes_list[i]->getStartWard();
    x_min = startWard->GetGate1_x() - startWard->GetLength()/2 - 1.5F;
    x_max = x_min + startWard->GetLength() + 3.0;
    y_min = startWard->GetGate2_y() + 2.0F;
    y_max = startWard->GetGate1_y() - 2.0F;
    if(y_max >= 11)y_max =10.6; 
    points.reserve(M); // Dự trữ không gian cho M phần tử
        Point3f p;
        // Tạo giá trị ngẫu nhiên cho x trong khoảng [x_min, x_max]
        p.x = x_min + static_cast<double>(rand()) / (static_cast<double>(RAND_MAX / (x_max - x_min)));
        // Tạo giá trị ngẫu nhiên cho y trong khoảng [y_min, y_max]
        p.y = y_min + static_cast<double>(rand()) / (static_cast<double>(RAND_MAX / (y_max - y_min)));
        p.z = 0.0;
        // Thêm điểm vào vector
        points.push_back(p);
        pedes_list[i]->setPosition(p.x , p.y);
    }
    return points;
}


// Bai 4: So lan xuat hien cac phong theo luat phan phoi chuan
std::vector<std::pair<std::shared_ptr<Ward>, int>>  Create_pairWard_list(std::vector<std::shared_ptr<Ward>> Ward_list, int triple, int single)
{
    std::vector<std::pair<std::shared_ptr<Ward>, int>> rs ;
    int total = triple*3 + single;
    if((triple+single) != numberOfAgent ) 
    {
        std::cout<<"mainHandle line 170: Gia tri triple va single khong thoa man  ="<<(int)inputData["numOfAgents"]["value"]<<"!  ";
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
    std::vector<std::pair<std::shared_ptr<Ward>, int>> pair_list = Create_pairWard_list(Ward_list, triple, single); 

    int sizeofPairlist = pair_list.size();
    // Hiện để kiểm tra thông tin ----------
    std::shared_ptr<Ward> ward;
    for(int i = 0;  i < sizeofPairlist; i++ )
    {
        ward = pair_list[i].first;
        // std::cout<<"mainHandle line 219: Room: "<<ward->GetID()<<" repeat: "<<pair_list[i].second<<"\n";
    }
    //--------------------------------------

    // CHat GPT 
    std::unordered_map<char, int> ward_count;
    for (const auto& pair : pair_list) {
        ward_count[pair.first->GetID()] = pair.second;
    }

    // Hàm để lấy ký tự từ từ điển với số lần xuất hiện còn lại lớn nhất
    auto getMaxWard = [&](int n) -> std::vector<char> {
        std::vector<std::pair<char, int>> sorted_wards(ward_count.begin(), ward_count.end());
        std::sort(sorted_wards.begin(), sorted_wards.end(), [](const std::pair<char, int>& a, const std::pair<char, int>& b) {
            return a.second > b.second;
        });

        std::vector<char> result;
        for (int i = 0; i < n && i < sorted_wards.size(); ++i) {
            result.push_back(sorted_wards[i].first);
        }
        return result;
    };

    // Gán journey cho từng pedestrian
    for (std::shared_ptr<Pendestrian> &pedestrian : pedestrian_list) {
        if (pedestrian->GetPedesType() == PedesType::visitor) {
            // Loại pedestrian nhận 1 Ward
            std::vector<char> maxWard = getMaxWard(1);
            if (!maxWard.empty()) {
                // pedestrian.journey.push_back(Ward(maxWard[0]));
                bool isAdded = false;
                if(!isAdded)for(auto ward : Ward_list) if(ward->GetID() == maxWard[0])
                {
                    pedestrian->addWard(ward);
                    isAdded = true;
                }
                ward_count[maxWard[0]]--;
                if (ward_count[maxWard[0]] == 0) {
                    ward_count.erase(maxWard[0]);
                }
            }
        } else {
            // Loại pedestrian nhận 3 Ward
            std::vector<char> maxWards = getMaxWard(3);
            for (char id : maxWards) {
                for(auto ward : Ward_list) if(ward->GetID() == id)pedestrian->addWard(ward);
                ward_count[id]--;
                if (ward_count[id] == 0) {
                    ward_count.erase(id);
                }
            }
        }
    }

    // Kiểm tra xem tất cả các ký tự trong từ điển đã được sử dụng hết chưa
    for (const auto& count : ward_count) {
        if (count.second != 0) {
            std::cerr << "Error: Not all wards have been fully assigned!" << std::endl;
        }
    }

    // Thiet lap start ward cho cac pedestrian
    for(auto pedes : pedestrian_list)
    {
        pedes->SetStartWard(pedes->getJourney()[0]);
    }
    // test infor ---------------------------------
    // std::cout<<"\n====================\n";
    // for(int i = 0; i<= 10; i++){
    // std::string type;
    // if(pedestrian_list[i]->GetPedesType() == PedesType::patient) type = "Patient ";
    // else if(pedestrian_list[i]->GetPedesType() == PedesType::personel) type = "Personel ";
    // else if(pedestrian_list[i]->GetPedesType() == PedesType::visitor) type = "Visitor ";
    //     std::cout<<pedestrian_list[i]->GetID()<<" \t"<<"type:"<<type<<"\t";
    //     std::vector<std::shared_ptr<Ward>> jouney_test = pedestrian_list[i]->getJourney();
    //     for(auto ward : jouney_test) std:cout<<ward->GetID()<<" ";
    //     std::cout<<"\n----------------\n";
    // }
    // -------------------------------------------------
}


// First destination.
void SetPedesDestination(std::vector<std::shared_ptr<Pendestrian>> &Pedes_list)
{
    for(std::shared_ptr<Pendestrian>  pedes : Pedes_list)
    {
        std::shared_ptr<Ward> fWard = pedes->getStartWard();
        Point3f Destination_position = GetWardRandomPosition(fWard);
        // std::cout<<"myAP linr 416 : pos"<<Destination_position<<"\n";
        pedes->setDestination(Destination_position.x, Destination_position.y);
        pedes->setPath(Destination_position.x, Destination_position.y, pedes->getRadius());
    }
}