#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(char, __cdecl, CoreFilesystemRtfs_DestroyEntryRange, 0x0069EB70, (int a1, int a2));
CSF_FUNC(unsigned int, __fastcall, CoreFilesystemRtfs_ReserveEntries, 0x0069F0B0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(uint32_t*, __fastcall, CoreFilesystemRtfs_AppendEntry, 0x0069F250, (void* self, void* edx, void* a3));

CSF_GLOBAL(uint32_t, g_RtfsEntryOffsetBase, 0x008BB3FC);

static char __fastcall S_0069f3b0_LoadFileIndex(
    S_0069f3b0* self, void* /*edx*/, int /*unused*/)
{
    char filename[0x100];
    std::memset(filename, 0, sizeof(filename));

    void* entries = reinterpret_cast<uint8_t*>(self) + 0xC;
    unsigned int entryCount = 0;

    if (self->field_0010 != 0)
    {
        CoreFilesystemRtfs_DestroyEntryRange(
            static_cast<int>(self->field_0010),
            static_cast<int>(CSF_AT(uint32_t, self, 0x14)));

        if (self->field_0010 != 0)
            Mem_FreeDebug(reinterpret_cast<void*>(self->field_0010));
    }

    self->field_0010 = 0;
    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint32_t, self, 0x18) = 0;

    void* stream = reinterpret_cast<void*>(self->field_0004);
    reinterpret_cast<void(__fastcall*)(void*, void*, void*, int)>(
        CSF_AT(void**, stream, 0)[8 / 4])(
            stream, nullptr, &entryCount, 4);

    if (entryCount != 0)
    {
        CoreFilesystemRtfs_ReserveEntries(entries, nullptr, entryCount);

        // String, offset, length, trailing eight bytes, and loop counter
        // occupy the original contiguous stack storage.
        alignas(8) uint8_t entryBytes[0x30]{};
        C_0082d410& entryName =
            *reinterpret_cast<C_0082d410*>(entryBytes);
        CSF_AT(uint32_t, entryBytes, 0x2C) = 0;

        if (entryCount != 0)
        {
            do
            {
                CSF_AT(uint32_t, entryBytes, 0x18) = 15;
                CSF_AT(uint32_t, entryBytes, 0x14) = 0;
                CSF_AT(uint8_t, entryBytes, 0x4) = 0;
                CSF_AT(uint32_t, entryBytes, 0x1C) = 0;
                CSF_AT(uint32_t, entryBytes, 0x20) = 0;

                stream = reinterpret_cast<void*>(self->field_0004);
                reinterpret_cast<void(__fastcall*)(void*, void*, char*, int)>(
                    CSF_AT(void**, stream, 0)[32 / 4])(
                        stream, nullptr, filename, 255);

                AssignStringRange_429B60(
                    &entryName, nullptr, filename,
                    static_cast<unsigned int>(std::strlen(filename)));

                stream = reinterpret_cast<void*>(self->field_0004);
                reinterpret_cast<void(__fastcall*)(void*, void*, void*, int)>(
                    CSF_AT(void**, stream, 0)[8 / 4])(
                        stream, nullptr, entryBytes + 0x1C, 4);

                stream = reinterpret_cast<void*>(self->field_0004);
                reinterpret_cast<void(__fastcall*)(void*, void*, void*, int)>(
                    CSF_AT(void**, stream, 0)[8 / 4])(
                        stream, nullptr, entryBytes + 0x20, 4);

                stream = reinterpret_cast<void*>(self->field_0004);
                reinterpret_cast<void(__fastcall*)(void*, void*, void*, int)>(
                    CSF_AT(void**, stream, 0)[8 / 4])(
                        stream, nullptr, entryBytes + 0x24, 8);

                CSF_AT(uint32_t, entryBytes, 0x1C) -= g_RtfsEntryOffsetBase;
                CoreFilesystemRtfs_AppendEntry(entries, nullptr, &entryName);

                if (CSF_AT(uint32_t, entryBytes, 0x18) >= 0x10 &&
                    entryName.field_0004 != 0)
                {
                    Mem_FreeDebug(
                        reinterpret_cast<void*>(entryName.field_0004));
                }

                const uint32_t nextIndex =
                    CSF_AT(uint32_t, entryBytes, 0x2C) + 1;
                const bool moreEntries = nextIndex < entryCount;

                CSF_AT(uint32_t, entryBytes, 0x18) = 15;
                CSF_AT(uint32_t, entryBytes, 0x14) = 0;
                CSF_AT(uint8_t, entryBytes, 0x4) = 0;
                CSF_AT(uint32_t, entryBytes, 0x2C) = nextIndex;

                if (!moreEntries)
                    break;
            } while (true);
        }
    }

    stream = reinterpret_cast<void*>(self->field_0004);
    self->field_0008 =
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, stream, 0)[16 / 4])(stream, nullptr);
    return 1;
}

HOOK(0x0069F3B0, S_0069f3b0_LoadFileIndex, "auto");
