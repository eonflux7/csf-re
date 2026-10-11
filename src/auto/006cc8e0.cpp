#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpWorldAddCamera, 0x0058DD90, (int a1, int a2));
CSF_FUNC(int, __cdecl, RpWorldRemoveCamera, 0x0058DDC0, (int a1, int a2));
CSF_FUNC(int, __cdecl, RwCameraGetWorld, 0x0058DDF0, (int a1));
CSF_FUNC(int, __cdecl, RwCameraEndUpdate, 0x00628E40, (int camera));
CSF_FUNC(int, __cdecl, RwCameraBeginUpdate, 0x00628E50, (int camera));
CSF_FUNC(int, __fastcall, ResolveClumpManager, 0x00652020, (int managerId, void* edx));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetWorldObjectCollection, 0x006B6AF0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, SceneLoader_RenderWorld, 0x006B7930, (void* self, void* edx, uint8_t a2));
CSF_FUNC(char, __fastcall, SceneObject_RenderWorld, 0x006BD7F0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(char, __fastcall, S_006cbc80_IsWorldBoundsExcluded, 0x006CBC80,
         (void* self, void* edx, float* a2, float* a3, int a4));
CSF_FUNC(int, __fastcall, ClumpManager_SelectWorld, 0x006D0FB0,
         (void* self, void* edx, int a2));

static bool __fastcall S_006cbc80_RenderReferencedWorlds(
    S_006cbc80* self, void* /*edx*/)
{
    if (!self->field_0095)
        return true;

    bool updateSucceeded = true;
    void* globals = reinterpret_cast<void*>(GetGlobalValue_6C5760());
    const int camera = reinterpret_cast<int>(
        GetCachedObjectProvider(CSF_AT(void*, globals, 0xF0), nullptr));
    const int originalWorld = RwCameraGetWorld(camera);
    void* manager = reinterpret_cast<void*>(
        ResolveClumpManager(GetGlobalInt(), nullptr));

    alignas(8) uint8_t savedState[0x10];
    CSF_AT(uint32_t, savedState, 0x0) = 0;
    CSF_AT(uint32_t, savedState, 0x4) = 0;
    CSF_AT(uint32_t, savedState, 0x8) = 0;
    CSF_AT(uint32_t, savedState, 0xC) = 1065353216;

    if (CSF_AT(uint8_t, self, 0xB4) &&
        ((void*(__fastcall*)(void*, void*))
             CSF_AT(void**, manager, 0)[0x44 / 4])(manager, nullptr))
    {
        void* stateObject =
            ((void*(__fastcall*)(void*, void*))
                 CSF_AT(void**, manager, 0)[0x44 / 4])(manager, nullptr);
        void* state =
            ((void*(__fastcall*)(void*, void*))
                 CSF_AT(void**, stateObject, 0)[0x38 / 4])(stateObject, nullptr);
        CSF_AT(uint32_t, savedState, 0x0) = CSF_AT(uint32_t, state, 0x0);
        CSF_AT(uint32_t, savedState, 0x4) = CSF_AT(uint32_t, state, 0x4);
        CSF_AT(uint32_t, savedState, 0x8) = CSF_AT(uint32_t, state, 0x8);
        CSF_AT(uint32_t, savedState, 0xC) = CSF_AT(uint32_t, state, 0xC);
    }

    if (CSF_AT(uint8_t, self, 0x96) &&
        ((uint32_t(__fastcall*)(void*, void*))
             CSF_AT(void**, self, 0)[0x88 / 4])(self, nullptr) > 0)
    {
        RwCameraEndUpdate(camera);
        if (originalWorld)
            RpWorldRemoveCamera(originalWorld, camera);

        const uint32_t count =
            ((uint32_t(__fastcall*)(void*, void*))
                 CSF_AT(void**, self, 0)[0x88 / 4])(self, nullptr);

        for (uint32_t index = 0; index < count; ++index)
        {
            if (!updateSucceeded)
                return false;

            void* entry = CSF_AT(void**, self, 0xA0)[index];
            if (!entry)
                continue;

            int world;
            if (self->field_006b)
            {
                void* loader = CSF_AT(void*, entry, 0x3C);
                if (!loader)
                    continue;
                world = SceneLoader_GetWorldObjectCollection(loader, nullptr);
            }
            else
            {
                void* object = CSF_AT(void*, entry, 0x38);
                if (!object)
                    continue;
                world = reinterpret_cast<int>(GetReferencedObject(object, nullptr));
            }

            if (!world)
                continue;

            void* worldObject = reinterpret_cast<void*>(world);
            alignas(8) uint8_t upperBoundsBytes[0xC];
            alignas(8) uint8_t lowerBoundsBytes[0x24];
            float* upperBounds = reinterpret_cast<float*>(upperBoundsBytes);
            float* lowerBounds = reinterpret_cast<float*>(lowerBoundsBytes);

            lowerBounds[0] = CSF_AT(float, worldObject, 0x50);
            lowerBounds[1] = CSF_AT(float, worldObject, 0x54);
            lowerBounds[2] = CSF_AT(float, worldObject, 0x58);
            upperBounds[0] = CSF_AT(float, worldObject, 0x5C);
            upperBounds[1] = CSF_AT(float, worldObject, 0x60);
            upperBounds[2] = CSF_AT(float, worldObject, 0x64);

            if (!S_006cbc80_IsWorldBoundsExcluded(
                    self, nullptr, upperBounds, lowerBounds, 0))
            {
                if (CSF_AT(uint8_t, self, 0xB4))
                {
                    if (index == CSF_AT(uint32_t, self, 0xB8))
                    {
                        ((void(__fastcall*)(void*, void*, void*))
                             CSF_AT(void**, manager, 0)[0x10 / 4])(
                            manager, nullptr,
                            reinterpret_cast<void*>(CSF_ADDR_DATA(0x008A7EF8)));
                    }
                    else
                    {
                        ((void(__fastcall*)(void*, void*, void*))
                             CSF_AT(void**, manager, 0)[0x10 / 4])(
                            manager, nullptr,
                            reinterpret_cast<void*>(CSF_ADDR_DATA(0x008A7F08)));
                    }
                }

                ClumpManager_SelectWorld(manager, nullptr, world);
                RpWorldAddCamera(world, camera);
                updateSucceeded = RwCameraBeginUpdate(camera) != 0;
                if (updateSucceeded)
                {
                    if (self->field_006b)
                    {
                        SceneLoader_RenderWorld(
                            CSF_AT(void*, entry, 0x3C), nullptr, self->field_006c);
                    }
                    else
                    {
                        SceneObject_RenderWorld(
                            CSF_AT(void*, entry, 0x38), nullptr);
                    }
                    RwCameraEndUpdate(camera);
                }
                RpWorldRemoveCamera(world, camera);
            }
        }

        if (!updateSucceeded)
            return false;

        if (originalWorld)
        {
            if (CSF_AT(uint8_t, self, 0xB4))
            {
                ((void(__fastcall*)(void*, void*, void*))
                     CSF_AT(void**, manager, 0)[0x10 / 4])(
                    manager, nullptr, savedState);
            }
            ClumpManager_SelectWorld(manager, nullptr, originalWorld);
            RpWorldAddCamera(originalWorld, camera);
            return RwCameraBeginUpdate(camera) != 0;
        }
    }

    return updateSucceeded;
}

HOOK(0x006CC8E0, S_006cbc80_RenderReferencedWorlds, "auto");
