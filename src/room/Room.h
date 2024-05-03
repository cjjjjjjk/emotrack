
class Room
{
private:
    int G1_x, G1_y, G2_x, G2_y;
    int length;
    char ID;
public:

    Room(int G1_x, int G1_y, int G2_x, int G2_y, int length, char ID);
    ~Room();

    char GetID() {return this->ID;};
    int GetGate1_x() {return this->G1_x;};
    int GetGate1_y() {return this->G1_y;};
    int GetGate2_x() {return this->G2_x;} 
    int GetGate2_y() {return this->G2_y;}
    int GetLength()  {return this->length;}
    void Draw(){};
};


