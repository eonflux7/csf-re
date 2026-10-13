#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(unsigned int, __fastcall, MissionScript_BitArraySetCount, 0x00470F50, (S_00470440* self, void* , unsigned int newCount, char fillValue));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(std::uint32_t, __cdecl, CloseSceneFile, 0x00697980, (void* file));
CSF_FUNC(void*, __cdecl, CoreFiles_OpenSceneFile, 0x00698350, (const char* filename, char* mode, int openFlags));
CSF_FUNC(void*, __fastcall, S_006c20f0_Destroy, 0x006C2CF0, (void* self, void* edx));

CSF_GLOBAL(std::uint8_t, g_byte_82E65C, 0x0082E65C);

static char __fastcall C_00860954_LoadSceneEntries(
    C_00860954* self, void* /*edx*/, const char* filename)
{
    alignas(8) std::uint8_t seenBytes[0x14];
    auto& seen = *reinterpret_cast<S_00470440*>(seenBytes);
    seen.field_0000 = 0;
    seen.field_0008 = 0;
    CSF_AT(std::uint32_t, seenBytes, 0x0C) = 0;
    CSF_AT(std::uint32_t, seenBytes, 0x10) = 0;

    unsigned int entryCount = 0;
    if (self->field_0008)
    {
        entryCount = static_cast<unsigned int>(
            static_cast<std::int32_t>(self->field_000c - self->field_0008) / 36);
    }
    MissionScript_BitArraySetCount(&seen, nullptr, entryCount, 0);

    void* file = CoreFiles_OpenSceneFile(
        filename, const_cast<char*>(CSF_STR(0x0083E130, "r")), 0);
    if (!file)
    {
        if (seen.field_0008)
            Mem_FreeDebug(reinterpret_cast<void*>(seen.field_0008));
        return 0;
    }

    alignas(8) std::uint8_t nameBytes[0x100];
    alignas(8) std::uint8_t valueBytes[0x4];
    valueBytes[0] = 0;
    valueBytes[1] = 0;
    int nameLength = 0;

    reinterpret_cast<void (__fastcall*)(void*, void*, void*, int)>(
        CSF_AT(void**, file, 0)[8 / 4])(file, nullptr, &nameLength, 4);

    while (nameLength)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, void*, int)>(
            CSF_AT(void**, file, 0)[8 / 4])(file, nullptr, nameBytes, nameLength);
        nameBytes[nameLength] = 0;

        reinterpret_cast<void (__fastcall*)(void*, void*, void*, int)>(
            CSF_AT(void**, file, 0)[8 / 4])(file, nullptr, valueBytes, 2);

        unsigned int index = self->vSlot18(reinterpret_cast<char*>(nameBytes));
        if (index == static_cast<unsigned int>(-1))
        {
            self->vSlot10(reinterpret_cast<char*>(nameBytes),
                          reinterpret_cast<std::int16_t*>(valueBytes));
        }
        else
        {
            CSF_AT(std::uint32_t, reinterpret_cast<void*>(seen.field_0008),
                   4 * (index >> 5)) |= std::uint32_t{1} << (index & 0x1F);
        }

        reinterpret_cast<void (__fastcall*)(void*, void*, void*, int)>(
            CSF_AT(void**, file, 0)[8 / 4])(file, nullptr, &nameLength, 4);
    }

    CloseSceneFile(file);

    int index = static_cast<std::int32_t>(seen.field_0000 - 1);
    int entryOffset = 36 * index;
    for (; index >= 0; --index, entryOffset -= 36)
    {
        const unsigned int bitIndex = static_cast<unsigned int>(index);
        if ((CSF_AT(std::uint32_t, reinterpret_cast<void*>(seen.field_0008),
                    4 * (bitIndex >> 5)) &
             (std::uint32_t{1} << (bitIndex & 0x1F))) == 0)
        {
            void* ownedObject = CSF_AT(
                void*, reinterpret_cast<void*>(self->field_0008), entryOffset + 28);
            if (ownedObject)
            {
                void* block = CSF_AT(
                    void*, reinterpret_cast<void*>(self->field_0008), entryOffset + 28);
                S_006c20f0_Destroy(ownedObject, nullptr);
                Mem_Free(block);
                CSF_AT(std::uint32_t, reinterpret_cast<void*>(self->field_0008),
                       entryOffset + 28) = 0;
            }

            AssignStringRange_429B60(
                reinterpret_cast<std::uint8_t*>(self->field_0008) + entryOffset,
                nullptr, reinterpret_cast<char*>(&g_byte_82E65C), 0);
        }
    }

    self->vSlot14();
    if (seen.field_0008)
        Mem_FreeDebug(reinterpret_cast<void*>(seen.field_0008));
    return 1;
}

HOOK(0x006C9810, C_00860954_LoadSceneEntries, "auto");
