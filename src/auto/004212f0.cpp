#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee 0x004DB140: __fastcall-style (ecx = this, one stack argument), returns eax.
CSF_FUNC(int, __fastcall, Node_UnlinkSelfReference, 0x004DB140, (void* self, void* edx, int a2));

// __thiscall C_0082f9f8::ResolveSelfLinkedChild(void** a2), ret 4.
static void** __fastcall C_0082f9f8_ResolveSelfLinkedChild(C_0082f9f8* self, void* /*edx*/, void** a2)
{
    void** result = a2;
    if (a2[1] == self && a2[0] != static_cast<void*>(self))
    {
        int returned = Node_UnlinkSelfReference(self, nullptr,
            static_cast<int>(reinterpret_cast<uintptr_t>(reinterpret_cast<uint8_t*>(self) + 0x20)));
        return reinterpret_cast<void**>(static_cast<uintptr_t>(static_cast<uint32_t>(returned)));
    }
    return result;
}

HOOK(0x004212F0, C_0082f9f8_ResolveSelfLinkedChild, "auto");
