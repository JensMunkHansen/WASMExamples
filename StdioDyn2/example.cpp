#include <emscripten.h>
#include <iostream>
#include <string>

// Define a reusable JavaScript function to read input
EM_JS(const char*, getInputFromStdin, (), {
  const readlineSync = require('readline-sync');
  const input = readlineSync.question("Enter input (or type 'exit' to quit): ");
  if (input === null || input === undefined)
  {
    return 0; // Return null pointer for invalid input
  }
  return Module.stringToNewUTF8(input);
});

std::atomic<int> main_call_count{0};

int main()
{
  int current_count = main_call_count.fetch_add(1, std::memory_order_acquire);

  // With '-sINVOKE_RUN=0`, we don't need this.
  if (current_count > 0) {
      // If main_called was already true, main has been called before
      std::cerr << "Error: main_function has already been called!" << std::endl;
      return 0;
  }
  std::cout << "Interactive mode. Type something:\n";

  while (true)
  {
    std::cout << "> ";
    const char* jsInput = getInputFromStdin();
    if (jsInput == nullptr)
    {
      std::cout << "No input received. Exiting...\n";
      break;
    }

    std::string input(jsInput);
    bool doExit = false;

    if (input == "exit")
    {
      std::cout << "Exiting...\n";
      doExit = true;
    }

    std::cout << "You entered: " << input << "\n";

    EM_ASM({ Module._free($0); }, jsInput);

    if (doExit)
    {
      return 0;
      break;
    }
  }

  return 0;
}
