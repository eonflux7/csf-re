#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, MenuProgressInput_UpdateGlobalState, 0x00521DE0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuOptions_ApplyTargetOffset, 0x0053AD90, (void* self, void* edx));

template <typename Fn>
static Fn Slot(void* obj, int offset)
{
    return reinterpret_cast<Fn>(CSF_AT(void**, obj, 0)[offset / 4]);
}

// Sets a volume slider (+0x40 music, +0x44 effects): 30 steps, then position (steps - 1) * volume, truncated.
static void InitializeVolumeSlider(void* slider, float volume)
{
    Slot<void(__fastcall*)(void*, void*, int, int, int)>(slider, 0x260)(slider, nullptr, 30, 1, 0);
    // The original pushes 0 for slot 0x264, which takes no argument; slot 0x26C (two arguments) then takes that
    // word as its second one.
    const int steps = Slot<int(__fastcall*)(void*, void*)>(slider, 0x264)(slider, nullptr);
    const int position = static_cast<int>(static_cast<long long>(static_cast<double>(steps - 1) * volume));
    Slot<void(__fastcall*)(void*, void*, int, int)>(slider, 0x26C)(slider, nullptr, position, 0);
}

// Opens the sound options page on `owner`: reads the two volumes from the sound settings (+0x2C slots 0x18/0x20
// into +0x30/+0x34), sets up the sliders, the OK question, back button, acoustics group and sound switch, then
// refreshes the owner. Returns the page's panel (+0x1C). The second argument is unused.
static std::uint32_t __fastcall CMenuSoundOptions_InitializeControls(void* self, void* /*edx*/, std::uint32_t owner, int)
{
    Slot<void(__fastcall*)(void*, void*)>(self, 0x58)(self, nullptr);
    CSF_AT(std::uint32_t, self, 0x14) = owner;
    void* panel = CSF_AT(void*, self, 0x1C);
    Slot<void(__fastcall*)(void*, void*)>(panel, 0x16C)(panel, nullptr);
    MenuProgressInput_UpdateGlobalState(reinterpret_cast<void*>(GetGlobalValue_521BB0()), nullptr);

    if (void* settings = CSF_AT(void*, self, 0x2C))
    {
        CSF_AT(float, self, 0x30) = Slot<float(__fastcall*)(void*, void*)>(settings, 0x18)(settings, nullptr);
        settings = CSF_AT(void*, self, 0x2C);
        CSF_AT(float, self, 0x34) = Slot<float(__fastcall*)(void*, void*)>(settings, 0x20)(settings, nullptr);
        Slot<void(__fastcall*)(void*, void*)>(self, 0x60)(self, nullptr);
        Slot<void(__fastcall*)(void*, void*)>(self, 0x64)(self, nullptr);
        Slot<void(__fastcall*)(void*, void*)>(self, 0x68)(self, nullptr);
    }
    if (void* music = CSF_AT(void*, self, 0x40))
        InitializeVolumeSlider(music, CSF_AT(float, self, 0x30));
    if (void* effects = CSF_AT(void*, self, 0x44))
        InitializeVolumeSlider(effects, CSF_AT(float, self, 0x34));

    Slot<void(__fastcall*)(void*, void*)>(self, 0x50)(self, nullptr);
    Slot<void(__fastcall*)(void*, void*, std::uint32_t)>(self, 0x44)(self, nullptr, CSF_AT(std::uint32_t, self, 0x1C));
    Slot<void(__fastcall*)(void*, void*, std::uint32_t)>(self, 0x84)(self, nullptr, CSF_AT(std::uint32_t, self, 0x1C));
    Slot<void(__fastcall*)(void*, void*, std::uint32_t)>(self, 0x88)(self, nullptr, CSF_AT(std::uint32_t, self, 0x1C));
    Slot<void(__fastcall*)(void*, void*, std::uint32_t)>(self, 0x8C)(self, nullptr, CSF_AT(std::uint32_t, self, 0x1C));
    CMenuOptions_ApplyTargetOffset(self, nullptr);

    void* owner14 = CSF_AT(void*, self, 0x14);
    // Same stray push: the 0 pushed for slot 0x9C (no argument here, 0x41ECE0's owner class) is slot 0xA8's.
    Slot<void(__fastcall*)(void*, void*)>(owner14, 0x9C)(owner14, nullptr);
    owner14 = CSF_AT(void*, self, 0x14);
    Slot<void(__fastcall*)(void*, void*, int)>(owner14, 0xA8)(owner14, nullptr, 0);
    return CSF_AT(std::uint32_t, self, 0x1C);
}

HOOK(0x0053CDA0, CMenuSoundOptions_InitializeControls, "auto");
