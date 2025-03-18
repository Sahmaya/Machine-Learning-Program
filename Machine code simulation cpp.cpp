// Sahmaya Anderson-Edwards
// Student ID: 24012404 
// Assignment 2 Comupter Science and Programming


#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

int memory[256]; // Memory array for each byte within the computer 
int reg[16];  // Array of 16 registers, reg = register 
int pc = 0;  // pc = program counter    
int ir = 0;  // ir = instruction register 

void execute(int instruction);

int main() {
    string filename; 
    int hexnumber, i;
    cout << "Enter the file name of MASSEY machine code: ";
    cin >> filename;

    ifstream inputfile(filename.c_str());

    if (!inputfile) {
        cerr << "Error: unable to open file " << filename << endl;
        return 1; // Return 1 = error, Return 0 = successful 
    }

    i = 0;
    while (inputfile >> hex >> hexnumber) {
        memory[i] = hexnumber;
        i++;
    }
    inputfile.close();

    for (int k = 0; k < i; ++k) {
        cout << "Memory[" << setw(2) << setfill('0') << k << "] = " 
             << setw(4) << setfill('0') << hex << uppercase << memory[k] << endl;
    }


    while (true) {
        ir = memory[pc];  

        execute(ir);


        if (ir == 0xE000) {  
            cout << "PC: " << setw(2) << setfill('0') << pc << " IR: " 
                 << setw(4) << setfill('0') << hex << uppercase << ir << " Halt\n";
            exit(0); // program halts and is exited 
        }

        if (ir != 0xE000) {  
            pc++;  
        }
    }

    return 0;
}
// Repsonse instructions for each instruction line of the Massey Machine Code
void execute(int instruction) {  
    int opcode = (instruction & 0xF000) >> 12;  // The opcode is the first hexadecimal digit
    int digit2 = (instruction & 0x0F00) >> 8;    // The second decimal digit is the register (we will call it digit2 to simplify it)
    int digit3 = (instruction & 0x00F0) >> 4;    // The third hexidecimal digit is used for operations
    int digit4 = (instruction & 0x000F);          // The fourth hexidecimal digit is used for operations
    int address = instruction & 0x00FF;         // When the last two hexidecimal digits are used together they represent an address. 

    // 1RXY Load Register R with the value XY
    if (opcode == 0x1) {
        reg[digit2] = address;
        cout << "PC: " << setw(2) << setfill('0') << pc << " IR: "  //set the width and full with zeros for accurate readings
             << setw(4) << setfill('0') << hex << uppercase << instruction // hex ensures it is a hexadecimal number and uppercase ensures it is displayed correctly 
             << " Load R" << digit2 << " with " << setw(4) << setfill('0') << address << endl;
    }
    // 20RS Load register R with the number in register S
    else if (opcode == 0x2) {
        reg[digit2] = reg[digit3];
        cout << "PC: " << setw(2) << setfill('0') << pc << " IR: " 
             << setw(4) << setfill('0') << hex << uppercase << instruction
             << " Load R" << digit2 << " with R" << digit3 << " = " << setw(4) << setfill('0') << reg[digit2] << endl;
    }
    // 3RXY Load register R with the number in the memory location at address XY
    else if (opcode == 0x3) {
        reg[digit2] = memory[address];
        cout << "PC: " << setw(2) << setfill('0') << pc << " IR: " 
             << setw(4) << setfill('0') << hex << uppercase << instruction
             << " Load R" << digit2 << " from Memory[" << setw(2) << setfill('0') << address << "] = " 
             << setw(4) << setfill('0') << memory[address] << endl;
    }
    // 4RXY Store the number in register R in the memory location at address XY
    else if (opcode == 0x4) {
        memory[address] = reg[digit2];
        cout << "PC: " << setw(2) << setfill('0') << pc << " IR: " 
             << setw(4) << setfill('0') << hex << uppercase << instruction
             << " Store R" << digit2 << " to Memory[" << setw(2) << setfill('0') << address << "]" << endl;
    }
    // 5RST Add the numbers in register S and T, load the result into register R (floating point addition)
    else if (opcode == 0x5) {
        reg[digit2] = reg[digit3] + reg[digit4]; 
        cout << "PC: " << setw(2) << setfill('0') << pc << " IR: " 
             << setw(4) << setfill('0') << hex << uppercase << instruction
             << " Add R" << digit3 << " + R" << digit4 << " to R" << digit2 << " = " << setw(4) << setfill('0') << reg[digit2] << endl;
    }
    //6RST Add the numbers in register S and T, load the result into register R (integer addition)
    else if (opcode == 0x6) {
        reg[digit2] = reg[digit3] + reg[digit4]; 
        cout << "PC: " << setw(2) << setfill('0') << pc << " IR: " 
             << setw(4) << setfill('0') << hex << uppercase << instruction
             << " Integer Add R" << digit3 << " + R" << digit4 << " to R" << digit2 << " = " << setw(4) << setfill('0') << reg[digit2] << endl;
    }
    // 70R0  Negate register R. (Carry out complement and add 1 – in effect, multiply R with -1)
    else if (opcode == 0x7) { 
    reg[digit2] = ~reg[digit2] + 1; // Performing two's complement negation
    cout << "PC: " << setw(2) << setfill('0') << pc << " IR: " 
         << setw(4) << setfill('0') << hex << uppercase << instruction
         << " Negate R" << digit2 << " = " << setw(4) << setfill('0') << reg[digit2] << endl;
}
    //  8R0X Shift the number in register R to the right X times.
    else if (opcode == 0x8) {
        reg[digit2] >>= address; 
        cout << "PC: " << setw(2) << setfill('0') << pc << " IR: " 
             << setw(4) << setfill('0') << hex << uppercase << instruction
             << " Shift R" << digit2 << " right by " << address << " = " << setw(4) << setfill('0') << reg[digit2] << endl;
    }
    // 9R0X Shift the number in register R to the left X times.
    else if (opcode == 0x9) {
        reg[digit2] <<= address; 
        cout << "PC: " << setw(2) << setfill('0') << pc << " IR: " 
             << setw(4) << setfill('0') << hex << uppercase << instruction
             << " Shift R" << digit2 << " left by " << address << " = " << setw(4) << setfill('0') << reg[digit2] << endl;
    }
    // ARST AND the numbers in registers S and T and load the result into register R.
    else if (opcode == 0xA) {
        reg[digit2] = reg[digit3] & reg[digit4]; 
        cout << "PC: " << setw(2) << setfill('0') << pc << " IR: " 
             << setw(4) << setfill('0') << hex << uppercase << instruction
             << " AND R" << digit3 << " and R" << digit4 << " to R" << digit2 << " = " << setw(4) << setfill('0') << reg[digit2] << endl;
    }
    // BRST  OR the numbers in registers S and T and load the result into register R.
    else if (opcode == 0xB) {
        reg[digit2] = reg[digit3] | reg[digit4];
        cout << "PC: " << setw(2) << setfill('0') << pc << " IR: " 
             << setw(4) << setfill('0') << hex << uppercase << instruction
             << " OR R" << digit3 << " and R" << digit4 << " to R" << digit2 << " = " << setw(4) << setfill('0') << reg[digit2] << endl;
    }
    // CRST XOR the numbers in registers S and T and load the result into register R.
    else if (opcode == 0xC) {
        reg[digit2] = reg[digit3] ^ reg[digit4]; 
        cout << "PC: " << setw(2) << setfill('0') << pc << " IR: " 
             << setw(4) << setfill('0') << hex << uppercase << instruction
             << " XOR R" << digit3 << " and R" << digit4 << " to R" << digit2 << " = " << setw(4) << setfill('0') << reg[digit2] << endl;
    }
    // DRXY Jump to the instruction at address XY if the value in register R is equal to the value in register R0. 
    else if (opcode == 0xD) {
        if (reg[digit2] == reg[0]) { 
            cout << "True - jump to location  " << address << endl;
            pc = address; // Set pc to the new address after jump is performed 
        } else {
            cout << "PC: " << setw(2) << setfill('0') << pc << " IR: " 
                 << setw(4) << setfill('0') << hex << uppercase << instruction
                 << "Register " << digit2 << "is not equal to R0. Jump can not be perfomed";
        }
    }
    // E000 Halt
    else if (opcode == 0xE) {
        cout << "PC: " << setw(2) << setfill('0') << pc << " IR: " 
             << setw(4) << setfill('0') << hex << uppercase << instruction << " Program halts\n";
    }
    // If instruction does not follow format 
    else {
        cout << "PC: " << setw(2) << setfill('0') << pc << " IR: " 
             << setw(4) << setfill('0') << hex << uppercase << instruction
             << " Instruction does not follow Massey Machine program -  please try again.\n";
    }
}

