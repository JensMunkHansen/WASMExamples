#include <cstdlib>
#include <emscripten.h>
#include <iostream>
#include <string>

// extern "C" void __funcs_on_exit();

#if 1
// Fallback input using EM_JS
EM_JS(const char*, getInputFromJavaScript, (), {
  const prompt = require('prompt-sync')();
  const input = prompt("Enter input (or type 'exit' to quit): ");
  const buffer = Module._malloc((input.length + 1) * Module.HEAP8.BYTES_PER_ELEMENT);
  Module.stringToUTF8(input, buffer, input.length + 1);
  return buffer;
});
#else
// Add a mutex to serialize input handling
EM_JS(const char*, getInputFromJavaScript, (), {
  if (typeof Module.inputLock == = 'undefined')
  {
    Module.inputLock = false;
  }

  // Wait for the lock to be released
  while (Module.inputLock)
  {
    // Busy wait (in a real-world scenario, you might use async-await here)
  }

  // Acquire the lock
  Module.inputLock = true;

  try
  {
    const prompt = require('prompt-sync')();
    const input = prompt("Enter input (or type 'exit' to quit): ");
    if (input == = null)
    {
      Module.inputLock = false;
      return 0; // Return null pointer if input is invalid
    }
    const buffer = Module._malloc((input.length + 1) * Module.HEAP8.BYTES_PER_ELEMENT);
    Module.stringToUTF8(input, buffer, input.length + 1);

    Module.inputLock = false; // Release the lock
    return buffer;
  }
  catch (e)
  {
    Module.inputLock = false; // Ensure lock is released on error
    console.error("Error during input:", e);
    return 0; // Return null pointer on error
  }
});
#endif
int main()
{
  std::cout << "Interactive mode. Type something:\n";

  while (true)
  {
    std::cout << "> ";
    std::string input;

    // Attempt to read from stdin
    if (std::getline(std::cin, input))
    {
      if (input == "exit")
      {
        std::cout << "Exiting..." << std::endl;
        break;
      }
      std::cout << "You entered (via stdin): " << input << std::endl;
    }
    else
    {
      // Fallback to JavaScript prompt if stdin is unavailable
      const char* jsInput = getInputFromJavaScript();
      input = std::string(jsInput);
      std::cout << "You entered (via JavaScript): " << input << std::endl;

      if (input == "exit")
      {
        std::cout << "Exiting..." << std::endl;
        // emscripten_force_exit(0); // Force exit
        break;
      }
    }
  }

  return 0;
}
