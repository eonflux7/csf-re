#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GlobalStateValue_50DCF0, 0x0050DCF0, (void* self, void* edx));

static void* __fastcall C_0085e7b8_GetSoundFilename(
    C_0085e7b8* self, void* /*edx*/, unsigned int index)
{
    auto* table = reinterpret_cast<C_0085e7b8*>(self->field_0004);
    const uint32_t firstRecord = table->field_0004;

    if (!firstRecord ||
        index >= static_cast<uint32_t>(
            static_cast<int32_t>(table->field_0008 - firstRecord) / 68))
    {
        return reinterpret_cast<void*>(CSF_ADDR_DATA(0x00899D7C));
    }

    auto* record = reinterpret_cast<uint8_t*>(table->field_0004 + 68 * index);
    const uint32_t firstFilename = CSF_AT(uint32_t, record, 0x08);
    if (!firstFilename)
        return reinterpret_cast<void*>(CSF_ADDR_DATA(0x00899D7C));

    const uint32_t filenameCount = static_cast<uint32_t>(
        static_cast<int32_t>(
            CSF_AT(uint32_t, record, 0x0C) - firstFilename) / 28);

    if (!filenameCount)
        return reinterpret_cast<void*>(CSF_ADDR_DATA(0x00899D7C));

    if (filenameCount == 1)
        return reinterpret_cast<void*>(CSF_AT(uint32_t, record, 0x08));

    const int globalState = GetGlobalState_50D190();
    const uint32_t selection = static_cast<uint32_t>(
        GlobalStateValue_50DCF0(reinterpret_cast<void*>(globalState), nullptr));

    return reinterpret_cast<void*>(
        CSF_AT(uint32_t, record, 0x08) + 28 * (selection % filenameCount));
}

HOOK(0x0051BA50, C_0085e7b8_GetSoundFilename, "auto");
