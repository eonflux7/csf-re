#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

// Local event argument block written on the stack: { int kind = 4; byte value; }
struct SceneControlEventArg {
    int32_t kind;
    uint8_t value;
};

static int __stdcall RouteHandleEventToSceneControl(int a1, int a2, uint32_t* a3)
{
    // v3 = *(a3)->[+4]  (event record pointer held by the first argument block)
    uint8_t* eventRecord = reinterpret_cast<uint8_t*>(a3[0]);
    uint8_t* handleEntry = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, eventRecord, 4));

    // v4 = [v3 + 12]; a negative value means the handle is invalid
    int32_t entryFlags = CSF_AT(int32_t, handleEntry, 12);
    int result = reinterpret_cast<int>(handleEntry + 4);

    if (entryFlags >= 0)
    {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, result);
        if (result)
        {
            void* controlObj = reinterpret_cast<void*>(CSF_AT(uint32_t, reinterpret_cast<void*>(result), 52));
            if (controlObj)
            {
                // v7 = a3[1]; value = byte at [[v7 + 4] + 1]
                uint8_t* senderBlock = reinterpret_cast<uint8_t*>(a3[1]);
                uint8_t* senderData = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, senderBlock, 4));

                SceneControlEventArg eventArg;
                eventArg.kind = 4;
                eventArg.value = CSF_AT(uint8_t, senderData, 1);

                // Virtual slot 0xA8 / 4 = 42 on the control object (__thiscall)
                return ((int(__fastcall*)(void*, void*, void*))(CSF_AT(void**, controlObj, 0)[42]))(
                    controlObj, nullptr, &eventArg);
            }
        }
    }
    return result;
}

HOOK(0x460DC0, RouteHandleEventToSceneControl, "auto");
