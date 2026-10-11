#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));

static int __fastcall C_00862490_GetShellCasingResource(C_00862490* self, void* /*edx*/)
{
    int resource;
    std::memcpy(&resource, &self->field_002c, sizeof(resource));
    if (resource != 0)
        return resource;

    void* provider =
        ((void* (__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0xA8 / 4])
        (self, nullptr);

    int resourceId = 2100;
    if (C_008319c0_GetSubObjectField74(
            reinterpret_cast<C_008319c0*>(self), nullptr) == 30)
        resourceId = 2101;

    if (!provider)
    {
        void* owner = reinterpret_cast<void*>(self->field_0004);
        provider =
            ((void* (__fastcall*)(void*, void*))CSF_AT(void**, owner, 0)[0x28 / 4])
            (owner, nullptr);
    }

    int index =
        ((int (__fastcall*)(void*, void*, int))CSF_AT(void**, provider, 0)[0x98 / 4])
        (provider, nullptr, resourceId);

    if (index == -1)
    {
        Handle_GetValuePointer(self, nullptr);
        resource = 0;
    }
    else
    {
        resource =
            ((int (__fastcall*)(void*, void*, int))CSF_AT(void**, provider, 0)[0x8C / 4])
            (provider, nullptr, index);
    }

    std::memcpy(&self->field_002c, &resource, sizeof(resource));
    return resource;
}

HOOK(0x0047AD90, C_00862490_GetShellCasingResource, "auto");
