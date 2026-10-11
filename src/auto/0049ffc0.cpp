#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_008343a4_ClearAuxiliaryState, 0x0049FF30, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x006923C0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));

static int __fastcall C_008343a4_ClearIcons(C_008343a4* self, void* /*edx*/)
{
    for (uint32_t i = 0; i < self->field_0018; ++i)
    {
        void* icon = reinterpret_cast<void**>(self->field_0010)[i];
        void* object = CSF_AT(void*, icon, 0x0C);
        if (object)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
            CSF_AT(uint32_t, icon, 0x0C) = 0;
        }

        if (CSF_AT(uint32_t, icon, 0x14))
        {
            void* global = reinterpret_cast<void*>(GetGlobalInt());
            void* registry = reinterpret_cast<void*>(
                SceneLoader_GetObjectRegistry(global, nullptr));
            uint32_t registeredObject = CSF_AT(uint32_t, icon, 0x14);
            reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t)>(
                CSF_AT(void**, registry, 0)[0x18 / 4])(
                    registry, nullptr, registeredObject);
        }

        CSF_AT(void**, icon, 0) = CSF_VTBL(0x0082D410);
        Mem_Free(icon);
    }

    void* icons = reinterpret_cast<void*>(self->field_0010);
    self->field_0014 = 0;
    self->field_0018 = 0;
    if (icons)
    {
        Mem_Free(icons);
        self->field_0010 = 0;
    }

    void* secondaryArray = reinterpret_cast<void*>(self->field_0020);
    self->field_0024 = 0;
    self->field_0028 = 0;
    if (secondaryArray)
    {
        Mem_Free(secondaryArray);
        self->field_0020 = 0;
    }

    void* reference = reinterpret_cast<void*>(self->field_0008);
    if (reference)
    {
        ReleaseObjectReference(reference, nullptr);
        self->field_0008 = 0;
    }

    C_008343a4_ClearAuxiliaryState(self, nullptr);

    int result = static_cast<int>(self->field_0030);
    if (result)
    {
        result = Mem_Free(reinterpret_cast<void*>(self->field_0030));
        self->field_0030 = 0;
    }
    return result;
}

HOOK(0x0049FFC0, C_008343a4_ClearIcons, "auto");
