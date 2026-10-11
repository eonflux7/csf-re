#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Cheats_PopulateFirstGroup, 0x0042AE00,
         (int context, int listBox, int unused));
CSF_FUNC(void*, __cdecl, Cheats_PopulateSecondGroup, 0x0042AE60,
         (int context, int listBox, int unused));
CSF_FUNC(void*, __cdecl, Cheats_PopulateThirdGroup, 0x0042AEB0,
         (int context, int listBox, int unused));
CSF_FUNC(void*, __cdecl, Cheats_PopulateFourthGroup, 0x0042AF00,
         (int context, int listBox));

CSF_FUNC(int, __fastcall, CGuiListBoxBase_ClearStorageAndSendEvent, 0x00715660, (CGuiListBoxBase* self, void*));
CSF_FUNC(void, __fastcall, CGuiListBoxBase_SetColumnWidth, 0x00716520, (CGuiListBoxBase* self, void* , std::uint32_t columnIndex, std::int32_t width));

static void __cdecl Cheats_RefreshSelection(
    int group, int context, CGuiListBoxBase* listBox)
{
    if (!context || !listBox)
        return;

    reinterpret_cast<void (__fastcall*)(CGuiListBoxBase*, void*)>(
        reinterpret_cast<void**>(listBox->vtbl)[0x298 / 4])(
            listBox, nullptr);

    CGuiListBoxBase_ClearStorageAndSendEvent(listBox, nullptr);

    void* size = listBox->vGetSize();
    CGuiListBoxBase_SetColumnWidth(
        listBox, nullptr, 0, CSF_AT(std::int32_t, size, 0x0) / 2);

    size = listBox->vGetSize();
    CGuiListBoxBase_SetColumnWidth(
        listBox, nullptr, 1, CSF_AT(std::int32_t, size, 0x0) / 2);

    const int listBoxAddress = static_cast<int>(
        reinterpret_cast<std::intptr_t>(listBox));

    switch (group)
    {
    case 0:
        Cheats_PopulateFirstGroup(context, listBoxAddress, 0);
        break;
    case 1:
        Cheats_PopulateSecondGroup(context, listBoxAddress, 0);
        break;
    case 2:
        Cheats_PopulateThirdGroup(context, listBoxAddress, 0);
        break;
    case 3:
        Cheats_PopulateFourthGroup(context, listBoxAddress);
        break;
    default:
        break;
    }

    alignas(8) std::uint8_t colorBytes[0x10];
    auto* color = reinterpret_cast<float*>(colorBytes);

    color[0] = 192.0f;
    color[1] = 214.0f;
    color[2] = 223.0f;
    color[3] = 255.0f;
    reinterpret_cast<void (__fastcall*)(CGuiListBoxBase*, void*, int, float*)>(
        reinterpret_cast<void**>(listBox->vtbl)[0x29C / 4])(
            listBox, nullptr, 0, color);

    reinterpret_cast<void (__fastcall*)(CGuiListBoxBase*, void*, int, int)>(
        reinterpret_cast<void**>(listBox->vtbl)[0x2A8 / 4])(
            listBox, nullptr, 0, 0);

    color[0] = 220.0f;
    color[1] = 214.0f;
    color[2] = 223.0f;
    color[3] = 255.0f;
    reinterpret_cast<void (__fastcall*)(CGuiListBoxBase*, void*, int, float*)>(
        reinterpret_cast<void**>(listBox->vtbl)[0x29C / 4])(
            listBox, nullptr, 1, color);
}

HOOK(0x0042AF90, Cheats_RefreshSelection, "auto");
