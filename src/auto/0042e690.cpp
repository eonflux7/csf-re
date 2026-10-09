#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_unknown_GetLinkedDword, 0x00476080, (void* self, void*));
CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));
CSF_FUNC(double, __fastcall, C_008319c0_GetTransitionScale, 0x00476B80, (C_008319c0* self, void*));
CSF_FUNC(int, __fastcall, Hud_GetMenuObjectState, 0x00484380, (void* self, void* edx));
CSF_FUNC(char*, __fastcall, S_00492b20_GetPtr0x1B4, 0x00492B20, (void* self, void*));
CSF_FUNC(int, __fastcall, C_004950e0_GetField398, 0x004950E0, (void* self, void*));
CSF_FUNC(char, __fastcall, C_00495570_GetByte3DE, 0x00495570, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char __fastcall Hud_UpdateActiveWidget(void* self, void* /*edx*/)
{
    void* widget = nullptr;
    CSF_AT(void*, self, 0x154) = nullptr;

    const uint32_t contextAddress = GetSceneMenuContext(g_pCurrentScene, nullptr);
    if (!contextAddress)
        return static_cast<char>(contextAddress);

    void* context = reinterpret_cast<void*>(contextAddress);
    const char disabled = CSF_AT(char, context, 0x3A4);
    void* menuObject = CSF_AT(void*, context, 0x44);
    if (disabled)
        return disabled;

    const int state = Hud_GetMenuObjectState(menuObject, nullptr);
    if (state != -1 && state != 2)
        return static_cast<char>(state);

    const uint32_t flags = CSF_AT(uint32_t, context, 0x2E0);
    if (C_004950e0_GetField398(context, nullptr))
    {
        CSF_AT(void*, self, 0x154) =
            CSF_AT(void*, CSF_AT(void*, self, 0x140), 0);
        return static_cast<char>(reinterpret_cast<uintptr_t>(self));
    }

    C_008319c0* selectedObject;
    if (flags & 0x2000)
    {
        void* scene = g_pCurrentScene;

        // The packet's vtable annotation aliases this direct-call address.
        (void)CSF_VTBL(0x00492B20);
        char* handle = S_00492b20_GetPtr0x1B4(context, nullptr);
        void* resolved = reinterpret_cast<void*>(
            CHandleTable_Resolve(scene, nullptr,
                                 static_cast<int>(reinterpret_cast<uintptr_t>(handle))));
        if (!resolved)
            goto Finish;

        void* object = CSF_AT(void*, resolved, 0x44);
        if (!object)
            goto Finish;

        selectedObject =
            reinterpret_cast<C_008319c0* (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, object, 0)[0xC8 / 4])(object, nullptr, 0);
        if (!selectedObject)
            goto Finish;
    }
    else
    {
        void* object = CSF_AT(void*, context, 0x44);
        const int firstIndex =
            reinterpret_cast<int (__fastcall*)(void*, void*)>(
                CSF_AT(void**, object, 0)[0xFC / 4])(object, nullptr);
        if (firstIndex == -1)
            goto Finish;

        void** table = CSF_AT(void**, object, 0);
        const int index =
            reinterpret_cast<int (__fastcall*)(void*, void*)>(
                table[0xFC / 4])(object, nullptr);
        selectedObject =
            reinterpret_cast<C_008319c0* (__fastcall*)(void*, void*, int)>(
                table[0xC8 / 4])(object, nullptr, index);
        if (!selectedObject)
            goto Finish;

        const unsigned char eligible =
            reinterpret_cast<unsigned char (__fastcall*)(void*, void*)>(
                CSF_AT(void**, object, 0)[0x10C / 4])(object, nullptr);
        if (!eligible)
            goto Finish;

        if (C_008319c0_GetSubObjectField74(selectedObject, nullptr) == 19)
            goto Finish;
        if ((flags & 0x40) || (flags & 0x1000) ||
            ((flags & 1) && (flags & 2)))
            goto Finish;
        if (C_00495570_GetByte3DE(context, nullptr))
            goto Finish;
    }

    if (C_unknown_GetLinkedDword(selectedObject, nullptr) != 2)
    {
        widget = CSF_AT(void*, CSF_AT(void*, self, 0x140), 0);
        goto Finish;
    }

    widget = CSF_AT(void*, CSF_AT(void*, self, 0x140), 0xC);
    if (widget)
    {
        void* globalValues = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
        const float baseline = CSF_AT(float, globalValues, 0xAF8);
        const float remaining = g_TransitionOne - baseline;

        if (remaining > g_fZero && remaining <= g_TransitionOne)
        {
            double scale =
                C_008319c0_GetTransitionScale(selectedObject, nullptr) *
                remaining + baseline;
            if (scale > g_TransitionOne)
                scale = g_TransitionOne;

            void* configuration = CSF_AT(void*, self, 0x140);
            void** table = CSF_AT(void**, widget, 0);
            float size[2];
            size[1] = static_cast<float>(scale * CSF_AT(float, configuration, 0x14));
            size[0] = static_cast<float>(scale * CSF_AT(float, configuration, 0x10));

            reinterpret_cast<void (__fastcall*)(void*, void*, float*)>(
                table[0x34 / 4])(widget, nullptr, size);
        }
    }

Finish:
    CSF_AT(void*, self, 0x154) = widget;
    return static_cast<char>(reinterpret_cast<uintptr_t>(self));
}

HOOK(0x0042E690, Hud_UpdateActiveWidget, "auto");
