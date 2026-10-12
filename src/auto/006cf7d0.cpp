#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the cached 0x48-byte shape block stored inline at this+0x18 (plus a
// 2-dword pair at this+0x60). Refreshes the cache when it is empty or when
// forceRefresh is nonzero.
static int __fastcall C_00860cbc_GetCachedShapeData(C_00860cbc* self, void* /*edx*/, char forceRefresh)
{
    if (!self->field_0068 || forceRefresh)
    {
        alignas(8) uint8_t blockBytes[0x48];
        alignas(8) uint8_t pairBytes[8];

        // Slot 0x2C / 4 = 11 on the field_0004 object: returns pointer to 0x48-byte block.
        void* blockObj = reinterpret_cast<void*>(self->field_0004);
        const void* block =
            ((const void* (__fastcall*)(void*, void*, uint8_t*))CSF_AT(void**, blockObj, 0)[11])
                (blockObj, nullptr, blockBytes);
        memcpy(self->pad_0018, block, 0x48);

        // Slot 0x20 / 4 = 8 on the field_0008 object: returns pointer to a dword pair.
        void* pairObj = reinterpret_cast<void*>(self->field_0008);
        const uint32_t* pair =
            ((const uint32_t* (__fastcall*)(void*, void*, uint8_t*))CSF_AT(void**, pairObj, 0)[8])
                (pairObj, nullptr, pairBytes);
        CSF_AT(uint32_t, self, 0x60) = pair[0];
        CSF_AT(uint32_t, self, 0x64) = pair[1];

        self->field_0068 = 1;
    }
    return static_cast<int>(reinterpret_cast<uintptr_t>(self->pad_0018));
}

HOOK(0x6CF7D0, C_00860cbc_GetCachedShapeData, "auto");
