#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_0082d58c_ResetField0008, 0x00409290, (C_0082d58c* self, void*));
CSF_FUNC(void*, __fastcall, SpawnPoints_GrowEntryBuffer, 0x00551CF0, (void* self, void* , int extraEntries));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(uint32_t*, __fastcall, GfxScene_ConstructSceneInterval, 0x006CB160, (void* self, void* edx));
CSF_FUNC(char, __fastcall, GfxScene_InitializeSceneInterval, 0x006F6EB0, (void* self, void* edx, void** a2, int a3, int a4, char a5, char a6));

static char __fastcall GfxScene_AddSceneInterval(
    void* self, void* /*edx*/, S_006b0320* scene,
    int intervalStart, int intervalEnd, char optionA, char optionB)
{
    // The constructor performs the associated vtable initialization.
    (void)CSF_VTBL(0x006CB160);

    void* storage = Mem_Alloc(240);
    S_006b0320* interval = storage
        ? reinterpret_cast<S_006b0320*>(
              GfxScene_ConstructSceneInterval(storage, nullptr))
        : nullptr;

    char result = GfxScene_InitializeSceneInterval(
        interval, nullptr, reinterpret_cast<void**>(scene),
        intervalStart, intervalEnd, optionA, optionB);

    if (result)
    {
        if (CSF_AT(uint32_t, self, 0x7C) >=
            CSF_AT(uint32_t, self, 0x78))
        {
            SpawnPoints_GrowEntryBuffer(
                reinterpret_cast<uint8_t*>(self) + 0x74, nullptr,
                CSF_AT(int, self, 0x80));
        }

        uint32_t count = CSF_AT(uint32_t, self, 0x7C);
        uint8_t* entry =
            reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x74))
            + count * 8;

        ++count;
        CSF_AT(uint32_t, self, 0x7C) = count;
        CSF_AT(S_006b0320*, entry, 0) = interval;
        CSF_AT(uint32_t, entry, 4) = 0;
        return static_cast<char>(count);
    }

    if (interval)
    {
        C_0082d58c_ResetField0008(
            reinterpret_cast<C_0082d58c*>(interval), nullptr);
        if (interval->field_0000)
        {
            Mem_Free(reinterpret_cast<void*>(interval->field_0000));
            interval->field_0000 = 0;
        }
        result = static_cast<char>(Mem_Free(interval));
    }

    return result;
}

HOOK(0x006CCC80, GfxScene_AddSceneInterval, "auto");
