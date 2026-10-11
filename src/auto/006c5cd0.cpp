#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, GlobalHud_Commit_6C7C30, 0x006C7C30, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxSkin_SetRenderMode, 0x006C8390, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_008655e8_EndCameraUpdate, 0x006D2BE0, (C_008655e8* self, void*));
CSF_FUNC(bool, __fastcall, GfxSkin_BeginCameraUpdate, 0x006D3010, (void* self, void* edx, int arg0, float a3, float a2, float a5, int a6));
CSF_FUNC(unsigned int, __fastcall, GfxSkin_RenderGeometry, 0x006D9430, (void* self, void* edx));
CSF_FUNC(void, __cdecl, GfxSkin_SortRenderEntries, 0x007AEAE0,
         (void* base, std::size_t count, std::size_t size,
          int (__cdecl* compare)(const void*, const void*)));

CSF_GLOBAL(float, g_ColorBlack, 0x008A7DA0);
CSF_GLOBAL(float, g_ColorBlack_G, 0x008A7DA4);
CSF_GLOBAL(float, g_ColorBlack_B, 0x008A7DA8);
CSF_GLOBAL(float, g_ColorBlack_A, 0x008A7DAC);

// Some legacy bindings describe float stack arguments as integers.
static int GfxSkin_PackFloatArgument(float value)
{
    int argument;
    std::memcpy(&argument, &value, sizeof(argument));
    return argument;
}

static bool __fastcall S_006c5cd0_Render(S_006c5cd0* self, void* /*edx*/)
{
    bool success = true;
    self->field_00f5 = 1;
    GfxSkin_SetRenderMode(reinterpret_cast<void*>(self->field_0004), nullptr, 4);

    if ((self->field_0008 & 0x400) != 0)
    {
        void* object = reinterpret_cast<void*>(self->field_002c);
        if (object)
        {
            reinterpret_cast<void (__fastcall*)(void*, void*)>(
                CSF_AT(void**, object, 0)[0x14 / 4])(object, nullptr);
        }
    }

    if ((self->field_0008 & 2) != 0)
    {
        GfxSkin_SetRenderMode(reinterpret_cast<void*>(self->field_0004), nullptr, 7);
        for (std::uint32_t index = 0; index < self->field_00e8; ++index)
        {
            GfxSkin_RenderGeometry(
                CSF_AT(void*, reinterpret_cast<void*>(self->field_00e0), 4 * index),
                nullptr);
        }
        GlobalHud_Commit_6C7C30(reinterpret_cast<void*>(self->field_0004), nullptr);
    }

    if ((self->field_0008 & 0x200) != 0)
    {
        if (self->field_0038 || self->field_0048)
        {
            // These slots return floating-point values in ST(0), unlike the
            // incompatible generic wrappers declared for C_008655e8.
            void* camera = reinterpret_cast<void*>(self->field_00f0);
            const float savedNearPlane = static_cast<float>(
                reinterpret_cast<double (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, camera, 0)[0x44 / 4])(camera, nullptr));

            camera = reinterpret_cast<void*>(self->field_00f0);
            const float savedFarPlane = static_cast<float>(
                reinterpret_cast<double (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, camera, 0)[0x3c / 4])(camera, nullptr));

            C_008655e8_EndCameraUpdate(
                reinterpret_cast<C_008655e8*>(self->field_00f0), nullptr);
            reinterpret_cast<C_008655e8*>(self->field_00f0)
                ->vCreateResourceWrapper(1112014848);
            reinterpret_cast<C_008655e8*>(self->field_00f0)
                ->vCreateTextureWrapper(1195593728);

            const bool beganUpdate = GfxSkin_BeginCameraUpdate(
                reinterpret_cast<void*>(self->field_00f0), nullptr, 0,
                g_ColorBlack, g_ColorBlack_G, g_ColorBlack_B,
                GfxSkin_PackFloatArgument(g_ColorBlack_A));

            int clearFlags = 0;
            if (self->field_0038)
            {
                clearFlags = 2;
                for (std::uint32_t index = 0; index < self->field_0038; ++index)
                {
                    void* object = CSF_AT(
                        void*, reinterpret_cast<void*>(self->field_0030), 4 * index);
                    if (object)
                    {
                        reinterpret_cast<void (__fastcall*)(void*, void*)>(
                            CSF_AT(void**, object, 0)[0x14 / 4])(object, nullptr);
                    }
                }
            }

            if (self->field_0048)
            {
                GfxSkin_SortRenderEntries(
                    reinterpret_cast<void*>(self->field_0040),
                    self->field_0048, 0xC,
                    reinterpret_cast<int (__cdecl*)(const void*, const void*)>(
                        CSF_ADDR_FN(0x006C5AC0)));

                GfxSkin_SetRenderMode(
                    reinterpret_cast<void*>(self->field_0004), nullptr, 7);
                std::uint32_t entryOffset = 0;
                for (std::uint32_t index = 0; index < self->field_0048; ++index)
                {
                    void* object = CSF_AT(
                        void*, reinterpret_cast<void*>(self->field_0040), entryOffset);
                    reinterpret_cast<void (__fastcall*)(void*, void*)>(
                        CSF_AT(void**, object, 0)[0x14 / 4])(object, nullptr);
                    entryOffset += 12;
                }
                GlobalHud_Commit_6C7C30(
                    reinterpret_cast<void*>(self->field_0004), nullptr);
            }

            C_008655e8_EndCameraUpdate(
                reinterpret_cast<C_008655e8*>(self->field_00f0), nullptr);
            reinterpret_cast<C_008655e8*>(self->field_00f0)
                ->vCreateResourceWrapper(GfxSkin_PackFloatArgument(savedNearPlane));
            reinterpret_cast<C_008655e8*>(self->field_00f0)
                ->vCreateTextureWrapper(GfxSkin_PackFloatArgument(savedFarPlane));

            success = beganUpdate && GfxSkin_BeginCameraUpdate(
                reinterpret_cast<void*>(self->field_00f0), nullptr, clearFlags,
                g_ColorBlack, g_ColorBlack_G, g_ColorBlack_B,
                GfxSkin_PackFloatArgument(g_ColorBlack_A));
        }
    }

    GlobalHud_Commit_6C7C30(reinterpret_cast<void*>(self->field_0004), nullptr);
    self->field_00f5 = 0;
    return success;
}

HOOK(0x006C5CD0, S_006c5cd0_Render, "auto");
