#include <iostream>

#include "Ward.h"
#include "../wall/Wall.h"
#include "../model/SocialForce.h"


Ward::Ward(int G1_x, int G1_y, int G2_x, int G2_y, int length, char ID) : G1_x(G1_x), G1_y(G1_y), G2_x(G2_x), G2_y(G2_y), length(length)
{
    InPosition = Point2(G1_x, G1_y);
    OutPosition = Point2(G2_x, G2_y);
}

Ward::~Ward()
{
//  std::cout<<"\nDestructor Room ~";    
}

Point2<int> Ward::GetInposition()
{
    return InPosition;
}
Point2<int> Ward::GetOutposition()
{
    return OutPosition;
}