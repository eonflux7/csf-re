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

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int*, __fastcall, S_0047ea40_AssignPointerStorage, 0x0047EA40, (S_0047ea40* self, void*, S_0047ea40* source));
CSF_FUNC(int*, __fastcall, InitSubObjectFromSource, 0x00732230, (void* self, void* edx, int a5));

// Creates a 44-byte copy object (vtable 0x86709C) seeded from this object's
// field_0028, and initializes its two embedded sub-objects from this object's
// sub-object regions at +0x08 and +0x18.
static int* __fastcall C_0086709c_CreateCopy(C_0086709c* self, void* /*edx*/)
{
    // Vtable of S_0047ea40, referenced by the original's call to AssignPointerStorage.
    // Code-address annotation: function value, not a vtable.
    [[maybe_unused]] void* assignStorageVtbl = reinterpret_cast<void**>(CSF_ADDR_FN(0x0047EA40));

    uint8_t* created = nullptr;
    int rawAllocated = CoreMemory_AllocUntagged(44);
    uint8_t* allocated = reinterpret_cast<uint8_t*>(static_cast<std::uintptr_t>(rawAllocated));
    if (allocated) {
        CSF_AT(uint32_t, allocated, 0x04) = 1;
        CSF_AT(void*, allocated, 0x00) = CSF_VTBL(0x0086709C);
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

    InitSubObjectFromSource(created + 0x08, nullptr,
                            static_cast<int>(reinterpret_cast<std::uintptr_t>(reinterpret_cast<uint8_t*>(self) + 0x08)));

    S_0047ea40_AssignPointerStorage(reinterpret_cast<S_0047ea40*>(created + 0x18), nullptr,
                                    reinterpret_cast<S_0047ea40*>(reinterpret_cast<uint8_t*>(self) + 0x18));

    return reinterpret_cast<int*>(created);
}

HOOK(0x007321D0, C_0086709c_CreateCopy, "auto");
