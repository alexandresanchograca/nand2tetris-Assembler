#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <vector>
#include "UtilData.h"

class FileOperations
{
public:
    static int parser(std::string filename, std::vector < std::string >* parsedStrings) {
        //Opens the file
        std::ifstream file(filename);
        if (file.is_open()) {
            std::string line;
            int lineCount = 0;
            while (std::getline(file, line)) { //Checks until it hits an \n
                //Clear all the comments and indentation and assign it to a string vector
                if (line.find("//") != std::string::npos)
                    line.erase(line.find("//"));

                if (line.empty()) //Ignores white lines
                    continue;

                line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end()); //Removes whitespaces

                if (line[0] == '(') {
                    symbolTable["@" + line.substr(1, line.size() - 2)] = lineCount; //Removing ()
                    continue;
                }
                else
                    lineCount++;

                parsedStrings->push_back(line);
            }
            file.close();
            return 1;
        }
        else {
            std::cout << "Can't open the file, please try again!" << std::endl;
            return 0;
        }
    }

    static int writeFile(std::vector<std::string>* translatedBinary, std::string outFilename) {
       
        outFilename = outFilename.substr(0, outFilename.find_first_of('.')) + ".hack";
        std::cout << "Generating file " << outFilename << std::endl;
        
        std::ofstream outputFile(outFilename);

        if (outputFile.is_open()) {
            for (int i = 0; i < translatedBinary->size(); i++) {
                outputFile << translatedBinary->at(i) + "\n";
            }
            outputFile.close();
            return 1;
        }
        else {
            std::cout << "Unable to write to file!";
            outputFile.close();
            return 0;
        }
    }
};

