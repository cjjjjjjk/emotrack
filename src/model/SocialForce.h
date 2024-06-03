#ifndef SOCIAL_FORCE_H
#define SOCIAL_FORCE_H

#include <iostream>
#include <memory>
#include "src/agv/AGV.h"
#include "src/agent/Agent.h"
#include "src/wall/Wall.h"
#include "src/Pedestrian/Pendestrian.h"
#include <vector>
#include <algorithm>

class SocialForce {
private:
    std::vector<std::shared_ptr<Pendestrian> > Pendestrian_list;
    std::vector<std::shared_ptr<Ward>> Ward_list;
    std::vector<Agent *> crowd;
    std::vector<Wall *> walls;
    std::vector<AGV *> agvs;

public:
    // SocialForce();
    ~SocialForce();

    // Danh sach nguoi di bo ----------------------- author: Hai
    void addPedestrian(std::shared_ptr<Pendestrian> pendes);
    int GetPes_listSize(){return Pendestrian_list.size();};
    void SetPedeslist(std::vector<std::shared_ptr<Pendestrian> > list){Pendestrian_list = list; };
    std::vector<std::shared_ptr<Pendestrian> > getPedes_list() {return Pendestrian_list;};
    void removePedes(int pedesID);
    void removePedeslist();
    // Pedestrian move
    // ---------------------------------------------------------
    // Ward list -----------------------------------------------
    void setWard_list(std::vector<std::shared_ptr<Ward>> wardList) { this->Ward_list = wardList;};
    std::vector<std::shared_ptr<Ward>> GetWard_list() {return Ward_list;};
    //----------------------------------------------------------
    void addAgent(Agent *agent);

    void addWall(Wall *wall);

    void addAGV(AGV *agv);

    const std::vector<Agent *> getCrowd() const { return crowd; }

    int getCrowdSize() const { return crowd.size(); }

    const std::vector<Wall *> getWalls() const { return walls; }

    int getNumWalls() const { return walls.size(); }

    const std::vector<AGV *> getAGVs() const { return agvs; }

    int getNumAGVs() const { return agvs.size(); }

    void removeAgent(int agentId);
    void removeCrowd(); // Remove all individuals and groups
    void removeWalls();

    void removeAGV();
    void removeAGVs();

    void moveCrowd(float stepTime);
    void movePedes(float stepTime);
    void moveAGVs(float stepTime);
};

#endif