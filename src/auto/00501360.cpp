#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(void*, __fastcall, C_0082d410_CopyConstruct, 0x00500FE0,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(uint32_t*, __fastcall, SceneLoader_SetIndexedColor, 0x006B7F70,
         (void* self, void* edx, int a2, int* a3));

static int __stdcall SceneLoader_LoadIndexedColors(int destination)
{
    void* manager = reinterpret_cast<void*>(GetValue8B0554());
    void* entries = CSF_AT(void*, manager, 0x1FD4);
    void* storage = CSF_AT(void*, entries, 0x04);
    uint32_t begin = CSF_AT(uint32_t, storage, 0x04);

    int result = reinterpret_cast<int>(storage);
    uint32_t count = 0;
    if (begin != 0)
    {
        int byteCount = static_cast<int>(
            CSF_AT(uint32_t, storage, 0x08) - begin);

        // The original also leaves the multiplication's low word as its
        // return value if this produces an empty range.
        int64_t product = 1717986919LL * byteCount;
        result = static_cast<int>(static_cast<uint32_t>(product));
        count = static_cast<uint32_t>(byteCount / 0x50);
    }

    alignas(4) uint8_t entryCopy[0x50];
    for (uint32_t index = 0; index < count; ++index)
    {
        void* source =
            reinterpret_cast<void*(__fastcall*)(void*, void*, uint32_t)>(
                CSF_AT(void**, entries, 0)[0x08 / 4])(
                    entries, nullptr, index);

        C_0082d410_CopyConstruct(entryCopy, nullptr,
                                reinterpret_cast<int>(source));

        uint32_t color = CSF_AT(uint32_t, entryCopy, 0x4C);
        uint8_t colorBytes[4] = {
            static_cast<uint8_t>(color >> 16),
            static_cast<uint8_t>(color >> 8),
            static_cast<uint8_t>(color),
            static_cast<uint8_t>(color >> 24)
        };
        int packedColor;
        std::memcpy(&packedColor, colorBytes, sizeof(packedColor));

        result = reinterpret_cast<int>(
            SceneLoader_SetIndexedColor(
                reinterpret_cast<void*>(destination), nullptr,
                static_cast<int>(index), &packedColor));

        if (CSF_AT(uint32_t, entryCopy, 0x48) >= 0x10)
        {
            void* block = CSF_AT(void*, entryCopy, 0x34);
            result = reinterpret_cast<int>(block);
            if (block)
                result = Mem_FreeDebug(block);
        }

        bool releaseFirstString =
            CSF_AT(uint32_t, entryCopy, 0x1C) >= 0x10;

        CSF_AT(uint32_t, entryCopy, 0x48) = 15;
        CSF_AT(uint32_t, entryCopy, 0x44) = 0;
        CSF_AT(uint8_t, entryCopy, 0x34) = 0;

        if (releaseFirstString)
        {
            auto* copy = reinterpret_cast<C_0082d410*>(entryCopy);
            void* block = reinterpret_cast<void*>(copy->field_0008);
            result = reinterpret_cast<int>(block);
            if (block)
                result = Mem_FreeDebug(block);
        }

        CSF_AT(uint32_t, entryCopy, 0x1C) = 15;
        CSF_AT(uint32_t, entryCopy, 0x18) = 0;
        CSF_AT(uint8_t, entryCopy, 0x08) = 0;
    }

    return result;
}

HOOK(0x00501360, SceneLoader_LoadIndexedColors, "auto");
