#include "SocialForce.h"

using namespace std;

SocialForce::~SocialForce()
{
    removePedeslist();
    removeCrowd();
    removeWalls();
    removeAGVs();
}

void SocialForce::addPedestrian(std::shared_ptr<Pendestrian> pendes)
{
    Pendestrian_list.push_back(pendes);
}

void SocialForce::removePedes(int pedesID)
{
    if (!Pendestrian_list.empty())
    {
        Pendestrian_list.erase(remove_if(begin(Pendestrian_list), end(Pendestrian_list), [pedesID](std::shared_ptr<Pendestrian> pedes)
                              { return pedes->getId() == pedesID; }),end(Pendestrian_list));
    }
}

void SocialForce::removePedeslist()
{
    Pendestrian_list.clear();
}

void SocialForce::addAgent(Agent *agent) { crowd.push_back(agent); }

void SocialForce::addWall(Wall *wall) { walls.push_back(wall); }

void SocialForce::addAGV(AGV *agv) { agvs.push_back(agv); }

void SocialForce::removeAgent(int agentId)
{
    if (!crowd.empty())
    {
        crowd.erase(remove_if(begin(crowd), end(crowd), [agentId](Agent *u)
                              { return u->getId() == agentId; }),
                    end(crowd));
    }
}

void SocialForce::removeCrowd()
{
    for (unsigned int idx = 0; idx < crowd.size(); idx++)
        delete crowd[idx];

    crowd.clear();
}

void SocialForce::removeWalls()
{
    for (unsigned int idx = 0; idx < walls.size(); idx++)
        delete walls[idx];

    walls.clear();
}

void SocialForce::removeAGV()
{
    int lastIdx;
    if (!agvs.empty())
    {
        lastIdx = agvs.size() - 1; // Assign index of last element

        delete agvs[lastIdx];
        agvs.pop_back();
    }
}

void SocialForce::removeAGVs()
{
    for (unsigned int idx = 0; idx < crowd.size(); idx++)
        delete agvs[idx];

    agvs.clear();
}

void SocialForce::moveCrowd(float stepTime)
{
    for (unsigned int idx = 0; idx < crowd.size(); idx++)
    {
        // if (crowd[idx]->getIsMoving())
        // {
        //     crowd[idx]->move(crowd, walls, agvs, stepTime);
        // }
        crowd[idx]->move(crowd, walls, agvs, stepTime);
    }
}

void SocialForce::movePedes(float stepTime)
{
        for (unsigned int idx = 0; idx < Pendestrian_list.size(); idx++)
    {
        // if (crowd[idx]->getIsMoving())
        // {
        //     crowd[idx]->move(crowd, walls, agvs, stepTime);
        // }
        Pendestrian_list[idx]->move(Pendestrian_list, walls, stepTime);
    }
}

void SocialForce::moveAGVs(float stepTime)
{
    vector<Point3f> position_list;
    for (Agent *agent : crowd)
    {
        if (agent->getPosition().distance(Vector3f(0, 0, 0)) > 12.0F)
            continue;
        position_list.push_back(agent->getPosition());
    }

    for (AGV *agv : agvs)
    {
        if (agv->getIsMoving())
        {
            agv->move(stepTime, position_list);
        }
    }
}