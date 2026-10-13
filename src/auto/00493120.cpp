#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_0048f360_ResetState_48F360, 0x0048F360, (S_0048f360* self, void*));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_MaxPercent, 0x0082E120);
CSF_GLOBAL(float, g_DefaultCoreValue, 0x00833C54);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

// The packet also lists the reset callee address as a vtable reference.
[[maybe_unused]] static const auto kResetStateVtable = CSF_VTBL(0x0048F360);

static int __fastcall S_0048f360_ResetCoreState(
    S_0048f360* self, void* /*edx*/)
{
    S_0048f360_ResetState_48F360(self, nullptr);

    auto* owner = reinterpret_cast<std::uint8_t*>(self) - 12;
    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, owner, 0)[0xB4 / 4])(owner, nullptr, 0);

    float coreValue = CSF_AT(
        float, reinterpret_cast<void*>(self->field_0020), 0xE4);
    if (!(coreValue > g_fZero))
        coreValue = self->field_01b8 ? g_MaxPercent : g_DefaultCoreValue;
    CSF_AT(float, self, 0x1C8) = coreValue;

    auto* secondaryBase = owner + 4;
    if (CSF_AT(std::uint32_t, self, 0x1CC))
    {
        void** vtable = CSF_AT(void**, secondaryBase, 0);
        CSF_AT(std::uint32_t, self, 0x1CC) = 0;
        reinterpret_cast<void (__fastcall*)(void*, void*)>(
            vtable[0x34 / 4])(secondaryBase, nullptr);
    }

    self->field_01d0 = 0;
    if (CSF_AT(std::uint32_t, self, 0x1D4))
    {
        void** vtable = CSF_AT(void**, secondaryBase, 0);
        CSF_AT(std::uint32_t, self, 0x1D4) = 0;
        reinterpret_cast<void (__fastcall*)(void*, void*)>(
            vtable[0x38 / 4])(secondaryBase, nullptr);
    }

    self->field_01e4 = 0;
    self->field_01ac = -1;
    self->field_01b0 = -1;
    self->field_01e8 = -1;
    self->field_01ec = -1;
    self->field_01b4 = 0;
    self->field_0178 = 0;
    self->field_017c = 0;
    self->field_01f0 = 0;
    self->field_01f4 = 0;
    self->field_01d8 = 1;
    CSF_AT(float, self, 0x1BC) = g_ZeroVectorX;
    CSF_AT(float, self, 0x1C0) = g_ZeroVectorY;
    CSF_AT(float, self, 0x1C4) = g_ZeroVectorZ;
    self->field_01fc = static_cast<std::uint16_t>(-1);
    self->field_01b8 = 0;

    const std::uint32_t flags = self->field_01e4 & 0xFFFFFFF5;
    self->field_01e4 &= ~2u;
    self->field_01e4 = flags;
    self->field_01e4 = flags | 4;
    return static_cast<int>(flags | 4);
}

HOOK(0x00493120, S_0048f360_ResetCoreState, "auto");
