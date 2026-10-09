#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Message block passed to the target object's vtable slot 42 (+0xA8).
struct SceneMessageBlock {
    int32_t msgId[2];   // {26, 2}
    uint8_t flag;       // set from the byte at (*(msgArg3 + 4)) + 1
    uint8_t pad[3];
    int32_t value;      // set from (*(msgArg2 + 4)) + 4
};

static int __stdcall RouteSceneHandleMessage(int /*unusedArg1*/, int /*unusedArg2*/, uint32_t* argsRef)
{
    uint32_t handleOwner = argsRef[0];
    uint32_t handleHolder = CSF_AT(uint32_t, handleOwner, 4);
    int32_t holderFlags = (int32_t)CSF_AT(uint32_t, handleHolder, 12);
    int result = (int)(handleHolder + 4);

    if (holderFlags >= 0) {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, result);
        if (result != 0) {
            uint32_t target = CSF_AT(uint32_t, result, 52);
            if (target != 0) {
                SceneMessageBlock msg;
                msg.flag = 1;
                msg.value = -1;
                msg.msgId[0] = 26;
                msg.msgId[1] = 2;
                msg.value = CSF_AT(int32_t, CSF_AT(uint32_t, argsRef[1], 4), 4);
                msg.flag = CSF_AT(uint8_t, CSF_AT(uint32_t, argsRef[2], 4), 1);
                return ((int(__fastcall*)(void*, void*, void*))CSF_AT(void**, target, 0)[42])(
                    (void*)target, nullptr, &msg);
            }
        }
    }
    return result;
}

HOOK(0x45FAD0, RouteSceneHandleMessage, "auto");
