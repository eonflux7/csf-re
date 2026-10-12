#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, ResolveRwManager, 0x006B13B0, (int managerId, void* edx));
CSF_FUNC(void**, __fastcall, ReleaseManagedResource, 0x006D0970, (void* self, void* edx, void** a2));
CSF_FUNC(void, __fastcall, DestroyScreenEffectBase, 0x006F8FF0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, ClearScreenEffectResources, 0x007090F0, (void* self, void* edx));

static void __fastcall C_008629f8_Destroy(C_008629f8* self, void* /*edx*/)
{
    const std::uint8_t ownsResources = self->field_0008;
    self->vtbl = CSF_VTBL(0x008629F8);
    self->field_0004 = reinterpret_cast<std::uint32_t>(CSF_ADDR_DATA(0x008629DC));

    if (ownsResources)
    {
        if (self->field_0060)
        {
            void** resource = reinterpret_cast<void**>(self->field_0060);
            int globalManager = GetGlobalInt();
            int manager = ResolveRwManager(globalManager, nullptr);
            ReleaseManagedResource(reinterpret_cast<void*>(manager), nullptr, resource);
            self->field_0060 = 0;
        }

        void* resource = reinterpret_cast<void*>(self->field_006c);
        if (resource)
        {
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, resource, 0)[1])(
                resource, nullptr, 1);
            self->field_006c = 0;
        }
    }

    ClearScreenEffectResources(self, nullptr);
    if (self->field_007c)
        RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(self->field_0078));
    DestroyScreenEffectBase(self, nullptr);
}

HOOK(0x006F90D0, C_008629f8_Destroy, "auto");
