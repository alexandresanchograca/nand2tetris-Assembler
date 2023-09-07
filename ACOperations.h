#pragma once
#include "FileOperations.h"

class ACommand
{
public:

    static std::string decimalToBinary(int n) {
        int arr[15], i = 0, num = n;
        std::string binaryString;

        // Until the value of n becomes 0.
        while (n != 0) {
            arr[i] = n % 2;
            i++;
            n = n / 2;
        }

        for (i = i - 1; i >= 0; i--) {
            binaryString.append(std::to_string(arr[i]));
        }
        return binaryString;
    }

    static std::string AssembleComputation(std::string comp) {

        if (comp.compare("0") == 0) comp = ("0101010");

        else if (comp.compare("1") == 0) comp = ("0111111");

        else if (comp.compare("-1") == 0) comp = ("0111010");

        else if (comp.compare("D") == 0) comp = ("0001100");

        else if (comp.compare("A") == 0) comp = ("0110000");

        else if (comp.compare("!D") == 0) comp = ("0001101");

        else if (comp.compare("!A") == 0) comp = ("0110001");

        else if (comp.compare("-D") == 0) comp = ("0001111");

        else if (comp.compare("-A") == 0) comp = ("0110011");

        else if (comp.compare("D+1") == 0) comp = ("0011111");

        else if (comp.compare("A+1") == 0) comp = ("0110111");

        else if (comp.compare("D-1") == 0) comp = ("0001110");

        else if (comp.compare("A-1") == 0) comp = ("0110010");

        else if (comp.compare("D+A") == 0) comp = ("0000010");

        else if (comp.compare("D-A") == 0) comp = ("0010011");

        else if (comp.compare("A-D") == 0) comp = ("0000111");

        else if (comp.compare("D&A") == 0) comp = ("0000000");

        else if (comp.compare("D|A") == 0) comp = ("0010101");

        else if (comp.compare("M") == 0) comp = ("1110000");

        else if (comp.compare("!M") == 0) comp = ("1110001");

        else if (comp.compare("-M") == 0) comp = ("1110011");

        else if (comp.compare("M+1") == 0) comp = ("1110111");

        else if (comp.compare("M-1") == 0) comp = ("1110010");

        else if (comp.compare("D+M") == 0) comp = ("1000010");

        else if (comp.compare("D-M") == 0) comp = ("1010011");

        else if (comp.compare("M-D") == 0) comp = ("1000111");

        else if (comp.compare("D&M") == 0) comp = ("1000000");

        else if (comp.compare("D|M") == 0) comp = ("1010101");

        else
            comp = ("0000000");

        return comp;
    }

    static std::string AssembleA(std::string parsedString) {
        std::string outString;
        std::string memoryNumber = parsedString.substr(1, sizeof(parsedString));
        int memInt = std::atoi(memoryNumber.c_str());

        outString.append(decimalToBinary(memInt));

        std::string remainingString;
        for (int c = 0; c < 16 - outString.size(); c++) {
            remainingString.append("0");
        }

        return remainingString + outString;
    }

    static std::string AssembleDestination(std::string parsedString) {
        parsedString = parsedString.substr(0, parsedString.find_first_of('='));
        std::string temp("000");
        for (int c = 0; c < parsedString.size(); c++) {

            switch (parsedString[c])
            {
            case 'A':
                temp[0] = '1';
                break;
            case 'D':
                temp[1] = '1';
                break;
            case 'M':
                temp[2] = '1';
                break;
            default:
                break;
            }
        }
        return temp;
    }

    static std::string AssembleJump(std::string parsedString) {
        parsedString = parsedString.substr(parsedString.find_first_of(';') + 1, parsedString.find_first_of(' '));

        if (parsedString.compare("JGT") == 0) parsedString = "001";
        else if (parsedString.compare("JEQ") == 0) parsedString = "010";
        else if (parsedString.compare("JGE") == 0) parsedString = "011";
        else if (parsedString.compare("JLT") == 0) parsedString = "100";
        else if (parsedString.compare("JNE") == 0) parsedString = "101";
        else if (parsedString.compare("JLE") == 0) parsedString = "110";
        else if (parsedString.compare("JMP") == 0) parsedString = "111";

        return parsedString;
    }

    static void AssembleInstructions(std::vector<std::string> &parsedStrings, std::vector<std::string> &translatedBinary) {
        for (int i = 0; i < parsedStrings.size(); i++) {
            std::string currentBinaryInstruction;
            bool isDest = false;
            bool isJump = false;

            //Is opcode a or c operation
            if (parsedStrings.at(i).at(0) == '@') { //if true opcode is @any
                currentBinaryInstruction = AssembleA(parsedStrings[i]);
            }
            else { //C command X=ZZ;YY
                currentBinaryInstruction.append("111");

                std::string dest;                
                if (parsedStrings[i].find("=") != std::string::npos) {
                    isDest = true;
                    dest = AssembleDestination(parsedStrings[i]);
                }
                else {
                    dest = "000";
                }

                std::string jump;
                if (parsedStrings[i].find(";") != std::string::npos) {
                    isJump = true;
                    jump = AssembleJump(parsedStrings[i]);
                }
                else
                {
                    jump = "000";
                }

                std::string comp;
                if (isJump && isDest)
                    comp = parsedStrings[i].substr(parsedStrings[i].find_first_of('=') + 1, parsedStrings[i].find_first_of(';'));
                else if (isJump && !isDest)
                    comp = parsedStrings[i].substr(0, parsedStrings[i].find_first_of(';'));
                else
                    comp = parsedStrings[i].substr(parsedStrings[i].find_first_of('=') + 1, parsedStrings[i].find_first_of(' '));

                comp = AssembleComputation(comp);

                currentBinaryInstruction.append(comp + dest + jump);
            }

            if(!currentBinaryInstruction.empty())
                translatedBinary.push_back(currentBinaryInstruction);
        }
    }

    static void HandleLabelVars(std::vector<std::string> &parsedStrings) {
        int counter = 16;
        for (int i = 0; i < parsedStrings.size(); i++) {

            if (symbolTable.find(parsedStrings[i]) != symbolTable.end()) {
                std::string tableValue = std::to_string(symbolTable[parsedStrings[i]]);
                parsedStrings[i] = "@" + tableValue;
            }
            else if (parsedStrings[i][0] == '@' && isalpha(parsedStrings[i][1])) {
                    symbolTable[parsedStrings[i]] = counter;
                    parsedStrings[i] = "@" + std::to_string(counter);
                    counter++;
            }
        }
    }

    static void HandleVars(std::vector<std::string>& parsedStrings) {
        for (int i = 0; i < parsedStrings.size(); i++) {
            if (parsedStrings[i][0] == '@' && isalpha(parsedStrings[i][1])) {
                if (symbolTable.find(parsedStrings[i].substr(0, parsedStrings[i].size())) != symbolTable.end()) {
                    std::string tableValue = std::to_string(symbolTable[parsedStrings[i].substr(1, parsedStrings[i].size() - 1)]);
                    parsedStrings[i] = "@" + tableValue;
                }
            }
        }
    }

};

