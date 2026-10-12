#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, DevConsole_DrawTextLine, 0x004FA900,
         (void* self, void* edx, int a2, int a3, int* a4));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(unsigned int, __cdecl, CopyTrucoStringToWide, 0x00695590,
         (const char* src, uint16_t* dst, int maxCount));
CSF_FUNC(void, __cdecl, AppendWideString, 0x00695700,
         (std::uint16_t* destination, const std::uint16_t* source));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

static void* DevConsole_GetObjectDimensions(void* object)
{
    return ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, object, 0)[6])(object, nullptr);
}

static char __fastcall S_004fa900_RefreshConsole(S_004fa900* self, void* /*edx*/)
{
    const char enabled = static_cast<char>(self->field_0010);
    if (!enabled)
        return enabled;

    // The position/size pair overlaps the last two components of the color.
    alignas(8) std::uint8_t geometryBytes[0x10];
    auto* pair = reinterpret_cast<std::uint32_t*>(geometryBytes + 8);
    auto* color = reinterpret_cast<float*>(geometryBytes);

    void* panel = reinterpret_cast<void*>(self->field_018c);
    pair[0] = 0;
    pair[1] = 0;
    ((void (__fastcall*)(void*, void*, std::uint32_t*))
     CSF_AT(void**, panel, 0)[7])(panel, nullptr, pair);

    void* heightObject = LookupObjectById(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    void* widthObject = LookupObjectById(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    void* dimensions = DevConsole_GetObjectDimensions(heightObject);
    const std::uint32_t halfHeight = CSF_AT(std::uint32_t, dimensions, 4) >> 1;
    dimensions = DevConsole_GetObjectDimensions(widthObject);
    pair[0] = CSF_AT(std::uint32_t, dimensions, 0);
    pair[1] = halfHeight;
    panel = reinterpret_cast<void*>(self->field_018c);
    ((void (__fastcall*)(void*, void*, std::uint32_t*))
     CSF_AT(void**, panel, 0)[9])(panel, nullptr, pair);

    color[0] = 0.0f;
    color[1] = 0.0f;
    color[2] = 0.0f;
    color[3] = 127.0f;
    panel = reinterpret_cast<void*>(self->field_018c);
    ((void (__fastcall*)(void*, void*, float*))
     CSF_AT(void**, panel, 0)[11])(panel, nullptr, color);

    panel = reinterpret_cast<void*>(self->field_018c);
    ((void (__fastcall*)(void*, void*))
     CSF_AT(void**, panel, 0)[2])(panel, nullptr);

    void* viewport = LookupObjectById(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    dimensions = DevConsole_GetObjectDimensions(viewport);
    pair[0] = 0;
    pair[1] = (CSF_AT(std::uint32_t, dimensions, 4) >> 1) - 3;
    panel = reinterpret_cast<void*>(self->field_018c);
    ((void (__fastcall*)(void*, void*, std::uint32_t*))
     CSF_AT(void**, panel, 0)[7])(panel, nullptr, pair);

    viewport = LookupObjectById(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    dimensions = DevConsole_GetObjectDimensions(viewport);
    pair[0] = CSF_AT(std::uint32_t, dimensions, 0);
    pair[1] = 3;
    panel = reinterpret_cast<void*>(self->field_018c);
    ((void (__fastcall*)(void*, void*, std::uint32_t*))
     CSF_AT(void**, panel, 0)[9])(panel, nullptr, pair);

    color[0] = 255.0f;
    color[1] = 0.0f;
    color[2] = 0.0f;
    color[3] = 127.0f;
    panel = reinterpret_cast<void*>(self->field_018c);
    ((void (__fastcall*)(void*, void*, float*))
     CSF_AT(void**, panel, 0)[11])(panel, nullptr, color);

    panel = reinterpret_cast<void*>(self->field_018c);
    ((void (__fastcall*)(void*, void*))
     CSF_AT(void**, panel, 0)[2])(panel, nullptr);

    viewport = LookupObjectById(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    dimensions = DevConsole_GetObjectDimensions(viewport);
    int textY = static_cast<std::int32_t>(
        (CSF_AT(std::uint32_t, dimensions, 4) >> 1) - 55);

    if (self->field_0174)
    {
        viewport = LookupObjectById(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
        dimensions = DevConsole_GetObjectDimensions(viewport);
        const std::uint64_t scaledHeight =
            std::uint64_t{0xCCCCCCCDu}
            * (CSF_AT(std::uint32_t, dimensions, 4) >> 1);
        const int capacity = static_cast<std::int32_t>(
            (static_cast<std::uint32_t>(scaledHeight >> 32) >> 4) - 2);
        const int available = static_cast<std::int32_t>(
            self->field_0174 - self->field_0160);
        pair[1] = static_cast<std::uint32_t>(
            capacity >= available ? available : capacity);

        for (int row = 0; row < static_cast<std::int32_t>(pair[1]); ++row)
        {
            const std::uint32_t lineAddress =
                304 * (pair[1] + self->field_0160 - static_cast<std::uint32_t>(row))
                + self->field_016c - 304;
            DevConsole_DrawTextLine(self, nullptr, 10, textY,
                                    reinterpret_cast<int*>(lineAddress));
            textY -= 20;
        }
    }

    // DrawTextLine consumes a style integer followed immediately by wide text.
    alignas(8) std::uint8_t promptBytes[0x130];
    alignas(8) std::uint8_t caretBytes[0xC];
    auto* promptLine = reinterpret_cast<int*>(promptBytes);
    auto* promptText = reinterpret_cast<std::uint16_t*>(promptBytes + 4);
    auto* caretText = reinterpret_cast<std::uint16_t*>(caretBytes);

    promptLine[0] = 3;
    const char* caret = self->field_0158
        ? CSF_STR(0x0083F264, "_")
        : CSF_STR(0x0083F284, " ");
    CopyTrucoStringToWide(caret, caretText, 2);
    CopyTrucoStringToWide(CSF_STR(0x0083F280, "]: "), promptText, 4);
    AppendWideString(promptText,
                     &CSF_AT(std::uint16_t, self, 0x18 + 2 * self->field_0164));
    AppendWideString(promptText, caretText);

    viewport = LookupObjectById(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    dimensions = DevConsole_GetObjectDimensions(viewport);
    const int promptY = static_cast<std::int32_t>(
        (CSF_AT(std::uint32_t, dimensions, 4) >> 1) - 35);
    return static_cast<char>(
        DevConsole_DrawTextLine(self, nullptr, 10, promptY, promptLine));
}

HOOK(0x004FAC20, S_004fa900_RefreshConsole, "auto");
