#include "Pendestrian.h"

std::shared_ptr<Ward> Pendestrian::nWard()
{
    upWard();
    if(cWard >= journey.size())
    {
        return nullptr;
    }
    else{
        return journey[cWard];
    }
}

float Pendestrian::getMinDistanceToWalls(std::vector<Wall *> walls, Point3f position, float radius)
{
    Point3f nearestPoint;
    Vector3f vector;
    float distanceSquared, minDistanceSquared = INFINITY;

    for (auto wall : walls)
    {
        nearestPoint = wall->getNearestPoint(position);
        vector = position - nearestPoint; // Vector from wall to agent i
        distanceSquared = vector.lengthSquared();

        // Store Nearest Wall Distance
        if (distanceSquared < minDistanceSquared)
        {
            minDistanceSquared = distanceSquared;
        }
    }

    return sqrt(minDistanceSquared) - radius; // Distance between wall and agent i
}

Vector3f Pendestrian::getDrivingForce(const Point3f destination)
{
    const float T = 0.54F; // Relaxation time based on (Moussaid et al., 2009)
    Vector3f e_i, f_i;

    // Compute Desired Direction
    // Formula: e_i = (destination - position_i) / ||(destination - position_i)||
    e_i = destination - position;
    e_i.normalize();

    // Compute Driving Force
    // Formula: f_i = ((desiredSpeed * e_i) - velocity_i) / T
    f_i = ((desiredSpeed * e_i) - velocity) * (1 / T);

    return f_i;
}

Vector3f Pendestrian::getAgentInteractForce(std::vector<std::shared_ptr<Pendestrian>> pedestrian_list)
{
    // Constant Values Based on (Moussaid et al., 2009)
    const float lambda = 2.0;  // Weight reflecting relative importance of velocity vector against position vector
    const float gamma = 0.35F; // Speed interaction
    const float n_prime = 3.0; // Angular interaction
    const float n = 2.0;       // Angular intaraction
    const float A = 4.5;       // Modal parameter A

    Vector3f distance_ij, e_ij, D_ij, t_ij, n_ij, f_ij;
    float B, theta, f_v, f_theta;
    int K;
    int numOfAgents = 0;

    f_ij.set(0.0, 0.0, 0.0);

    for (const std::shared_ptr<Pendestrian> &pedes : pedestrian_list)
    {
        // Do Not Compute Interaction Force to Itself
        if (pedes->GetID() != ID)
        {
            // Compute Distance Between Agent j and i
            distance_ij = pedes->getPosition() - position;
            pedes->getPosition();

            // Skip Computation if Agents i and j are Too Far Away
            if (distance_ij.lengthSquared() > (2.0 * 2.0))
                continue;

            numOfAgents++;

            // Compute Direction of Agent j from i
            // Formula: e_ij = (position_j - position_i) / ||position_j - position_i||
            e_ij = distance_ij;
            e_ij.normalize();

            // Compute Interaction Vector Between Agent i and j
            // Formula: D = lambda * (velocity_i - velocity_j) + e_ij
            D_ij = lambda * (velocity - pedes->getVelocity()) + e_ij;

            // Compute Modal Parameter B
            // Formula: B = gamma * ||D_ij||
            B = gamma * D_ij.length();

            // Compute Interaction Direction
            // Formula: t_ij = D_ij / ||D_ij||
            t_ij = D_ij;
            t_ij.normalize();

            // Compute Angle Between Interaction Direction (t_ij) and Vector Pointing from Agent i to j (e_ij)
            theta = t_ij.angle(e_ij);

            // Compute Sign of Angle 'theta'
            // Formula: K = theta / |theta|
            K = (theta == 0) ? 0 : static_cast<int>(theta / abs(theta));

            // Compute Amount of Deceleration
            // Formula: f_v = -A * exp(-distance_ij / B - ((n_prime * B * theta) * (n_prime * B * theta)))
            f_v = -A * exp(-distance_ij.length() / B - ((n_prime * B * theta) * (n_prime * B * theta)));

            // Compute Amount of Directional Changes
            // Formula: f_theta = -A * K * exp(-distance_ij / B - ((n * B * theta) * (n * B * theta)))
            f_theta = -A * K * exp(-distance_ij.length() / B - ((n * B * theta) * (n * B * theta)));

            // Compute Normal Vector of Interaction Direction Oriented to the Left
            n_ij.set(-t_ij.y, t_ij.x, 0.0);

            // Compute Interaction Force
            // Formula: f_ij = f_v * t_ij + f_theta * n_ij
            f_ij += f_v * t_ij + f_theta * n_ij;
        }
    }

    // ??
    //setImpatient(1 / exp(-numOfAgents / (3.14159265359F * 4)));

    return f_ij;
}

Vector3f Pendestrian::getWallInteractForce(std::vector<Wall *> walls)
{
    // const float repulsionRange = 0.3F;	// Repulsion range based on (Moussaid et al., 2009)
    const int a = 3;
    const float b = 0.1F;

    Point3f nearestPoint;
    Vector3f vector_wi, minVector_wi;
    float distanceSquared, minDistanceSquared = INFINITY, d_w, f_iw;

    for (Wall *wall : walls)
    {
        nearestPoint = wall->getNearestPoint(position);
        vector_wi = position - nearestPoint; // Vector from wall to agent i
        distanceSquared = vector_wi.lengthSquared();

        // Store Nearest Wall Distance
        if (distanceSquared < minDistanceSquared)
        {
            minDistanceSquared = distanceSquared;
            minVector_wi = vector_wi;
        }
    }

    d_w = sqrt(minDistanceSquared) - radius; // Distance between wall and agent i

    // Compute Interaction Force
    // Formula: f_iw = a * exp(-d_w / b)
    f_iw = a * exp(-d_w / b);
    minVector_wi.normalize();

    return f_iw * minVector_wi;
 
}

// Vector3f Pendestrian::getAgvInteractForce(std::vector<AGV *> agvs)
// {
//     return Vector3f();
// }
void Pendestrian::move(std::vector<std::shared_ptr<Pendestrian>> pedes_list, std::vector<Wall *> walls, float stepTime)
{
    Vector3f acceleration;

    // Compute Social Force
    acceleration = getDrivingForce(getPath()) +
                   getAgentInteractForce(pedes_list) +
                   getWallInteractForce(walls);
                   //+getAgvInteractForce(agvs);
    // std::cout<<"pede.cpp line 169: "<<acceleration<<"\n";
    // std::cout<<"AGV vector3f: "<<getAgvInteractForce(agvs)<<"\n";
    // Compute New Velocity
    velocity = velocity + acceleration * stepTime;
    // std::cout<<ID<<"\t"<<velocity<<"\n";
    // Truncate Velocity if Exceed Maximum Speed (Magnitude)
    if (velocity.lengthSquared() > (desiredSpeed * desiredSpeed))
    {
        velocity.normalize();
        velocity *= desiredSpeed;
    }

    // Compute New Position
    position = position + velocity * stepTime;
}

