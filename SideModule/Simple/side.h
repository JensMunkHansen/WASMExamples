#ifndef _SIDE_H
#define _SIDE_H

struct MyStruct; // WebAssembly itself doesn't export types like structs or classes. You must rely
                 // on ccall, cwrap or embind
void CreateMyStruct(struct MyStruct* s);

#endif
