#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(char, __cdecl, Path_ReplaceExtension, 0x006947D0, (char* Str, int a2, char* a3));
CSF_FUNC(bool, __cdecl, MenuManager_CheckPageAvailability, 0x00698B20, (const char* name, char mode));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetWorldObjectCollection, 0x006B6AF0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, GfxScene_ResetNamedConfiguration, 0x006B6B00, (void* self, void* edx));
CSF_FUNC(char, __fastcall, GfxScene_LoadNamedConfiguration, 0x006B7190, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, C_00860bb8_ConfigureScene, 0x006CAE30, (void* self, void* edx, char* String2, int a3, float a4, float a5, char a6));
CSF_FUNC(unsigned int, __fastcall, GfxScene_PrepareWorldObjectCollection, 0x006CE180, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_FinalizeObjectProvider, 0x006D4450, (void* self, void* edx));

static char __fastcall C_00860bb8_LoadScene(
    C_00860bb8* self, void* /*edx*/, int sceneAddress)
{
    void* scene = reinterpret_cast<void*>(sceneAddress);
    const int objectId = CSF_AT(int, scene, 0x04);
    if (!objectId)
        return 0;

    alignas(8) uint8_t descriptorBytes[0xC]{};
    descriptorBytes[0] = CSF_AT(uint8_t, scene, 0);
    CSF_AT(int, descriptorBytes, 0x04) = objectId;
    CSF_AT(int, descriptorBytes, 0x08) = CSF_AT(int, scene, 0x20);

    void* objectLoader = reinterpret_cast<void*>(self->field_0008);
    uint8_t result = static_cast<uint8_t>(
        ((int(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, objectLoader, 0)[0x28 / 4])(
                objectLoader, nullptr, descriptorBytes)) & 1;

    void* global = reinterpret_cast<void*>(GetGlobalInt());
    if (CSF_AT(uint8_t, global, 0x04))
    {
        alignas(8) uint8_t pathBytes[0x104];
        char* path = reinterpret_cast<char*>(pathBytes);
        const char* source = CSF_AT(const char*, scene, 0x24);
        char* destination = path;
        char character;
        do
        {
            character = *source++;
            *destination++ = character;
        } while (character);

        Path_ReplaceExtension(
            path, 256, reinterpret_cast<char*>(CSF_ADDR_DATA(0x00860BAC)));
        MenuManager_CheckPageAvailability(path, 0);
        MenuManager_CheckPageAvailability(CSF_AT(const char*, scene, 0x24), 0);
    }
    else
    {
        ((void(__fastcall*)(void*, void*, const char*))
            CSF_AT(void**, self, 0)[0x20 / 4])(
                self, nullptr, CSF_AT(const char*, scene, 0x24));
    }

    global = reinterpret_cast<void*>(GetGlobalInt());
    void* loader = reinterpret_cast<void*>(
        GlobalInt_ResolveAccessor(global, nullptr));
    void* collection = reinterpret_cast<void*>(
        SceneLoader_GetWorldObjectCollection(loader, nullptr));
    GfxScene_PrepareWorldObjectCollection(collection, nullptr);

    void* provider = GetCachedObjectProvider(loader, nullptr);
    (void)CSF_VTBL(0x006D4450);
    SceneLoader_FinalizeObjectProvider(provider, nullptr);

    void* finalizer = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, loader, 0)[0x14 / 4])(loader, nullptr);
    ((void(__fastcall*)(void*, void*))
        CSF_AT(void**, finalizer, 0)[0x14 / 4])(finalizer, nullptr);

    global = reinterpret_cast<void*>(GetGlobalInt());
    void* accessor = reinterpret_cast<void*>(
        GlobalInt_ResolveAccessor(global, nullptr));
    void* selector = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, accessor, 0)[0x10 / 4])(accessor, nullptr);
    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, selector, 0)[0x10 / 4])(
            selector, nullptr, CSF_AT(int, descriptorBytes, 0x08));

    objectLoader = reinterpret_cast<void*>(self->field_0008);
    result &= static_cast<uint8_t>(
        ((int(__fastcall*)(void*, void*, void*, int))
            CSF_AT(void**, objectLoader, 0)[0x2C / 4])(
                objectLoader, nullptr, descriptorBytes, 0));

    const char* configurationName = CSF_AT(const char*, scene, 0x08);
    uint32_t configurationLength = 0;
    if (configurationName)
    {
        const char* cursor = configurationName;
        while (*cursor++)
            ++configurationLength;
    }

    if (configurationName && configurationLength != 0)
    {
        result &= static_cast<uint8_t>(GfxScene_LoadNamedConfiguration(
            reinterpret_cast<void*>(self->field_000c), nullptr,
            reinterpret_cast<int>(configurationName)));
    }
    else
    {
        GfxScene_ResetNamedConfiguration(
            reinterpret_cast<void*>(self->field_000c), nullptr);
    }

    self->field_006b = 0;
    C_00860bb8_ConfigureScene(
        self, nullptr,
        CSF_AT(char*, scene, 0x0C),
        CSF_AT(int, scene, 0x10),
        CSF_AT(float, scene, 0x14),
        CSF_AT(float, scene, 0x18),
        static_cast<char>(CSF_AT(uint8_t, scene, 0x1C)));
    return static_cast<char>(result);
}

HOOK(0x006CB6A0, C_00860bb8_LoadScene, "auto");
