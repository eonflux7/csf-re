#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Hud_RebuildWidgetGroup, 0x00433910, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, ReleaseObjectReference, 0x006923C0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static int __fastcall Hud_RefreshWidgetTextures(void* self, void* /*edx*/)
{
    auto* settings = reinterpret_cast<uint8_t*>(GetGlobalValue_8B0568()) + 240;

    const bool showGroup0 =
        CSF_AT(uint8_t, settings, 5) != 0 ||
        CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0) != 0;

    const bool showGroup2 =
        CSF_AT(uint8_t, settings, 8) != 0 ||
        CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0) != 0;

    const bool showGroup3 =
        CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0) == 0 &&
        (CSF_AT(uint32_t, settings, 0) > 0 ||
         CSF_AT(uint8_t, settings, 7) != 0);

    const bool showGroup1 =
        CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0) == 0;

    const uint32_t begin = CSF_AT(uint32_t, self, 0x15C);
    const uint32_t count = begin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(CSF_AT(uint32_t, self, 0x160) - begin) / 48)
        : 0;

    for (uint32_t index = 0; index < count; ++index)
    {
        const uint32_t offset = 48 * index;
        auto* entry = CSF_AT(uint8_t*, self, 0x15C) + offset;

        if (CSF_AT(uint32_t, entry, 0x14) == 0)
            continue;

        // The original's empty-string comparison has a zero comparison length.
        // It selects the string storage, but does not read any characters.
        const char* comparisonText =
            CSF_AT(uint32_t, entry, 0x18) < 0x10u
                ? reinterpret_cast<const char*>(entry + 4)
                : CSF_AT(const char*, entry, 4);
        (void)comparisonText;
        (void)&g_byte_82E65C;

        bool enabled;
        switch (index)
        {
        case 0: enabled = showGroup0; break;
        case 1: enabled = showGroup1; break;
        case 2: enabled = showGroup2; break;
        case 3: enabled = showGroup3; break;
        default: enabled = true; break;
        }

        if (!enabled)
        {
            void* texture = CSF_AT(void*, entry, 0x1C);
            if (texture)
            {
                ReleaseObjectReference(static_cast<uint8_t*>(texture) + 4, nullptr);
                CSF_AT(void*, CSF_AT(uint8_t*, self, 0x15C) + offset, 0x1C) = nullptr;
            }
            continue;
        }

        if (CSF_AT(void*, entry, 0x1C))
            continue;

        void* accessor = reinterpret_cast<void*>(
            GlobalInt_ResolveAccessor(reinterpret_cast<void*>(GetGlobalInt()), nullptr));
        void* texturePaths =
            ((void* (__fastcall*)(void*, void*))CSF_AT(void**, accessor, 0)[4])(
                accessor, nullptr);
        ((void (__fastcall*)(void*, void*, const char*))CSF_AT(void**, texturePaths, 0)[4])(
            texturePaths, nullptr, CSF_STR(0x008308FC, "Gfx/Textures/"));

        void* textureManager =
            MenuControls_ResolveObject(reinterpret_cast<void*>(GetGlobalInt()), nullptr);

        entry = CSF_AT(uint8_t*, self, 0x15C) + offset;
        const char* textureName =
            CSF_AT(uint32_t, entry, 0x18) < 0x10u
                ? reinterpret_cast<const char*>(entry + 4)
                : CSF_AT(const char*, entry, 4);

        void* loadedTexture =
            ((void* (__fastcall*)(void*, void*, const char*))CSF_AT(void**, textureManager, 0)[14])(
                textureManager, nullptr, textureName);
        CSF_AT(void*, entry, 0x1C) = loadedTexture;

        entry = CSF_AT(uint8_t*, self, 0x15C) + offset;
        void* texture = CSF_AT(void*, entry, 0x1C);
        if (texture)
        {
            ((void (__fastcall*)(void*, void*, void*))CSF_AT(void**, texture, 0)[10])(
                texture, nullptr, entry + 0x20);

            entry = CSF_AT(uint8_t*, self, 0x15C) + offset;
            texture = CSF_AT(void*, entry, 0x1C);
            ((void (__fastcall*)(void*, void*, void*))CSF_AT(void**, texture, 0)[14])(
                texture, nullptr, entry + 0x28);

            texture = CSF_AT(void*, CSF_AT(uint8_t*, self, 0x15C) + offset, 0x1C);
            ((void (__fastcall*)(void*, void*, int))CSF_AT(void**, texture, 0)[17])(
                texture, nullptr, 4);
        }
    }

    auto* widgetSlot = static_cast<uint8_t*>(self) + 0x168;
    for (int group = 0; group < 4; ++group)
    {
        for (int widget = 0; widget < 8; ++widget, widgetSlot += 4)
        {
            void* object = CSF_AT(void*, widgetSlot, 0);
            if (object)
            {
                ((void (__fastcall*)(void*, void*, int))CSF_AT(void**, object, 0)[0])(
                    object, nullptr, 1);
                CSF_AT(void*, widgetSlot, 0) = nullptr;
            }
        }
    }

    if (showGroup0)
        Hud_RebuildWidgetGroup(self, nullptr, 0);
    if (showGroup2)
        Hud_RebuildWidgetGroup(self, nullptr, 2);

    const int state = GetGlobalState_50D190();
    if (CSF_AT(uint8_t, reinterpret_cast<void*>(state), 0xD0) == 0)
    {
        if (showGroup3)
            Hud_RebuildWidgetGroup(self, nullptr, 3);
        return Hud_RebuildWidgetGroup(self, nullptr, 1);
    }
    return state;
}

HOOK(0x00433BE0, Hud_RefreshWidgetTextures, "auto");
