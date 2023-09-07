// nand2tetris Assembler.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "ACOperations.h"

int main()
{
    std::string filename;
    std::vector < std::string > parsedStrings; //Storing the file input in memory
    std::vector<std::string> translatedBinary; //Storing the  translated binary code in memory

    std::cout << "Type the file name that you want to assemble: ";
    std::cin >> filename;

    std::cout << "Converting " << filename << " to binary code for our hack computer..." << std::endl;

    if (!FileOperations::parser(filename, &parsedStrings))
        return 0;

    ACommand::HandleLabelVars(parsedStrings);
    ACommand::AssembleInstructions(parsedStrings, translatedBinary);

    if (!FileOperations::writeFile(&translatedBinary, filename))
        return 0;

    std::cout << "Finished sucessfully, please check the output file." << std::endl;
    return 1;
}