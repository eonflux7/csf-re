#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pOptions, 0x008BAF8C);

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(unsigned int, __cdecl, CoreFiles_FindNameIndex, 0x006977C0, (char* name, void* entries));
CSF_FUNC(std::uint32_t, __cdecl, CloseSceneFile, 0x00697980, (void* file));
CSF_FUNC(C_0082d410*, __cdecl, CoreFiles_CopyStringRecords, 0x00697A40, (int first, int last, C_0082d410* destination));
CSF_FUNC(char, __cdecl, MissionScript_DestroyStringRange, 0x00697A80, (int a1, int a2));
CSF_FUNC(char, __fastcall, CoreFiles_DestroySceneFileState, 0x0069F2E0, (void* self, void* edx));

static int __cdecl CoreFiles_ResetArchiveSelection(char* name)
{
    const unsigned int index = CoreFiles_FindNameIndex(
        name, reinterpret_cast<std::uint8_t*>(g_pOptions) + 0x50);
    if (index == static_cast<unsigned int>(-1))
        return -1;

    const std::uint32_t recordOffset = index * 32;
    std::uint8_t* record =
        CSF_AT(std::uint8_t*, g_pOptions, 0x54) + recordOffset;

    alignas(8) std::uint8_t temporaryBytes[0x24];
    C_0082d410& temporary =
        *reinterpret_cast<C_0082d410*>(temporaryBytes);

    CSF_AT(std::uint32_t, temporaryBytes, 0x18) = 15;
    CSF_AT(std::uint32_t, temporaryBytes, 0x14) = 0;
    *reinterpret_cast<std::uint8_t*>(&temporary.field_0004) = 0;
    AssignStringSubstring(&temporary, nullptr, record, 0,
                          static_cast<std::uint32_t>(-1));

    CSF_AT(void*, temporaryBytes, 0x1C) = CSF_AT(void*, record, 0x1C);
    CloseSceneFile(CSF_AT(void*, CSF_AT(void*, temporaryBytes, 0x1C), 0x04));

    if (CSF_AT(void*, temporaryBytes, 0x1C))
    {
        void* state = CSF_AT(void*, temporaryBytes, 0x1C);
        CoreFiles_DestroySceneFileState(state, nullptr);
        CoreMemory_Free(state);
        CSF_AT(void*, temporaryBytes, 0x1C) = nullptr;
    }

    std::uint8_t* entries =
        reinterpret_cast<std::uint8_t*>(g_pOptions) + 0x50;
    std::uint8_t* destination =
        CSF_AT(std::uint8_t*, g_pOptions, 0x54) + recordOffset;
    std::uint8_t* last = CSF_AT(std::uint8_t*, g_pOptions, 0x58);

    CoreFiles_CopyStringRecords(
        reinterpret_cast<int>(destination + 32),
        reinterpret_cast<int>(last),
        reinterpret_cast<C_0082d410*>(destination));

    last = CSF_AT(std::uint8_t*, entries, 0x08);

    // The supplied binding says char, but the callee and this caller
    // use the complete 32-bit return value.
    int result =
        reinterpret_cast<int(__cdecl*)(int, int)>(MissionScript_DestroyStringRange)(
            reinterpret_cast<int>(last - 32), reinterpret_cast<int>(last));

    CSF_AT(std::uint8_t*, entries, 0x08) -= 32;

    if (CSF_AT(std::uint32_t, temporaryBytes, 0x18) >= 0x10)
    {
        result = static_cast<int>(temporary.field_0004);
        if (temporary.field_0004)
            return Mem_FreeDebug(
                reinterpret_cast<void*>(temporary.field_0004));
    }

    return result;
}

HOOK(0x006980D0, CoreFiles_ResetArchiveSelection, "auto");
