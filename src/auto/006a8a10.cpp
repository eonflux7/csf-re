#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __stdcall, NetGatheringDdl_ReleaseParticipant, 0x006A7A40, (void* a1));
CSF_FUNC(void, __fastcall, NetGatheringDdl_ResetData, 0x006A8940, (void* self, void* edx));

[[maybe_unused]] static const auto kResetDataVtableReference = CSF_VTBL(0x006A8940);

static void __fastcall S_006a8940_HandleGatheringShutdown(
    S_006a8940* self, void* /*edx*/)
{
    if (!self->field_0080)
        return;

    const uint32_t mode = self->field_0010;
    self->field_0008 = 3;
    self->field_000c = 3;

    switch (mode)
    {
    case 2u:
    {
        NetGatheringDdl_ReleaseParticipant(
            reinterpret_cast<void*>(self->field_0090));

        auto* participant =
            reinterpret_cast<C_0085e760*>(self->field_0090);
        self->field_0014 = 1;
        if (participant)
        {
            participant->vSlot0(1);
            self->field_0090 = 0;
        }
        [[fallthrough]];
    }
    case 0u:
    case 3u:
    case 4u:
        NetGatheringDdl_ResetData(self, nullptr);
        return;

    case 1u:
    {
        NetGatheringDdl_ReleaseParticipant(
            reinterpret_cast<void*>(self->field_0090));

        auto* participant =
            reinterpret_cast<C_0085e760*>(self->field_0090);
        if (participant)
        {
            participant->vSlot0(1);
            self->field_0090 = 0;
        }
        self->field_0008 = 2;
        self->field_000c = 0;
        self->field_0334 = 0;
        break;
    }
    }
}

HOOK(0x006A8A10, S_006a8940_HandleGatheringShutdown, "auto");
