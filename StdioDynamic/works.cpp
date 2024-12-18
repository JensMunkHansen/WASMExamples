#include <emscripten.h>
#include <iostream>

// Simplified EM_JS function
#if 0
EM_JS(const char*, getInputFromJavaScript, (), {
  const input = "Hello from JavaScript";
  const buffer = Module._malloc((input.length + 1) * Module.HEAP8.BYTES_PER_ELEMENT);
  Module.stringToUTF8(input, buffer, input.length + 1);
  return buffer;
});
#endif

int main()
{
  std::cout << "Interactive mode. Type something:\n";
  std::cout << "> ";
  std::string input(reinterpret_cast<const char*>(getInputFromJavaScript()));
  std::cout << "You entered: " << input << std::endl;

  return 0;
}
