#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, SceneObject_SetColorOverride, 0x0048E790,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SceneObject_ResetColorOverride, 0x0048E830,
         (void* self, void* edx));

static char __fastcall Scene_SetActionMode(void* self, void* /*edx*/, char actionMode)
{
    for (std::uint32_t i = 0; i < CSF_AT(std::uint32_t, self, 0x18); ++i)
    {
        void* object = CSF_AT(void**, self, 0x10)[i];
        if (!object)
            continue;

        if (actionMode)
        {
            if (((bool(__fastcall*)(void*, void*))
                 CSF_AT(void**, object, 0)[0x48 / 4])(object, nullptr))
            {
                void* state = ((void*(__fastcall*)(void*, void*))
                               CSF_AT(void**, object, 0)[0x28 / 4])(object, nullptr);

                if (!((bool(__fastcall*)(void*, void*))
                      CSF_AT(void**, state, 0)[0x54 / 4])(state, nullptr))
                {
                    float color[4] = {0.4f, 0.4f, 0.4f, 1.0f};
                    SceneObject_SetColorOverride(
                        object, nullptr,
                        static_cast<int>(reinterpret_cast<std::intptr_t>(color)));
                }
            }
        }
        else
        {
            SceneObject_ResetColorOverride(object, nullptr);
        }
    }

    CSF_AT(char, self, 0x180) = actionMode;
    if (!actionMode)
    {
        CSF_AT(int, self, 0x19C) = -1;
        CSF_AT(int, self, 0x1A0) = -1;
        return -1;
    }
    return actionMode;
}

HOOK(0x00452320, Scene_SetActionMode, "auto");
