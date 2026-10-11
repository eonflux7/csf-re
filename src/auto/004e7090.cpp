#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, PairLink_PrepareForRebind, 0x004E55A0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, DecrementByteCounter, 0x004E55B0, (void* self, void*));
CSF_FUNC(int, __fastcall, PairLink_FindEntryIndex, 0x004E58E0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, PairLink_TestEndpoints, 0x004E5B60, (void* self, void* edx, int a2));

static char __fastcall C_0083e9d0_AnyLinkActive(C_0083e9d0* self, void* /*edx*/, int listIndex)
{
    char result = 0;
    uint8_t* base = reinterpret_cast<uint8_t*>(self);

    PairLink_PrepareForRebind(self, nullptr);

    uint8_t* list = nullptr;
    switch (listIndex)
    {
    case 0: list = base + 128; break;
    case 1: list = base + 112; break;
    case 2: list = base + 96; break;
    case 3: list = base + 80; break;
    default: break;
    }

    if (list)
    {
        unsigned int count = CSF_AT(uint32_t, list, 8);
        bool found = false;

        for (unsigned int i = 0; i < count; ++i)
        {
            int item = CSF_AT(int, reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, list, 0)), 4 * i);
            int idx = PairLink_FindEntryIndex(self, nullptr, item + 20);
            if (idx != -1)
            {
                uint8_t* table = reinterpret_cast<uint8_t*>(self->field_0020);
                int state = CSF_AT(int, table, 20 * idx);
                if (state != -1 && state == 1)
                {
                    found = true;
                    break;
                }
            }
        }

        if (!found)
        {
            for (unsigned int i = 0; i < count; ++i)
            {
                int item = CSF_AT(int, reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, list, 0)), 4 * i);
                if (PairLink_TestEndpoints(self, nullptr, item + 12))
                {
                    found = true;
                    break;
                }
            }
        }

        if (found)
            result = 1;
    }

    DecrementByteCounter(self, nullptr);
    return result;
}

HOOK(0x004E7090, C_0083e9d0_AnyLinkActive, "auto");
