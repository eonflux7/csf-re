#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall S_0050af90_ApplyVideoPreset(
    S_0050af90* self, void* /*edx*/, int preset)
{
    using ResolutionSetter = void (__fastcall*)(S_0050af90*, void*, int, int);
    using OptionSetter = void (__fastcall*)(S_0050af90*, void*, int);
    using FinalOptionSetter = int (__fastcall*)(S_0050af90*, void*, int);

    switch (preset)
    {
    case 3:
        reinterpret_cast<ResolutionSetter>(
            reinterpret_cast<void**>(self->field_0000)[0x24 / 4])(self, nullptr, 1024, 768);
        reinterpret_cast<OptionSetter>(
            reinterpret_cast<void**>(self->field_0000)[0x44 / 4])(self, nullptr, 2);
        reinterpret_cast<OptionSetter>(
            reinterpret_cast<void**>(self->field_0000)[0x54 / 4])(self, nullptr, 2);
        reinterpret_cast<OptionSetter>(
            reinterpret_cast<void**>(self->field_0000)[0x64 / 4])(self, nullptr, 2);
        reinterpret_cast<OptionSetter>(
            reinterpret_cast<void**>(self->field_0000)[0x5C / 4])(self, nullptr, 2);
        reinterpret_cast<OptionSetter>(
            reinterpret_cast<void**>(self->field_0000)[0x4C / 4])(self, nullptr, 3);
        reinterpret_cast<OptionSetter>(
            reinterpret_cast<void**>(self->field_0000)[0x74 / 4])(self, nullptr, 1);
        return reinterpret_cast<FinalOptionSetter>(
            reinterpret_cast<void**>(self->field_0000)[0x6C / 4])(self, nullptr, 2);

    case 2:
        reinterpret_cast<ResolutionSetter>(
            reinterpret_cast<void**>(self->field_0000)[0x24 / 4])(self, nullptr, 800, 600);
        reinterpret_cast<OptionSetter>(
            reinterpret_cast<void**>(self->field_0000)[0x44 / 4])(self, nullptr, 1);
        reinterpret_cast<OptionSetter>(
            reinterpret_cast<void**>(self->field_0000)[0x54 / 4])(self, nullptr, 1);
        reinterpret_cast<OptionSetter>(
            reinterpret_cast<void**>(self->field_0000)[0x64 / 4])(self, nullptr, 1);
        reinterpret_cast<OptionSetter>(
            reinterpret_cast<void**>(self->field_0000)[0x5C / 4])(self, nullptr, 1);
        reinterpret_cast<OptionSetter>(
            reinterpret_cast<void**>(self->field_0000)[0x4C / 4])(self, nullptr, 2);
        reinterpret_cast<OptionSetter>(
            reinterpret_cast<void**>(self->field_0000)[0x74 / 4])(self, nullptr, 1);
        return reinterpret_cast<FinalOptionSetter>(
            reinterpret_cast<void**>(self->field_0000)[0x6C / 4])(self, nullptr, 1);

    case 1:
        reinterpret_cast<ResolutionSetter>(
            reinterpret_cast<void**>(self->field_0000)[0x24 / 4])(self, nullptr, 640, 480);
        reinterpret_cast<OptionSetter>(
            reinterpret_cast<void**>(self->field_0000)[0x44 / 4])(self, nullptr, 0);
        reinterpret_cast<OptionSetter>(
            reinterpret_cast<void**>(self->field_0000)[0x54 / 4])(self, nullptr, 0);
        reinterpret_cast<OptionSetter>(
            reinterpret_cast<void**>(self->field_0000)[0x64 / 4])(self, nullptr, 0);
        reinterpret_cast<OptionSetter>(
            reinterpret_cast<void**>(self->field_0000)[0x5C / 4])(self, nullptr, 0);
        reinterpret_cast<OptionSetter>(
            reinterpret_cast<void**>(self->field_0000)[0x4C / 4])(self, nullptr, 1);
        reinterpret_cast<OptionSetter>(
            reinterpret_cast<void**>(self->field_0000)[0x74 / 4])(self, nullptr, 0);
        return reinterpret_cast<FinalOptionSetter>(
            reinterpret_cast<void**>(self->field_0000)[0x6C / 4])(self, nullptr, 0);

    default:
        return preset;
    }
}

HOOK(0x0050AF90, S_0050af90_ApplyVideoPreset, "auto");
