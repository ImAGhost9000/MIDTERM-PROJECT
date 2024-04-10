#include<iostream>
#include<vector>
#include<fstream>
#include<sstream>
#include<string>
using namespace std;

/*
struct Memory{
    string operand;
    string op1;
    string op2;
};
*/

class Memory{
    public:

    const int start_instr_address_m = 0;
    const int start_data_address_m = 512;
    int storage_filled;

    string memory[1024] = {};

    Memory(){
        storage_filled = 0;
    }

    void storeMemory(string data, int address){
        memory[address] = data;
        storage_filled++;
    }

    string fetchMemory(int address){
        string fetchedData = memory[address];
        return fetchedData;
    }

    void displayMemory(){
        cout << "MAIN MEMORY:" << endl;
        for(int i = 0; i < 1024; i++){
            string temp = fetchMemory(i);
            if(temp != ""){
                cout << i << " " << temp << endl;
            } else {
                continue;
            }
        }

        cout << "Memory Consumed: " << storage_filled << endl;
    }
};


class Processor{
    public:
    
    int PC,IR,AC; //Special Registers
    int R0,R1,R2,R3,R4,R5,R6,R7,R8; // General Registers
    Memory MM;

    Processor(){
        PC = IR = AC = 0;
        R0 = R1 = R2 = R3 = R4 = R6 = R7 = R8 = 0;
    }

    enum Instructions {
        LOAD = 0000,
        STORE = 0010,
        CLEAR = 0011,
        ADD = 0100,
        SUB = 0101,
        MUL = 0110,
        DIV = 0111,
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

        string temp;
        PC = MM.start_instr_address_m;
        while(getline(readFile,temp)){
            MM.storeMemory(temp,PC);
            PC++;
        }

        cout << "Instructions Read" << endl;
        MM.displayMemory();
    }

    void Execute(){
        string opcode;
        string op1;
        string op2;
    }
};

int main(){
    Processor CPU;
    CPU.Read();
}
