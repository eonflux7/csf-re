#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(uint32_t*, __fastcall, ClearListAndFreeNodes, 0x004E9980, (uint32_t* self, void*));
CSF_FUNC(int, __fastcall, PrepareSpawnResourceForDestruction, 0x0073F750, (void* self, void* edx));

static int __fastcall MpSpawnPoints_Reset(void* self, void* /*edx*/)
{
    void* resource = CSF_AT(void*, self, 0x1474);
    if (resource)
    {
        PrepareSpawnResourceForDestruction(resource, nullptr);
        resource = CSF_AT(void*, self, 0x1474);
        if (resource)
        {
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, resource, 0)[0])(resource, nullptr, 1);
            CSF_AT(void*, self, 0x1474) = nullptr;
        }
    }

    void* associatedObject = CSF_AT(void*, self, 0x1470);
    if (associatedObject)
    {
        CSF_AT(uint32_t, associatedObject, 0x164) = 3;
        CSF_AT(void*, self, 0x1470) = nullptr;
    }

    void* sentinel = CSF_AT(void*, self, 0x1468);
    void* node = CSF_AT(void*, sentinel, 0);
    if (node != sentinel)
    {
        do
        {
            void* object = CSF_AT(void*, node, 0x8);
            if (object)
            {
                ((void(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
            }
            node = CSF_AT(void*, node, 0);
        }
        while (node != CSF_AT(void*, self, 0x1468));
    }

    uint32_t* result = ClearListAndFreeNodes(
        reinterpret_cast<uint32_t*>(static_cast<uint8_t*>(self) + 0x1464),
        nullptr);

    CSF_AT(uint8_t, self, 0x14A1) = 1;
    CSF_AT(uint32_t, self, 0x1478) = 0;
    CSF_AT(float, self, 0x147C) = 5.0f;

    return static_cast<int>(reinterpret_cast<intptr_t>(result));
}

HOOK(0x00553730, MpSpawnPoints_Reset, "auto");
