// Carolyn MacLeod - code challenge main application 
// Only command line validation in main file if I get time - but more functionality here to start
// December 4, 2025

#include <iostream>
#include <filesystem>
#include "inputLoader.h"
#include "VisualizeEvent.h"

int main(int argc, char* argv[]) {
    std::cout << "Welcome to my code \n";
	if (argc != 2)
	{
		std::cout << "Expect 1 argument - PATH path to input data\n";
		return 1;
	}
	const std::filesystem::path dataPath{argv[1]};
    if (std::filesystem::exists(dataPath)) {
        std::cout << "File '" << dataPath << "' exists." << std::endl;
    } else {
        std::cout << "File '" << dataPath << "' does not exist." << std::endl;
    }
	
	// would probably encapsulate this in a class later, but doing this semi quickly
	// also will add error handling string if I have time later
	std::vector<EventRecord> EventData;
	
	//awkward conversion - need to change the load code to support filesystem path
	std::string dataPathString = argv[1];
	
	auto loadResult = LoadEventRecords(dataPathString, EventData);
	
	if (loadResult)
	{
		std::cout << "Data Successfully Loaded\n";
	}
	else
	{
		std::cout << "Failure in Loading Data\n";
		return 2;
	}
	
	
	
	
	// Should handle errors here
	auto displayResult = DisplayData(EventData);
	
	return 0;
}