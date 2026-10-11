#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, sub_4E86B0, 0x004E86B0, (void* self, void* edx, int a2, float a3, int a4, int a5));
CSF_FUNC(int, __fastcall, sub_4E8AA0, 0x004E8AA0, (void* self, void* edx, int a2, float a3, int a4, int a5));

// Selector 0 builds a 164-byte object via sub_4E86B0; selector 1 builds an 80-byte
// object via sub_4E8AA0. Any other selector returns 0 without allocating.
static int __cdecl CreateSetupFloatParameters(int setupKind, int a2, float a3, int a4, int a5)
{
    int result = 0;

    if (setupKind == 0)
    {
        void* memory = Mem_Alloc(164);
        if (memory != nullptr)
            return sub_4E86B0(memory, nullptr, a2, a3, a4, a5);
    }
    else if (setupKind == 1)
    {
        void* memory = Mem_Alloc(80);
        if (memory != nullptr)
            return sub_4E8AA0(memory, nullptr, a2, a3, a4, a5);
    }

    return result;
}

HOOK(0x4E56E0, CreateSetupFloatParameters, "auto");
