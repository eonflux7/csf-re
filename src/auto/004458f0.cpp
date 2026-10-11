#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
static CSF_FUNC(unsigned int, __fastcall, RegisterSceneObjects, 0x00475800,
                (void* self, void* edx, int a2));
static CSF_FUNC(void*, __fastcall, GrowSceneObjectStorage, 0x0044DD40,
                (void* self, void* edx, int a2));

static unsigned int __fastcall SceneLoader_PrepareSceneObjects(
    void* self, void* /*edx*/)
{
    CSF_AT(uint8_t, self, 0xE8) = 0;
    CSF_AT(uint8_t, self, 0xFC) = 0;
    CSF_AT(uint8_t, self, 0x110) = 0;
    CSF_AT(uint32_t, self, 0xC4) = 0;
    CSF_AT(uint32_t, self, 0xC0) = 0;
    CSF_AT(uint32_t, self, 0x08) = 0;
    CSF_AT(uint32_t, self, 0x3C) = 0;

    const int sceneObjects =
        reinterpret_cast<int>(static_cast<uint8_t*>(self) + 4);
    void* globalObjects = reinterpret_cast<void*>(GetGlobalObjectField184());
    RegisterSceneObjects(globalObjects, nullptr, sceneObjects);

    void* storage = CSF_AT(void*, self, 0x114);
    CSF_AT(uint32_t, storage, 0x04) = 0;
    CSF_AT(uint32_t, storage, 0x08) = 0;
    CSF_AT(uint32_t, storage, 0x20) = 0;
    CSF_AT(uint32_t, storage, 0x24) = 0;
    CSF_AT(uint32_t, storage, 0x28) = 0;

    unsigned int result = CSF_AT(uint32_t, storage, 0x10);
    if (result < 0x1F4)
    {
        GrowSceneObjectStorage(static_cast<uint8_t*>(storage) + 0x0C,
                               nullptr, static_cast<int>(500 - result));
        result = CSF_AT(uint32_t, storage, 0x1C);
        CSF_AT(uint32_t, reinterpret_cast<void*>(result), 0x10) = 100;
    }
    else
    {
        void* allocation = CSF_AT(void*, storage, 0x1C);
        CSF_AT(uint32_t, allocation, 0x10) = 100;
    }
    return result;
}

HOOK(0x004458F0, SceneLoader_PrepareSceneObjects, "auto");
