#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, DestroySceneObjectOptionsStorage, 0x004018E0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, WeaponFirearms_InitializeSceneObjectOptions, 0x004777D0, (uint32_t* self, void*));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ResolveClumpManager, 0x00652020, (int managerId, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(char, __cdecl, NormalizeResourceName, 0x006949D0, (char* source, char* destination, unsigned int capacity));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));

static int __fastcall InitializeSceneObjectResources(void* self, void* /*edx*/, void* definition)
{
    alignas(8) uint8_t resourceNameBytes[0x100];
    alignas(8) uint8_t optionsBytes[0xB8];
    alignas(8) uint8_t sceneRequestBytes[0x18];
    alignas(8) uint8_t resourceRequestBytes[0x24];

    CSF_AT(uint32_t, self, 0x28) = CSF_AT(uint32_t, definition, 0x00);
    CSF_AT(uint8_t, self, 0x2C) = CSF_AT(uint8_t, definition, 0x50);

    char* resourceName;
    if (CSF_AT(uint32_t, definition, 0x38) < 0x10u)
        resourceName = reinterpret_cast<char*>(definition) + 0x24;
    else
        resourceName = CSF_AT(char*, definition, 0x24);

    NormalizeResourceName(resourceName, reinterpret_cast<char*>(resourceNameBytes), 256);

    const uint32_t resourceParameter1 = CSF_AT(uint32_t, definition, 0x54);
    const uint32_t resourceParameter2 = CSF_AT(uint32_t, definition, 0x58);
    CSF_AT(char*, resourceRequestBytes, 0x00) =
        reinterpret_cast<char*>(resourceNameBytes);
    const uint8_t resourceFlag = CSF_AT(uint8_t, definition, 0x50);
    std::memset(resourceRequestBytes + 0x04, 0, 12);
    CSF_AT(float, resourceRequestBytes, 0x10) = 1.0f;
    CSF_AT(float, resourceRequestBytes, 0x14) = 1.0f;
    CSF_AT(uint32_t, resourceRequestBytes, 0x18) = resourceParameter1;
    CSF_AT(uint32_t, resourceRequestBytes, 0x1C) = resourceParameter2;
    CSF_AT(int, resourceRequestBytes, 0x20) = 8;
    if (resourceFlag)
        CSF_AT(int, resourceRequestBytes, 0x20) = 9;

    void* registry = reinterpret_cast<void*>(
        SceneLoader_GetObjectRegistry(reinterpret_cast<void*>(GetGlobalInt()), nullptr));
    CSF_AT(void*, self, 0x04) =
        (reinterpret_cast<void* (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, registry, 0)[10]))(registry, nullptr, resourceRequestBytes);

    if (CSF_AT(uint32_t, definition, 0x1C) < 0x10u)
        CSF_AT(void*, sceneRequestBytes, 0x00) =
            reinterpret_cast<uint8_t*>(definition) + 0x08;
    else
        CSF_AT(void*, sceneRequestBytes, 0x00) = CSF_AT(void*, definition, 0x08);

    CSF_AT(uint8_t, sceneRequestBytes, 0x04) = 0;
    CSF_AT(uint32_t, sceneRequestBytes, 0x08) = 0;
    CSF_AT(uint32_t, sceneRequestBytes, 0x0C) = 0;
    CSF_AT(uint32_t, sceneRequestBytes, 0x10) = 0;
    CSF_AT(uint32_t, sceneRequestBytes, 0x14) = 0;

    void* controller = reinterpret_cast<void*>(
        SceneLoader_GetSceneController(reinterpret_cast<void*>(GetGlobalInt()), nullptr));
    void* sceneObject =
        (reinterpret_cast<void* (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, controller, 0)[5]))(controller, nullptr, sceneRequestBytes);
    CSF_AT(void*, self, 0x08) = sceneObject;

    if (sceneObject)
    {
        WeaponFirearms_InitializeSceneObjectOptions(
            reinterpret_cast<uint32_t*>(optionsBytes), nullptr);
        CSF_AT(uint32_t, optionsBytes, 0x00) &= 0xFFFFFF1F;

        void* object = CSF_AT(void*, self, 0x08);
        (reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t*)>(
            CSF_AT(void**, object, 0)[4]))(
                object, nullptr, reinterpret_cast<uint32_t*>(optionsBytes));

        if (CSF_AT(uint32_t, optionsBytes, 0x90) >= 0x10 &&
            CSF_AT(void*, optionsBytes, 0x7C))
        {
            Mem_FreeDebug(CSF_AT(void*, optionsBytes, 0x7C));
        }
        CSF_AT(uint32_t, optionsBytes, 0x90) = 15;
        CSF_AT(uint32_t, optionsBytes, 0x8C) = 0;
        CSF_AT(uint8_t, optionsBytes, 0x7C) = 0;
        DestroySceneObjectOptionsStorage(optionsBytes + 0x04, nullptr);
    }

    CSF_AT(uint32_t, self, 0x10) = CSF_AT(uint32_t, definition, 0x3C);
    CSF_AT(uint32_t, self, 0x14) = CSF_AT(uint32_t, definition, 0x40);
    CSF_AT(uint32_t, self, 0x18) = CSF_AT(uint32_t, definition, 0x44);
    CSF_AT(uint32_t, self, 0x1C) = CSF_AT(uint32_t, definition, 0x48);
    CSF_AT(uint32_t, self, 0x20) = CSF_AT(uint32_t, definition, 0x4C);

    void* clumpManager = reinterpret_cast<void*>(
        ResolveClumpManager(GetGlobalInt(), nullptr));
    void* clump =
        (reinterpret_cast<void* (__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, clumpManager, 0)[10]))(clumpManager, nullptr, 2, 3);
    CSF_AT(void*, self, 0x0C) = clump;

    (reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, clump, 0)[13]))(
            clump, nullptr, reinterpret_cast<uint8_t*>(self) + 0x10);

    clump = CSF_AT(void*, self, 0x0C);
    (reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t)>(
        CSF_AT(void**, clump, 0)[15]))(
            clump, nullptr, CSF_AT(uint32_t, self, 0x20));

    clump = CSF_AT(void*, self, 0x0C);
    return (reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, clump, 0)[11]))(clump, nullptr, 0);
}

HOOK(0x006C89E0, InitializeSceneObjectResources, "auto");
