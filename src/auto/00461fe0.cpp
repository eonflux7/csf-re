#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(char, __fastcall, GlobalObjectAllowsRequest, 0x00521C00, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GlobalObjectSubmitRequest, 0x005242B0, (void* self, void* edx, void* a2, int a3));

// Reinterprets a 32-bit word read from game memory as a pointer.
static inline void* PointerFromWord(int word) {
    return reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(word)));
}

// __thiscall(this in ecx, 3 stack args) -> callee pops 12 bytes. Returns char (AL).
static char __fastcall TryStartGlobalObjectRequest(void* self, void* /*edx*/,
                                                   int /*unusedArg58*/, int /*unusedArg5C*/,
                                                   int* a4) {
    // Request block passed to GlobalObjectSubmitRequest. Written at the same offsets the
    // original uses in its frame; 0x37 bytes, last written byte at +0x36.
    uint8_t requestBlock[0x37];

    CSF_AT(int, requestBlock, 0x00) = 2;
    CSF_AT(int, requestBlock, 0x04) = 1;
    CSF_AT(uint8_t, requestBlock, 0x35) = 1;
    CSF_AT(uint8_t, requestBlock, 0x36) = 1;
    CSF_AT(int, requestBlock, 0x18) = 0;
    CSF_AT(int, requestBlock, 0x1C) = 0;
    CSF_AT(int, requestBlock, 0x20) = 0;
    CSF_AT(void*, requestBlock, 0x24) = CSF_VTBL(0x0082D448);
    CSF_AT(int, requestBlock, 0x28) = -1;
    CSF_AT(int, requestBlock, 0x2C) = -1;
    CSF_AT(int, requestBlock, 0x10) = 0;
    CSF_AT(int, requestBlock, 0x14) = 0;
    CSF_AT(int, requestBlock, 0x30) = 0;
    CSF_AT(uint8_t, requestBlock, 0x34) = 0;
    CSF_AT(int, requestBlock, 0x08) = -1;
    CSF_AT(int, requestBlock, 0x0C) = -1;

    int commandHeader = CSF_AT(int, PointerFromWord(a4[0]), 4);
    int targetId = CSF_AT(int, PointerFromWord(commandHeader), 4);
    int resultWord = targetId;

    if (targetId != -1) {
        CSF_AT(int, requestBlock, 0x08) = targetId;

        int secondHeader = CSF_AT(int, PointerFromWord(a4[1]), 4);
        CSF_AT(int, requestBlock, 0x28) = CSF_AT(int, PointerFromWord(secondHeader), 8);
        CSF_AT(int, requestBlock, 0x2C) = CSF_AT(int, PointerFromWord(secondHeader), 12);

        int thirdHeader = CSF_AT(int, PointerFromWord(a4[2]), 4);
        CSF_AT(int, requestBlock, 0x14) = CSF_AT(int, PointerFromWord(thirdHeader), 4);
        CSF_AT(int, requestBlock, 0x10) = 0;
        CSF_AT(int, requestBlock, 0x04) = 4;
        CSF_AT(uint8_t, requestBlock, 0x36) = 0;

        void* globalObject = PointerFromWord(GetGlobalValue_521BB0());
        uint8_t requestAccepted = static_cast<uint8_t>(GlobalObjectAllowsRequest(globalObject, nullptr));
        resultWord = (resultWord & ~0xFF) | requestAccepted;

        if (requestAccepted != 0) {
            int a3[4];

            // a3[3] is loaded and stored as a float (fld/fstp dword in the original).
            float payloadValue = CSF_AT(float, PointerFromWord(CSF_AT(int, PointerFromWord(a4[3]), 4)), 4);
            CSF_AT(float, a3, 12) = payloadValue;

            a3[1] = CSF_AT(int, requestBlock, 0x28);
            CSF_AT(void*, a3, 0) = CSF_VTBL(0x0082D448);
            a3[2] = CSF_AT(int, requestBlock, 0x2C);

            void* submitObject = PointerFromWord(GetGlobalValue_521BB0());
            int submitResult = GlobalObjectSubmitRequest(
                submitObject,
                nullptr,
                requestBlock,
                static_cast<int>(reinterpret_cast<uintptr_t>(a3)));

            resultWord = submitResult;
            CSF_AT(int, self, 0x7C) = submitResult;
            if (submitResult != 0) {
                CSF_AT(int, self, 0x18) = 8;
                CSF_AT(uint8_t, self, 0x82) = 0;
            }
        }
    }

    return static_cast<char>(resultWord);
}

HOOK(0x00461FE0, TryStartGlobalObjectRequest, "auto");
