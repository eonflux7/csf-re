#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __cdecl, RwFrameGetRoot, 0x00623740, (int a1));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GfxObject_CollectFrameEntries, 0x006B41F0, (int a1, void* a2));
CSF_FUNC(int, __cdecl, GfxObjectProps_StoreEntryTableValue, 0x006B88A0, (int offset, int value));
CSF_FUNC(uint32_t*, __fastcall, C_00861dd8_ConstructTrackEntry, 0x006E7E60, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_00861dd8_InitializeTrackEntry, 0x006E7F90, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __cdecl, IsHAnimSpecialTrackType, 0x006E8440, (int trackType));
CSF_FUNC(uint32_t*, __fastcall, GetHanimKeyframeEntry, 0x006E89A0, (void* self, void* , int frameIndex, uint32_t* outFirst, uint32_t* outSecond));

static char* __fastcall C_00860288_BuildEntryTable(
    C_00860288* self, void* /*edx*/)
{
    alignas(8) std::uint8_t entriesBytes[0x14];
    S_0041e5f0& entries =
        *reinterpret_cast<S_0041e5f0*>(entriesBytes);
    entries.field_0000 = 0;
    entries.field_0004 = 0;
    entries.field_0008 = 0;
    CSF_AT(int, entriesBytes, 0x0C) = 10;

    const int root = RwFrameGetRoot(static_cast<int>(self->field_003c));
    if (root)
        GfxObject_CollectFrameEntries(root, &entries);

    if (self->field_0028)
    {
        void* animation = reinterpret_cast<void*>(self->field_0028);
        int& trackCount = CSF_AT(int, entriesBytes, 0x10);
        trackCount = reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, animation, 0)[0x64 / 4])(animation, nullptr);

        for (int trackIndex = 0; trackIndex < trackCount; ++trackIndex)
        {
            std::uint32_t trackType = 0;
            std::uint32_t trackData = 0;
            GetHanimKeyframeEntry(
                reinterpret_cast<void*>(self->field_0028), nullptr,
                trackIndex, &trackType, &trackData);

            if (static_cast<std::uint8_t>(
                    IsHAnimSpecialTrackType(static_cast<int>(trackType))) &&
                trackData)
            {
                C_00861dd8* entry = nullptr;
                void* storage = Mem_Alloc(60);
                if (storage)
                {
                    entry = reinterpret_cast<C_00861dd8*>(
                        C_00861dd8_ConstructTrackEntry(storage, nullptr));
                }

                if (C_00861dd8_InitializeTrackEntry(
                        entry, nullptr, static_cast<int>(trackData),
                        static_cast<int>(trackType)))
                {
                    if (entries.field_0008 >= entries.field_0004)
                    {
                        // The packet also labels this callee address as a vtable.
                        (void)CSF_VTBL(0x0041E5F0);
                        S_0041e5f0_GrowArrayCapacity(
                            &entries, nullptr,
                            CSF_AT(int, entriesBytes, 0x0C));
                    }

                    auto* destination =
                        reinterpret_cast<std::uint32_t*>(entries.field_0000) +
                        entries.field_0008;
                    ++entries.field_0008;
                    *destination = reinterpret_cast<std::uintptr_t>(entry);
                }
                else if (entry)
                {
                    entry->vSlot4(1);
                }
            }
        }
    }

    const std::uint32_t entryCount = entries.field_0008;
    if (entryCount)
    {
        self->field_0024 = static_cast<std::uint16_t>(entryCount);
        self->field_0020 = reinterpret_cast<std::uintptr_t>(
            Mem_Alloc(4 * entryCount));

        for (std::uint32_t index = 0; index < entryCount; ++index)
        {
            auto* entry = reinterpret_cast<C_00861dd8*>(
                reinterpret_cast<std::uint32_t*>(entries.field_0000)[index]);
            char* properties = entry->vSlot18();
            GfxObjectProps_StoreEntryTableValue(
                CSF_AT(int, properties, 0x0C),
                static_cast<int>(reinterpret_cast<std::uintptr_t>(self)));

            reinterpret_cast<std::uint32_t*>(self->field_0020)[index] =
                reinterpret_cast<std::uint32_t*>(entries.field_0000)[index];
        }
    }

    char* buffer = reinterpret_cast<char*>(entries.field_0000);
    if (buffer)
    {
        return reinterpret_cast<char*>(
            static_cast<std::uintptr_t>(Mem_Free(buffer)));
    }
    return buffer;
}

HOOK(0x006B48D0, C_00860288_BuildEntryTable, "auto");
