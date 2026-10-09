#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callee bindings (one name and signature per address).
CSF_FUNC(int, __cdecl, Mem_AllocDebug, 0x00690DC0, (std::size_t size, int flags, int tag));
CSF_FUNC(int, __fastcall, MissionScript_ProcessSectionTable, 0x0069B460, (void* self, void* edx));

// Section table objects referenced by the switch cases.
// Each is passed by address as `this` to ProcessSectionTable.
static void* SectionTableFor1() { return reinterpret_cast<void*>(CSF_ADDR_DATA(0x008B032C)); }
static void* SectionTableFor2() { return reinterpret_cast<void*>(CSF_ADDR_DATA(0x008B0370)); }
static void* SectionTableFor8() { return reinterpret_cast<void*>(CSF_ADDR_DATA(0x008B03B4)); }
static void* SectionTableFor12() { return reinterpret_cast<void*>(CSF_ADDR_DATA(0x008B02E8)); }
static void* SectionTableFor20() { return reinterpret_cast<void*>(CSF_ADDR_DATA(0x008B03F8)); }

// Maps a section kind id to its parser table; any other id falls back to a debug allocation of that size.
static int __cdecl MissionScript_DispatchSectionTable(std::size_t sectionKind) {
    int result;
    switch (sectionKind) {
    case 1u:
        result = MissionScript_ProcessSectionTable(SectionTableFor1(), nullptr);
        break;
    case 2u:
        result = MissionScript_ProcessSectionTable(SectionTableFor2(), nullptr);
        break;
    case 8u:
        result = MissionScript_ProcessSectionTable(SectionTableFor8(), nullptr);
        break;
    case 0xCu:
        result = MissionScript_ProcessSectionTable(SectionTableFor12(), nullptr);
        break;
    case 0x14u:
        result = MissionScript_ProcessSectionTable(SectionTableFor20(), nullptr);
        break;
    default:
        result = Mem_AllocDebug(sectionKind, 0, 0);
        break;
    }
    return result;
}

HOOK(0x00471450, MissionScript_DispatchSectionTable, "auto");
