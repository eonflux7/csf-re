#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CGuiPanel_ConstructFromSource, 0x0072C7F0,
         (void* self, void* edx, void* builderContext));

static int __fastcall CGuiBuilder_CreateEmptyPanel(CGuiBuilder* self, void* /*edx*/)
{
    // Canonical game-heap binding to 0x006910F0.
    void* panel = game::Mem_Alloc(0x18Cu);
    if (!panel)
        return 0;

    return CGuiPanel_ConstructFromSource(
        panel, nullptr, reinterpret_cast<std::uint8_t*>(self) + 0x0C);
}

HOOK(0x0071DA40, CGuiBuilder_CreateEmptyPanel, "auto");
