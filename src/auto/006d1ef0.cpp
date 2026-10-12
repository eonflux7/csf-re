#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(char, __fastcall, sub_6D1DB0, 0x006D1DB0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(uint32_t*, __fastcall, sub_6FD540, 0x006FD540, (void* self, void* edx, int a2, int a3));

// Allocates a 132-byte light object, initializes it for this owner, stores it in
// this->field_0084 and registers it through sub_6D1DB0. Returns the light object
// (or nullptr when the allocation fails).
static uint32_t* __fastcall LightWorld_CreateAndAttachLight(C_00860e68* self, void* /*edx*/, int a2)
{
    void* allocatedLight = Mem_Alloc(132);
    uint32_t* lightObject = nullptr;
    if (allocatedLight)
        lightObject = sub_6FD540(allocatedLight, nullptr, a2, reinterpret_cast<int>(self));

    self->field_0084 = reinterpret_cast<uint32_t>(lightObject);
    sub_6D1DB0(self, nullptr, lightObject);
    return lightObject;
}

HOOK(0x006D1EF0, LightWorld_CreateAndAttachLight, "auto");
