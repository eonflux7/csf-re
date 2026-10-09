#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void, __cdecl, Hud_RenderWidgetNode, 0x42F4F0, (void* node, int renderFlags));

static void __fastcall Hud_PlaceAndDrawEntry(
    uint8_t* self, void* /*edx*/, int nodeIndex, int rectIndex,
    const uint32_t* origin, uint8_t useAltList)
{
    if (!CSF_AT(uint8_t, self, 0x11B))
        return;

    auto* rect = reinterpret_cast<const uint32_t*>(
        CSF_AT(uint8_t*, self, 0x34) +
        static_cast<uint32_t>(rectIndex) * 20);

    const uint32_t left = rect[0];
    const uint32_t top = rect[1];
    const uint32_t right = rect[2];
    const uint32_t bottom = rect[3];

    uint32_t size[2];
    size[1] = bottom - top;
    const uint32_t width = right - left;
    const uint32_t horizontalSpan = CSF_AT(uint32_t, self, 0x94) + width;
    size[0] = width;

    const uint32_t horizontalAdjustment = horizontalSpan * origin[1];
    const uint32_t x = origin[0] - horizontalAdjustment;
    const uint32_t verticalOffset = CSF_AT(uint32_t, self, 0x98);
    uint32_t position[2] = { x, top + verticalOffset };

    void* node;
    if (useAltList)
        node = CSF_AT(void**, self, 0x54)[nodeIndex];
    else
        node = CSF_AT(void**, self, 0x44)[nodeIndex];

    reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t*)>(
        CSF_AT(void**, node, 0)[10])(node, nullptr, position);
    reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t*)>(
        CSF_AT(void**, node, 0)[14])(node, nullptr, size);
    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, node, 0)[17])(node, nullptr, 0);

    // Verified against the callee's assembly: its incoming EDI is only
    // saved/restored. It is overwritten at 0x42F51C before being read.
    // The push edi at 0x42F504 is a register save, not a virtual-call argument.
    Hud_RenderWidgetNode(node, 0);
}

HOOK(0x431080, Hud_PlaceAndDrawEntry, "auto");
