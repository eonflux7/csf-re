#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Game heap allocator (cdecl, pops its own argument).
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// Setup-object initializer (thiscall-style: ecx = new object, five stack args, ret 20).
CSF_FUNC(int, __fastcall, sub_4E4630, 0x004E4630, (void* self, void* edx, int a2, int a3, int a4, int a5, int a6));

// Creates a 0x58-byte setup object when setupType == 3 and initializes it with the
// remaining five arguments. Returns the initializer's result, or 0 otherwise.
static int __cdecl CreateSetupParameters(int setupType, int a2, int a3, int a4, int a5, int a6)
{
    int result = 0;
    if (setupType == 3)
    {
        void* setupObject = Mem_Alloc(0x58);
        if (setupObject)
        {
            return sub_4E4630(setupObject, nullptr, a2, a3, a4, a5, a6);
        }
        return 0;
    }
    return result;
}

HOOK(0x004E10D0, CreateSetupParameters, "auto");
