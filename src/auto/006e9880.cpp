#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, GfxMatrix_InitializeQuaternion, 0x006AE6D0, (uint32_t* self, void*));
CSF_FUNC(int, __fastcall, HanimResolveNodeRecord, 0x006E8A50, (void* self, void* edx, int a2));

// Returns the quaternion slot (record+0x24) of the node record for a2, allocating and
// initializing a 28-byte quaternion block on first use.
static int __fastcall HanimGetOrCreateQuaternion(C_00861e98* self, void* /*edx*/, int a2)
{
    uint8_t* record = reinterpret_cast<uint8_t*>(
        static_cast<std::uintptr_t>(static_cast<std::uint32_t>(HanimResolveNodeRecord(self, nullptr, a2))));

    if (CSF_AT(int, record, 0x24) == 0)
    {
        uint32_t* quaternion = static_cast<uint32_t*>(Mem_Alloc(28));
        if (quaternion)
        {
            GfxMatrix_InitializeQuaternion(quaternion, nullptr);
            quaternion[4] = 0;
            quaternion[5] = 0;
            quaternion[6] = 0;
            CSF_AT(void*, record, 0x24) = quaternion;
            return CSF_AT(int, record, 0x24);
        }
        CSF_AT(int, record, 0x24) = 0;
    }
    return CSF_AT(int, record, 0x24);
}

HOOK(0x006E9880, HanimGetOrCreateQuaternion, "auto");
