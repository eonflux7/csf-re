#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_0044bc20_FindSlotIndex, 0x0044BC20,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, S_0044bc20_FindHandleIndex, 0x0044BC70,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, InitializeAssociationRecord, 0x0044C3D0,
         (int a1, int a2));
CSF_FUNC(uint32_t*, __fastcall, AppendAssociationRecord, 0x0044CDB0,
         (void* self, void* edx, int a3));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30,
         (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60,
         (void* block));

CSF_GLOBAL(int, g_CurrentSceneIndex, 0x0089972C);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall S_0044bc20_AssignHandleToSlot(
    S_0044bc20* self, void* /*edx*/, int slot, int handle, int* coordinates)
{
    // The packet also identifies this call target as a vtable reference.
    (void)CSF_VTBL(0x0044BC20);

    if (slot >= 0)
    {
        const int index = S_0044bc20_FindSlotIndex(self, nullptr, slot);
        if (index != -1)
        {
            const uint32_t offset = 100 * static_cast<uint32_t>(index);
            CSF_AT(int, reinterpret_cast<uint8_t*>(self->field_0010) + offset,
                   0x38) = -1;
            CSF_AT(uint8_t, reinterpret_cast<uint8_t*>(self->field_0010) + offset,
                   0x28) = 0;
        }
    }

    const int index = S_0044bc20_FindHandleIndex(self, nullptr, handle);
    if (index != -1)
    {
        const uint32_t offset = 100 * static_cast<uint32_t>(index);
        CSF_AT(int, reinterpret_cast<uint8_t*>(self->field_0010) + offset,
               0x38) = slot;
        return static_cast<int>(offset);
    }

    alignas(8) uint8_t recordBytes[0x64];
    InitializeAssociationRecord(
        static_cast<int>(reinterpret_cast<uintptr_t>(recordBytes)), 0);

    const int x = coordinates[0];
    const int y = coordinates[1];
    CSF_AT(int, recordBytes, 0x00) = 2;
    CSF_AT(int, recordBytes, 0x5C) = x;
    CSF_AT(int, recordBytes, 0x60) = y;
    CSF_AT(int, recordBytes, 0x38) = slot;

    const int resolved = CHandleTable_Resolve(g_pCurrentScene, nullptr, handle);
    if (resolved)
    {
        void* handleData = reinterpret_cast<void*>(handle);
        const int handlePart1 = CSF_AT(int, handleData, 0x04);
        const int handlePart2 = CSF_AT(int, handleData, 0x08);
        CSF_AT(int, recordBytes, 0x14) = handlePart1;
        CSF_AT(int, recordBytes, 0x18) = handlePart2;
        CSF_AT(int, recordBytes, 0x1C) = g_CurrentSceneIndex;
        CSF_AT(int, recordBytes, 0x24) =
            CSF_AT(int, reinterpret_cast<void*>(resolved), 0x78);
        CSF_AT(int, recordBytes, 0x20) =
            CSF_AT(int, reinterpret_cast<void*>(resolved), 0x50);
    }

    int result = static_cast<int>(reinterpret_cast<uintptr_t>(
        AppendAssociationRecord(
            reinterpret_cast<uint8_t*>(self) + 0x0C, nullptr,
            static_cast<int>(reinterpret_cast<uintptr_t>(recordBytes)))));

    if (CSF_AT(uint32_t, recordBytes, 0x58) >= 0x10)
    {
        void* block = CSF_AT(void*, recordBytes, 0x44);
        result = static_cast<int>(reinterpret_cast<uintptr_t>(block));
        if (block)
            return Mem_FreeDebug(block);
    }

    return result;
}

HOOK(0x0044D190, S_0044bc20_AssignHandleToSlot, "auto");
