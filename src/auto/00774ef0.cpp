#include <cstdint>
#include "globals.h"
#include "hook.h"

// __fastcall-bound callee (this = ecx, one stack arg), ret 4. The original passes
// a1 in ecx and a2 on the stack; the edx slot is unused.
static CSF_FUNC(char*, __fastcall, sub_774E20, 0x00774E20, (void* self, void* edx, int a2));

// Forwards a2 to sub_774E20 with a1 as the this pointer when a1 is non-null.
// Otherwise nothing is called. The original returned an uninitialized eax in that
// case; this returns nullptr instead.
static char* __stdcall ForwardToEntryIfNonNull(char* a1, std::uint32_t* a2)
{
    if (a1)
        return sub_774E20(a1, nullptr, static_cast<int>(reinterpret_cast<intptr_t>(a2)));
    return nullptr;
}

HOOK(0x00774EF0, ForwardToEntryIfNonNull, "auto");
