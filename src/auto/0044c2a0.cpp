#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char*, __fastcall, CopyRecordFromSource, 0x0044C0C0, (char* self, void*, char* source));

// EAX is unspecified for an empty range; otherwise retain the last callee result.
static char* __cdecl Assign(char* first, char* last, char* source) {
    char* result = nullptr;
    for (; first != last; first += 100)
        result = CopyRecordFromSource(first, nullptr, source);
    return result;
}
HOOK(0x0044C2A0, Assign, "stl");
