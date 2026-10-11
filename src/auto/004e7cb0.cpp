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
CSF_FUNC(char, __fastcall, PairLink_TestEndpoints, 0x004E5B60, (void* self, void* edx, int a2));

static int __fastcall C_0083e9d0_FindPairCandidate(C_0083e9d0* self, void* /*edx*/, int arg0, int* query, uint32_t* out)
{
    if (query[2] < 0)
        return 0;

    PairLink_PrepareForRebind(self, nullptr);

    out[1] = 0xFFFFFFFF;
    out[2] = 0xFFFFFFFF;
    CSF_AT(uint32_t, out, 0x2C) = 0x7F7FFFFF;
    CSF_AT(uint32_t, out, 0x30) = 0;

    C_0083e9d0* cur = self;

    if (static_cast<uint32_t>(arg0) <= 3) {
        uint8_t* list = nullptr;
        switch (arg0) {
        case 0: list = reinterpret_cast<uint8_t*>(self) + 0x80; break;
        case 1: list = reinterpret_cast<uint8_t*>(self) + 0x70; break;
        case 2: list = reinterpret_cast<uint8_t*>(self) + 0x60; break;
        case 3: list = reinterpret_cast<uint8_t*>(self) + 0x50; break;
        }
        if (list) {
            alignas(8) uint8_t queryBytes[0x18];
            CSF_AT(int, queryBytes, 0) = -1;
            CSF_AT(int, queryBytes, 4) = -1;
            int key = query[1];
            int val = query[2];
            uint32_t count = CSF_AT(uint32_t, list, 8);
            CSF_AT(void*, queryBytes, 8) = CSF_VTBL(0x0082D448);
            CSF_AT(int, queryBytes, 0xC) = key;
            CSF_AT(int, queryBytes, 0x10) = val;
            CSF_AT(C_0083e9d0*, queryBytes, 0x14) = self;

            for (uint32_t i = 0; i < count; ++i) {
                uint8_t* elem = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t*, list, 0)[i]);
                if (CSF_AT(int, elem, 0x18) == CSF_AT(int, queryBytes, 0xC)) {
                    C_0083e9d0* thisObj = CSF_AT(C_0083e9d0*, queryBytes, 0x14);
                    if (PairLink_TestEndpoints(thisObj, nullptr, static_cast<int>(reinterpret_cast<intptr_t>(queryBytes)))) {
                        AiCore_CopyCandidate(out, nullptr, reinterpret_cast<uint32_t*>(elem));
                        break;
                    }
                }
            }
            cur = CSF_AT(C_0083e9d0*, queryBytes, 0x14);
        }
    }

    DecrementByteCounter(cur, nullptr);
    return out[1] != 0xFFFFFFFF ? 1 : 0;
}

HOOK(0x004E7CB0, C_0083e9d0_FindPairCandidate, "auto");
