#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));

static int __stdcall AiOrder_SetLinkedObjectDefaultValue(int a1)
{
    int result = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(static_cast<uintptr_t>(a1)), nullptr);
    if (result != 0)
    {
        CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(result)), 0x698) = 1084227584u;
    }
    else
    {
        // Original writes through a null base pointer (eax == 0): absolute address 0x69C.
        void* nullBase = nullptr;
        CSF_AT(uint32_t, nullBase, 0x69C) = 1084227584u;
        return 0;
    }
    return result;
}

HOOK(0x4AD2D0, AiOrder_SetLinkedObjectDefaultValue, "auto");
