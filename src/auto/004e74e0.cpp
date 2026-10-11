#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, AiCore_CopyCandidate, 0x004B8FD0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, PairLink_PrepareForRebind, 0x004E55A0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, DecrementByteCounter, 0x004E55B0, (void* self, void*));
CSF_FUNC(int, __fastcall, PairLink_FindEntryIndex, 0x004E58E0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, PairLink_TestEndpoints, 0x004E5B60, (void* self, void* edx, int a2));

static char __fastcall C_0083e9d0_FindPairLink(C_0083e9d0* self, void* /*edx*/, int kind, int target, uint32_t* out)
{
    char result = 0;
    uint8_t* list = nullptr;

    PairLink_PrepareForRebind(self, nullptr);

    switch (kind) {
    case 0:
        list = (uint8_t*)self + 128;
        break;
    case 1:
        list = (uint8_t*)self + 112;
        break;
    case 2:
        list = (uint8_t*)self + 96;
        break;
    case 3:
        list = (uint8_t*)self + 80;
        break;
    default:
        goto done;
    }

    {
        uint32_t count = CSF_AT(uint32_t, list, 8);
        bool found = false;
        uint32_t i = 0;

        if (count == 0)
            goto done;

        do {
            uint32_t** arr = CSF_AT(uint32_t**, list, 0);
            uint32_t* entry = arr[i];
            if ((int)entry[2] == target) {
                found = true;
                int idx = PairLink_FindEntryIndex(self, nullptr, (int)(uintptr_t)(entry + 5));
                if (idx != -1) {
                    uint32_t* rec = reinterpret_cast<uint32_t*>((uint8_t*)(uintptr_t)self->field_0020 + 20 * idx);
                    if ((int)rec[1] == target && rec[0] != (uint32_t)-1 && rec[0] == 1) {
                        AiCore_CopyCandidate(out, nullptr, entry);
                        result = 1;
                        goto done;
                    }
                }
            }
            ++i;
        } while (i < count);

        if (!found)
            goto done;

        for (uint32_t j = 0; j < count; ++j) {
            uint32_t** arr = CSF_AT(uint32_t**, list, 0);
            uint32_t* entry = arr[j];
            if ((int)entry[2] == target) {
                if (PairLink_TestEndpoints(self, nullptr, (int)(uintptr_t)(entry + 3))) {
                    out[1] = entry[1];
                    out[2] = entry[2];
                    out[3] = entry[3];
                    out[4] = entry[4];
                    out[6] = entry[6];
                    out[7] = entry[7];
                    out[8] = entry[8];
                    out[9] = entry[9];
                    out[10] = entry[10];
                    out[11] = entry[11];
                    out[12] = entry[12];
                    result = 1;
                    goto done;
                }
            }
        }
    }

done:
    DecrementByteCounter(self, nullptr);
    return result;
}

HOOK(0x004E74E0, C_0083e9d0_FindPairLink, "auto");
