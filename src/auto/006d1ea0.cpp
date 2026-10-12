#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(char, __fastcall, sub_6D1DB0, 0x006D1DB0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int*, __fastcall, sub_6FD400, 0x006FD400, (void* self, void* edx, int a2, int a3));

static uint32_t* __fastcall C_00860e68_CreateLightObject(C_00860e68* self, void* /*edx*/, int a2, int a3)
{
    void* allocated = Mem_Alloc(132);
    int* lightObject = nullptr;
    if (allocated)
        lightObject = sub_6FD400(allocated, nullptr, a2, reinterpret_cast<int>(self));

    self->field_0084 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(lightObject));

    // Virtual slot +0x20 (index 8) on the light object's vtable, argument a3.
    ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, lightObject, 0)[0x20 / 4])(lightObject, nullptr, a3);

    sub_6D1DB0(self, nullptr, reinterpret_cast<uint32_t*>(lightObject));
    return reinterpret_cast<uint32_t*>(lightObject);
}

HOOK(0x006D1EA0, C_00860e68_CreateLightObject, "auto");
