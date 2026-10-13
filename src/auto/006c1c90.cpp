#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, MissionScript_DestroyElementRange, 0x00469760, (int* first, int* last));
CSF_FUNC(void*, __fastcall, MenuManager_AppendPageName, 0x00470FD0, (S_00470ab0* self, void* , void* a3));
CSF_FUNC(int*, __fastcall, MenuManager_ClearElementArray, 0x0050C230, (int* self, void*));
CSF_FUNC(int, __cdecl, RwTexDictionaryForAllTextures, 0x006316E0, (int dictionary, int (__cdecl* callback)(uint32_t*, int), int context));
CSF_FUNC(int, __cdecl, RwTextureDestroy, 0x006317E0, (int texture));
CSF_FUNC(void*, __cdecl, RwTexDictionaryRemoveTexture, 0x00631920, (void* texture));
CSF_FUNC(int, __cdecl, RwTexDictionaryGetCurrent, 0x006319B0, ());
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(char, __cdecl, NormalizeResourceName, 0x006949D0, (char* source, char* destination, unsigned int capacity));
CSF_FUNC(std::uint32_t, __cdecl, CloseSceneFile, 0x00697980, (void* file));
CSF_FUNC(void*, __cdecl, CoreFiles_OpenSceneFile, 0x00698350, (const char* filename, char* mode, int openFlags));
CSF_FUNC(char, __fastcall, TextureDictionary_InitializeTextureList, 0x006C1B70, (void* self, void* edx, unsigned int a2, int a3));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));
CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

static int* __fastcall C_008607f8_SynchronizeTexturesFromFile(
    C_008607f8* self, void* /*edx*/, const char* filename)
{
    alignas(8) uint8_t sourceBytes[0x100]{};
    alignas(8) uint8_t dictionaryInfoBytes[0x48]{};
    alignas(8) uint8_t normalizedPendingBytes[0x40]{};
    alignas(8) uint8_t normalizedSourceBytes[0x40]{};
    alignas(8) uint8_t stringBytes[0x20]{};
    alignas(8) uint8_t pendingBytes[0x18]{};
    alignas(8) uint8_t texturesBytes[0x10]{};

    auto& pending = *reinterpret_cast<S_0046bea0*>(pendingBytes);
    auto& temporaryString = *reinterpret_cast<C_0082d410*>(stringBytes);
    char* source = reinterpret_cast<char*>(sourceBytes);
    char* normalizedPending = reinterpret_cast<char*>(normalizedPendingBytes);
    char* normalizedSource = reinterpret_cast<char*>(normalizedSourceBytes);

    uint32_t pendingBase = 0;
    uint32_t progressCount = 0;
    std::memset(&pending.field_0004, 0, 12);

    void* file = CoreFiles_OpenSceneFile(
        filename, const_cast<char*>(CSF_STR(0x0083E130, "r")), 0);
    if (!file)
        return MenuManager_ClearElementArray(
            reinterpret_cast<int*>(pendingBytes), nullptr);

    CSF_AT(uint32_t, texturesBytes, 0x04) = 0;
    CSF_AT(uint32_t, texturesBytes, 0x08) = 0;
    CSF_AT(uint32_t, texturesBytes, 0x0C) = 0;

    reinterpret_cast<void(__fastcall*)(void*, void*, unsigned int*)>(
        self->vtbl[0x2C / 4])(
            self, nullptr, reinterpret_cast<unsigned int*>(dictionaryInfoBytes));

    TextureDictionary_InitializeTextureList(
        texturesBytes, nullptr, CSF_AT(unsigned int, dictionaryInfoBytes, 0), 0);

    int dictionary = RwTexDictionaryGetCurrent();
    RwTexDictionaryForAllTextures(
        dictionary,
        reinterpret_cast<int(__cdecl*)(uint32_t*, int)>(
            CSF_ADDR_FN(0x006C1C70)),
        reinterpret_cast<int>(texturesBytes));

    while (std::strlen(
        reinterpret_cast<char*(__fastcall*)(void*, void*, char*, int)>(
            CSF_AT(void**, file, 0)[0x20 / 4])(
                file, nullptr, source, 256)))
    {
        std::memset(normalizedSourceBytes, 0, 0x40);
        NormalizeResourceName(source, normalizedSource, 64);

        bool alreadyPresent = false;
        uint32_t textureBase = CSF_AT(uint32_t, texturesBytes, 0x04);
        for (uint32_t index = 0;
             textureBase &&
             index < static_cast<uint32_t>(
                 static_cast<int32_t>(
                     CSF_AT(uint32_t, texturesBytes, 0x08) - textureBase) >> 2);
             ++index)
        {
            uint32_t texture =
                reinterpret_cast<uint32_t*>(textureBase)[index];
            if (texture)
            {
                if (!__stricmp(
                        reinterpret_cast<char*>(texture + 16),
                        normalizedSource))
                {
                    uint32_t base = CSF_AT(uint32_t, texturesBytes, 0x04);
                    uint32_t destination = base + 4 * index;
                    uint32_t next = destination + 4;
                    uint32_t bytes = 4 * static_cast<uint32_t>(
                        static_cast<int32_t>(
                            CSF_AT(uint32_t, texturesBytes, 0x08) - next) >> 2);
                    _memcpy(
                        reinterpret_cast<void*>(destination),
                        reinterpret_cast<void*>(next), bytes);
                    CSF_AT(uint32_t, texturesBytes, 0x08) -= 4;
                    alreadyPresent = true;
                    break;
                }
                textureBase = CSF_AT(uint32_t, texturesBytes, 0x04);
            }
        }

        if (!alreadyPresent)
        {
            std::memset(normalizedPendingBytes, 0, 0x40);
            uint32_t stringAddress = pendingBase + 4;
            for (uint32_t index = 0;
                 pendingBase &&
                 index < static_cast<uint32_t>(
                     static_cast<int32_t>(
                         pending.field_0008 - pendingBase) / 28);
                 ++index, stringAddress += 28)
            {
                void* stringStorage = reinterpret_cast<void*>(stringAddress);
                char* text =
                    CSF_AT(uint32_t, stringStorage, 20) < 0x10u
                    ? reinterpret_cast<char*>(stringStorage)
                    : CSF_AT(char*, stringStorage, 0);
                NormalizeResourceName(text, normalizedPending, 64);
                if (!__stricmp(normalizedPending, source))
                {
                    alreadyPresent = true;
                    break;
                }
            }

            if (!alreadyPresent)
            {
                CSF_AT(uint32_t, stringBytes, 0x18) = 15;
                CSF_AT(uint32_t, stringBytes, 0x14) = 0;
                reinterpret_cast<uint8_t*>(&temporaryString.field_0004)[0] = 0;

                AssignStringRange_429B60(
                    &temporaryString, nullptr, source,
                    static_cast<unsigned int>(std::strlen(source)));
                MenuManager_AppendPageName(
                    reinterpret_cast<S_00470ab0*>(pendingBytes),
                    nullptr, &temporaryString);

                if (CSF_AT(uint32_t, stringBytes, 0x18) >= 0x10 &&
                    temporaryString.field_0004)
                {
                    Mem_FreeDebug(
                        reinterpret_cast<void*>(temporaryString.field_0004));
                }

                pendingBase = pending.field_0004;
                CSF_AT(uint32_t, stringBytes, 0x18) = 15;
                CSF_AT(uint32_t, stringBytes, 0x14) = 0;
                reinterpret_cast<uint8_t*>(&temporaryString.field_0004)[0] = 0;
            }
        }

        if (++progressCount >= 0xA)
        {
            auto callback =
                reinterpret_cast<void(__cdecl*)()>(self->field_0008);
            progressCount = 0;
            if (callback)
                callback();
        }
    }

    uint32_t textureBase = CSF_AT(uint32_t, texturesBytes, 0x04);
    for (uint32_t index = 0;
         textureBase &&
         index < static_cast<uint32_t>(
             static_cast<int32_t>(
                 CSF_AT(uint32_t, texturesBytes, 0x08) - textureBase) >> 2);
         ++index)
    {
        int texture = reinterpret_cast<int*>(textureBase)[index];
        if (texture)
        {
            if (CSF_AT(int, reinterpret_cast<void*>(texture), 84) <= 1)
            {
                RwTexDictionaryRemoveTexture(
                    reinterpret_cast<void*>(
                        reinterpret_cast<uint32_t*>(textureBase)[index]));
                RwTextureDestroy(texture);
                textureBase = CSF_AT(uint32_t, texturesBytes, 0x04);
            }
        }
    }

    uint32_t pendingStorage = pending.field_0004;
    uint32_t stringAddress = pendingStorage + 4;
    for (uint32_t index = 0;
         pendingStorage &&
         index < static_cast<uint32_t>(
             static_cast<int32_t>(
                 pending.field_0008 - pendingStorage) / 28);
         ++index, stringAddress += 28)
    {
        void* stringStorage = reinterpret_cast<void*>(stringAddress);
        char* text =
            CSF_AT(uint32_t, stringStorage, 20) < 0x10u
            ? reinterpret_cast<char*>(stringStorage)
            : CSF_AT(char*, stringStorage, 0);
        reinterpret_cast<void(__fastcall*)(void*, void*, char*)>(
            self->vtbl[0x1C / 4])(self, nullptr, text);
    }

    CloseSceneFile(file);

    int* result = CSF_AT(int*, texturesBytes, 0x04);
    if (result)
        result = reinterpret_cast<int*>(Mem_FreeDebug(result));
    CSF_AT(uint32_t, texturesBytes, 0x04) = 0;
    CSF_AT(uint32_t, texturesBytes, 0x08) = 0;
    CSF_AT(uint32_t, texturesBytes, 0x0C) = 0;

    if (pendingStorage)
    {
        MissionScript_DestroyElementRange(
            reinterpret_cast<int*>(pendingStorage),
            reinterpret_cast<int*>(pending.field_0008));
        return reinterpret_cast<int*>(
            Mem_FreeDebug(reinterpret_cast<void*>(pendingStorage)));
    }
    return result;
}

HOOK(0x006C1C90, C_008607f8_SynchronizeTexturesFromFile, "auto");
