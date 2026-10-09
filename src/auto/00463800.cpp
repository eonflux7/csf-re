#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Resolves a handle in the current scene and, if the resolved control has a receiver,
// forwards a type-18 pointer event carrying the coordinates taken from the message.
static int __stdcall SceneControl_ForwardPointerEvent(int a1, int a2, uint32_t* a3)
{
    (void)a1;
    (void)a2;

    // v3 = *(*a3 + 4)
    uint32_t* msgRoot = reinterpret_cast<uint32_t*>(a3[0]);
    uint32_t v3 = msgRoot[1];

    // v4 = *(v3 + 12), signed
    int v4 = *reinterpret_cast<int*>(v3 + 12);
    int result = static_cast<int>(v3 + 4);

    if (v4 >= 0)
    {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, result);
        if (result != 0)
        {
            void* receiver = *reinterpret_cast<void**>(result + 52);
            if (receiver != nullptr)
            {
                // Message payload: { 18, vtable of C_0082d448, x, y }
                uint32_t* v7 = reinterpret_cast<uint32_t*>(a3[1]);
                uint32_t* v8 = reinterpret_cast<uint32_t*>(v7[1]);
                int32_t msg[4];
                msg[0] = 18;
                msg[1] = static_cast<int32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D448)));
                msg[2] = static_cast<int32_t>(v8[2]);
                msg[3] = static_cast<int32_t>(v8[3]);

                // Virtual slot 168 / 4 = 42 on the receiver's vtable.
                void** vtbl = *reinterpret_cast<void***>(receiver);
                auto slot = reinterpret_cast<int(__fastcall*)(void*, void*, void*)>(vtbl[42]);
                return slot(receiver, nullptr, msg);
            }
        }
    }
    return result;
}

HOOK(0x00463800, SceneControl_ForwardPointerEvent, "auto");
