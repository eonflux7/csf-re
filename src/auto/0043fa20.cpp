#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char*, __fastcall, Campaign_AssignRecord, 0x0043F3E0, (char* self, void*, const char* source));

// cdecl; the element assignment pops its one source argument, not ours.
// EAX is unspecified for an empty range.
static char* __cdecl Assign(char* first, char* last, const char* source) {
    char* result = nullptr;
    for (; first != last; first += 196)
        result = Campaign_AssignRecord(first, nullptr, source);
    return result;
}
HOOK(0x0043FA20, Assign, "stl");
