#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectSectorData, 0x006B87C0, (int objectIndex));
CSF_FUNC(void**, __fastcall, RpWorld_ResolveSectorSlot, 0x0069A8A0, (void* self, void* edx, void** a2, uint32_t* a3));

// Resolves the sector reference for an object: clears the 4-byte output to 0xFF, looks up the
// object's sector-data entry from the table at self+0x10, and on success writes a 32-bit value
// into outBytes and returns the (possibly updated) object pointer. Returns nullptr when no entry exists.
static int* __fastcall GfxWorld_ResolveObjectSectorRef(void* self, void* /*edx*/, int* objectRef, uint8_t* outBytes)
{
    std::memset(outBytes, 0xFF, 4);

    uint8_t slotIndex = CSF_AT(uint8_t, objectRef, 6);
    uint32_t tableOwner = CSF_AT(uint32_t, self, 0x10);
    uint32_t sectorTable = CSF_AT(uint32_t, reinterpret_cast<void*>(tableOwner), 0x10);
    uint32_t entry = CSF_AT(uint32_t, reinterpret_cast<void*>(sectorTable), static_cast<uint32_t>(slotIndex) * 4);

    if (entry == 0)
        return nullptr;

    int* sector = reinterpret_cast<int*>(
        static_cast<intptr_t>(GfxObjectProps_GetObjectSectorData(static_cast<int>(entry))));
    if (sector == nullptr)
        return nullptr;

    uint32_t sectorValue = static_cast<uint32_t>(sector[2]);

    int* cursor = objectRef;
    RpWorld_ResolveSectorSlot(static_cast<uint8_t*>(self) + 4, nullptr,
                              reinterpret_cast<void**>(&cursor),
                              reinterpret_cast<uint32_t*>(&sectorValue));

    if (cursor != CSF_AT(int*, self, 8))
    {
        uint32_t cursorValue = CSF_AT(uint32_t, cursor, 0x10);
        CSF_AT(uint32_t, outBytes, 0) = cursorValue;
    }

    return cursor;
}

HOOK(0x006B7120, GfxWorld_ResolveObjectSectorRef, "auto");
