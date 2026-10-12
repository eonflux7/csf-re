#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, HudPanel_ReleaseReferences, 0x0042CF80, (int* self, void*));
CSF_FUNC(int, __fastcall, SceneLoader_PrepareSceneManager, 0x004FC550, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall C_0083f448_Destroy(C_0083f448* self, void* /*edx*/)
{
    void* ownedObject = reinterpret_cast<void*>(self->field_0078);
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0083F448);

    if (ownedObject)
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, ownedObject, 0)[0])(ownedObject, nullptr, 1);
        self->field_0078 = 0;
    }

    SceneLoader_PrepareSceneManager(self, nullptr);

    void* secondaryObject = reinterpret_cast<void*>(self->field_0074);
    if (secondaryObject)
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, secondaryObject, 0)[2])(secondaryObject, nullptr, 1);
        self->field_0074 = 0;
    }

    const uint32_t entryCount = self->field_002c;
    for (uint32_t index = 0; index < entryCount; ++index)
    {
        uint32_t* entries = reinterpret_cast<uint32_t*>(self->field_0024);
        int* entry = reinterpret_cast<int*>(entries[index]);
        if (entry)
        {
            HudPanel_ReleaseReferences(entry, nullptr);
            Mem_Free(entry);
            reinterpret_cast<uint32_t*>(self->field_0024)[index] = 0;
        }
    }

    if (self->field_00c4)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_00c4));

    self->field_00c4 = 0;
    self->field_00c8 = 0;
    self->field_00cc = 0;

    if (self->field_0024)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0024));
        self->field_0024 = 0;
    }

    if (self->field_0014)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0014));
        self->field_0014 = 0;
    }

    int result = static_cast<int>(self->field_0004);
    if (result)
    {
        result = Mem_Free(reinterpret_cast<void*>(self->field_0004));
        self->field_0004 = 0;
    }

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0082D410);
    return result;
}

HOOK(0x004FCFC0, C_0083f448_Destroy, "auto");
