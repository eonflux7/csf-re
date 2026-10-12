#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(int, g_Value_8BB450, 0x008BB450);

CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(char, __fastcall, GatherSceneCollisions, 0x006ECA90, (void* self, void* edx, void* a2));

static unsigned int __fastcall C_00862028_QuerySceneCollisions(
    C_00862028* self, void* /*edx*/)
{
    unsigned int result = static_cast<unsigned int>(g_Value_8BB450);
    unsigned int index = 0;
    if (result)
    {
        void* loader = reinterpret_cast<void*>(GetGlobalInt());
        void* controller = reinterpret_cast<void*>(
            SceneLoader_GetSceneController(loader, nullptr));

        self->field_005c = 0;
        void* scene = ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, controller, 0)[0x40 / 4])(controller, nullptr);
        GatherSceneCollisions(self, nullptr, scene);

        result = ((unsigned int (__fastcall*)(void*, void*))
            CSF_AT(void**, controller, 0)[0x88 / 4])(controller, nullptr);
        if (result)
        {
            do
            {
                void* child = ((void* (__fastcall*)(void*, void*, unsigned int))
                    CSF_AT(void**, controller, 0)[0x8C / 4])(
                        controller, nullptr, index);
                GatherSceneCollisions(self, nullptr, child);
                ++index;
                result = ((unsigned int (__fastcall*)(void*, void*))
                    CSF_AT(void**, controller, 0)[0x88 / 4])(controller, nullptr);
            } while (index < result);
        }
    }
    return result;
}

HOOK(0x006ECB70, C_00862028_QuerySceneCollisions, "auto");
