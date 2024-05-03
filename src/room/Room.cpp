#include <iostream>

#include "Room.h"
#include "../wall/Wall.h"
#include "../model/SocialForce.h"


Room::Room(int G1_x, int G1_y, int G2_x, int G2_y, int length, char ID) : G1_x(G1_x), G1_y(G1_y), G2_x(G2_x), G2_y(G2_y), length(length)
{
    
}

Room::~Room()
{
//  std::cout<<"\nDestructor Room ~";    
}