#include<iostream>
#include<vector>
#include<fstream>
using namespace std;

using Byte = unsigned char;
using Word = unsigned short;

class Memory{
    int memory[1024];
};


class Processor{
    public:
    
    int PC,IR,AC; //Special Registers
    int R0,R1,R2,R3,R4,R5,R6,R7,R8; // General Registers

    Processor(){
        PC = IR = AC = 0;
        R0 = R1 = R2 = R3 = R4 = R6 = R7 = R8 = 0;
    }

    enum Instructions {
        LOAD = 00,
        STORE = 10,
        CLEAR = 11,
        ADD = 100,
        SUB = 101,
        MUL = 110,
        DIV = 111,
        INC = 1000,
        DEC = 1001,
        CMP = 1010,
        PRINT = 1011,
        READ = 1100,
        HALT = 1101,
    };

    void Read(){
        ifstream readFile;
        readFile.open("source.txt");
    }

    void Execute(){
        /*Code to sequentially execute instructions from source.txt*/
    }
};

int main(){
    //code
}