#include <iostream>
#include <string>
#include "IO_Helper/Helper.h"
#include "Helper/Usage.h"

using namespace std;
using namespace Usage_Helper;
using namespace StreamControl;

int main(int argc, char* argv[]) {
    Usage cli(argc, argv);
    if(cli.isVersion()) Usage::printVersion();
    if(cli.isHelp()) Usage::printHelp();
    if(cli.isHelp() || cli.isVersion()) return 0;

    string inputPath = cli.getInputPath();
    string outputPath = cli.getOutputPath();

    if(std::filesystem::is_directory(inputPath)) {
        // Compile all .jack files in the directory
        std::filesystem::path inDir(inputPath);
        std::filesystem::path outDir(outputPath);
        for(auto& entry : std::filesystem::directory_iterator(inDir)) {
            std::cout << "Processing file: " << entry.path() << std::endl;
            if(entry.is_regular_file() && entry.path().extension() == ".jack") {
                string inFile = entry.path().string();
                // Determine output file path
                std::string outFile;
                if(std::filesystem::is_directory(outDir)) {
                    outFile = (outDir / entry.path().filename().replace_extension(".vm")).string();
                } else {
                    // If outputPath is not directory (should not happen due to usageError), default to same dir
                    outFile = (inDir / entry.path().filename().replace_extension(".vm")).string();
                }
                Helper helper(inFile, outFile);
                helper.compile();
            }
        }
    } else {
        // Single file compilation
        Helper helper(inputPath, outputPath);
        helper.compile();
    }
    return 0;
}
