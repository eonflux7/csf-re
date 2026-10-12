#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_AllocDebug, 0x00690DC0, (std::size_t size, int flags, int tag));
CSF_FUNC(int, __fastcall, MissionScript_ProcessSectionTable, 0x0069B460, (void* self, void* edx));

static int __cdecl MissionScript_CreateSection(int sectionType)
{
    int result = 0;

    // The original dispatches to small constructors by tail jump.
    switch (sectionType)
    {
    case 0:
    case 2:
    case 3:
    case 15:
        result = MissionScript_ProcessSectionTable(
            reinterpret_cast<void*>(CSF_ADDR_DATA(0x008B032C)), nullptr);
        if (result)
            CSF_AT(std::uint8_t, reinterpret_cast<void*>(result), 0x0) = 0;
        break;

    case 4:
        result = MissionScript_ProcessSectionTable(
            reinterpret_cast<void*>(CSF_ADDR_DATA(0x008B03F8)), nullptr);
        if (result)
        {
            void* section = reinterpret_cast<void*>(result);
            CSF_AT(std::uint8_t, section, 0x0) = 4;
            CSF_AT(void*, section, 0x4) = CSF_VTBL(0x0082D448);
            CSF_AT(int, section, 0x8) = -1;
            CSF_AT(int, section, 0xC) = -1;
            CSF_AT(int, section, 0x10) = -1;
        }
        break;

    case 5:
    case 20:
        result = Mem_AllocDebug(sectionType == 5 ? 0x18u : 0x10u, 0, 0);
        if (result)
        {
            void* section = reinterpret_cast<void*>(result);
            CSF_AT(std::uint8_t, section, 0x0) =
                static_cast<std::uint8_t>(sectionType);
            CSF_AT(int, section, 0x4) = 0;
            CSF_AT(int, section, 0x8) = 0;
            CSF_AT(int, section, 0xC) = 0;
        }
        break;

    case 11:
    case 16:
    case 23:
    case 18:
    case 37:
        result = Mem_AllocDebug(sectionType == 11 ? 0x24u : 0x20u, 0, 0);
        if (result)
        {
            void* section = reinterpret_cast<void*>(result);
            CSF_AT(std::uint8_t, section, 0x0) =
                static_cast<std::uint8_t>(sectionType == 16 ? 23 : sectionType);
            CSF_AT(int, section, 0x1C) = 15;
            CSF_AT(int, section, 0x18) = 0;
            CSF_AT(std::uint8_t, section, 0x8) = 0;
        }
        break;

    case 21:
        result = MissionScript_ProcessSectionTable(
            reinterpret_cast<void*>(CSF_ADDR_DATA(0x008B0370)), nullptr);
        if (result)
        {
            void* section = reinterpret_cast<void*>(result);
            CSF_AT(std::uint8_t, section, 0x0) = 21;
            CSF_AT(std::uint8_t, section, 0x1) = 0;
        }
        break;

    case 33:
    case 34:
    case 35:
        result = MissionScript_ProcessSectionTable(
            reinterpret_cast<void*>(CSF_ADDR_DATA(0x008B0370)), nullptr);
        if (result)
        {
            CSF_AT(std::uint8_t, reinterpret_cast<void*>(result), 0x0) =
                static_cast<std::uint8_t>(sectionType);
        }
        break;

    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 12:
    case 13:
    case 14:
    case 17:
    case 22:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:
    case 32:
    case 36:
    case 38:
    case 39:
    case 40:
    case 41:
    case 42:
    case 43:
    case 44:
    case 45:
    case 46:
    case 47:
    case 48:
    case 49:
    case 50:
        result = MissionScript_ProcessSectionTable(
            reinterpret_cast<void*>(CSF_ADDR_DATA(0x008B03B4)), nullptr);
        if (result)
        {
            void* section = reinterpret_cast<void*>(result);
            CSF_AT(std::uint8_t, section, 0x0) =
                static_cast<std::uint8_t>(sectionType == 6 ? 7 : sectionType);
            if (sectionType == 10)
                CSF_AT(int, section, 0x4) = -1;
            else if (sectionType == 22)
                CSF_AT(int, section, 0x4) = 0;
        }
        break;

    default:
        break;
    }

    return result;
}

HOOK(0x004723D0, MissionScript_CreateSection, "auto");
