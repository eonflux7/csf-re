#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, sub_688470, 0x00688470, (void* self, void* edx));
CSF_FUNC(void, __cdecl, NetSession_ReleaseTransportStorage, 0x006471C0, (int a2));

static void __fastcall S_0078cea0_ReleaseTransportSlots(S_0078cea0* self, void* /*edx*/)
{
    constexpr int kSlotCount = 2;
    constexpr int kFirstSlotOffset = 0x4;
    int remaining = kSlotCount;
    int slotOffset = kFirstSlotOffset;
    do
    {
        C_008310b8* entry = CSF_AT(C_008310b8*, self, slotOffset);
        if (entry)
        {
            sub_688470(entry, nullptr);
            NetSession_ReleaseTransportStorage(reinterpret_cast<int>(entry));
        }
        CSF_AT(C_008310b8*, self, slotOffset) = nullptr;
        slotOffset += 4;
        --remaining;
    }
    while (remaining != 0);

    CSF_AT(uint8_t, self, 0x10) = 0;
}

HOOK(0x0078CFD0, S_0078cea0_ReleaseTransportSlots, "auto");
