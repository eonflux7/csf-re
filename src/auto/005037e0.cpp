#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, SceneLoader_UpdateAttachedObject, 0x0044BD70, (void* self, void* edx, float a3));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneObject_IsSecondCheckTrue, 0x0048E140, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ProcessPosition, 0x004FE580, (void* self, void* edx, float* a2));
CSF_FUNC(int, __fastcall, SceneLoader_ProcessMenuModeOne, 0x004FE730, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ProcessSubmodeOne, 0x004FF680, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ProcessSubmodeTwo, 0x004FF840, (void* self, void* edx));
CSF_FUNC(void, __fastcall, SceneLoader_ProcessOtherSubmode, 0x00501940, (int a1, int a2, int a3));
CSF_FUNC(void, __fastcall, SceneLoader_ProcessOtherMenuMode, 0x005028E0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SceneLoader_GetGlobalContext, 0x00652010, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, InitStringFromLiteral, 0x00692720, (void* returnSlot, const char* text, int count));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(char, __fastcall, SceneLoader_ProcessContextObject, 0x006BE9C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_006cabc0_GetField0098, 0x006CABC0, (void* self, void*));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Retain the packet's table binding; the disassembly does not install it.
static const auto g_SceneLoaderReferencedTable = CSF_VTBL(0x004FE580);

static void __fastcall S_004fe580_UpdateSceneLoading(
    S_004fe580* self, void* /*edx*/, float* position, float elapsedTime)
{
    const int positionResult =
        SceneLoader_ProcessPosition(self, nullptr, position);

    if (!self->field_00cc)
    {
        InitStringFromLiteral(
            reinterpret_cast<std::uint8_t*>(self) + 0xB8,
            CSF_STR(0x0083F5C4, "st01"), 1);
    }

    switch (self->field_0020)
    {
    case 0:
    {
        void* menuContext = reinterpret_cast<void*>(
            GetSceneMenuContext(g_pCurrentScene, nullptr));

        if (menuContext &&
            static_cast<std::uint8_t>(
                SceneObject_IsSecondCheckTrue(menuContext, nullptr)) != 0)
        {
            if (self->field_0028 == 1)
                SceneLoader_ProcessMenuModeOne(self, nullptr);
            else
                SceneLoader_ProcessOtherMenuMode(self, nullptr, positionResult);
        }
        break;
    }

    case 1:
        if (self->field_0024 == 1)
            SceneLoader_ProcessSubmodeOne(self, nullptr);
        else if (self->field_0024 == 2)
            SceneLoader_ProcessSubmodeTwo(self, nullptr);
        else
            SceneLoader_ProcessOtherSubmode(
                reinterpret_cast<int>(self), 0, positionResult);
        break;
    }

    if (self->field_00b5)
    {
        void* globalObject = reinterpret_cast<void*>(GetGlobalInt());
        void* context = reinterpret_cast<void*>(
            SceneLoader_GetGlobalContext(globalObject, nullptr));
        void* contextObject = reinterpret_cast<void*>(
            C_006cabc0_GetField0098(context, nullptr));

        if (contextObject)
            SceneLoader_ProcessContextObject(contextObject, nullptr);
    }

    if (self->field_00ec)
    {
        SceneLoader_UpdateAttachedObject(
            reinterpret_cast<void*>(self->field_00ec), nullptr, elapsedTime);
    }
}

HOOK(0x005037E0, S_004fe580_UpdateSceneLoading, "auto");
