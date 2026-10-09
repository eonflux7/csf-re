#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, MissionScript_AllocateScratchObject, 0x00471450, (uint32_t a1));
CSF_FUNC(int, __cdecl, MissionScript_FreeScratchObject, 0x004714E0, (void* Block));
CSF_FUNC(uint8_t*, __fastcall, ReserveScratchEntries, 0x004725C0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_768E70, 0x00768E70, ());
CSF_FUNC(int, __fastcall, MissionScript_FindDefinition, 0x00769360, (void* self, void* edx, unsigned int a2));

static uint32_t* __fastcall MissionScript_ResetScratchEntries(
    void* self, void* /*edx*/, int definitionId)
{
    void* previous = CSF_AT(void*, self, 0x4);
    if (previous)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t)>(
            CSF_AT(void**, previous, 0x0)[0])(previous, nullptr, 0);
        MissionScript_FreeScratchObject(previous);
    }

    CSF_AT(void*, self, 0x4) = nullptr;
    CSF_AT(uint16_t, self, 0x0) = static_cast<uint16_t>(definitionId);

    if (static_cast<uint16_t>(definitionId))
    {
        CSF_AT(uint16_t, self, 0x2) = 0;
        void* definitions = reinterpret_cast<void*>(GetGlobalValue_768E70());
        void* definition = reinterpret_cast<void*>(
            MissionScript_FindDefinition(
                definitions, nullptr, static_cast<unsigned int>(definitionId)));

        auto* scratch = reinterpret_cast<C_008313f8*>(
            MissionScript_AllocateScratchObject(12));
        if (scratch)
        {
            scratch->vtbl = CSF_VTBL(0x008313F8);
            scratch->field_0004 = 0;
            scratch->field_0008 = -1;
        }
        CSF_AT(void*, self, 0x4) = scratch;

        return reinterpret_cast<uint32_t*>(
            ReserveScratchEntries(
                scratch, nullptr, CSF_AT(int, definition, 0x8)));
    }

    auto* scratch = reinterpret_cast<C_008313f8*>(
        MissionScript_AllocateScratchObject(12));
    if (scratch)
    {
        scratch->field_0004 = 0;
        scratch->vtbl = CSF_VTBL(0x008313F8);
        scratch->field_0008 = -1;
    }
    CSF_AT(void*, self, 0x4) = scratch;
    return reinterpret_cast<uint32_t*>(scratch);
}

HOOK(0x0046F2B0, MissionScript_ResetScratchEntries, "auto");
