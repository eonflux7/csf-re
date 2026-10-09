#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, CHud_BeginWidgetDraw, 0x0042E150, (void* self, void* edx, int* a2));
CSF_FUNC(int, __fastcall, CHud_EndWidgetDraw, 0x0042E1A0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetEntryCount, 0x0044BB80, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(void*, __fastcall, DefaultDestination_Get, 0x004FE4A0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, MenuMessageBox_ResolveEntryByKey, 0x00692440, (int code, int flag));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static void __fastcall CHud_DrawWidgetEntries(CHud* self, void* /*edx*/)
{
    void* destination = DefaultDestination_Get(
        reinterpret_cast<void*>(GetGlobalValue_8B0568()), nullptr);
    if (!destination || !GetEntryCount(destination, nullptr))
        return;

    float visibility;
    std::memcpy(&visibility, &self->field_0270, sizeof(visibility));
    if (visibility == g_fZero || !self->field_0268 || !self->field_026c)
        return;

    void* widget = reinterpret_cast<void*>(self->field_0268);
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, widget, 0)[2])(
        widget, nullptr);

    uint32_t remaining = self->field_027c;
    if (!remaining)
        return;

    uint32_t entryOffset = 0;
    do
    {
        auto* entry = reinterpret_cast<uint8_t*>(self->field_0274) + entryOffset;
        int* entryWidget = CSF_AT(int*, entry, 0);
        if (entryWidget)
        {
            CHud_BeginWidgetDraw(self, nullptr, entryWidget);

            widget = CSF_AT(void*, entry, 0);
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, widget, 0)[17])(
                widget, nullptr, 0);

            widget = CSF_AT(void*, entry, 0);
            ((void(__fastcall*)(void*, void*, int*))CSF_AT(void**, widget, 0)[10])(
                widget, nullptr, reinterpret_cast<int*>(entry + 0x04));

            widget = CSF_AT(void*, entry, 0);
            auto* currentColor =
                ((uint32_t*(__fastcall*)(void*, void*))CSF_AT(void**, widget, 0)[27])(
                    widget, nullptr);
            uint32_t color[4];
            color[0] = CSF_AT(uint32_t, currentColor, 0x00);
            color[1] = CSF_AT(uint32_t, currentColor, 0x04);
            color[2] = CSF_AT(uint32_t, currentColor, 0x08);
            color[3] = CSF_AT(uint32_t, currentColor, 0x0C);
            color[3] = CSF_AT(uint32_t, entry, 0x14);

            widget = CSF_AT(void*, entry, 0);
            ((void(__fastcall*)(void*, void*, uint32_t*))CSF_AT(void**, widget, 0)[26])(
                widget, nullptr, color);

            widget = CSF_AT(void*, entry, 0);
            ((void(__fastcall*)(void*, void*, int*))CSF_AT(void**, widget, 0)[14])(
                widget, nullptr, reinterpret_cast<int*>(entry + 0x0C));

            widget = CSF_AT(void*, entry, 0);
            ((void(__fastcall*)(void*, void*))CSF_AT(void**, widget, 0)[2])(
                widget, nullptr);

            CHud_EndWidgetDraw(self, nullptr, CSF_AT(int, entry, 0));

            int resolvedEntry = MenuMessageBox_ResolveEntryByKey(
                CSF_AT(int, entry, 0x38), 4);
            widget = reinterpret_cast<void*>(self->field_026c);
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, widget, 0)[16])(
                widget, nullptr, resolvedEntry);
        }
        else
        {
            widget = reinterpret_cast<void*>(self->field_026c);
            void** vtable = CSF_AT(void**, widget, 0);
            int resolvedEntry = MenuMessageBox_ResolveEntryByKey(
                CSF_AT(int, entry, 0x38), 1);
            widget = reinterpret_cast<void*>(self->field_026c);
            ((void(__fastcall*)(void*, void*, int))vtable[16])(
                widget, nullptr, resolvedEntry);
        }

        widget = reinterpret_cast<void*>(self->field_026c);
        ((void(__fastcall*)(void*, void*, int*))CSF_AT(void**, widget, 0)[20])(
            widget, nullptr, reinterpret_cast<int*>(entry + 0x28));

        widget = reinterpret_cast<void*>(self->field_026c);
        ((void(__fastcall*)(void*, void*, int*))CSF_AT(void**, widget, 0)[22])(
            widget, nullptr, reinterpret_cast<int*>(entry + 0x30));

        widget = reinterpret_cast<void*>(self->field_026c);
        ((void(__fastcall*)(void*, void*, int*))CSF_AT(void**, widget, 0)[24])(
            widget, nullptr, reinterpret_cast<int*>(entry + 0x18));

        widget = reinterpret_cast<void*>(self->field_026c);
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, widget, 0)[2])(
            widget, nullptr);

        entryOffset += 60;
        --remaining;
    } while (remaining);
}

HOOK(0x0042EC60, CHud_DrawWidgetEntries, "auto");
