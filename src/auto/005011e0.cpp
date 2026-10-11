#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void, __fastcall, ResetInnerBuffer, 0x00501170, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(std::uint32_t, __fastcall, SaveGame_DestroyRecord, 0x00500A80, (void* record, void* edx));

// Original: unsigned int __stdcall sub_5011E0(int a1, C_0082d410 *a2); ret 8
static unsigned int __stdcall SceneLoader_CopyRecordNameToString(int recordId, C_0082d410* outString)
{
    // sized by the original's frame (0xC8 bytes): struct sizes in auto_classes.h are lower bounds
    alignas(8) uint8_t recordBytes[0xC8];
    S_004f21d0& record = *reinterpret_cast<S_004f21d0*>(recordBytes);

    ResetInnerBuffer(&record, nullptr);

    // Global manager at GetGlobalValue_8B056C()->+4, slot 7 (offset 0x1C) of its vtable.
    void* globalBase = reinterpret_cast<void*>(static_cast<std::uintptr_t>(GetGlobalValue_8B056C()));
    void* manager = CSF_AT(void*, globalBase, 4);
    using LoadRecordFn = void(__fastcall*)(void* self, void* edx, S_004f21d0* out, int id);
    auto loadRecord = reinterpret_cast<LoadRecordFn>(CSF_AT(void**, manager, 0)[0x1C / 4]);
    loadRecord(manager, nullptr, &record, recordId);

    // push -1; push 0; push &record+0x6C; ecx = outString
    AssignStringSubstring(outString, nullptr, &CSF_AT(char, &record, 0x6C), 0u, 0xFFFFFFFFu);

    // lea ecx, [ebp-0xC8]: the record itself is `this`
    return SaveGame_DestroyRecord(&record, nullptr);
}

HOOK(0x005011E0, SceneLoader_CopyRecordNameToString, "auto");

