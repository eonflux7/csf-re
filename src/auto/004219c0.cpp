#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callee bound by address; its real name is unknown, so it gets a neutral one here.
CSF_FUNC(int, __fastcall, FinalizeSceneEntityTask, 0x004D3680, (void* self, void* edx));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Walks the current scene's entity list (array at +0x88, count at +0x90) and,
// for each non-null entity whose sub-object at +0x34 is non-null, finalizes that sub-object.
static void __cdecl ProcessSceneEntityTaskStates()
{
    void* scene = g_pCurrentScene;
    uint32_t entityCount = CSF_AT(uint32_t, scene, 0x90);

    for (uint32_t i = 0; i < entityCount; ++i)
    {
        void** entityArray = CSF_AT(void**, scene, 0x88);
        void* entity = entityArray[i];
        if (entity)
        {
            void* taskObject = CSF_AT(void*, entity, 0x34);
            if (taskObject)
            {
                FinalizeSceneEntityTask(taskObject, nullptr);
            }
        }
    }
}

HOOK(0x004219C0, ProcessSceneEntityTaskStates, "auto");
