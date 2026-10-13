#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, SkinGeometry_SetMaterialAlpha, 0x006C4E60, (int atomicPtr, int alpha));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(void, __cdecl, InvokeTargetFunction, 0x006C5B20, (int atomicPtr));
CSF_FUNC(char, __fastcall, GlobalHud_Commit_6C7C30, 0x006C7C30, (C_0085e7b8* self, void*));
CSF_FUNC(int, __fastcall, C_0085e7b8_AttachWidget, 0x006C8420, (C_0085e7b8* self, void* , C_0085e7b8* widget));

static char __fastcall GlobalHud_RefreshWidgets(void* self, void* /*edx*/)
{
    char result = CSF_AT(char, self, 0x01);
    CSF_AT(char, self, 0x48) = 1;

    if (result)
    {
        void* hud = reinterpret_cast<void*>(GetGlobalValue_6C5760());
        C_0085e7b8_AttachWidget(
            CSF_AT(C_0085e7b8*, hud, 0x04), nullptr,
            reinterpret_cast<C_0085e7b8*>(
                static_cast<uint8_t*>(self) + 0x50));

        for (uint32_t i = 0; i < CSF_AT(uint32_t, self, 0x40); ++i)
        {
            void* entry = CSF_AT(void**, self, 0x38)[i];
            int atomic = CSF_AT(int, entry, 0x0C);
            SkinGeometry_SetMaterialAlpha(atomic, 0);
            InvokeTargetFunction(atomic);
            SkinGeometry_SetMaterialAlpha(atomic, 255);
        }

        CSF_AT(uint32_t, self, 0x40) = 0;
        hud = reinterpret_cast<void*>(GetGlobalValue_6C5760());
        GlobalHud_Commit_6C7C30(CSF_AT(C_0085e7b8*, hud, 0x04), nullptr);

        CSF_AT(char, self, 0xA0) = 1;
        hud = reinterpret_cast<void*>(GetGlobalValue_6C5760());
        C_0085e7b8_AttachWidget(
            CSF_AT(C_0085e7b8*, hud, 0x04), nullptr,
            reinterpret_cast<C_0085e7b8*>(
                static_cast<uint8_t*>(self) + 0x78));

        for (uint32_t i = 0; i < CSF_AT(uint32_t, self, 0x0C); ++i)
        {
            void* widget = CSF_AT(void**, self, 0x04)[i];
            if (widget)
            {
                hud = reinterpret_cast<void*>(GetGlobalValue_6C5760());
                uint32_t context = CSF_AT(uint32_t, hud, 0xF0);
                reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t)>(
                    CSF_AT(void**, widget, 0)[0x18 / 4])(
                        widget, nullptr, context);
                CSF_AT(char, self, 0xA0) = 0;
            }
        }

        hud = reinterpret_cast<void*>(GetGlobalValue_6C5760());
        result = GlobalHud_Commit_6C7C30(
            CSF_AT(C_0085e7b8*, hud, 0x04), nullptr);
    }

    CSF_AT(char, self, 0x48) = 0;
    return result;
}

HOOK(0x006D5AD0, GlobalHud_RefreshWidgets, "auto");
