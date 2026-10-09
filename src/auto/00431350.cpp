#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, sub_42EB60, 0x0042EB60, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __stdcall, sub_42EC20, 0x0042EC20, (int a1, int a2));
CSF_FUNC(void, __cdecl, Hud_RenderWidgetAtScaledRect, 0x0042F4F0, (void* widget, const int* rect));
CSF_FUNC(void, __fastcall, sub_431130, 0x00431130, (void* self, void* edx, unsigned int a2, int a3, int a4, char a5));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetMemberValueD4, 0x00476090, (void* self, void*));
CSF_FUNC(uint64_t, __fastcall, sub_4769F0, 0x004769F0, (void* self, void* edx));
CSF_FUNC(uint64_t, __fastcall, sub_476A50, 0x00476A50, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_486F50, 0x00486F50, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_52D5C0, 0x0052D5C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_681F40, 0x00681F40, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_70E390, 0x0070E390, (void* self, void* edx));

static void __fastcall S_0042eb60_RenderCurrentObjectHud(
    S_0042eb60* self, void* /*edx*/)
{
    void* menuContext =
        reinterpret_cast<void*>(GetSceneMenuContext(g_pCurrentScene, nullptr));
    if (!menuContext)
        return;

    void* sceneObject = CSF_AT(void*, menuContext, 0x44);
    if (!sceneObject)
        return;

    void* object = reinterpret_cast<void*>(sub_486F50(sceneObject, nullptr));
    if (!object || !self->field_0120)
        return;

    int* valuePointer =
        reinterpret_cast<int*>(Handle_GetValuePointer(object, nullptr));

    int* widget = reinterpret_cast<int*>(self->field_00e8);
    if (widget)
        Hud_RenderWidgetAtScaledRect(widget, nullptr);

    if (valuePointer)
    {
        int* associatedWidget = reinterpret_cast<int*>(sub_42EC20(
            *valuePointer,
            reinterpret_cast<int>(&CSF_AT(uint8_t, self, 0xC8))));
        if (associatedWidget)
            Hud_RenderWidgetAtScaledRect(associatedWidget, nullptr);
    }

    if (!self->field_011b || !self->field_0124 || !self->field_0125)
        return;

    // Virtual slot at byte offset 0x9C.
    if (!((uint8_t(__fastcall*)(void*, void*))
              CSF_AT(void**, object, 0)[39])(object, nullptr))
        return;

    if (GetMemberValueD4(object, nullptr) == 2)
    {
        int value = sub_681F40(object, nullptr);
        unsigned int displayValue = sub_42EB60(self, nullptr, value, 0);
        sub_431130(self, nullptr, displayValue, 0, 1, 0);

        value = sub_52D5C0(object, nullptr);
        unsigned int secondaryDisplayValue =
            sub_42EB60(self, nullptr, value, 1);
        int currentValue = sub_52D5C0(object, nullptr);
        int limit = static_cast<int32_t>(
            static_cast<uint32_t>(sub_4769F0(object, nullptr)));
        sub_431130(self, nullptr, secondaryDisplayValue, 1, 1,
                   static_cast<char>(currentValue >= limit));
    }

    if (GetMemberValueD4(object, nullptr) == 1)
    {
        int value = sub_681F40(object, nullptr);
        unsigned int displayValue = sub_42EB60(self, nullptr, value, 0);
        sub_431130(self, nullptr, displayValue, 0, 1, 0);

        value = sub_70E390(object, nullptr);
        unsigned int secondaryDisplayValue =
            sub_42EB60(self, nullptr, value, 1);
        int currentValue = sub_70E390(object, nullptr);
        int limit = static_cast<int32_t>(
            static_cast<uint32_t>(sub_476A50(object, nullptr)));
        sub_431130(self, nullptr, secondaryDisplayValue, 1, 1,
                   static_cast<char>(currentValue >= limit));
    }

    if (!GetMemberValueD4(object, nullptr))
    {
        int value = sub_681F40(object, nullptr);
        unsigned int displayValue = sub_42EB60(self, nullptr, value, 0);
        int currentValue = sub_681F40(object, nullptr);
        int limit = static_cast<int32_t>(
            static_cast<uint32_t>(sub_476A50(object, nullptr)));
        sub_431130(self, nullptr, displayValue, 0, 1,
                   static_cast<char>(currentValue >= limit));
    }
}

HOOK(0x00431350, S_0042eb60_RenderCurrentObjectHud, "auto");
