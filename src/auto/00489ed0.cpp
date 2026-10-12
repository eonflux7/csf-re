#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));
CSF_FUNC(int, __fastcall, GetControlEntryByIndex, 0x0049ED10, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_008342e8_GetControlFlagsAddress, 0x0049ED30, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall C_00833708_ResolveEligibleControl(
    C_00833708* self, void* /*edx*/, int index)
{
    auto* controls = reinterpret_cast<C_008342e8*>(self->field_0008);
    const uint32_t entryOffset = static_cast<uint32_t>(index) * 96;
    auto* entries = reinterpret_cast<uint8_t*>(self->field_000c);
    auto* object = CSF_AT(C_008319c0*, entries + entryOffset, 0);
    const int type = C_008319c0_GetSubObjectField74(object, nullptr);
    if (type != 13 && type != 20)
        return 0;

    const int entry = GetControlEntryByIndex(controls, nullptr, 0);
    alignas(8) uint8_t handleBytes[0xC];
    CSF_AT(void**, handleBytes, 0) = CSF_VTBL(0x0082D448);
    CSF_AT(uint32_t, handleBytes, 4) = CSF_AT(uint32_t, reinterpret_cast<void*>(entry), 4);
    CSF_AT(int, handleBytes, 8) = CSF_AT(int, reinterpret_cast<void*>(entry), 8);
    if (CSF_AT(int, handleBytes, 8) < 0)
        return 0;

    const int flagsAddress = C_008342e8_GetControlFlagsAddress(controls, nullptr, 0);
    if ((CSF_AT(uint8_t, reinterpret_cast<void*>(flagsAddress), 0) & 2) != 0)
        return 0;

    return CHandleTable_Resolve(
        g_pCurrentScene, nullptr, reinterpret_cast<int>(handleBytes));
}

HOOK(0x00489ED0, C_00833708_ResolveEligibleControl, "auto");
