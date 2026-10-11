#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CScene_RemoveObjectFromCategories, 0x004528D0, (CScene* self, void* , int handleAddress, int categoryMask));
CSF_FUNC(int, __fastcall, CScene_RemoveObjectSlot, 0x00453390, (CScene* self, void* , int a2));
CSF_FUNC(int, __fastcall, ReleaseSpawnObjectReference, 0x0073F750, (void* self, void* edx));

static int __fastcall S_00552b80_RemoveSceneObject(
    S_00552b80* self, void* /*edx*/)
{
    auto* object = reinterpret_cast<C_00833908*>(self->field_0c8c);
    if (!object)
        return 0;

    const int result = CSF_AT(int, object, 0x18);

    CScene_RemoveObjectFromCategories(
        reinterpret_cast<CScene*>(g_pCurrentScene), nullptr,
        reinterpret_cast<int>(
            &reinterpret_cast<C_00833908*>(self->field_0c8c)->field_001c),
        4);

    auto* scene = reinterpret_cast<CScene*>(g_pCurrentScene);
    const uint32_t objectCount = CSF_AT(uint32_t, scene, 0x28);

    for (uint32_t index = 0; index < objectCount; ++index)
    {
        void* candidate = CSF_AT(void**, scene, 0x20)[index];
        if (CSF_AT(uint32_t, candidate, 0x20) ==
            reinterpret_cast<C_00833908*>(self->field_0c8c)->field_0020)
        {
            CScene_RemoveObjectFromCategories(
                reinterpret_cast<CScene*>(g_pCurrentScene), nullptr,
                reinterpret_cast<int>(
                    &reinterpret_cast<C_00833908*>(self->field_0c8c)->field_001c),
                1);
            scene = reinterpret_cast<CScene*>(g_pCurrentScene);
            break;
        }
    }

    CScene_RemoveObjectSlot(
        scene, nullptr,
        reinterpret_cast<int>(
            &reinterpret_cast<C_00833908*>(self->field_0c8c)->field_001c));

    ReleaseSpawnObjectReference(
        &reinterpret_cast<C_00833908*>(self->field_0c8c)->field_0014,
        nullptr);

    self->field_0c8c = 0;
    return result;
}

HOOK(0x00552B80, S_00552b80_RemoveSceneObject, "auto");
