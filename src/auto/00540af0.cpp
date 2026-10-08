#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callees, bound by address.
CSF_FUNC(int, __cdecl, GetGlobalValue, 0x521BB0, ());
CSF_FUNC(void, __fastcall, GlobalValueObject_Apply, 0x521DE0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuMpScoreBar_UpdateDisplay, 0x540940, (CMenuMpScoreBar* self, void* edx));

// The menu object at +0x30 is re-read from the score bar each time the original reloads it.
static void* GetMenuObject(CMenuMpScoreBar* self)
{
    return reinterpret_cast<void*>(self->field_0030);
}

// Vtable slot +0x0FC (index 63): __thiscall (const char* childName) -> int.
static int MenuObject_LookupByName(void* menuObject, const char* childName)
{
    return ((int(__fastcall*)(void*, void*, const char*))CSF_AT(void**, menuObject, 0)[63])(menuObject, nullptr, childName);
}

static int __fastcall CMenuMpScoreBar_InitializeControls(CMenuMpScoreBar* self, void* /*edx*/, int a2, [[maybe_unused]] int a3)
{
    self->field_0038 = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_002c = static_cast<uint32_t>(a2);

    GlobalValueObject_Apply(reinterpret_cast<void*>(GetGlobalValue()), nullptr);

    // Vtable slot +0x16C (index 91): __thiscall () -> void.
    {
        void* menuObject = GetMenuObject(self);
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, menuObject, 0)[91])(menuObject, nullptr);
    }

    self->field_0014 = static_cast<uint32_t>(MenuObject_LookupByName(GetMenuObject(self), CSF_STR(0x843188, "EstaticoTiempo")));
    self->field_0018 = static_cast<uint32_t>(MenuObject_LookupByName(GetMenuObject(self), CSF_STR(0x843178, "EstaticoAliados")));
    self->field_001c = static_cast<uint32_t>(MenuObject_LookupByName(GetMenuObject(self), CSF_STR(0x843164, "EstaticoAlemanes")));
    self->field_0010 = static_cast<uint32_t>(MenuObject_LookupByName(GetMenuObject(self), CSF_STR(0x843154, "EstaticoModo")));
    self->field_0020 = static_cast<uint32_t>(MenuObject_LookupByName(GetMenuObject(self), CSF_STR(0x843148, "LogoAliados")));
    self->field_0024 = static_cast<uint32_t>(MenuObject_LookupByName(GetMenuObject(self), CSF_STR(0x843138, "LogoAlemanes")));

    CMenuMpScoreBar_UpdateDisplay(self, nullptr);

    // Vtable slot +0x0A8 (index 42): __thiscall (int 0) -> void.
    {
        void* menuObject = GetMenuObject(self);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, menuObject, 0)[42])(menuObject, nullptr, 0);
    }

    return static_cast<int>(self->field_0030);
}

HOOK(0x540AF0, CMenuMpScoreBar_InitializeControls, "auto");
