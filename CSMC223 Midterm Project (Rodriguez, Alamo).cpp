#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>

using namespace std;


struct ReadInstruction{
    string operation;
    string op1;
    string op2;
};


class Memory{
    public:

    const int start_Index_Instr_Address_Memory = 0;
    const int start_Index_Data_Address_Memory = 512;
    int storage_Capacity;

    string memory[1024] = {};

    Memory(){
        storage_Capacity = 0;
    }

    void storeMemory(string data, int address){
        memory[address] = data;
        storage_Capacity++;
    }

    string fetchMemory(int address){
        string fetchedData = memory[address];
        return fetchedData;
    }

    void displayMemory(){
        cout << "-----------------------------\n";
        cout << "MAIN MEMORY:" << endl;
        for(int i = 0; i < 1024; i++){
            string temp = fetchMemory(i);
            if(temp != ""){
                cout << "Address " << i << ": " << temp << endl;
            } else {
                continue;
            }
        }

        cout << "Memory Consumed: " << storage_Capacity << endl;
        cout << "-----------------------------\n\n";
    }
};


class Processor{
    public:
    
    int PC,IR; //Special Registers
    int registers[8] = {}; // General Registers
    int address_count;

    bool GT_Flag, LT_Flag , EQ_Flag;        // Flags for CMP Instruction
    bool halt_loop;                         // For HALT Instruction
    bool error_stop;                        // Prevents infinite loop

    bool op1_reg, op1_data;                 // For decoding Operands
    bool op2_reg, op2_data;

    Memory MM;
    int MAR;


    Processor(){
        PC = IR = 0;
        GT_Flag = LT_Flag = EQ_Flag = false;
        halt_loop = false;
        error_stop = false;
        MAR = MM.start_Index_Data_Address_Memory;
        address_count = 0;
    }

    enum Instructions {     
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
        MOV = 010001,
    };

    void Read(){            // Reading 'source.txt' to Memory
        ifstream readFile;
        readFile.open("source.txt");

        string temp;
        address_count = MM.start_Index_Instr_Address_Memory;

        while(getline(readFile,temp)){
            MM.storeMemory(temp,address_count);
            address_count++;
        }
    }

    void FetchDecodeExecute(){
        try {
        PC = MM.start_Index_Instr_Address_Memory;
        ReadInstruction Decode;
        while(!halt_loop && PC < address_count){
            IR = PC;
            string CurrInstr = MM.fetchMemory(IR);     //fetch
            istringstream iss(CurrInstr);

            iss >> Decode.operation >> Decode.op1 >> Decode.op2;

            execute(Decode);
            PC++;
        }

        MM.displayMemory();
        } catch (const exception& e){
            cerr << e.what() << endl;
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
            case SUB: // SUBTRACT the values of the two registers
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
            case READ:  //READ value from user and store to register
                cout << "INPUT: ";
                cin >> registers[op1];
                cout << "Value in R" << op1 << ": " << registers[op1] << endl;
                break;

            case MOV: // TRANSFER values from one register to another
                move(op1, op2);
                break;
            case HALT: // TERMINATE the program
                halt();
                break;  
            default:
                cout << "ERROR OCCURED" << endl;
                halt();
                break;
        }
    }
    
    
    void load(int reg_address,int data){
        if(op1_data || op2_reg){
            throw invalid_argument("Invalid Operand for LOAD Instruction!");
        }
        registers[reg_address] = data;
    }

    void Store(int value){
         if(op1_reg){
            MM.storeMemory(to_string(registers[value]),MAR);
            MAR++;
        } else {
            MM.storeMemory(to_string(value),MAR);
            MAR++;
        }
    }

    void clear(int reg_address){
        if(op1_data) {
            throw invalid_argument("Invalid Operand for CLEAR Instruction!");
        }
        registers[reg_address] = 0;
    }
    
    void add(int reg_address_1, int reg_address_2){
        if(op1_data || op2_data) {
            throw invalid_argument("Invalid Operand for ADD Instruction!");
        }
        registers[reg_address_1] += registers[reg_address_2];
    }

    void sub(int reg_address_1, int reg_address_2){
        if(op1_data || op2_data){
            throw invalid_argument("Invalid Operand for SUB Instruction!");
        }
        registers[reg_address_1] -= registers[reg_address_2];
    }

    void mul(int reg_address_1, int reg_address_2){
        if(op1_data || op2_data){
            throw invalid_argument("Invalid Operand for MUL Instruction!");
        }
        registers[reg_address_1] *= registers[reg_address_2];   
    }

    void div(int reg_address_1, int reg_address_2){
        if(op1_data || op2_data){
            throw invalid_argument("Invalid Operand for DIV Instruction!");
        }
        registers[reg_address_1] /= registers[reg_address_2]; 
    }

    void inc(int reg_address_1) {
        if(op1_data){
            throw invalid_argument("Invalid Operand for INC Instruction!");
        }
        registers[reg_address_1]++;
    }

    void dec(int reg_address_1) {
        if(op1_data){
            throw invalid_argument("Invalid Operand for DEC Instruction!");
        }
        registers[reg_address_1]--;
    }

    void compare(int reg_address_1, int reg_address_2){
        if(op1_data || op2_data){
            throw invalid_argument("Invalid Operand for CMP Instruction!");
        }


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

    void print(int value){
        if(op1_reg){
            cout << registers[value] << '\n';
        } else {
            cout << value << "\n";
        }    
    }

    void move(int reg_address_1, int reg_address_2){
        if(op1_data || op2_data){
            throw invalid_argument("Invalid Operand for MOV Instruction!");
        }
        registers[reg_address_1] = registers[reg_address_2];
        printRegisters();
    }


    void halt(){
        halt_loop = true;
        return;
    }

    void decodeOperands(ReadInstruction Decode, int &op1, int &op2){
        if(Decode.op1[0] == 'R'){               //register check
            op1 = stoi(Decode.op1.substr(1));
            op1_reg = true;
            op1_data = false;

            if(op1 > 9){
                throw invalid_argument("Register out of bounds");
            }
        } else if(Decode.op1 == ""){
            op1 = 0;
        } else {                                  //data value check
            op1_reg = false;
            op1_data = true;
            op1 = stoi(Decode.op1);
        }


        if(Decode.op2[0] == 'R'){               //register check
            op2 = stoi(Decode.op2.substr(1));
            op2_reg = true;
            op2_data = false;
            
            if(op1 > 9){
                throw invalid_argument("Register out of bounds");
            }
        } else if(Decode.op2 == ""){
            op2 = 0;
        } else {                                  //data value check
            op2_reg = false;
            op2_data = true;
            op2 = stoi(Decode.op2);
        }
    }
   
    void printRegisters(){
        for(int i = 0; i < 8; i++){
            cout << "R" << i << ": " << registers[i] << endl;
        }
    }

    Processor::Instructions DecodeInstructions(string oper){        // decodes instructions on which operation to be used
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
        } else if(oper == "MOV") {
            return Processor::MOV;
        } else {
            return Processor::HALT;
        }
    }  

};



int main(){
    Processor CPU;
    CPU.Read();
    CPU.FetchDecodeExecute();

    return 0;
}