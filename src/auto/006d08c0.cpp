#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, C_00862e88_Construct, 0x006FC900, (C_00862e88* self, void*));
CSF_FUNC(int, __fastcall, GfxLightObject_ApplyViewportSettings, 0x006FC310, (void* self, void*, const void* settings));
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void*, int additionalCapacity));

// Vtable of the S_0041e5f0 sub-object referenced by the list-append path (no runtime effect).
[[maybe_unused]] static const auto kSubobjectListVtable = CSF_VTBL(0x0041E5F0);

// Allocates a 104-byte C_00862e88 render target, constructs it, applies the viewport
// settings to the resulting handle, and appends the handle to the list embedded at +0x14.
static int __fastcall C_00860db0_AllocConstructAndAppend(C_00860db0* self, void* /*edx*/, int settings)
{
    C_00862e88* renderTarget = reinterpret_cast<C_00862e88*>(CoreMemory_AllocUntagged(104));
    int renderTargetHandle = 0;
    if (renderTarget)
        renderTargetHandle = C_00862e88_Construct(renderTarget, nullptr);

    GfxLightObject_ApplyViewportSettings(
        reinterpret_cast<void*>(static_cast<std::intptr_t>(renderTargetHandle)),
        nullptr,
        reinterpret_cast<const void*>(static_cast<std::intptr_t>(settings)));

    S_0041e5f0* list = reinterpret_cast<S_0041e5f0*>(reinterpret_cast<uint8_t*>(self) + 0x14);

    uint32_t count = self->field_001c;
    uint32_t capacity = self->field_0018;
    if (count >= capacity)
        S_0041e5f0_GrowArrayCapacity(list, nullptr, static_cast<int>(list->field_000c));

    uint32_t index = list->field_0008;
    int* slot = reinterpret_cast<int*>(list->field_0000 + 4 * index);
    list->field_0008 = index + 1;
    *slot = renderTargetHandle;
    return renderTargetHandle;
}

HOOK(0x006D08C0, C_00860db0_AllocConstructAndAppend, "auto");
