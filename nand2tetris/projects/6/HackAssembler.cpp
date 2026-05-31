// HackAssembler.cpp
// Hack Assembly Language Assembler
// Translates .asm files to .hack binary files
// Part of Nand2Tetris Project 6

#include "HackAssembler.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <cstdlib>

// ===== Main function =====
int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: HackAssembler <input.asm> [output.hack]" << endl;
        cerr << "  If output is omitted, it is derived from input filename." << endl;
        return 1;
    }

    string inputPath = argv[1];
    string outputPath;

    if (argc >= 3) {
        outputPath = argv[2];
    } else {
        // Derive .hack filename from .asm filename
        size_t dotPos = inputPath.rfind('.');
        if (dotPos != string::npos) {
            outputPath = inputPath.substr(0, dotPos) + ".hack";
        } else {
            outputPath = inputPath + ".hack";
        }
    }

    HackAssembler assembler;
    if (assembler.assemble(inputPath, outputPath)) {
        cout << "Assembled: " << inputPath << " -> " << outputPath << endl;
        return 0;
    } else {
        return 1;
    }
}

// ===== Entry Point =====
bool HackAssembler::assemble(const string& inputPath, const string& outputPath) {
    // 1. Read and preprocess the file
    vector<string> lines = preprocess(inputPath);
    if (lines.empty()) {
        cerr << "Error: No instructions found or file could not be read: " << inputPath << endl;
        return false;
    }
    
    // 2. First pass: build symbol table from labels
    unordered_map<string, int> symbolTable = predefinedSymbols();
    int romAddress = 0;
    firstPass(lines, symbolTable, romAddress);
    
    // 3. Second pass: translate to binary
    int nextVarAddress = 16;  // Variables start at RAM[16]
    vector<string> binary = secondPass(lines, symbolTable, nextVarAddress);
    
    // 4. Write output file
    ofstream out(outputPath);
    if (!out.is_open()) {
        cerr << "Error: Could not write to output file: " << outputPath << endl;
        return false;
    }
    
    for (const string& line : binary) {
        out << line << endl;
    }
    
    out.close();
    return true;
}

// ===== Utility Functions =====
string HackAssembler::strip(const string& s) const {
    size_t start = 0;
    while (start < s.length() && (s[start] == ' ' || s[start] == '\t')) {
        start++;
    }
    size_t end = s.length();
    while (end > start && (s[end-1] == ' ' || s[end-1] == '\t' || s[end-1] == '\r')) {
        end--;
    }
    return s.substr(start, end - start);
}

bool HackAssembler::isNumber(const string& s) const {
    if (s.empty()) return false;
    for (char c : s) {
        if (!isdigit(c)) return false;
    }
    return true;
}

string HackAssembler::intToBinary16(int value) const {
    string result;
    for (int i = 15; i >= 0; i--) {
        result += ((value >> i) & 1) ? '1' : '0';
    }
    return result;
}

unordered_map<string, int> HackAssembler::predefinedSymbols() const {
    unordered_map<string, int> symbols;
    symbols["SP"] = 0;
    symbols["LCL"] = 1;
    symbols["ARG"] = 2;
    symbols["THIS"] = 3;
    symbols["THAT"] = 4;
    symbols["R0"] = 0;
    symbols["R1"] = 1;
    symbols["R2"] = 2;
    symbols["R3"] = 3;
    symbols["R4"] = 4;
    symbols["R5"] = 5;
    symbols["R6"] = 6;
    symbols["R7"] = 7;
    symbols["R8"] = 8;
    symbols["R9"] = 9;
    symbols["R10"] = 10;
    symbols["R11"] = 11;
    symbols["R12"] = 12;
    symbols["R13"] = 13;
    symbols["R14"] = 14;
    symbols["R15"] = 15;
    symbols["SCREEN"] = 16384;
    symbols["KBD"] = 24576;
    return symbols;
}

// ===== Preprocessor =====
vector<string> HackAssembler::preprocess(const string& inputPath) {
    ifstream in(inputPath);
    if (!in.is_open()) {
        cerr << "Error: Could not open input file: " << inputPath << endl;
        return {};
    }
    
    vector<string> result;
    string line;
    
    while (getline(in, line)) {
        // Remove full-line comments
        size_t commentPos = line.find("//");
        if (commentPos != string::npos) {
            line = line.substr(0, commentPos);
        }
        
        line = strip(line);
        
        // Skip empty lines
        if (line.empty()) {
            continue;
        }
        
        result.push_back(line);
    }
    
    in.close();
    return result;
}

// ===== Instruction Type =====
HackAssembler::InstrType HackAssembler::parseType(const string& line) const {
    if (line.empty()) return InstrType::A_INSTR;  // shouldn't happen
    
    if (line[0] == '@') {
        return InstrType::A_INSTR;
    } else if (line[0] == '(') {
        return InstrType::L_INSTR;
    } else {
        return InstrType::C_INSTR;
    }
}

// ===== First Pass =====
void HackAssembler::firstPass(const vector<string>& lines, 
                               unordered_map<string, int>& symbolTable,
                               int& romAddress) {
    romAddress = 0;
    
    for (const string& line : lines) {
        if (parseType(line) == InstrType::L_INSTR) {
            // Label declaration: (XXX)
            string label = line.substr(1, line.length() - 2);
            label = strip(label);
            symbolTable[label] = romAddress;
        } else {
            // A-instruction or C-instruction - occupies a ROM slot
            romAddress++;
        }
    }
}

// ===== Second Pass =====
vector<string> HackAssembler::secondPass(const vector<string>& lines,
                                          unordered_map<string, int>& symbolTable,
                                          int& nextVarAddress) {
    vector<string> result;
    
    for (const string& line : lines) {
        InstrType type = parseType(line);
        
        if (type == InstrType::L_INSTR) {
            // Skip label declarations (already processed in first pass)
            continue;
        } else if (type == InstrType::A_INSTR) {
            result.push_back(translateA(line, symbolTable, nextVarAddress));
        } else {
            // C_INSTR
            result.push_back(translateC(line));
        }
    }
    
    return result;
}

// ===== A-Instruction Translation =====
string HackAssembler::translateA(const string& line, 
                                  unordered_map<string, int>& symbolTable,
                                  int& nextVarAddress) {
    // Format: @value or @symbol
    string valueStr = line.substr(1);  // Remove '@'
    valueStr = strip(valueStr);
    
    int value;
    
    if (isNumber(valueStr)) {
        value = atoi(valueStr.c_str());
    } else {
        // It's a symbol
        if (symbolTable.find(valueStr) == symbolTable.end()) {
            // New variable - assign next available address
            symbolTable[valueStr] = nextVarAddress;
            nextVarAddress++;
        }
        value = symbolTable[valueStr];
    }
    
    return "0" + intToBinary16(value).substr(1);  // A-instruction: 0 + 15-bit value
}

// ===== C-Instruction Translation =====
string HackAssembler::translateC(const string& line) {
    // Format: dest=comp;jump  (dest and jump are optional)
    string part = line;
    
    // Parse dest (if present)
    string dest, comp, jump;
    
    size_t eqPos = part.find('=');
    if (eqPos != string::npos) {
        dest = part.substr(0, eqPos);
        part = part.substr(eqPos + 1);
    }
    
    // Parse jump (if present)
    size_t semiPos = part.find(';');
    if (semiPos != string::npos) {
        comp = part.substr(0, semiPos);
        jump = part.substr(semiPos + 1);
    } else {
        comp = part;
    }
    
    string compB = compBits(comp);
    string destB = destBits(dest);
    string jumpB = jumpBits(jump);
    
    return "111" + compB + destB + jumpB;
}

// ===== Comp Bits =====
// ALU control bits: zx nx zy ny f no
// a=0 means A register, a=1 means M (memory)
string HackAssembler::compBits(const string& comp) const {
    string c = comp;
    // Remove spaces
    c.erase(remove_if(c.begin(), c.end(), ::isspace), c.end());
    
    // a=0 cases (A reference)
    if (c == "0")    return "0101010";
    if (c == "1")    return "0111111";
    if (c == "-1")   return "0111010";
    if (c == "D")    return "0001100";
    if (c == "A")    return "0110000";
    if (c == "!D")   return "0001101";
    if (c == "!A")   return "0110001";
    if (c == "-D")   return "0001111";
    if (c == "-A")   return "0110011";
    if (c == "D+1")  return "0011111";
    if (c == "A+1")  return "0110111";
    if (c == "D-1")  return "0001110";
    if (c == "A-1")  return "0110010";
    if (c == "D+A")  return "0000010";
    if (c == "D-A")  return "0010011";
    if (c == "A-D")  return "0000111";
    if (c == "D&A")  return "0000000";
    if (c == "D|A")  return "0010101";
    
    // a=1 cases (M reference)
    if (c == "M")    return "1110000";
    if (c == "!M")   return "1110001";
    if (c == "-M")   return "1110011";
    if (c == "M+1")  return "1110111";
    if (c == "M-1")  return "1110010";
    if (c == "D+M")  return "1000010";
    if (c == "D-M")  return "1010011";
    if (c == "M-D")  return "1000111";
    if (c == "D&M")  return "1000000";
    if (c == "D|M")  return "1010101";
    
    cerr << "Warning: Unknown comp mnemonic: '" << comp << "'" << endl;
    return "0000000";  // Default fallback (0)
}

// ===== Dest Bits =====
string HackAssembler::destBits(const string& dest) const {
    if (dest.empty()) return "000";
    
    string d = dest;
    d.erase(remove_if(d.begin(), d.end(), ::isspace), d.end());
    
    int bits = 0;
    if (d.find('A') != string::npos) bits |= 0b100;
    if (d.find('D') != string::npos) bits |= 0b010;
    if (d.find('M') != string::npos) bits |= 0b001;
    
    string result;
    result += (bits & 0b100) ? '1' : '0';
    result += (bits & 0b010) ? '1' : '0';
    result += (bits & 0b001) ? '1' : '0';
    return result;
}

// ===== Jump Bits =====
string HackAssembler::jumpBits(const string& jump) const {
    if (jump.empty()) return "000";
    
    string j = jump;
    j.erase(remove_if(j.begin(), j.end(), ::isspace), j.end());
    
    if (j == "JGT") return "001";
    if (j == "JEQ") return "010";
    if (j == "JGE") return "011";
    if (j == "JLT") return "100";
    if (j == "JNE") return "101";
    if (j == "JLE") return "110";
    if (j == "JMP") return "111";
    
    cerr << "Warning: Unknown jump mnemonic: '" << jump << "'" << endl;
    return "000";
}
