#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall CCamera_SetSubtitlePanelSize(
    CCamera* self, void* /*edx*/, int width, int height)
{
    std::memcpy(&self->field_005c, &width, sizeof(width));
    self->field_0060 = static_cast<uint32_t>(height);
    return width;
}

HOOK(0x004F1D70, CCamera_SetSubtitlePanelSize, "auto");
