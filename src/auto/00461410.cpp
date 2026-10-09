#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CurrentScene_SetFocusEntry, 0x0044FCE0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Entry_ActivateInteraction, 0x0048FE90, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_GetByte023C, 0x00495490, (char* self, void*));

static C_00833ee8* __stdcall SceneHandle_ResolveAndActivate(int /*unusedA1*/, int /*unusedA2*/, int a3)
{
    // a3 points to a record whose first dword is a pointer to a block; the block's +4 is the handle
    // sub-object, and its +12 dword is the signed handle value.
    uint8_t* recordPtr = CSF_AT(uint8_t*, reinterpret_cast<uint8_t*>(a3), 0);
    uint8_t* handleBlock = CSF_AT(uint8_t*, recordPtr, 4);
    int handleValue = CSF_AT(int, handleBlock, 12);

    C_00833ee8* result = reinterpret_cast<C_00833ee8*>(handleBlock + 4);

    if (handleValue >= 0)
    {
        int resolved = CHandleTable_Resolve(
            g_pCurrentScene,
            nullptr,
            static_cast<int>(reinterpret_cast<uintptr_t>(result)));
        result = reinterpret_cast<C_00833ee8*>(static_cast<uintptr_t>(resolved));

        if (result)
        {
            C_00833ee8* entry = result;

            if (entry->field_0050)
            {
                return reinterpret_cast<C_00833ee8*>(
                    static_cast<uintptr_t>(Entry_ActivateInteraction(entry, nullptr)));
            }

            if (!C_GetByte023C(reinterpret_cast<char*>(entry), nullptr)
                || (CurrentScene_SetFocusEntry(
                        g_pCurrentScene,
                        nullptr,
                        static_cast<int>(reinterpret_cast<uintptr_t>(entry))),
                    result = reinterpret_cast<C_00833ee8*>(
                        static_cast<uintptr_t>(GetSceneMenuContext(g_pCurrentScene, nullptr))),
                    result != entry))
            {
                return reinterpret_cast<C_00833ee8*>(
                    static_cast<uintptr_t>(Entry_ActivateInteraction(entry, nullptr)));
            }
        }
    }
    return result;
}

HOOK(0x00461410, SceneHandle_ResolveAndActivate, "auto");
