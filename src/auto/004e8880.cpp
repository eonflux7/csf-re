#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* edx, int additionalCapacity));
CSF_FUNC(int, __fastcall, C_0044d9a0_InvokeMemberVirtual, 0x0044D9A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t*, __fastcall, AiCore_CopyCandidate, 0x004B8FD0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(char, __fastcall, Squad_CheckEntry, 0x004E60B0, (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __fastcall, Squad_RegisterEntry, 0x004E6750, (void* self, void* edx, uint32_t* a2, int a3));
CSF_FUNC(int, __fastcall, Squad_HandleSpecialEntry, 0x004E6990, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, Squad_ProcessResolvedEntry, 0x004E87D0, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void __fastcall C_0083eb70_AddEntry(C_0083eb70* self, void* /*edx*/, uint32_t* entry)
{
    if (entry[2] == 1) {
        int obj = CHandleTable_Resolve(g_pCurrentScene, nullptr, (int)(entry + 5));
        if (obj != 0 && CSF_AT(int, (void*)(uintptr_t)obj, 0x50) == 20 &&
            Squad_CheckEntry(self, nullptr, obj + 392)) {
            Squad_ProcessResolvedEntry(self, nullptr, obj + 28);
            return;
        }
    }

    Squad_RegisterEntry(self, nullptr, entry + 3, (int)entry[2]);

    uintptr_t inner = (uintptr_t)CSF_AT(uint32_t, g_pCurrentScene, 0xB0);
    uintptr_t target = (uintptr_t)CSF_AT(uint32_t, (void*)inner, 0x114);
    int handle = C_0044d9a0_InvokeMemberVirtual((void*)target, nullptr);
    AiCore_CopyCandidate((void*)(uintptr_t)handle, nullptr, entry);

    S_0041e5f0* list;
    switch (entry[1]) {
    case 0:
        list = reinterpret_cast<S_0041e5f0*>(&self->field_0080);
        break;
    case 1:
        list = reinterpret_cast<S_0041e5f0*>(&self->field_0070);
        break;
    case 2:
        list = reinterpret_cast<S_0041e5f0*>(&self->field_0060);
        break;
    case 3:
        if (entry[2] == 12 || entry[2] == 14)
            Squad_HandleSpecialEntry(self, nullptr, (int)(uintptr_t)entry);
        list = reinterpret_cast<S_0041e5f0*>(&self->field_0050);
        break;
    default:
        return;
    }

    if (CSF_AT(uint32_t, list, 8) >= CSF_AT(uint32_t, list, 4))
        S_0041e5f0_GrowArrayCapacity(list, nullptr, (int)CSF_AT(uint32_t, list, 0xC));
    uint32_t count = CSF_AT(uint32_t, list, 8);
    uint32_t* slot = reinterpret_cast<uint32_t*>(CSF_AT(uint32_t, list, 0) + 4 * count);
    CSF_AT(uint32_t, list, 8) = count + 1;
    *slot = (uint32_t)handle;
}

HOOK(0x004E8880, C_0083eb70_AddEntry, "auto");
