#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpClumpStreamRead, 0x0058FFC0, (int a1));
CSF_FUNC(int, __cdecl, RwStreamClose, 0x00629810, (uint32_t* a1, uint32_t* a2));
CSF_FUNC(uint32_t*, __cdecl, RwStreamOpen, 0x006298E0, (int a3, int a4, int a5));
CSF_FUNC(char, __fastcall, C_008605d8_ProcessLoadedClump, 0x006BE640, (void* self, void* edx, int a1));

static char __fastcall C_008605d8_ProcessContextObject(
    C_008605d8* self, void* /*edx*/)
{
    if (self->field_00c0)
    {
        alignas(8) uint8_t streamDataBytes[0xC] = {};
        auto* streamData = reinterpret_cast<uint32_t*>(streamDataBytes);
        auto* entry = reinterpret_cast<uint8_t*>(self) + 0xC4;

        for (int remaining = 15; remaining != 0; --remaining, entry += 12)
        {
            if (!CSF_AT(uint8_t, entry, 0x8))
                continue;

            streamData[1] = CSF_AT(uint32_t, entry, 0x4);
            streamData[0] = CSF_AT(uint32_t, entry, 0x0);

            uint32_t* stream = RwStreamOpen(
                3, 1, reinterpret_cast<int>(streamData));
            int clump = RpClumpStreamRead(reinterpret_cast<int>(stream));
            if (clump)
                C_008605d8_ProcessLoadedClump(self, nullptr, clump);

            RwStreamClose(stream, streamData);

            void* context = reinterpret_cast<void*>(self->field_0178);
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, context, 0)[0x4 / 4])(context, nullptr, 0);

            self->field_00bc -= CSF_AT(uint32_t, entry, 0x4);
            CSF_AT(uint8_t, entry, 0x8) = 0;
            CSF_AT(uint32_t, entry, 0x0) = 0;
            CSF_AT(uint32_t, entry, 0x4) = 0;

            uint32_t pendingCount = self->field_00c0;
            context = reinterpret_cast<void*>(self->field_0178);
            self->field_00c0 = pendingCount - 1;

            ((void(__fastcall*)(void*, void*))
                CSF_AT(void**, context, 0)[0x8 / 4])(context, nullptr);
        }
    }

    return 1;
}

HOOK(0x006BE9C0, C_008605d8_ProcessContextObject, "auto");
