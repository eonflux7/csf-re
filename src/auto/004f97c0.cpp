#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_MenuWidthScale, 0x0083F24C);
CSF_GLOBAL(float, g_ConsoleLineScale, 0x0083F248);

static char __fastcall C_0083f1f8_AppendCharacter(
    C_0083f1f8* self, void* /*edx*/, int16_t character)
{
    char result = static_cast<char>(self->field_0010);
    if (!result || static_cast<int32_t>(self->field_00dc) >= 100)
        return result;

    void* firstObject = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    void* secondObject = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);

    void* firstState = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    void* secondState = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());

    int32_t* firstValue =
        reinterpret_cast<int32_t* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, firstObject, 0)[0x18 / 4])(firstObject, nullptr);
    int32_t* secondValue =
        reinterpret_cast<int32_t* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, secondObject, 0)[0x18 / 4])(secondObject, nullptr);

    const int32_t length = static_cast<int32_t>(self->field_00dc);
    const int32_t span = static_cast<int32_t>(
        CSF_AT(uint32_t, firstState, 0x223C) -
        CSF_AT(uint32_t, secondState, 0x2234));

    const int32_t available = static_cast<int32_t>(static_cast<int64_t>(
        static_cast<double>(span) *
        (static_cast<double>(*firstValue) * g_MenuWidthScale)));
    const int32_t lineWidth = static_cast<int32_t>(static_cast<int64_t>(
        static_cast<double>(*secondValue) * g_MenuWidthScale *
        g_ConsoleLineScale));

    if (length > available / lineWidth)
        ++self->field_00e8;

    CSF_AT(int16_t, self, 0x12 + static_cast<ptrdiff_t>(length) * 2) = character;

    const uint32_t newLength = self->field_00dc + 1;
    self->field_00dc = newLength;
    CSF_AT(int16_t, self,
        0x12 + static_cast<ptrdiff_t>(static_cast<int32_t>(newLength)) * 2) = 0;

    return static_cast<char>(newLength);
}

HOOK(0x004F97C0, C_0083f1f8_AppendCharacter, "auto");
