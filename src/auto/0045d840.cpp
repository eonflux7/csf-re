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

CSF_FUNC(int32_t, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(int, __fastcall, SinkTextAndValue, 0x0042DF60, (void* self, void* edx, int a1, float a2));

// Reads a text pointer and a float from the stdcall argument chain, then
// forwards them to the sink object. Returns the sink's result, or the
// (zero-extended) readiness flag / null pointer when the chain is not ready.
static uint32_t* __stdcall DispatchStringAndValueToSink(int a1, int a2, uint32_t* a3)
{
    [[maybe_unused]] int unusedA1 = a1;
    [[maybe_unused]] int unusedA2 = a2;

    uint8_t* globalBlock = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(GetGlobalValue_8B056C())));
    uint32_t* result = CSF_AT(uint32_t*, globalBlock, 0x18);
    if (result)
    {
        result = CSF_AT(uint32_t*, result, 0x10);
        if (result)
        {
            void* readyTarget = result;
            uint8_t ready = ((uint8_t(__fastcall*)(void*, void*))CSF_AT(void**, readyTarget, 0)[8])(readyTarget, nullptr);
            if (ready)
            {
                // Text pointer: small-buffer string stored inline when capacity < 0x10.
                uint8_t* nodeA = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a3[0]));
                uint8_t* stringHolder = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, nodeA, 4)));
                uint32_t capacity = CSF_AT(uint32_t, stringHolder, 0x1C);
                uintptr_t textAddress = (capacity < 0x10)
                    ? reinterpret_cast<uintptr_t>(stringHolder + 8)
                    : static_cast<uintptr_t>(CSF_AT(uint32_t, stringHolder, 8));

                // Float value: a3[1] -> +4 -> +4.
                uint8_t* nodeB = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, a3, 4)));
                uint8_t* nodeC = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, nodeB, 4)));
                float value = CSF_AT(float, nodeC, 4);

                void* sinkSelf = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(GetGlobal8afc78())));
                int sinkResult = SinkTextAndValue(sinkSelf, nullptr, static_cast<int>(textAddress), value);
                return reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(sinkResult)));
            }
            result = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(ready));
        }
    }
    return result;
}

HOOK(0x0045D840, DispatchStringAndValueToSink, "auto");
