#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(void*, __fastcall, C_00861188_ClearParticleResources, 0x006D8210, (C_00861188* self, void*));
CSF_FUNC(int, __cdecl, ReleaseSharedEmitterBuffer, 0x006E27A0, ());
CSF_FUNC(int, __fastcall, C_00863180_ReleaseEmitterObjects, 0x007028C0, (C_00863180* self, void*));

static int __fastcall GfxScene_ClearObjectRegistry(void* self, void* /*edx*/)
{
    for (std::uint32_t i = 0; i < CSF_AT(std::uint32_t, self, 0x1C); ++i)
    {
        void* object = CSF_AT(void**, self, 0x14)[i];
        if (object)
        {
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, object, 0)[1])(object, nullptr, 1);
            CSF_AT(void**, self, 0x14)[i] = nullptr;
        }
    }

    void* objectArray = CSF_AT(void*, self, 0x14);
    CSF_AT(std::uint32_t, self, 0x18) = 0;
    CSF_AT(std::uint32_t, self, 0x1C) = 0;
    if (objectArray)
    {
        CoreMemory_Free(objectArray);
        CSF_AT(void*, self, 0x14) = nullptr;
    }

    for (std::uint32_t j = 0; j < CSF_AT(std::uint32_t, self, 0x2C); ++j)
    {
        void* object = CSF_AT(void**, self, 0x24)[j];
        if (object)
        {
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, object, 0)[1])(object, nullptr, 1);
            CSF_AT(void**, self, 0x24)[j] = nullptr;
        }
    }

    void* secondaryArray = CSF_AT(void*, self, 0x24);
    CSF_AT(std::uint32_t, self, 0x28) = 0;
    CSF_AT(std::uint32_t, self, 0x2C) = 0;
    if (secondaryArray)
    {
        CoreMemory_Free(secondaryArray);
        CSF_AT(void*, self, 0x24) = nullptr;
    }

    C_00861188* particleResources = CSF_AT(C_00861188*, self, 0x48);
    if (particleResources)
    {
        C_00861188_ClearParticleResources(particleResources, nullptr);
        void* object = CSF_AT(void*, self, 0x48);
        if (object)
        {
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, object, 0)[1])(object, nullptr, 1);
            CSF_AT(void*, self, 0x48) = nullptr;
        }
    }

    C_00863180_ReleaseEmitterObjects(
        CSF_AT(C_00863180*, self, 0x44), nullptr);
    ReleaseSharedEmitterBuffer();
    CSF_AT(std::uint8_t, self, 0x10) = 0;
    return ReleaseSharedEmitterBuffer();
}

HOOK(0x006DFAE0, GfxScene_ClearObjectRegistry, "auto");
