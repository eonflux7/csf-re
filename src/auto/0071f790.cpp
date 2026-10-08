#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, AssignCurrentResource, 0x0071D850,
         (CGuiBuilder* self, void* edx, void* control));
CSF_FUNC(uint8_t*, __fastcall, ReadControlProps, 0x0071EBD0,
         (CGuiBuilder* self, void* edx, void* control, uint8_t* node));
CSF_FUNC(void, __fastcall, ReadAdditionalControlProps, 0x0071F680,
         (CGuiBuilder* self, void* edx, void* control, uint8_t* node));
CSF_FUNC(uint8_t*, __fastcall, ReadControlAppearance, 0x0071EFC0,
         (CGuiBuilder* self, void* edx, void* control, uint8_t* node));
CSF_FUNC(uint8_t*, __fastcall, FindChildNode, 0x00693360,
         (uint8_t* node, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, ParseControls, 0x0071DBE0,
         (CGuiBuilder* self, void* edx, void* control, uint8_t* node));
CSF_FUNC(uint8_t*, __stdcall, ResolveObjectFromAttribute, 0x0071DAF0,
         (void* control, uint8_t* node));

CSF_GLOBAL(char*, g_ControlChildrenKeyword, 0x008A86B0);

static void __fastcall CGuiBuilder_InitializeControl(
    CGuiBuilder* self, void* /*edx*/, void* control, uint8_t* node)
{
    if (!control || !node)
        return;

    AssignCurrentResource(self, nullptr, control);
    ReadControlProps(self, nullptr, control, node);

    void* parser = reinterpret_cast<void*>(self->field_001c);
    uint8_t* properties =
        reinterpret_cast<uint8_t* (__fastcall*)(void*, void*, uint8_t*)>(
            CSF_AT(void**, parser, 0)[1])(parser, nullptr, node);
    ReadAdditionalControlProps(self, nullptr, control, properties);

    parser = reinterpret_cast<void*>(self->field_001c);
    properties =
        reinterpret_cast<uint8_t* (__fastcall*)(void*, void*, uint8_t*)>(
            CSF_AT(void**, parser, 0)[1])(parser, nullptr, node);
    ReadControlAppearance(self, nullptr, control, properties);

    uint8_t* children = FindChildNode(node, nullptr, g_ControlChildrenKeyword);
    ParseControls(self, nullptr, control, children);

    parser = reinterpret_cast<void*>(self->field_001c);
    children =
        reinterpret_cast<uint8_t* (__fastcall*)(void*, void*, uint8_t*)>(
            CSF_AT(void**, parser, 0)[2])(parser, nullptr, node);
    ParseControls(self, nullptr, control, children);

    ResolveObjectFromAttribute(control, node);

    parser = reinterpret_cast<void*>(self->field_001c);
    properties =
        reinterpret_cast<uint8_t* (__fastcall*)(void*, void*, uint8_t*)>(
            CSF_AT(void**, parser, 0)[1])(parser, nullptr, node);
    ResolveObjectFromAttribute(control, properties);

    void** controlVtable = CSF_AT(void**, control, 0);
    int value = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
        controlVtable[0x148 / 4])(control, nullptr, 1);
    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        controlVtable[0x14C / 4])(control, nullptr, value);

    CSF_AT(uint32_t, control, 4) = self->field_0080;
}

HOOK(0x0071F790, CGuiBuilder_InitializeControl, "auto");
