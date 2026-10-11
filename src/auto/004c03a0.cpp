#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Reads the three debug values for the action from the object at this+0xCC.
CSF_FUNC(int, __fastcall, Ai_ReadDebugTriple, 0x004B70B0,
         (void* self, void* edx, uint32_t* a2, uint32_t* a3, uint32_t* a4));

// Dispatches debug action 33 with the three values read above.
CSF_FUNC(int, __fastcall, Ai_DispatchDebugAction, 0x004BC870,
         (void* self, void* edx, int a2, int a3, int a4, int a5));

// Debug action 33: reads three values from this+0xCC and dispatches them
// as action 33 to the sub-object at this+0x6C. Always returns true.
static bool __fastcall S_004bc930_ExecuteDebugAction33(S_004bc930* self, void* /*edx*/, int /*a2*/)
{
    uint32_t valueA = 0;   // [ebp-4]
    uint32_t valueB = 0;   // [ebp-8]
    uint32_t valueC = 0;   // [ebp-0xC]

    void* readerObject = reinterpret_cast<uint8_t*>(self) + 0xCC;
    Ai_ReadDebugTriple(readerObject, nullptr, &valueC, &valueB, &valueA);

    void* dispatcherObject = reinterpret_cast<uint8_t*>(self) + 0x6C;
    Ai_DispatchDebugAction(dispatcherObject, nullptr, 33,
                           static_cast<int>(valueC),
                           static_cast<int>(valueB),
                           static_cast<int>(valueA));
    return true;
}

HOOK(0x4C03A0, S_004bc930_ExecuteDebugAction33, "auto");
