#include <cstdint>
#include "globals.h"
#include "hook.h"
CSF_FUNC(int, __fastcall, AssignWideStringRecords, 0x00723950,
         (void* self, void*, void* source));

static int __cdecl Assign(char* first, char* last, void* source)
{
    // Empty ranges have no defined return value in the original; callers ignore it.
    int result = 0;
    for (; first != last; first += 16)
        result = AssignWideStringRecords(first, nullptr, source);
    return result;
}
HOOK(0x00723FD0, Assign, "stl");
