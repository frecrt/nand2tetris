#ifndef HACKASSEMBLER_H
#define HACKASSEMBLER_H

#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class HackAssembler {
public:
    // Assemble an .asm file into .hack file
    // Returns true on success, false on failure
    bool assemble(const string& inputPath, const string& outputPath);

private:
    // ===== Parser =====
    // Remove comments and whitespace, return clean instruction lines
    vector<string> preprocess(const string& inputPath);
    
    // Determine instruction type: A, C, or L (label/pseudo)
    enum class InstrType { A_INSTR, C_INSTR, L_INSTR };
    InstrType parseType(const string& line) const;
    
    // ===== First Pass: Build Symbol Table =====
    // Scan for label declarations (XXX) and assign ROM addresses
    void firstPass(const vector<string>& lines, unordered_map<string, int>& symbolTable, int& romAddress);
    
    // ===== Second Pass: Translate =====
    // Convert instructions to binary, handle variables
    vector<string> secondPass(const vector<string>& lines, unordered_map<string, int>& symbolTable, int& nextVarAddress);
    
    // ===== Code Module =====
    string translateA(const string& line, unordered_map<string, int>& symbolTable, int& nextVarAddress);
    string translateC(const string& line);
    
    string compBits(const string& comp) const;
    string destBits(const string& dest) const;
    string jumpBits(const string& jump) const;
    
    // Check if string is a number
    bool isNumber(const string& s) const;
    
    // Strip whitespace
    string strip(const string& s) const;
    
    // Helper: int to 16-bit binary string
    string intToBinary16(int value) const;
    
    // Predefined symbols
    unordered_map<string, int> predefinedSymbols() const;
};

#endif // HACKASSEMBLER_H
