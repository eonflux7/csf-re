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
CSF_FUNC(int, __fastcall, C_00860a30_InitBase, 0x006CA660, (C_00860a30* self, void*));
CSF_FUNC(uint32_t*, __fastcall, C_00862598_SetChildObject, 0x006CA030, (C_00862598* self, void*, uint32_t* a2));

// Allocates a 64-byte C_00860a30 child for this object, initializes it with the
// C_008622e8 vtable set, copies this object's field_0020 into it, and links it
// through C_00862598_SetChildObject. On allocation failure, links nullptr instead.
static uint32_t* __fastcall C_008622e8_CreateAndLinkChild(C_008622e8* self, void* /*edx*/)
{
    int rawAllocation = CoreMemory_AllocUntagged(64);
    C_00860a30* childObject = reinterpret_cast<C_00860a30*>(rawAllocation);

    if (childObject != nullptr) {
        C_00860a30_InitBase(childObject, nullptr);

        CSF_AT(void*, childObject, 0)  = CSF_VTBL(0x008622E8);
        CSF_AT(void*, childObject, 4)  = CSF_VTBL(0x008622E0);
        CSF_AT(void*, childObject, 8)  = CSF_VTBL(0x008622C4);

        CSF_AT(uint32_t, childObject, 0x28) = 0;
        CSF_AT(uint32_t, childObject, 0x2C) = 0;
        CSF_AT(uint32_t, childObject, 0x30) = 0;
        CSF_AT(int32_t,  childObject, 0x38) = -1;
        CSF_AT(uint32_t, childObject, 0x3C) = 0;
        CSF_AT(uint32_t, childObject, 0x20) = 0;
        CSF_AT(uint32_t, childObject, 0x24) = 0;
        CSF_AT(float,    childObject, 0x34) = 1.0f;

        CSF_AT(uint32_t, childObject, 0x20) = self->field_0020;

        C_00862598_SetChildObject(reinterpret_cast<C_00862598*>(self), nullptr,
                                  reinterpret_cast<uint32_t*>(childObject));
        return reinterpret_cast<uint32_t*>(childObject);
    }

    // Allocation failed: the original writes this->field_0020 to address 0x20
    // (esi == 0) and links a null child.
    CSF_AT(uint32_t, static_cast<uint8_t*>(nullptr), 0x20) = self->field_0020;
    C_00862598_SetChildObject(reinterpret_cast<C_00862598*>(self), nullptr, nullptr);
    return nullptr;
}

HOOK(0x006F0B30, C_008622e8_CreateAndLinkChild, "auto");
