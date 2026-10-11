#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint16_t*, __fastcall, CoreZlibLog_WriteWideString, 0x00694F10, (void* self, void* edx, void* a2, char* a3));
CSF_FUNC(unsigned int, __fastcall, CoreZlibLog_EndGroup, 0x00695260, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, CoreZlibLog_WriteInteger, 0x006952F0, (void* self, void* edx, int a2, char* a3));
CSF_FUNC(unsigned int, __fastcall, CoreZlibLog_WriteFloat, 0x00695330, (void* self, void* edx, float a2, char* a3));
CSF_FUNC(unsigned int, __fastcall, CoreZlibLog_WriteBoolean, 0x00695370, (void* self, void* edx, uint8_t a2, char* a3));
CSF_FUNC(unsigned int, __fastcall, CoreZlibLog_WriteString, 0x006953B0, (void* self, void* edx, char* Source, char* a3));
CSF_FUNC(int, __fastcall, CoreZlibLog_BeginGroup, 0x00695470, (void* self, void* edx, char* a2, int a3, char a4));

static uint16_t* __stdcall CoreZlibLog_WriteTypedValue(
    S_00694cd0* writer, void* descriptor)
{
    const uint32_t type = CSF_AT(uint32_t, descriptor, 0x0C) & 0xDFFFFFFF;

    switch (type)
    {
    case 2:
        return reinterpret_cast<uint16_t*>(CoreZlibLog_WriteBoolean(
            writer, nullptr,
            CSF_AT(uint8_t, CSF_AT(void*, descriptor, 0x04), 0x00),
            CSF_AT(char*, descriptor, 0x00)));

    case 4:
        return reinterpret_cast<uint16_t*>(CoreZlibLog_WriteInteger(
            writer, nullptr,
            CSF_AT(uint8_t, CSF_AT(void*, descriptor, 0x04), 0x00),
            CSF_AT(char*, descriptor, 0x00)));

    case 8:
    case 0x10:
    case 4096:
    case 0x4000:
    case 0x8000:
    case 0x20000:
    case 0x80000:
    case 0x1000000:
        return reinterpret_cast<uint16_t*>(CoreZlibLog_WriteInteger(
            writer, nullptr,
            CSF_AT(int, CSF_AT(void*, descriptor, 0x04), 0x00),
            CSF_AT(char*, descriptor, 0x00)));

    case 32:
        return reinterpret_cast<uint16_t*>(CoreZlibLog_WriteFloat(
            writer, nullptr,
            CSF_AT(float, CSF_AT(void*, descriptor, 0x04), 0x00),
            CSF_AT(char*, descriptor, 0x00)));

    case 0x40:
    {
        void* string = CSF_AT(void*, descriptor, 0x04);
        char* source;
        if (CSF_AT(uint32_t, string, 0x18) >= 0x10u)
            source = CSF_AT(char*, string, 0x04);
        else
            source = reinterpret_cast<char*>(string) + 0x04;

        return reinterpret_cast<uint16_t*>(CoreZlibLog_WriteString(
            writer, nullptr, source, CSF_AT(char*, descriptor, 0x00)));
    }

    case 512:
    case 1024:
    {
        void* object = CSF_AT(void*, descriptor, 0x04);
        auto* string = reinterpret_cast<uint8_t*>(object) + 0x41C;
        char* source;
        if (CSF_AT(uint32_t, object, 0x434) >= 0x10u)
            source = CSF_AT(char*, string, 0x04);
        else
            source = reinterpret_cast<char*>(string) + 0x04;

        return reinterpret_cast<uint16_t*>(CoreZlibLog_WriteString(
            writer, nullptr, source, CSF_AT(char*, descriptor, 0x00)));
    }

    case 0x80:
    {
        void* values = CSF_AT(void*, descriptor, 0x04);
        const float x = CSF_AT(float, values, 0x00);
        const float y = CSF_AT(float, values, 0x04);
        CoreZlibLog_BeginGroup(writer, nullptr, CSF_AT(char*, descriptor, 0x00), 1, 1);
        CoreZlibLog_WriteFloat(writer, nullptr, x, nullptr);
        CoreZlibLog_WriteFloat(writer, nullptr, y, nullptr);
        return reinterpret_cast<uint16_t*>(CoreZlibLog_EndGroup(writer, nullptr));
    }

    case 256:
    {
        void* values = CSF_AT(void*, descriptor, 0x04);
        const float x = CSF_AT(float, values, 0x00);
        const float y = CSF_AT(float, values, 0x04);
        const float z = CSF_AT(float, values, 0x08);
        CoreZlibLog_BeginGroup(writer, nullptr, CSF_AT(char*, descriptor, 0x00), 1, 1);
        CoreZlibLog_WriteFloat(writer, nullptr, x, nullptr);
        CoreZlibLog_WriteFloat(writer, nullptr, y, nullptr);
        CoreZlibLog_WriteFloat(writer, nullptr, z, nullptr);
        return reinterpret_cast<uint16_t*>(CoreZlibLog_EndGroup(writer, nullptr));
    }

    case 0x2000:
    case 0x10000:
    {
        void* values = CSF_AT(void*, descriptor, 0x04);
        const float x = CSF_AT(float, values, 0x00);
        const float y = CSF_AT(float, values, 0x04);
        const float z = CSF_AT(float, values, 0x08);
        const float w = CSF_AT(float, values, 0x0C);
        CoreZlibLog_BeginGroup(writer, nullptr, CSF_AT(char*, descriptor, 0x00), 1, 1);
        CoreZlibLog_WriteFloat(writer, nullptr, x, nullptr);
        CoreZlibLog_WriteFloat(writer, nullptr, y, nullptr);
        CoreZlibLog_WriteFloat(writer, nullptr, z, nullptr);
        CoreZlibLog_WriteFloat(writer, nullptr, w, nullptr);
        return reinterpret_cast<uint16_t*>(CoreZlibLog_EndGroup(writer, nullptr));
    }

    case 0x800000:
    {
        void* string = CSF_AT(void*, descriptor, 0x04);
        void* source;
        if (CSF_AT(uint32_t, string, 0x18) < 8u)
            source = reinterpret_cast<uint8_t*>(string) + 0x04;
        else
            source = CSF_AT(void*, string, 0x04);

        return CoreZlibLog_WriteWideString(
            writer, nullptr, source, CSF_AT(char*, descriptor, 0x00));
    }

    default:
        return reinterpret_cast<uint16_t*>(descriptor);
    }
}

HOOK(0x006A20E0, CoreZlibLog_WriteTypedValue, "auto");
