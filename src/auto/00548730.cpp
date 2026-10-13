#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, MenuExtras_PerformAlternateAction, 0x00548650, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x006923C0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));

static char __fastcall S_00548430_HandleSelect(
    S_00548430* self, void* /*edx*/, int eventAddress)
{
    void* menu = reinterpret_cast<void*>(self->field_0018);
    int result = ((int(__fastcall*)(void*, void*))
        CSF_AT(void**, menu, 0)[260 / 4])(menu, nullptr);
    if (!result)
        return static_cast<char>(result);

    menu = reinterpret_cast<void*>(self->field_0018);
    void* event = reinterpret_cast<void*>(eventAddress);
    int selected = CSF_AT(int, event, 0x04);
    result = ((int(__fastcall*)(void*, void*, const char*))
        CSF_AT(void**, menu, 0)[252 / 4])(
            menu, nullptr, CSF_STR(0x00843C50, "Accept"));

    if (selected == result)
    {
        self->field_0046 = 1;
        return static_cast<char>(result);
    }

    selected = CSF_AT(int, event, 0x04);
    void* preview = CSF_AT(void*, self, 0x2C);
    if (static_cast<uint32_t>(selected) ==
        reinterpret_cast<uintptr_t>(preview))
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, preview, 0)[168 / 4])(preview, nullptr, 1);

        void* control = CSF_AT(void*, self, 0x30);
        self->field_0044 = 1;
        self->field_0045 = 1;
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, control, 0)[168 / 4])(control, nullptr, 1);

        control = CSF_AT(void*, self, 0x30);
        return static_cast<char>(
            ((int(__fastcall*)(void*, void*, int))
                CSF_AT(void**, control, 0)[200 / 4])(control, nullptr, 0));
    }

    menu = reinterpret_cast<void*>(self->field_0018);
    result = ((int(__fastcall*)(void*, void*, const char*))
        CSF_AT(void**, menu, 0)[252 / 4])(
            menu, nullptr, CSF_STR(0x00843C58, "MakingOf"));
    if (selected == result)
        return MenuExtras_PerformAlternateAction(self, nullptr);

    int index = CSF_AT(int, self, 0x28);
    if (index == -1)
        return static_cast<char>(index);

    uint32_t entriesAddress = CSF_AT(uint32_t, self, 0x38);
    uint32_t entryOffset = static_cast<uint32_t>(index) * 68;
    void* entry = reinterpret_cast<void*>(entriesAddress + entryOffset);
    int entryState = CSF_AT(int, entry, 0x24);
    if (entryState)
        return static_cast<char>(reinterpret_cast<uintptr_t>(entry));

    void* global = reinterpret_cast<void*>(GetGlobalInt());
    void* resolver = MenuControls_ResolveObject(global, nullptr);

    index = CSF_AT(int, self, 0x28);
    entriesAddress = CSF_AT(uint32_t, self, 0x38);
    entryOffset = static_cast<uint32_t>(index) * 68;
    entry = reinterpret_cast<void*>(entriesAddress + entryOffset);

    const char* name;
    if (CSF_AT(uint32_t, entry, 0x1C) < 0x10)
        name = reinterpret_cast<const char*>(entry) + 8;
    else
        name = CSF_AT(const char*, entry, 0x08);

    void* object = ((void*(__fastcall*)(void*, void*, const char*))
        CSF_AT(void**, resolver, 0)[64 / 4])(resolver, nullptr, name);
    if (!object)
        return 0;

    preview = CSF_AT(void*, self, 0x2C);
    ((void(__fastcall*)(void*, void*, void*))
        CSF_AT(void**, preview, 0)[580 / 4])(preview, nullptr, object);

    ReleaseObjectReference(static_cast<uint8_t*>(object) + 4, nullptr);

    preview = CSF_AT(void*, self, 0x2C);
    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, preview, 0)[168 / 4])(preview, nullptr, 0);

    void* control = CSF_AT(void*, self, 0x30);
    self->field_0044 = 1;
    self->field_0045 = 0;
    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, control, 0)[168 / 4])(control, nullptr, 0);

    control = CSF_AT(void*, self, 0x30);
    return static_cast<char>(
        ((int(__fastcall*)(void*, void*, int))
            CSF_AT(void**, control, 0)[200 / 4])(control, nullptr, 1));
}

HOOK(0x00548730, S_00548430_HandleSelect, "auto");
