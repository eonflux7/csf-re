#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Fills the 2-float HUD scale vector it is given.
CSF_FUNC(unsigned int, __cdecl, Hud_ComputeScaleVector, 0x0042DE60, (float* scale));

// Widget vtable slots (slot = byte offset / 4), all thiscall:
//   2 (+0x08) Render()                    9 (+0x24) SetPosition(const float pos[2])
//   12 (+0x30) GetPosition() -> float[2]  14 (+0x38) SetSize(const int size[2])
//   15 (+0x3C) GetSize() -> int[2]
// Hex-Rays gives this an a1@<edi>: that is only the `push edi` callee-save (MSVC saves edi after the null check,
// so the early exit skips its pop); all 22 callers push 2 args and clean 8 bytes.
using GetVec = void*(__fastcall*)(void* self, void* edx);
using SetVec = void(__fastcall*)(void* self, void* edx, const void* v);
using Render = void(__fastcall*)(void* self, void* edx);

static inline void** Vtbl(void* w) { return CSF_AT(void**, w, 0); }

// Renders the widget at rect {left, top, right, bottom} (or where it is, at its size, when rect is null), both
// scaled by the HUD scale, then puts its position and size back.
static void __cdecl Hud_RenderWidgetAtScaledRect(void* widget, const int* rect)
{
    if (!widget)
        return;

    const float* p = static_cast<const float*>(reinterpret_cast<GetVec>(Vtbl(widget)[12])(widget, nullptr));
    float savedPos[2] = {p[0], p[1]};
    const int* s = static_cast<const int*>(reinterpret_cast<GetVec>(Vtbl(widget)[15])(widget, nullptr));
    int savedSize[2] = {s[0], s[1]};

    float pos[2];
    int size[2];
    if (rect) {
        pos[0] = static_cast<float>(rect[0]);
        pos[1] = static_cast<float>(rect[1]);
        size[0] = rect[2] - rect[0];
        size[1] = rect[3] - rect[1];
    } else {
        pos[0] = savedPos[0];
        pos[1] = savedPos[1];
        size[0] = savedSize[0];
        size[1] = savedSize[1];
    }

    float scale[2] = {0.0f, 0.0f};
    Hud_ComputeScaleVector(scale);
    pos[0] = scale[0] * pos[0];
    pos[1] = pos[1] * scale[1];
    scale[0] = 0.0f;
    scale[1] = 0.0f;
    Hud_ComputeScaleVector(scale);
    size[0] = static_cast<int>(static_cast<int64_t>(static_cast<double>(size[0]) * scale[0]));
    size[1] = static_cast<int>(static_cast<int64_t>(static_cast<double>(size[1]) * scale[1]));

    reinterpret_cast<SetVec>(Vtbl(widget)[9])(widget, nullptr, pos);
    reinterpret_cast<SetVec>(Vtbl(widget)[14])(widget, nullptr, size);
    reinterpret_cast<Render>(Vtbl(widget)[2])(widget, nullptr);
    reinterpret_cast<SetVec>(Vtbl(widget)[9])(widget, nullptr, savedPos);
    reinterpret_cast<SetVec>(Vtbl(widget)[14])(widget, nullptr, savedSize);
}

HOOK(0x0042F4F0, Hud_RenderWidgetAtScaledRect, "auto");
