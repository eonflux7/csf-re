#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x521BB0, ());
CSF_FUNC(void, __fastcall, MenuProgressInput_UpdateGlobalState, 0x521DE0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuOptions_LayoutOptionButtons, 0x53AD90, (CMenuOptions* self, void* edx));

// Returns the child screen object stored at +0x1C, re-read on every use.
static void* GetChildScreenObject(CMenuOptions* self)
{
    return reinterpret_cast<void*>(self->field_001c);
}

static int __fastcall CMenuOptions_SetupScreen(CMenuOptions* self, void* /*edx*/, int screenArg, int /*unusedArg*/)
{
    self->field_0014 = 0;
    self->field_0034 = 0;
    self->field_0020 = 0;
    self->field_0018 = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;

    // Virtual slot 0x40 on self (no arguments).
    void** selfVtbl = *reinterpret_cast<void***>(self);
    reinterpret_cast<void(__fastcall*)(void*, void*)>(selfVtbl[0x40 / 4])(self, nullptr);

    self->field_0014 = static_cast<uint32_t>(screenArg);

    int globalValue = GetGlobalValue_521BB0();
    MenuProgressInput_UpdateGlobalState(reinterpret_cast<void*>(static_cast<uintptr_t>(globalValue)), nullptr);

    // Child object virtual slot 0x16C (no arguments, result ignored).
    {
        void** childVtbl = *reinterpret_cast<void***>(GetChildScreenObject(self));
        reinterpret_cast<void(__fastcall*)(void*, void*)>(childVtbl[0x16C / 4])(GetChildScreenObject(self), nullptr);
    }

    // Child object virtual slot 0xFC: find "BotonVideo".
    {
        void** childVtbl = *reinterpret_cast<void***>(GetChildScreenObject(self));
        int buttonVideo = reinterpret_cast<int(__fastcall*)(void*, void*, const char*)>(childVtbl[0xFC / 4])(
            GetChildScreenObject(self), nullptr, CSF_STR(0x008426AC, "BotonVideo"));
        self->field_0024 = static_cast<uint32_t>(buttonVideo);
    }

    // Child object virtual slot 0xFC: find "BotonSonido".
    {
        void** childVtbl = *reinterpret_cast<void***>(GetChildScreenObject(self));
        int buttonSound = reinterpret_cast<int(__fastcall*)(void*, void*, const char*)>(childVtbl[0xFC / 4])(
            GetChildScreenObject(self), nullptr, CSF_STR(0x008426A0, "BotonSonido"));
        self->field_0028 = static_cast<uint32_t>(buttonSound);
    }

    // Child object virtual slot 0xFC: find "BotonJuego".
    {
        void** childVtbl = *reinterpret_cast<void***>(GetChildScreenObject(self));
        int buttonGame = reinterpret_cast<int(__fastcall*)(void*, void*, const char*)>(childVtbl[0xFC / 4])(
            GetChildScreenObject(self), nullptr, CSF_STR(0x00842694, "BotonJuego"));
        self->field_002c = static_cast<uint32_t>(buttonGame);
    }

    // Self virtual slot 0x44, passing the child screen object as its single stack argument.
    selfVtbl = *reinterpret_cast<void***>(self);
    reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(selfVtbl[0x44 / 4])(
        self, nullptr, GetChildScreenObject(self));

    CMenuOptions_LayoutOptionButtons(self, nullptr);

    return static_cast<int>(self->field_001c);
}

HOOK(0x0053AE60, CMenuOptions_SetupScreen, "auto");
