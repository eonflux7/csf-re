#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, CharTaskBase_Init_4DB3C0, 0x004DB3C0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Teardown for TConversacion: resets the embedded CharTaskBase sub-objects, then
// frees the object itself when bit 0 of the flags argument is set.
static TConversacion* __fastcall TConversacion_DestroyWithOptionalFree(TConversacion* self, void* /*edx*/, char flags)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);

    // Sub-object at +0x64 gets the C_0082d410 vtable (stored at +0x78) before its init.
    CSF_AT(void*, self, 0x78) = CSF_VTBL(0x0082D410);

    CharTaskBase_Init_4DB3C0(base + 0x64, nullptr);
    CharTaskBase_Init_4DB3C0(base + 0x50, nullptr);
    CharTaskBase_Init_4DB3C0(base + 0x3C, nullptr);
    CharTaskBase_Init_4DB3C0(base + 0x28, nullptr);
    CharTaskBase_Init_4DB3C0(base + 0x14, nullptr);
    CharTaskBase_Init_4DB3C0(self, nullptr);

    if ((flags & 1) != 0) {
        Mem_Free(self);
    }

    return self;
}

HOOK(0x4D9990, TConversacion_DestroyWithOptionalFree, "auto");
