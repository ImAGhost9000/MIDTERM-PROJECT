#include<iostream>
#include<vector>
#include<fstream>
#include<sstream>
#include<string>
using namespace std;


struct ReadInstruction{
    string operation;
    string op1;
    string op2;
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
        R0 = R1 = R2 = R3 = R4 = 0;
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
        PC = MM.start_instr_address_m;
        ReadInstruction Decode;
        while(address_count >= 0){
            IR = PC;
            string CurrInstr = MM.fetchMemory(IR);     //fetch
            istringstream iss(CurrInstr);

            iss >> Decode.operation >> Decode.op1 >> Decode.op2;

            execute(Decode);

            address_count--;
            PC++;
        }
    }

    void Store(int address, int R1){
        MM.memory[address] = R1;
    }

    void execute(ReadInstruction Decode){
        int opcode = DecodeInstructions(Decode.operation);

        switch(opcode){
            case LOAD:
                cout << "This is a LOAD statement" << endl;
                
                break; 
            case STORE:
                cout << "This is a STORE statement" << endl;
                void Store();

                break;
            case CLEAR:
                cout << "This is a CLEAR statement" << endl;
                break;
            case ADD:
                cout << "This is a ADD statement" << endl;
        

                break;
            case SUB:
                cout << "This is a SUB statement" << endl;
                break;
            case MUL:
                cout << "This is a MUL statement" << endl;
                break;
            case DIV:
                cout << "This is a DIV statement" << endl;
                break;
            case INC:
                cout << "This is a INC statement" << endl;
                break;
            case DEC:
                cout << "This is a DEC statement" << endl;
                break;
            case CMP:
                cout << "This is a CMP statement" << endl;
                break;
            case PRINT:
                cout << "This is a PRINT statement" << endl;
                break;
            case READ:
                cout << "This is a READ statement" << endl;
                break;
            case HALT:
                cout << "This is a HALT statement" << endl;
                break;
            default:
                cout << "ERROR" << endl;
                break;
        }
    }

    Processor::Instructions DecodeInstructions(string oper){
        if(oper == "LOAD"){
            return Processor::LOAD;
        } else if (oper == "STORE"){
            return Processor::STORE;
        } else if (oper == "CLEAR"){
            return Processor::CLEAR;
        } else if (oper == "ADD"){
            return Processor::ADD;
        } else if (oper == "SUB"){
            return Processor::SUB;
        } else if (oper == "MUL"){
            return Processor::MUL;
        } else if (oper == "DIV"){
            return Processor::DIV;
        } else if (oper == "INC"){
            return Processor::INC;
        } else if (oper == "DEC"){
            return Processor::DEC;
        } else if (oper == "CMP"){
            return Processor::CMP;
        } else if (oper == "PRINT"){
            return Processor::PRINT;
        } else if (oper == "READ"){
            return Processor::READ;
        } else if (oper == "HALT"){
            return Processor::HALT;
        } else {
            return Processor::HALT;
        }
    }    
};


int main(){
    Processor CPU;
    CPU.Read();
    CPU.FetchDecodeExecute();
}