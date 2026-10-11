#include <cstdint>
#include "globals.h"
#include "hook.h"

// __thiscall (this = ecx, one stack arg), ret 4. The second argument is passed
// through untouched as an integer-sized value.
CSF_FUNC(char*, __fastcall, WaypointGraph_ParseEntry, 0x0075DEC0, (void* self, void* edx, int a2));

// Forwards a2 to sub_75DEC0 (with a1 as the this pointer) when a1 is non-null;
// otherwise nothing is done. The original returned an uninitialized eax in that
// case; this returns nullptr instead.
static char* __stdcall WaypointGraph_ForwardToParseIfNonNull(char* a1, std::uint32_t* a2)
{
    if (a1)
        return WaypointGraph_ParseEntry(reinterpret_cast<void*>(a1), nullptr,
                                        static_cast<int>(reinterpret_cast<intptr_t>(a2)));
    return nullptr;
}

HOOK(0x0075DF90, WaypointGraph_ForwardToParseIfNonNull, "auto");
