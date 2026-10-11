#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, NetGatheringDdl_InitializeScratchObject, 0x00657590, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, ScratchObj_CopyFromSource, 0x0068E2E0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(char, __fastcall, S_006a7640_RefreshEntry, 0x006A78D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, S_006a7640_UpdateEntryFromSource, 0x006A7B50, (void* self, void* edx, int a2, int* a3));

static char __fastcall S_006a7640_ProcessPendingEntry(
    S_006a7640* self, void* /*edx*/, int /*unused*/)
{
    alignas(8) uint8_t scratchBytes[0xC];
    NetGatheringDdl_InitializeScratchObject(scratchBytes, nullptr);

    auto* entry = reinterpret_cast<C_0085e760*>(self->field_0090);
    const uint32_t state = CSF_AT(uint32_t, entry, 0xC);
    if (state == 1)
        return 0;

    if (state == 2)
    {
        auto* source = reinterpret_cast<uint32_t*>(
            reinterpret_cast<uint8_t*>(self) + 0xA0);
        ScratchObj_CopyFromSource(scratchBytes, nullptr, source);
        if (CSF_AT(int, self, 0xA0) < 0)
        {
            S_006a7640_UpdateEntryFromSource(
                self, nullptr, 1, reinterpret_cast<int*>(source));
        }
        entry = reinterpret_cast<C_0085e760*>(self->field_0090);
    }
    else if (state == 3)
    {
        auto* source = reinterpret_cast<uint32_t*>(
            reinterpret_cast<uint8_t*>(self) + 0xA0);
        ScratchObj_CopyFromSource(scratchBytes, nullptr, source);
        S_006a7640_UpdateEntryFromSource(
            self, nullptr, 1, reinterpret_cast<int*>(source));
        entry = reinterpret_cast<C_0085e760*>(self->field_0090);
    }

    if (entry)
    {
        entry->vSlot0(1);
        self->field_0090 = 0;
    }

    return S_006a7640_RefreshEntry(self, nullptr);
}

HOOK(0x006A8780, S_006a7640_ProcessPendingEntry, "auto");
