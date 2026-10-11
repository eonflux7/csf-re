#include <cstddef>
#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(uint8_t*, __fastcall, WaterEffect_Initialize, 0x0070A2C0, (void* self, void* edx));

// Allocates a 0x108-byte water effect object, runs its initializer and
// stamps the vtable. Returns nullptr when the allocation fails.
static char* __cdecl GfxWater_CreateEffectObject() {
    void* allocated = Mem_Alloc(0x108);
    char* object = static_cast<char*>(allocated);
    if (!object) {
        return nullptr;
    }
    WaterEffect_Initialize(object, nullptr);
    CSF_AT(void*, object, 0) = CSF_VTBL(0x863548);
    return object;
}

HOOK(0x70A350, GfxWater_CreateEffectObject, "auto");
