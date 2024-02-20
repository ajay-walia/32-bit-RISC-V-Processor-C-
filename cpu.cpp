#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <iomanip>
using namespace std;

//instruction format declaration --opcode dependent
#define iType 10011//0010011
#define rType 110011 //011011

//instruction type declaration --func3 dependent
//I-type
#define ADDI 0 //000                      -done
#define STLI 10 //010
#define SLTIU 11 //011
#define XORI 100 
#define ORI 110 
#define ANDI 111 
#define SLLI 1 //001
#define SRLISRAI 101 
//R-type
#define ADDSUB 0 //000
#define SLL 1 //001
#define SLT 10 //010
#define SLTU 11 
#define XOR 100 
#define SRLSRA 101
#define OR 110 
#define AND 111 

//keep track of instruction we are reading in + seperate it to find opcode, etc.
class imem{
public:
    string instruction;
    string opcode; //change to int??? figure out how to keep leading zeros??? -- stoi doesn't do that
    string rd;
    string func3;
    string rs1;
    string immed;
    string Rimmed; //first seven bits of immed
    string rs2;
};

//keep track of reg values
class reg{
public:
    int value;
    bool isSet;
};

//function declarations
void Imem_Init(imem& ob);
void Reg_Init(reg& rd_write);
int binaryToDecimal(long n);
void print(imem& ob); //implement to print stuff ???

int main(){

    imem ob[30]; //consider 30 instructions
    for (int i=0;i<30;i++){
        Imem_Init(ob[i]);
    }

    reg rd_write[32]; //32 registers
    for (int i=0;i<33;i++){
        Reg_Init(rd_write[i]);
    }
    
    string filename = "r_type.dat"; //change file for testing

    //opening file
    ifstream inputFile;
    inputFile.open(filename);
    if(!inputFile.is_open()){
        cout<<"unable to open file"<<endl;
        exit(1);

    }

    string line;
    string subInstruction = "";

    int lineCount=0;
    int total =0; //instruction number

    while(getline(inputFile,line)){ //read line by line of file until EOF 
        
        //====================================================//
        //FETCH STAGE

        //instructions read in as little endian
        subInstruction = line + subInstruction; 
        lineCount++;

        if(lineCount == 4){ //go through five stages every 4 lines
            ob[total].instruction = subInstruction;
            string temp = subInstruction;
            lineCount=0;
            subInstruction="";
            long immed=0; //stoi does not work with this as int bc number>int size --out of space error 
            long rs1=0;
            long rd=0;
            int opcode =0;
            int func3=0;
            int Rimmed = 0;
            int rs2=0;
            
            //====================================================//
            //DECODE

            //sort into I-type format
            ob[total].immed = ob[total].instruction.substr(0,12);
            immed=binaryToDecimal(stol(ob[total].immed,nullptr,10));

            ob[total].rs1 = ob[total].instruction.substr(12,5);
            rs1=binaryToDecimal(stoi(ob[total].rs1,nullptr,10));

            ob[total].func3 = ob[total].instruction.substr(17,3);
            func3 = stoi(ob[total].func3,nullptr,10);

            ob[total].rd = ob[total].instruction.substr(20,5);
            rd=binaryToDecimal(stoi(ob[total].rd,nullptr,10));

            ob[total].opcode = ob[total].instruction.substr(25,7);
            opcode = stoi(ob[total].opcode,nullptr,10);

            //R-type format
            ob[total].Rimmed = ob[total].instruction.substr(0,7);
            Rimmed=binaryToDecimal(stol(ob[total].Rimmed,nullptr,10));

            ob[total].rs2 = ob[total].instruction.substr(7,5);
            rs2=binaryToDecimal(stoi(ob[total].rs2,nullptr,10));
            
            //====================================================//
            //EXECUTE
            
            switch(opcode){
                case iType:{
                    switch(func3){
                        case ADDI:{
                            cout << "ADDI" << endl;
                            if(!rd_write[rs1].isSet){ 
                                rd_write[rd].value = rs1 + immed;
                                rd_write[rd].isSet=true;
                            }
                            else {
                                rd_write[rd].value = rd_write[rs1].value + immed;
                                rd_write[rd].isSet=true;
                            }
                            cout << "result: " << rd_write[rd].value <<endl;
                            break;
                        }
                        case STLI:{ //set less than signed
                            cout << "STLI" << endl;
                            if(rs1 < immed){ 
                                rd_write[rd].value = 1;
                                rd_write[rd].isSet=true;
                            }
                            else {
                                rd_write[rd].value = 0;
                                rd_write[rd].isSet=true;
                            }
                            cout << "result: " << rd_write[rd].value <<endl;
                            break;
                        }
                        case SLTIU:{ //set less than unsigned
                            cout << "SLTIU" << endl;
                            if((unsigned int) rs1 < immed){ 
                                rd_write[rd].value = 1;
                                rd_write[rd].isSet=true;
                            }
                            else {
                                rd_write[rd].value = 0;
                                rd_write[rd].isSet=true;
                            }
                            cout << "result: " << rd_write[rd].value <<endl;
                            break;
                        }
                        case XORI:{
                            cout << "XORI" << endl;
                            if(!rd_write[rs1].isSet){ 
                                rd_write[rd].value = rs1 ^ immed;
                                rd_write[rd].isSet=true;
                            }
                            else {
                                rd_write[rd].value = rd_write[rs1].value ^ immed;
                                rd_write[rd].isSet=true;
                            }
                            cout << "result: " << rd_write[rd].value <<endl;
                            break;
                        }
                        case ORI:{
                            cout << "ORI" << endl;
                            if(!rd_write[rs1].isSet){ 
                                rd_write[rd].value =  rs1 | immed;
                                rd_write[rd].isSet=true;
                            }
                            else {
                                rd_write[rd].value = rd_write[rs1].value | immed;
                                rd_write[rd].isSet=true;
                            }
                            cout << "result: " << rd_write[rd].value <<endl;
                            break;
                        }
                        case ANDI:{
                            cout << "ANDI" << endl;
                            if(!rd_write[rs1].isSet){ 
                                rd_write[rd].value = rs1 & immed;
                                rd_write[rd].isSet=true;
                            }
                            else {
                                rd_write[rd].value = rd_write[rs1].value & immed;
                                rd_write[rd].isSet=true;
                            }
                            cout << "result: " << rd_write[rd].value <<endl;
                            break;
                        }
                        case SLLI:{
                            cout << "SLLI" << endl;
                            if(!rd_write[rs1].isSet){ 
                                rd_write[rd].value = rs1 << immed;
                                rd_write[rd].isSet=true;
                            }
                            else {
                                rd_write[rd].value = rd_write[rs1].value << immed;
                                rd_write[rd].isSet=true;
                            }
                            cout << "result: " << rd_write[rd].value <<endl;
                            break;
                        }
                        case SRLISRAI:{
                            int temp_rs1 = rd_write[rs1].isSet ? rd_write[rs1].value : rs1;

                            // Immediate value contains the shift amount (lower 5 bits)
                            int shift_amount = immed & 0b00011111;
                            if(Rimmed == 0b0100000){ 
                                cout << "SRAI" << endl;
                                rd_write[rd].value = temp_rs1 >> shift_amount;
                                rd_write[rd].isSet=true;
                            }
                            else {
                                cout << "SRLI" << endl;
                                rd_write[rd].value = temp_rs1 >> shift_amount;
                                rd_write[rd].isSet=true;
                            }
                            cout << "result: " << rd_write[rd].value <<endl;
                            break;
                        }

                        default:{
                            // cout<<"not valid I-TYPE instruction"<<endl;
                            break;
                        }
                    }
                    break;
                } 
                case rType:{
                    switch(func3){
                        case ADDSUB:{
                            int temp_rs1 = rd_write[rs1].isSet ? rd_write[rs1].value : rs1;
                            int temp_rs2 = rd_write[rs2].isSet ? rd_write[rs2].value : rs2;
                            if(Rimmed == 0b0100000){ // func7 code (0100000 = sub)
                                cout << "SUB" << endl;
                                rd_write[rd].value = temp_rs1 - temp_rs2;
                                rd_write[rd].isSet=true; 
                            }
                            else {
                                cout << "ADD" << endl;
                                rd_write[rd].value = temp_rs1 + temp_rs2;
                                rd_write[rd].isSet=true;
                            }
                            cout << "result: " << rd_write[rd].value <<endl;
                            break;
                        }
                        case SLL:{
                            cout << "SLL" << endl;
                            int temp_rs1 = rd_write[rs1].isSet ? rd_write[rs1].value : rs1;
                            int temp_rs2 = rd_write[rs2].isSet ? rd_write[rs2].value : rs2;
                            rd_write[rd].value = temp_rs1 << temp_rs2;
                            rd_write[rd].isSet=true;
                            cout << "result: " << rd_write[rd].value <<endl;
                            break;
                        }
                        case SLT:{
                            cout << "SLT" << endl;
                            int temp_rs1 = rd_write[rs1].isSet ? rd_write[rs1].value : rs1;
                            int temp_rs2 = rd_write[rs2].isSet ? rd_write[rs2].value : rs2;
                           
                             if(temp_rs1 < temp_rs2){ 
                                rd_write[rd].value = 1;
                                rd_write[rd].isSet=true;
                            }
                            else {
                                rd_write[rd].value = 0;
                                rd_write[rd].isSet=true;
                            }
                            cout << "result: " << rd_write[rd].value <<endl;
                            break;
                        }
                        case SLTU:{
                            cout << "SLTU" << endl;
                            int temp_rs1 = rd_write[rs1].isSet ? rd_write[rs1].value : rs1;
                            int temp_rs2 = rd_write[rs2].isSet ? rd_write[rs2].value : rs2;
                            if(temp_rs1 < temp_rs2){ 
                                rd_write[rd].value = 1;
                                rd_write[rd].isSet=true;
                            }
                            else {
                                rd_write[rd].value = 0;
                                rd_write[rd].isSet=true;
                            }
                            cout << "result: " << rd_write[rd].value <<endl;
                            break;
                        }
                        case XOR:{
                            cout << "XOR" << endl;
                            int temp_rs1 = rd_write[rs1].isSet ? rd_write[rs1].value : rs1;
                            int temp_rs2 = rd_write[rs2].isSet ? rd_write[rs2].value : rs2;
                            rd_write[rd].value = temp_rs1 ^ temp_rs2;
                            rd_write[rd].isSet=true;
                            cout << "result: " << rd_write[rd].value <<endl;
                            break;
                        }
                        case SRLSRA:{
                            int temp_rs1 = rd_write[rs1].isSet ? rd_write[rs1].value : rs1;
                            int temp_rs2 = rd_write[rs2].isSet ? rd_write[rs2].value : rs2;
                            //Bitmask to extract last 5 bits of the shift amount
                            int bitmask = 0b00011111;
                            temp_rs2=temp_rs2 & bitmask;
                            if(Rimmed == 0b0100000){ //shift amount is the last 5 bits of rd_write[rs2]
                                cout << "SRA" << endl;
                                rd_write[rd].value = (unsigned int)temp_rs1 >> temp_rs2;
                                rd_write[rd].isSet = true;
                            } 
                            else {
                                cout << "SRL" << endl;
                                rd_write[rd].value = (unsigned int)temp_rs1 >> temp_rs2;
                                rd_write[rd].isSet = true;
                            }
                            cout << "result: " << rd_write[rd].value <<endl;
                            break;
                        }
                        case OR:{
                            cout << "OR" << endl;
                            int temp_rs1 = rd_write[rs1].isSet ? rd_write[rs1].value : rs1;
                            int temp_rs2 = rd_write[rs2].isSet ? rd_write[rs2].value : rs2;
                            rd_write[rd].value = temp_rs1 | temp_rs2;
                            rd_write[rd].isSet=true;
                            cout << "result: " << rd_write[rd].value <<endl;
                            break;
                        }
                        case AND:{
                            cout << "AND" << endl;
                            int temp_rs1 = rd_write[rs1].isSet ? rd_write[rs1].value : rs1;
                            int temp_rs2 = rd_write[rs2].isSet ? rd_write[rs2].value : rs2;
                            rd_write[rd].value = temp_rs1 & temp_rs2;
                            rd_write[rd].isSet = true;
                            cout << "result: " << rd_write[rd].value <<endl;
                            break;
                        }

                        default:{
                            // cout<<"not valid R-TYPE instruction"<<endl;
                            break;
                        }
                    }
                    break;
                }
                default:{
                    // cout<<"not valid instruction"<<endl;
                    break;
                }
            }

            total++;
            //====================================================//
            //MEM ACCESS

            
        }

    }

    // printing instructions --like pc.txt 
    // cout <<"Instructions in binary" << endl;
    // for(int i=0;i<total;i++){
    //     
    //     cout << ob[i].instruction << endl << endl; //expected 32 bits
    // }

    //printing instruction,rs1,rs2,immed --result? values
    // for(int i=0;i<total;i++){
    //     cout<< "INSTRUCTION TYPE, rs1: " << ob[i].rs1 << ", rs2: " << ob[i].rs2 << ", immed: " << ob[i].immed <<endl; 
    //     cout << ob[i].instruction << endl;
    // }

    //printing reg number and value --like reg.txt
    for(int i = 0; i < 32; i++){
        cout << "reg number: " << i << ", reg value: " << rd_write[i].value << endl;
    }
    return 0;
}

void Imem_Init(imem& ob){
    ob.instruction = "";
    ob.opcode = "";
    ob.rd = "";
    ob.func3 = "";
    ob.rs1 = "";
    ob.immed = "";
}

void Reg_Init(reg& rd_write){
    rd_write.value = 0;
    rd_write.isSet = false;
}

//converting signed binary to decimal
int binaryToDecimal(long n){
    long temp=n;
    long dec=0;
    long base=1;

    while(temp){
        long last=temp%10;
        temp=temp/10;
        dec+=last*base;
        base*=2;
    }
    dec=(dec+128)%256 -128;
    return dec;
}