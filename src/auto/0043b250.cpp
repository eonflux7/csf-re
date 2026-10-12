#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneObjList_FindIndex, 0x00458930, (void* self, void* edx, int a2));

// Walks the current scene's object array (count at +0x28, array pointer at +0x20).
// For each visible, active object that is not already in the minimap list (self->field_0004),
// calls the virtual slot at +0x2C of this object to register it.
static int __fastcall C_00830e5c_RefreshVisibleSceneObjects(C_00830e5c* self, void* /*edx*/)
{
    const uint32_t count = CSF_AT(uint32_t, g_pCurrentScene, 0x28);
    int result = static_cast<int>(count);

    for (uint32_t index = 0; index < count; ++index)
    {
        // The scene array pointer is reloaded from the scene on every iteration.
        uint32_t* sceneArray = CSF_AT(uint32_t*, g_pCurrentScene, 0x20);
        C_00833b88* sceneObj = reinterpret_cast<C_00833b88*>(sceneArray[index]);

        if (sceneObj)
        {
            // vGetField016c: vtable slot +0x28
            void** objVtbl = CSF_AT(void**, sceneObj, 0);
            int isVisible = reinterpret_cast<int (__fastcall*)(void*, void*)>(objVtbl[0x28 / 4])(sceneObj, nullptr);

            if (isVisible)
            {
                if (!SceneObj_IsInactive(sceneObj, nullptr))
                {
                    uint32_t listPtr = self->field_0004;
                    bool needsAdd = (listPtr == 0);
                    if (!needsAdd)
                    {
                        (void)CSF_VTBL(0x00458930);
                        int foundIndex = SceneObjList_FindIndex(
                            reinterpret_cast<void*>(listPtr),
                            nullptr,
                            static_cast<int>(reinterpret_cast<uintptr_t>((uint8_t*)sceneObj + 0x1c)));
                        needsAdd = (foundIndex == -1);
                    }

                    if (needsAdd)
                    {
                        // Virtual slot +0x2C on this object.
                        void** selfVtbl = CSF_AT(void**, self, 0);
                        reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(selfVtbl[0x2C / 4])(
                            self, nullptr, sceneObj);
                    }
                }
            }
        }
        result = static_cast<int>(count);
    }

    return result;
}

HOOK(0x0043B250, C_00830e5c_RefreshVisibleSceneObjects, "auto");
