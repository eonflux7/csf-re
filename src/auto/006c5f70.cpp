#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, GlobalHud_Commit_6C7C30, 0x006C7C30, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxSkin_SetRenderPhase, 0x006C8390, (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __fastcall, GfxSkin_PrepareSkinObject, 0x006D82E0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxSkin_FinishSkinObject, 0x006D94B0, (void* self, void* edx));
CSF_FUNC(void, __cdecl, GfxSkin_SortRenderEntries, 0x007AEAE0,
         (void* base, std::size_t count, std::size_t size,
          int (__cdecl* compare)(const void*, const void*)));

static char __fastcall S_006c5cd0_ProcessRenderQueues(
    S_006c5cd0* self, void* /*edx*/)
{
    self->field_0118 += self->field_0078
                      + self->field_0088
                      + self->field_0098
                      + self->field_00a8
                      + self->field_0048
                      + self->field_00d8;

    for (std::uint32_t i = 0; i < self->field_00e8; ++i)
    {
        void* object = CSF_AT(void*, reinterpret_cast<void*>(self->field_00e0), 4 * i);
        const std::uint32_t* count =
            ((const std::uint32_t* (__fastcall*)(void*, void*))
             CSF_AT(void**, object, 0)[0x28 / 4])(object, nullptr);
        self->field_0110 += *count;
    }

    if (self->field_0078)
    {
        GfxSkin_SetRenderPhase(reinterpret_cast<void*>(self->field_0004), nullptr, 6);
        for (std::uint32_t i = 0; i < self->field_0078; ++i)
        {
            void* object = CSF_AT(void*, reinterpret_cast<void*>(self->field_0070), 0xC * i);
            ((void (__fastcall*)(void*, void*))
             CSF_AT(void**, object, 0)[0x14 / 4])(object, nullptr);
        }
        GlobalHud_Commit_6C7C30(reinterpret_cast<void*>(self->field_0004), nullptr);
    }

    GfxSkin_SetRenderPhase(reinterpret_cast<void*>(self->field_0004), nullptr, 7);

    if (self->field_0008 & 2)
    {
        for (std::uint32_t i = 0; i < self->field_00e8; ++i)
        {
            GfxSkin_PrepareSkinObject(
                CSF_AT(void*, reinterpret_cast<void*>(self->field_00e0), 4 * i),
                nullptr);
        }
    }

    if (self->field_00a8 && (self->field_0008 & 0x1000))
    {
        GfxSkin_SortRenderEntries(
            reinterpret_cast<void*>(self->field_00a0), self->field_00a8, 0xC,
            reinterpret_cast<int (__cdecl*)(const void*, const void*)>(
                CSF_ADDR_FN(0x006C5AC0)));
        for (std::uint32_t i = 0; i < self->field_00a8; ++i)
        {
            void* object = CSF_AT(void*, reinterpret_cast<void*>(self->field_00a0), 0xC * i);
            ((void (__fastcall*)(void*, void*))
             CSF_AT(void**, object, 0)[0x14 / 4])(object, nullptr);
        }
    }

    if (self->field_0098 && (self->field_0008 & 0x2000))
    {
        GfxSkin_SortRenderEntries(
            reinterpret_cast<void*>(self->field_0090), self->field_0098, 0xC,
            reinterpret_cast<int (__cdecl*)(const void*, const void*)>(
                CSF_ADDR_FN(0x006C5AC0)));
        for (std::uint32_t i = 0; i < self->field_0098; ++i)
        {
            void* object = CSF_AT(void*, reinterpret_cast<void*>(self->field_0090), 0xC * i);
            ((void (__fastcall*)(void*, void*))
             CSF_AT(void**, object, 0)[0x14 / 4])(object, nullptr);
        }
    }

    if (self->field_0088 && (self->field_0008 & 0x4000))
    {
        GfxSkin_SortRenderEntries(
            reinterpret_cast<void*>(self->field_0080), self->field_0088, 0xC,
            reinterpret_cast<int (__cdecl*)(const void*, const void*)>(
                CSF_ADDR_FN(0x006C5AC0)));
        for (std::uint32_t i = 0; i < self->field_0088; ++i)
        {
            void* object = CSF_AT(void*, reinterpret_cast<void*>(self->field_0080), 0xC * i);
            ((void (__fastcall*)(void*, void*))
             CSF_AT(void**, object, 0)[0x14 / 4])(object, nullptr);
        }
    }

    if (self->field_0008 & 2)
    {
        for (std::uint32_t i = 0; i < self->field_00e8; ++i)
        {
            GfxSkin_FinishSkinObject(
                CSF_AT(void*, reinterpret_cast<void*>(self->field_00e0), 4 * i),
                nullptr);
        }
    }

    return GlobalHud_Commit_6C7C30(reinterpret_cast<void*>(self->field_0004), nullptr);
}

HOOK(0x006C5F70, S_006c5cd0_ProcessRenderQueues, "auto");
