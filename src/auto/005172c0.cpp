#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(int, __fastcall, C_00840af8_FindNamedEntry, 0x00515FC0, (void* self, void* edx, unsigned int* a2));
CSF_FUNC(unsigned int, __fastcall, ProfilesSavegames_InitializeEntry, 0x005166E0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, ProfilesSavegames_AppendEntry, 0x00517230, (void* self, void* edx, int a3));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());

static char __fastcall C_00840af8_AddNamedEntry(
    C_00840af8* self, void* /*edx*/, unsigned int* name)
{
    if (C_00840af8_FindNamedEntry(self, nullptr, name) != -1)
        return 0;

    void* provider = CSF_AT(
        void*, reinterpret_cast<void*>(GetGlobalValue_8B056C()), 0x4);

    // One 80-byte save record, not two independent stack objects. The original
    // passes its start to ResetSaveData/AppendEntry and start+0x1C to the
    // provider. ResetSaveData accesses the vector at +0x38 and flags at +0x4C.
    alignas(8) std::uint8_t entryBytes[0x50]{};
    auto* detailsBytes = entryBytes + 0x1C;

    CSF_AT(std::uint32_t, entryBytes, 0x18) = 7;
    CSF_AT(int, entryBytes, 0x14) = 0;
    CSF_AT(std::uint16_t, entryBytes, 0x4) = 0;

    std::memset(detailsBytes, 255, 20);
    CSF_AT(void*, detailsBytes, 0x14) = nullptr;
    CSF_AT(void*, detailsBytes, 0x1C) = nullptr;
    CSF_AT(int, detailsBytes, 0x20) = 0;
    CSF_AT(int, detailsBytes, 0x24) = 0;

    ProfilesSavegames_InitializeEntry(entryBytes, nullptr);
    AssignStringRange_488830(entryBytes, nullptr, name, 0, 0xFFFFFFFF);

    void* settings = reinterpret_cast<void*>(GetGlobalValue_75FDF0());
    if (CSF_AT(std::uint8_t, settings, 0xCC))
        CSF_AT(unsigned int, detailsBytes, 0x2C) = self->vSlot88();
    else
        CSF_AT(unsigned int, detailsBytes, 0x2C) = 0;

    ((void(__fastcall*)(void*, void*, void*))
        CSF_AT(void**, provider, 0)[0xC / 4])(
            provider, nullptr, detailsBytes);

    ProfilesSavegames_AppendEntry(
        reinterpret_cast<std::uint8_t*>(self) + 4,
        nullptr,
        reinterpret_cast<int>(entryBytes));

    if (CSF_AT(void*, detailsBytes, 0x1C))
        Mem_FreeDebug(CSF_AT(void*, detailsBytes, 0x1C));

    CSF_AT(void*, detailsBytes, 0x1C) = nullptr;
    CSF_AT(int, detailsBytes, 0x20) = 0;
    CSF_AT(int, detailsBytes, 0x24) = 0;

    if (CSF_AT(std::uint32_t, entryBytes, 0x18) >= 8)
    {
        if (CSF_AT(void*, entryBytes, 0x4))
            Mem_FreeDebug(CSF_AT(void*, entryBytes, 0x4));
    }

    return 1;
}

HOOK(0x005172C0, C_00840af8_AddNamedEntry, "auto");
