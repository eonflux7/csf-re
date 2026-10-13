#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, Hud_IsStringEqualIgnoreCase, 0x0042D5F0, (char* String1, char* String2));
CSF_FUNC(int, __fastcall, S_00692b80_ParseIntegerComponent, 0x00692C40, (S_00692b80* self, void*));
CSF_FUNC(double, __fastcall, ReadComponentFloat, 0x00692CC0, (std::uint32_t component));
CSF_FUNC(char, __fastcall, CopyAttributeText, 0x00692D40, (std::uint8_t* attribute, void* edx, char* buffer, int capacity));
CSF_FUNC(void, __fastcall, CoreZlibLog_ReadVector2, 0x00692F60, (void* self, void* edx, float* a2));
CSF_FUNC(int, __fastcall, DataNode_ReadVector3, 0x00693020, (void* self, void* edx, float* a2));

static void __fastcall NetPerfCounters_ParseValueComponent(
    void* self, void* /*edx*/, S_00692b80* component)
{
    alignas(8) std::uint8_t textBytes[0x100]{};
    char* text = reinterpret_cast<char*>(textBytes);

    switch (CSF_AT(std::uint32_t, self, 0x4))
    {
    case 4:
    case 6:
    case 7:
    case 0xA:
    case 0xC:
    case 0xD:
    case 0xE:
    case 0x10:
    case 0x20:
    case 0x21:
    case 0x22:
    case 0x23:
    case 0x24:
    case 0x26:
    case 0x27:
    case 0x28:
    case 0x29:
    case 0x2F:
    case 0x30:
    case 0x31:
    case 0x32:
        CSF_AT(float, self, 0x10) =
            static_cast<float>(S_00692b80_ParseIntegerComponent(component, nullptr));
        break;

    case 5:
    {
        alignas(8) std::uint8_t vectorBytes[0x8]{};
        CoreZlibLog_ReadVector2(
            component, nullptr, reinterpret_cast<float*>(vectorBytes));

        const std::uint32_t x = CSF_AT(std::uint32_t, vectorBytes, 0x0);
        const std::uint32_t y = CSF_AT(std::uint32_t, vectorBytes, 0x4);
        CSF_AT(std::uint32_t, self, 0x38) = x;
        CSF_AT(std::uint32_t, self, 0x3C) = y;
    }
        [[fallthrough]];

    case 8:
    case 9:
    case 0xB:
    case 0x12:
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x1A:
    case 0x1B:
    case 0x1D:
    case 0x1E:
    case 0x1F:
    case 0x25:
    case 0x2A:
    case 0x2B:
    case 0x2C:
    case 0x2D:
    case 0x2E:
        CopyAttributeText(
            reinterpret_cast<std::uint8_t*>(component), nullptr, text, 256);
        AssignStringRange_429B60(
            static_cast<std::uint8_t*>(self) + 0x18,
            nullptr, text, static_cast<unsigned int>(std::strlen(text)));
        break;

    case 0x14:
    {
        alignas(8) std::uint8_t vectorBytes[0xC]{};
        DataNode_ReadVector3(
            component, nullptr, reinterpret_cast<float*>(vectorBytes));

        const std::uint32_t x = CSF_AT(std::uint32_t, vectorBytes, 0x0);
        const std::uint32_t y = CSF_AT(std::uint32_t, vectorBytes, 0x4);
        const std::uint32_t z = CSF_AT(std::uint32_t, vectorBytes, 0x8);
        CSF_AT(std::uint32_t, self, 0x38) = x;
        CSF_AT(std::uint32_t, self, 0x3C) = y;
        CSF_AT(std::uint32_t, self, 0x40) = z;
        break;
    }

    case 0x15:
        CopyAttributeText(
            reinterpret_cast<std::uint8_t*>(component), nullptr, text, 256);
        CSF_AT(std::uint8_t, self, 0xA) =
            static_cast<std::uint8_t>(Hud_IsStringEqualIgnoreCase(
                text, const_cast<char*>(CSF_STR(0x00831610, "TRUE"))));
        break;

    case 0x16:
        CSF_AT(float, self, 0x10) = static_cast<float>(
            ReadComponentFloat(reinterpret_cast<std::uint32_t>(component)));
        break;

    default:
        return;
    }
}

HOOK(0x0078CC90, NetPerfCounters_ParseValueComponent, "auto");
