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
        cout << "STORING in MEM" << endl;
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
    int registers[8] = {}; // General Registers

    bool GT_Flag, LT_Flag , EQ_Flag;
    bool halt_loop;


    Memory MM;
    int MAR;

    Processor(){
        PC = IR = AC = 0;
        GT_Flag = LT_Flag = EQ_Flag = false;
        halt_loop = false;
        MAR = MM.start_data_address_m;
    }

    enum Instructions {     //as of 2:45 Thu, from 4 bits to 6 bits
        LOAD = 000000,
        STORE = 000010,
        CLEAR = 000011,
        ADD = 000100,
        SUB = 000101,
        MUL = 000110,
        DIV = 000111,
        INC = 001000,
        DEC = 001001,
        CMP = 001010,
        PRINT = 001011,
        READ = 001100,
        HALT = 001101,
        JL = 001110,
        JG = 001111,
        JE = 010000,
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

    void FetchDecodeExecute(){
        PC = MM.start_instr_address_m;
        ReadInstruction Decode;
        while(!halt_loop){
            IR = PC;
            string CurrInstr = MM.fetchMemory(IR);     //fetch
            istringstream iss(CurrInstr);

            iss >> Decode.operation >> Decode.op1 >> Decode.op2;

            execute(Decode);
            PC++;
        }

        MM.displayMemory();
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
        } else if (oper == "JL") {
            return Processor::JL;
        } else if (oper == "JG") {
            return Processor::JG;
        } else if (oper == "JE") {
            return Processor::JE;
        } else {
            return Processor::HALT;
        }
    }  


    void execute(ReadInstruction Decode){
        int opcode = DecodeInstructions(Decode.operation);
        int op1, op2;
        decodeOperands(Decode, op1, op2);
        switch(opcode){
            case LOAD: // LOAD to Register 
                load(op1,op2);
                break; 
            case STORE: // STORE to the Memory
                Store(op1);
                break;
            case CLEAR: // CLEAR the content of the register
                clear(op1);
                break;
            case ADD: // ADD the values of the two registers
                add(op1, op2);
                break;
            case SUB: //SUBTRACT the values of the two registers
                sub(op1,op2);
                break;
            case MUL: // MULTIPLY the values of the two registers
                mul(op1,op2);
                break;
            case DIV:// DIVIDE the values of the two registers
                div(op1,op2);
                break;
            case INC: // INCREMENT the value of the register by 1
                inc(op1);
                break;
            case DEC: // DECREMENT the value of the register by 1
                dec(op1);
                break;
            case CMP: // COMPARE the values of the two registers
                compare(op1, op2);
                break;

            case JG:    // JUMP IF GREATER THAN
                if(!GT_Flag) {
                    break;
                }
                else if(op1 < 0 || op1 > 1024){
                    cerr << "Out of bounds" << '\n';
                    break;
                }
                else if(MM.fetchMemory(op1) == "") {
                    cerr << "No Value Found in Instruction " << op1 << '\n';
                    break;
                }
                    PC = op1;
                    break;

            case JL:    // JUMP IF LESS THAN 
                if(!LT_Flag){
                    break;
                }
                else if(op1 < 0 || op1 > 1024 ){
                    cerr << "Out of bounds" << '\n';
  
                }
                else if(MM.fetchMemory(op1) == "") {
                    cerr << "No Value Found in Instruction " << op1 << '\n';

                }
                    PC = op1;
                    break;

            case JE:    // JUMP IF EQUAL 
                if(!EQ_Flag){
                    break;
                }
                else if(op1 < 0 || op1 > 1024){
                    cerr << "Out of bounds" << '\n';
                    break;
                }
                else if(MM.fetchMemory(op1) == "") {
                    cerr << "No Value Found in Instruction " << op1 << '\n';
                    break;
                }
                    PC = op1;
                    break;
            
            case PRINT: // PRINT the value of the register
                print(op1);
                break;

            case READ:
                cout << "This is a READ statement" << endl;
                break;

            case HALT: // TERMINATE the program
                halt();
                break;
                
            default:
                cout << "ERROR" << endl;
                break;
        }
    }
    
    
    void load(int reg_address,int data){
        registers[reg_address] = data;
        cout << "Value in R" << reg_address << ": " << registers[reg_address] << endl;
    }

    void Store(int R1){
        cout << "STORING" << endl;
        MM.storeMemory(to_string(R1),MAR);
        MAR++;
    }

    void clear(int reg_address){
        registers[reg_address] = 0;
    }
    
    void add(int reg_address_1, int reg_address_2){
        registers[reg_address_1] += registers[reg_address_2];
       
    }

    void sub(int reg_address_1, int reg_address_2){
        registers[reg_address_1] -= registers[reg_address_2];
        
    }

    void mul(int reg_address_1, int reg_address_2){
        registers[reg_address_1] *= registers[reg_address_2];
        
    }

    void div(int reg_address_1, int reg_address_2){
        registers[reg_address_1] /= registers[reg_address_2];
        
    }

    void inc(int reg_address_1) {
        registers[reg_address_1]++;
        
    }

    void dec(int reg_address_1) {
        registers[reg_address_1]--;
        
    }

    void compare(int reg_address_1, int reg_address_2){
        if(registers[reg_address_1] < registers[reg_address_2]){            //less than
            LT_Flag = true;
            GT_Flag = EQ_Flag = false;


        }
        else if(registers[reg_address_1] > registers[reg_address_2]) {      //greater than
            GT_Flag = true;
            LT_Flag = EQ_Flag = false;


        }
        else if (registers[reg_address_1] == registers[reg_address_2]){     // equal 
            EQ_Flag = true;
            GT_Flag = LT_Flag = false;

        }
    }

    void print(int reg_address){
        cout << registers[reg_address] <<  '\n';
    }

    void halt(){
        halt_loop = true;
        return;
    }

    
    void decodeOperands(ReadInstruction Decode, int &op1, int &op2){
        
        
        if(Decode.op1[0] == 'R'){               //register cheking
            op1 = stoi(Decode.op1.substr(1));
        } 
        else {                                  //data value
            op1 = stoi(Decode.op1);
        }

        if(Decode.op2[0] == 'R'){               //register checking
            op2 = stoi(Decode.op2.substr(1));
        }
        else {                                  //data value
            op2 = stoi(Decode.op2);
        }
    }
};



int main(){
    Processor CPU;
    CPU.Read();
    CPU.FetchDecodeExecute();
}