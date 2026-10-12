#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);

CSF_FUNC(void, __fastcall, sub_738EA0, 0x00738EA0, (void* self, void* edx, float a2, int a3));

// Packs four source bytes into one dword per 28-byte table slot, starting at 0x8BBF48
// and stopping at 0x8BBF9C (three slots). The alpha byte is halved when the object's
// flag at +0x44 is set. Afterwards, sub_738EA0 is called when a4 is in [0, 1].
static void __fastcall C_008673b0_StorePackedColorTable(C_008673b0* self, void* /*edx*/, float a2, uint8_t* a3, uint32_t a4)
{
    uint8_t* slot = reinterpret_cast<uint8_t*>(CSF_ADDR_DATA(0x008BBF48));
    uint8_t* end = reinterpret_cast<uint8_t*>(CSF_ADDR_DATA(0x008BBF9C));

    do
    {
        uint8_t alpha;
        if (self->field_0044)
            alpha = static_cast<uint8_t>(static_cast<int>(static_cast<float>(a3[3]) * g_HalfDurationScale));
        else
            alpha = a3[3];

        uint32_t packed = (static_cast<uint32_t>(alpha) << 24)
                        | (static_cast<uint32_t>(a3[0]) << 16)
                        | (static_cast<uint32_t>(a3[1]) << 8)
                        | static_cast<uint32_t>(a3[2]);

        *reinterpret_cast<uint32_t*>(slot) = packed;
        slot += 28;
    }
    while (slot < end);

    if (static_cast<int>(a4) >= 0 && static_cast<int>(a4) <= 1)
        sub_738EA0(self, nullptr, a2, static_cast<int>(a4));
}

HOOK(0x00739170, C_008673b0_StorePackedColorTable, "auto");
