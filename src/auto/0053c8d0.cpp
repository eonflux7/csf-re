#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

using AcousticDispatchTable = std::uint32_t[4];
CSF_GLOBAL(AcousticDispatchTable, g_AcousticSelectionDispatch, 0x0053C9A0);

static int __fastcall CMenuSoundOptions_InitializeAcousticRadioGroup(
    CMenuSoundOptions* self, void* /*edx*/, int /*unused*/)
{
    // The original uses this table to dispatch the four selection cases.
    (void)&g_AcousticSelectionDispatch;

    if (!self->field_002c)
        return 0;

    void* controls = reinterpret_cast<void*>(self->field_001c);
    void* group =
        ((void*(__fastcall*)(void*, void*, const char*))
            CSF_AT(void**, controls, 0)[0xFC / 4])(
                controls, nullptr,
                CSF_STR(0x00842A6C, "GroupRadioAcoustic"));

    self->field_0020 = reinterpret_cast<std::uint32_t>(group);
    if (!group)
        return 0;

    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, group, 0)[0x29C / 4])(
            group, nullptr, 0x534F3035);

    group = reinterpret_cast<void*>(self->field_0020);
    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, group, 0)[0x29C / 4])(
            group, nullptr, 0x534F3036);

    group = reinterpret_cast<void*>(self->field_0020);
    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, group, 0)[0x29C / 4])(
            group, nullptr, 0x534F3037);

    group = reinterpret_cast<void*>(self->field_0020);
    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, group, 0)[0x29C / 4])(
            group, nullptr, 0x534F3038);

    void* settings = reinterpret_cast<void*>(self->field_002c);
    const int mode =
        ((int(__fastcall*)(void*, void*))
            CSF_AT(void**, settings, 0)[0x28 / 4])(settings, nullptr);

    const std::uint32_t selection = static_cast<std::uint32_t>(mode) - 1u;
    if (selection > 3u)
        return static_cast<std::int32_t>(selection);

    group = reinterpret_cast<void*>(self->field_0020);
    return ((int(__fastcall*)(void*, void*, int))
        CSF_AT(void**, group, 0)[0x2A4 / 4])(
            group, nullptr, 0x534F3035 + static_cast<int>(selection));
}

HOOK(0x0053C8D0, CMenuSoundOptions_InitializeAcousticRadioGroup, "auto");
