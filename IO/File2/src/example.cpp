#include "json.hpp" // Include JSON library
#include <emscripten.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using json = nlohmann::json;

extern "C"
{

  EMSCRIPTEN_KEEPALIVE
  int TestMe(int argc, char* argv[])
  {
    std::string inputFile, outputFile;

    // Parse arguments
    for (int i = 1; i < argc; ++i)
    {
      if (std::string(argv[i]) == "-D" && i + 1 < argc)
      {
        inputFile = argv[++i];
      }
      else if (std::string(argv[i]) == "-T" && i + 1 < argc)
      {
        outputFile = argv[++i];
      }
    }

    if (inputFile.empty() || outputFile.empty())
    {
      std::cerr << "Usage: TestMe -D <inputFile> -T <outputFile>" << std::endl;
      return 1;
    }

    // Print input file for debugging
    std::cout << "Input file: " << inputFile << std::endl;

    std::ifstream inFile(inputFile);
    if (!inFile.is_open())
    {
      std::cerr << "Error: Could not open input file: " << inputFile << std::endl;
      return 1;
    }

    // Parse JSON array
    json inputJson;
    try
    {
      inFile >> inputJson; // Parse JSON content
    }
    catch (json::parse_error& e)
    {
      std::cerr << "Error parsing JSON: " << e.what() << std::endl;
      return 1;
    }
    inFile.close();

    if (!inputJson.is_array())
    {
      std::cerr << "Error: Input file does not contain a JSON array." << std::endl;
      return 1;
    }

    // Compute the sum of the JSON array
    int sum = 0;
    for (const auto& item : inputJson)
    {
      if (item.is_number())
      {
        sum += item.get<int>();
      }
    }

    // Write result to output file
    std::ofstream outFile(outputFile);
    if (!outFile.is_open())
    {
      std::cerr << "Error: Could not open output file: " << outputFile << std::endl;
      return 1;
    }

    outFile << "{\"Sum\": " << sum << "}" << std::endl;
    std::cout << "Sum computed in WASM: " << sum << std::endl;
    outFile.close();

    std::cout << "Processed successfully. Results written to " << outputFile << std::endl;
    return 0;
  }
}
