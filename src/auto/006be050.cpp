#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));
CSF_FUNC(int, __fastcall, Environment_TidyStringOrArray, 0x006BDA20, (void* self, void* , char flags));
CSF_FUNC(void*, __fastcall, SceneLoader_FinalizeRenderResources, 0x006BDCB0, (void* self, void* edx));

static void __fastcall C_008605d8_Destroy(C_008605d8* self, void* /*edx*/)
{
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x008605D8);
    SceneLoader_FinalizeRenderResources(self, nullptr);

    void* ownedObject = reinterpret_cast<void*>(self->field_0178);
    if (ownedObject)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, ownedObject, 0)[0])(ownedObject, nullptr, 1);
        self->field_0178 = 0;
    }

    if (self->field_0184)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0184));
    self->field_0184 = 0;
    self->field_0188 = 0;
    self->field_018c = 0;

    if (self->field_0094)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0094));
        self->field_0094 = 0;
    }

    if (self->field_007c)
    {
        Environment_TidyStringOrArray(
            reinterpret_cast<void*>(self->field_007c), nullptr, 3);
        self->field_007c = 0;
    }

    if (self->field_006c)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_006c));
        self->field_006c = 0;
    }

    if (self->field_005c)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_005c));
        self->field_005c = 0;
    }

    auto* resourceSlot = reinterpret_cast<uint8_t*>(&self->field_005c);
    int remaining = 3;
    do
    {
        resourceSlot -= 0x10;
        uint32_t block = CSF_AT(uint32_t, resourceSlot, 0);
        if (block)
        {
            Mem_Free(reinterpret_cast<void*>(block));
            CSF_AT(uint32_t, resourceSlot, 0) = 0;
        }
    } while (--remaining);

    if (self->field_001c)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_001c));
        self->field_001c = 0;
    }

    if (self->field_000c)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_000c));
        self->field_000c = 0;
    }

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00860578);
    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);
}

HOOK(0x006BE050, C_008605d8_Destroy, "auto");
