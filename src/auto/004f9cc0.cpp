#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __fastcall, ConstructWideString, 0x00696F50, (void* self, void* edx, wchar_t* String));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_MenuWidthScale, 0x0083F24C);
CSF_GLOBAL(float, g_MenuHeightScale, 0x0083F250);

static void* __fastcall S_004f21d0_DrawText(
    S_004f21d0* self, void* /*edx*/, float x, float y,
    S_004f21d0* text, int* color)
{
    // The string includes an allocator word before its inline storage.
    alignas(8) uint8_t stringBytes[0x1C];
    alignas(8) uint8_t colorBytes[0x18];
    alignas(8) uint8_t pairBytes[0x8];

    ConstructWideString(stringBytes, nullptr, reinterpret_cast<wchar_t*>(text));
    const uint32_t length = CSF_AT(uint32_t, stringBytes, 0x14);

    void* renderer = CSF_AT(void*, self, 0xF4);
    void* font =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, renderer, 0)[0x28 / 4])(renderer, nullptr);

    for (uint32_t index = 0; index < length; ++index)
    {
        void* characters = CSF_AT(uint32_t, stringBytes, 0x18) < 8
            ? static_cast<void*>(stringBytes + 4)
            : CSF_AT(void*, stringBytes, 0x4);
        const uint32_t character =
            reinterpret_cast<uint16_t*>(characters)[index];

        if (!((int (__fastcall*)(void*, void*, uint32_t))
                  CSF_AT(void**, font, 0)[0x10 / 4])
                 (font, nullptr, character))
        {
            characters = CSF_AT(uint32_t, stringBytes, 0x18) < 8
                ? static_cast<void*>(stringBytes + 4)
                : CSF_AT(void*, stringBytes, 0x4);
            reinterpret_cast<uint16_t*>(characters)[index] = 63;
        }
    }

    if (color)
    {
        CSF_AT(int, colorBytes, 0x0) = color[0];
        CSF_AT(int, colorBytes, 0x4) = color[1];
        CSF_AT(int, colorBytes, 0x8) = color[2];
        CSF_AT(int, colorBytes, 0xC) = color[3];
    }
    else
    {
        CSF_AT(float, colorBytes, 0x10) = 255.0f;
        CSF_AT(float, colorBytes, 0x14) = 255.0f;
        CSF_AT(float, colorBytes, 0x0) = 255.0f;
        CSF_AT(float, colorBytes, 0x4) = 255.0f;
        CSF_AT(float, colorBytes, 0x8) = 255.0f;
        CSF_AT(float, colorBytes, 0xC) = 255.0f;
    }

    renderer = CSF_AT(void*, self, 0xF4);
    ((void (__fastcall*)(void*, void*, void*))
        CSF_AT(void**, renderer, 0)[0x60 / 4])
        (renderer, nullptr, colorBytes);

    void* heightObject = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    void* widthObject = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    void* globalFirst = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    void* globalSecond = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());

    void* dimensions =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, heightObject, 0)[0x18 / 4])
            (heightObject, nullptr);
    const uint32_t heightBits = CSF_AT(uint32_t, dimensions, 0x4);

    dimensions =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, widthObject, 0)[0x18 / 4])
            (widthObject, nullptr);

    const int32_t horizontalExtent = static_cast<int32_t>(
        CSF_AT(uint32_t, globalFirst, 0x223C) -
        CSF_AT(uint32_t, globalSecond, 0x2234));
    const int64_t scaledExtent = static_cast<int64_t>(
        static_cast<float>(horizontalExtent) *
        (static_cast<float>(CSF_AT(int32_t, dimensions, 0)) *
         g_MenuWidthScale));

    CSF_AT(uint32_t, pairBytes, 0) = static_cast<uint32_t>(scaledExtent);
    CSF_AT(uint32_t, pairBytes, 4) = heightBits;
    renderer = CSF_AT(void*, self, 0xF4);
    ((void (__fastcall*)(void*, void*, void*))
        CSF_AT(void**, renderer, 0)[0x58 / 4])
        (renderer, nullptr, pairBytes);

    heightObject = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    widthObject = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);

    dimensions =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, heightObject, 0)[0x18 / 4])
            (heightObject, nullptr);
    const float heightScale =
        static_cast<float>(CSF_AT(int32_t, dimensions, 4)) *
        g_MenuHeightScale;

    dimensions =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, widthObject, 0)[0x18 / 4])
            (widthObject, nullptr);
    CSF_AT(float, pairBytes, 4) = heightScale;
    CSF_AT(float, pairBytes, 0) =
        static_cast<float>(CSF_AT(int32_t, dimensions, 0)) *
        g_MenuWidthScale;

    renderer = CSF_AT(void*, self, 0xF4);
    ((void (__fastcall*)(void*, void*, void*))
        CSF_AT(void**, renderer, 0)[0x98 / 4])
        (renderer, nullptr, pairBytes);

    CSF_AT(float, pairBytes, 0) = x;
    CSF_AT(float, pairBytes, 4) = y;
    renderer = CSF_AT(void*, self, 0xF4);
    ((void (__fastcall*)(void*, void*, void*))
        CSF_AT(void**, renderer, 0)[0x50 / 4])
        (renderer, nullptr, pairBytes);

    void* characters = CSF_AT(uint32_t, stringBytes, 0x18) < 8
        ? static_cast<void*>(stringBytes + 4)
        : CSF_AT(void*, stringBytes, 0x4);
    renderer = CSF_AT(void*, self, 0xF4);
    ((void (__fastcall*)(void*, void*, void*))
        CSF_AT(void**, renderer, 0)[0x40 / 4])
        (renderer, nullptr, characters);

    renderer = CSF_AT(void*, self, 0xF4);
    void* result =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, renderer, 0)[0x8 / 4])
            (renderer, nullptr);

    if (CSF_AT(uint32_t, stringBytes, 0x18) >= 8)
    {
        result = CSF_AT(void*, stringBytes, 0x4);
        if (result)
            return reinterpret_cast<void*>(Mem_FreeDebug(result));
    }
    return result;
}

HOOK(0x004F9CC0, S_004f21d0_DrawText, "auto");
