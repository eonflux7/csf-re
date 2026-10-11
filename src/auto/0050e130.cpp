#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, SceneLoader_SetActiveContext, 0x00697360, (int a1));
CSF_FUNC(int, __fastcall, ResetFourDwords, 0x0069AEB0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, SceneLoader_BeginContext, 0x0069AEC0, (void* self, void* edx));

CSF_GLOBAL(int, g_MenuResourceDelayMs, 0x0083F4B0);
CSF_GLOBAL(const char*, g_pszMenu_P_Carga, 0x00899AEC);

static int __fastcall SceneLoader_ResetMenuResources(void* self, void* /*edx*/)
{
    int result = CSF_AT(int, self, 0x1C);
    uint8_t* ctx = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(result));

    if (!CSF_AT(uint8_t, ctx, 0x50))
    {
        // Ask the menu object (self->field_0010) to enter its loading state.
        void* menuObj = CSF_AT(void*, self, 0x10);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, menuObj, 0)[0x70 / 4])(menuObj, nullptr, 1);

        // Store the configured delay into the context.
        ctx = CSF_AT(uint8_t*, self, 0x1C);
        CSF_AT(float, ctx, 0x48) = static_cast<float>(g_MenuResourceDelayMs);

        // self->vtbl[0x2C](context, g_pszMenu_P_Carga)
        ctx = CSF_AT(uint8_t*, self, 0x1C);
        ((void(__fastcall*)(void*, void*, uint8_t*, const char*))CSF_AT(void**, self, 0)[0x2C / 4])(
            self, nullptr, ctx, g_pszMenu_P_Carga);

        // context->vtbl[0x4C]()
        ctx = CSF_AT(uint8_t*, self, 0x1C);
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, ctx, 0)[0x4C / 4])(ctx, nullptr);

        ctx = CSF_AT(uint8_t*, self, 0x1C);
        CSF_AT(uint8_t, ctx, 0x50) = 1;

        ctx = CSF_AT(uint8_t*, self, 0x1C);
        ResetFourDwords(ctx + 0x10, nullptr);

        ctx = CSF_AT(uint8_t*, self, 0x1C);
        CSF_AT(int, ctx, 0x1C) = 1036831949;

        ctx = CSF_AT(uint8_t*, self, 0x1C);
        SceneLoader_BeginContext(ctx + 0x10, nullptr);

        uint8_t* activeCtx = CSF_AT(uint8_t*, self, 0x1C);
        if (activeCtx)
            return SceneLoader_SetActiveContext(static_cast<int>(reinterpret_cast<uintptr_t>(activeCtx + 0x10)));
        return SceneLoader_SetActiveContext(0);
    }
    return result;
}

HOOK(0x0050E130, SceneLoader_ResetMenuResources, "auto");
