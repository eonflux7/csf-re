#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, sub_476300, 0x00476300, (void* self, void* edx, int a2));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall C_008317c8_PropagateState(C_008317c8* self, void* /*edx*/, int state)
{
    sub_476300(self, nullptr, state);

    void* linkedObject = reinterpret_cast<void*>(self->field_0070);
    if (linkedObject)
    {
        void* entry = ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, linkedObject, 0)[0xA8 / 4])(linkedObject, nullptr);
        if (entry)
        {
            void* receiver = ((void*(__fastcall*)(void*, void*))
                CSF_AT(void**, entry, 0)[0x50 / 4])(entry, nullptr);
            if (receiver)
            {
                ((void(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, receiver, 0)[4 / 4])(receiver, nullptr, state);
            }
        }
    }

    int result = static_cast<int32_t>(self->field_006c);
    if (result >= 0)
    {
        result = CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x64));
        if (result)
        {
            void* receiver = reinterpret_cast<void*>(result);
            return ((int(__fastcall*)(void*, void*, int))
                CSF_AT(void**, receiver, 0)[0x70 / 4])(receiver, nullptr, state);
        }
    }
    return result;
}

HOOK(0x00478AE0, C_008317c8_PropagateState, "auto");
