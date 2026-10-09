#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, SceneLoader_DispatchSectorMenuRequest,
         0x005018D0, (int a1, int a2));
CSF_FUNC(int, __fastcall, SceneLoader_GetResourceOwner,
         0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_ReleaseResources,
         0x006CADD0, (void* self, void* edx));

static void __fastcall S_004fe580_HandleSectorRequest(
    S_004fe580* self, void* /*edx*/, int requestAddress)
{
    if (!requestAddress)
        return;

    void* request = reinterpret_cast<void*>(requestAddress);

    switch (CSF_AT(int, request, 0x04))
    {
    case 0:
    {
        int sectorIndex = CSF_AT(int, request, 0x38);
        if (CSF_AT(int, self, 0x0C) != sectorIndex &&
            CSF_AT(uint8_t,
                   reinterpret_cast<uint8_t*>(self->field_0030) +
                       88 * sectorIndex,
                   0x1C))
        {
            CSF_AT(int, self, 0x100) = requestAddress;
            SceneLoader_DispatchSectorMenuRequest(
                reinterpret_cast<int>(self), 0);
            self->field_0028 = 1;
        }
        break;
    }

    case 1:
    {
        int sectorIndex = CSF_AT(int, request, 0x38);
        if (CSF_AT(int, self, 0x0C) != sectorIndex &&
            !CSF_AT(uint8_t,
                    reinterpret_cast<uint8_t*>(self->field_0030) +
                        88 * sectorIndex,
                    0x1C))
        {
            reinterpret_cast<void (__fastcall*)(S_004fe580*, void*, int)>(
                reinterpret_cast<void**>(self->field_0000)[0x34 / 4])(
                    self, nullptr, sectorIndex);
        }
        break;
    }

    case 2:
    {
        reinterpret_cast<void (__fastcall*)(S_004fe580*, void*)>(
            reinterpret_cast<void**>(self->field_0000)[0x48 / 4])(
                self, nullptr);

        int globalObject = GetGlobalInt();
        int resourceOwner = SceneLoader_GetResourceOwner(
            reinterpret_cast<void*>(globalObject), nullptr);
        SceneLoader_ReleaseResources(
            reinterpret_cast<void*>(resourceOwner), nullptr);

        int sectorIndex = CSF_AT(int, request, 0x38);
        if (CSF_AT(uint8_t,
                   reinterpret_cast<uint8_t*>(self->field_0030) +
                       88 * sectorIndex,
                   0x1C))
        {
            reinterpret_cast<void (__fastcall*)(S_004fe580*, void*, int)>(
                reinterpret_cast<void**>(self->field_0000)[0x38 / 4])(
                    self, nullptr, sectorIndex);
        }
        break;
    }
    }
}

HOOK(0x005028E0, S_004fe580_HandleSectorRequest, "auto");
