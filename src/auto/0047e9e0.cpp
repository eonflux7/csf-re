#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int*, __fastcall, sub_47EA40, 0x0047EA40, (void* self, void* edx, int a5));
CSF_FUNC(int*, __fastcall, sub_47EC10, 0x0047EC10, (void* self, void* edx, int a5));

// Creates a 44-byte copy-state object (vtable 0x832EEC) seeded from this task's
// field_0028, and initializes its two embedded sub-objects from this object's
// sub-object regions at +0x08 and +0x18.
static int* __fastcall C_00832eec_CreateTaskCopy(C_00832eec* self, void* /*edx*/)
{
    uint8_t* created = nullptr;
    uint8_t* allocated = static_cast<uint8_t*>(Mem_Alloc(44));
    if (allocated) {
        CSF_AT(uint32_t, allocated, 0x04) = 1;
        CSF_AT(void*, allocated, 0x00) = CSF_VTBL(0x00832EEC);
        CSF_AT(uint32_t, allocated, 0x0C) = 0;
        CSF_AT(uint32_t, allocated, 0x10) = 0;
        CSF_AT(uint32_t, allocated, 0x14) = 0;
        CSF_AT(uint32_t, allocated, 0x1C) = 0;
        CSF_AT(uint32_t, allocated, 0x20) = 0;
        CSF_AT(uint32_t, allocated, 0x24) = 0;
        CSF_AT(uint32_t, allocated, 0x28) = 0;
        created = allocated;
    }

    CSF_AT(uint32_t, created, 0x28) = self->field_0028;

    uint8_t* sourceSub1 = reinterpret_cast<uint8_t*>(self) + 0x08;
    sub_47EC10(created + 0x08, nullptr,
               static_cast<int>(reinterpret_cast<std::uintptr_t>(sourceSub1)));

    uint8_t* sourceSub2 = reinterpret_cast<uint8_t*>(self) + 0x18;
    sub_47EA40(created + 0x18, nullptr,
               static_cast<int>(reinterpret_cast<std::uintptr_t>(sourceSub2)));

    return reinterpret_cast<int*>(created);
}

HOOK(0x0047E9E0, C_00832eec_CreateTaskCopy, "auto");
