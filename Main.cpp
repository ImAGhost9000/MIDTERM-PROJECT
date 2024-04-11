#include<iostream>
#include<vector>
#include<fstream>
#include<sstream>
#include<string>
using namespace std;


struct ReadInstruction{
    string operand;
    int op1;
    int op2;
};

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
    int R0,R1,R2,R3,R4,R5,R6,R7; // General Registers
    int address_count;
    Memory MM;

    Processor(){
        address_count = 0;
        PC = IR = AC = 0;
        R0 = R1 = R2 = R3 = R4 = R6 = R7 = 0;
    }

    /*
    enum Registers{
        R0 = 0x00,
        R1 = 0x01,
        R2 = 0x02,
        R3 = 0x03,
        R4 = 0x04,
        R5 = 0x05,
        R6 = 0x06,
        R7 = 0x07,
    };
    */

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
            address_count++;
        }

        cout << "Instructions Read" << endl;
        MM.displayMemory();
    }

    void FetchDecodeExecute(){
        PC = MM.start_data_address_m;
        ReadInstruction Decode;
        while(address_count >= 0){
            IR = PC;
            string CurrInstr = MM.fetchMemory(IR);     //fetch
            istringstream iss(CurrInstr);

            iss >> Decode.operand >> Decode.op1 >> Decode.op2;

            Execute(Decode.operand,Decode.op1,Decode.op2);

            PC++;
        }
    }

    void Execute(string operand, int op1, int op2){
        cout << operand << op1 << op2;
    }
};

int main(){
    Processor CPU;
    CPU.Read();
    CPU.FetchDecodeExecute();
}
