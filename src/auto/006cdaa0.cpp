#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(void, __fastcall, GfxScene_FinalizeVisibilityResource, 0x005655B0, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(bool, __cdecl, SceneNamesMatch, 0x00694AA0, (char* first, char* second));
CSF_FUNC(unsigned int, __cdecl, ResourceName_Transform, 0x00694B20, (char* a1, char* a2, char* a3, unsigned int a4));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, RenderHandle_FromObject, 0x006B2D50, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, GfxScene_CreateVisibilityResource, 0x006B6AD0, ());
CSF_FUNC(uint32_t*, __fastcall, GfxScene_InitializeVisibilityResource, 0x006B6B00, (void* self, void* edx));
CSF_FUNC(char, __fastcall, GfxScene_LoadVisibilityResource, 0x006B7190, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __cdecl, GfxScene_CreateWorldResource, 0x006BBFF0, ());
CSF_FUNC(int, __fastcall, GfxScene_InitializeWorldResource, 0x006BC010, (void* self, void* edx));
CSF_FUNC(char, __fastcall, GfxScene_FinalizeWorldResource, 0x006BD3D0, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, SceneLoader_AttachVisibilityProxy, 0x006D6EE0, (void* self, void* edx, void* a2));

static int __fastcall GfxScene_LoadWorld(
    void* self, void* /*edx*/, const char* worldName,
    const char* visibilityName, int loadMode)
{
    alignas(8) uint8_t texturePath[0x200];
    alignas(8) uint8_t loadParameters[0x14] = {};

    CSF_AT(void*, loadParameters, 0x0C) = self;
    CSF_AT(uint8_t, loadParameters, 0x13) = 1;
    if (!CSF_AT(uint8_t, self, 0x96))
        return -1;

    uint32_t index = 0;
    void* entry = nullptr;
    bool found = false;
    if (CSF_AT(uint32_t, self, 0xA8))
    {
        do
        {
            void* candidate = CSF_AT(void**, self, 0xA0)[index];
            if (candidate)
            {
                char* storedName = CSF_AT(uint32_t, candidate, 0x18) < 0x10u
                    ? reinterpret_cast<char*>(candidate) + 4
                    : CSF_AT(char*, candidate, 0x04);
                if (SceneNamesMatch(const_cast<char*>(worldName), storedName))
                {
                    found = true;
                    break;
                }
            }
            ++index;
        } while (index < CSF_AT(uint32_t, self, 0xA8));
    }

    if (found)
    {
        entry = CSF_AT(void**, self, 0xA0)[index];
        if (CSF_AT(void*, entry, 0x38))
            return static_cast<int>(index);
        CSF_AT(uint8_t, loadParameters, 0x13) = 0;
    }
    else
    {
        index = static_cast<uint32_t>(loadMode);
    }

    if (!entry)
    {
        entry = Mem_Alloc(64);
        if (entry)
        {
            CSF_AT(uint32_t, entry, 0x14) = 0;
            CSF_AT(uint32_t, entry, 0x18) = 15;
            CSF_AT(uint8_t, entry, 0x04) = 0;
            CSF_AT(uint32_t, entry, 0x30) = 0;
            CSF_AT(uint32_t, entry, 0x34) = 15;
            CSF_AT(uint8_t, entry, 0x20) = 0;
            CSF_AT(void*, entry, 0x38) = nullptr;
            CSF_AT(void*, entry, 0x3C) = nullptr;
        }
    }

    AssignStringRange_429B60(entry, nullptr, const_cast<char*>(worldName),
                           static_cast<unsigned int>(std::strlen(worldName)));
    void* world = GfxScene_CreateWorldResource();
    CSF_AT(void*, entry, 0x38) = world;
    GfxScene_InitializeWorldResource(world, nullptr);

    CSF_AT(const char*, loadParameters, 0x04) = worldName;
    ResourceName_Transform(const_cast<char*>(worldName),
                           const_cast<char*>(CSF_STR(0x00860C74, "Textures/")),
                           reinterpret_cast<char*>(texturePath), 512);
    CSF_AT(uint8_t*, loadParameters, 0x08) = texturePath;
    CSF_AT(uint8_t, loadParameters, 0x00) = static_cast<uint8_t>(loadMode);

    if (texturePath[0])
    {
        void* global = reinterpret_cast<void*>(GetGlobalInt());
        void* accessor = reinterpret_cast<void*>(GlobalInt_ResolveAccessor(global, nullptr));
        void* textureManager =
            reinterpret_cast<void*>(
                (reinterpret_cast<int(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, accessor, 0)[0x10 / 4]))(accessor, nullptr));
        (reinterpret_cast<void(__fastcall*)(void*, void*, uint8_t*)>(
            CSF_AT(void**, textureManager, 0)[0x10 / 4]))(
                textureManager, nullptr, CSF_AT(uint8_t*, loadParameters, 0x08));
    }

    world = CSF_AT(void*, entry, 0x38);
    if (!(reinterpret_cast<bool(__fastcall*)(void*, void*, uint8_t*, int)>(
            CSF_AT(void**, world, 0)[0x2C / 4]))(
                world, nullptr, loadParameters, 0))
        return -1;

    AssignStringRange_429B60(static_cast<uint8_t*>(entry) + 0x1C, nullptr,
                           const_cast<char*>(visibilityName),
                           static_cast<unsigned int>(std::strlen(visibilityName)));
    void* visibility = GfxScene_CreateVisibilityResource();
    CSF_AT(void*, entry, 0x3C) = visibility;
    GfxScene_InitializeVisibilityResource(visibility, nullptr);
    if (!GfxScene_LoadVisibilityResource(CSF_AT(void*, entry, 0x3C), nullptr,
                                        reinterpret_cast<int>(visibilityName)))
        return -1;

    GfxScene_FinalizeWorldResource(CSF_AT(void*, entry, 0x38), nullptr);
    GfxScene_FinalizeVisibilityResource(CSF_AT(void*, entry, 0x3C), nullptr);
    void* proxy = CSF_AT(void*, entry, 0x3C);
    void* global = reinterpret_cast<void*>(GetGlobalInt());
    void* renderHandle = reinterpret_cast<void*>(RenderHandle_FromObject(global, nullptr));
    SceneLoader_AttachVisibilityProxy(renderHandle, nullptr, proxy);

    if (!CSF_AT(uint8_t, loadParameters, 0x13))
        return static_cast<int>(index);

    void* loader = CSF_AT(void*, loadParameters, 0x0C);
    uint32_t count = CSF_AT(uint32_t, loader, 0xA8);
    if (count)
    {
        void** entries = CSF_AT(void**, loader, 0xA0);
        for (uint32_t slot = 0; slot < count; ++slot)
        {
            if (!entries[slot])
            {
                entries[slot] = entry;
                return static_cast<int>(slot);
            }
        }
    }

    if (CSF_AT(uint32_t, loader, 0xA8) >= CSF_AT(uint32_t, loader, 0xA4))
    {
        // The packet also identifies this call address as a vtable reference.
        (void)CSF_VTBL(0x0041E5F0);
        S_0041e5f0_GrowArrayCapacity(
            reinterpret_cast<S_0041e5f0*>(static_cast<uint8_t*>(loader) + 0xA0),
            nullptr, CSF_AT(int, loader, 0xAC));
    }

    uint32_t appendIndex = CSF_AT(uint32_t, loader, 0xA8);
    void** destination = CSF_AT(void**, loader, 0xA0) + appendIndex;
    CSF_AT(uint32_t, loader, 0xA8) = appendIndex + 1;
    *destination = entry;
    return static_cast<int>(CSF_AT(uint32_t, loader, 0xA8) - 1);
}

HOOK(0x006CDAA0, GfxScene_LoadWorld, "auto");
