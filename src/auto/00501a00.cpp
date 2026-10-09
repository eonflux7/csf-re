#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));
CSF_FUNC(char, __fastcall, S_004ff0f0_CheckNameAndLoad, 0x004FF0F0, (void* self, void* edx, char* String1));
CSF_FUNC(unsigned int, __stdcall, SceneLoader_CopyRecordNameToString, 0x005011E0, (int a1, void* a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

CSF_GLOBAL(std::uint8_t, g_byte_82E65C, 0x0082E65C);

static char __fastcall S_004ff0f0_SetRecordNameAndLoad(
    S_004ff0f0* self, void* /*edx*/, C_0082d410* name)
{
    auto* storedName = reinterpret_cast<std::uint8_t*>(self) + 0x98;
    AssignStringSubstring(storedName, nullptr, name, 0, -1);

    char* emptyText = reinterpret_cast<char*>(&g_byte_82E65C);
    int comparison = CompareStringSubstring(
        storedName, nullptr, 0,
        CSF_AT(std::uint32_t, storedName, 0x14), emptyText, 0);
    if (comparison == 0)
        return static_cast<char>(comparison);

    AssignStringRange_429B60(
        reinterpret_cast<std::uint8_t*>(self) + 0x7C,
        nullptr, emptyText, 0);

    alignas(C_0082d410) std::uint8_t temporaryStorage[0x1C];
    auto* temporary = reinterpret_cast<C_0082d410*>(temporaryStorage);

    self->field_005c = 1;
    CSF_AT(std::uint32_t, temporary, 0x18) = 15;
    CSF_AT(std::uint32_t, temporary, 0x14) = 0;
    *reinterpret_cast<std::uint8_t*>(&temporary->field_0004) = 0;

    SceneLoader_CopyRecordNameToString(
        static_cast<int>(reinterpret_cast<std::uintptr_t>(storedName)),
        temporary);

    char* resolvedName;
    if (CSF_AT(std::uint32_t, temporary, 0x18) < 0x10)
        resolvedName = reinterpret_cast<char*>(&temporary->field_0004);
    else
        resolvedName = reinterpret_cast<char*>(temporary->field_0004);

    // The packet's CSF_VTBL(0x004FF0F0) entry is actually this direct call.
    char result = S_004ff0f0_CheckNameAndLoad(self, nullptr, resolvedName);

    if (CSF_AT(std::uint32_t, temporary, 0x18) >= 0x10)
    {
        std::uint32_t allocation = temporary->field_0004;
        result = static_cast<char>(allocation);
        if (allocation != 0)
            result = static_cast<char>(
                Mem_FreeDebug(reinterpret_cast<void*>(allocation)));
    }

    return result;
}

HOOK(0x00501A00, S_004ff0f0_SetRecordNameAndLoad, "auto");
