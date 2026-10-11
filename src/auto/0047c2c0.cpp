#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(uint32_t*, __fastcall, CharTask_InitBase, 0x004DB4C0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, sub_47BD60, 0x0047BD60, (void* self, void* edx));

// TFunctionRecargar constructor body: initializes the CharTask base at +0x00,
// a second base at +0x14 (its vtables at +0x14/+0x18), a sub-object at +0x28,
// and a third base at +0x78 (vtables at +0x78/+0x7C).
static uint32_t* __fastcall TFunctionRecargar_InitializeSubobjects(TFunctionRecargar* self, void* /*edx*/)
{
    uint8_t* raw = reinterpret_cast<uint8_t*>(self);

    CharTask_InitBase(self, nullptr);
    CSF_AT(void*, self, 0x00) = CSF_VTBL(0x00832E40);
    CSF_AT(void*, self, 0x04) = CSF_VTBL(0x00832E20);

    CharTask_InitBase(raw + 0x14, nullptr);
    CSF_AT(void*, self, 0x14) = CSF_VTBL(0x00831E98);
    CSF_AT(void*, self, 0x18) = CSF_VTBL(0x00831E78);

    sub_47BD60(raw + 0x28, nullptr);

    CharTask_InitBase(raw + 0x78, nullptr);
    CSF_AT(void*, self, 0x78) = CSF_VTBL(0x00832188);
    CSF_AT(void*, self, 0x7C) = CSF_VTBL(0x00832168);

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0047C2C0, TFunctionRecargar_InitializeSubobjects, "auto");
