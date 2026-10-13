#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneLoader_GetWorldObjectCollection, 0x006B6AF0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, GrowPositionBuffer, 0x006D64A0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, Rt2dObjectSetRenderCallBack, 0x007F8320, (int a1, int a2));
CSF_FUNC(int, __cdecl, Rt2dObjectGetRenderCallBack, 0x007F8330, (int a1));
CSF_FUNC(uint32_t*, __cdecl, Rt2dObjectSetColorOffset, 0x007F8340, (uint32_t* a1, int a2, int a3, int a4));
CSF_FUNC(uint32_t*, __cdecl, Rt2dObjectSetColorMultiplier, 0x007F8390, (uint32_t* a1, int a2));
CSF_FUNC(uint32_t*, __cdecl, RtDictAddEntry, 0x007FDA30, (uint32_t* a1, int a2, int a3));
CSF_FUNC(uint32_t*, __cdecl, Gfx2d_CreateVisibilityProxy, 0x007FF0E0, (int a1, int a2));

CSF_GLOBAL(int, g_VisibilityRenderCallback, 0x008BB704);

static unsigned int __fastcall C_00863ac0_AttachVisibilityProxy(
    C_00863ac0* self, void* /*edx*/, C_00863ac0* object)
{
    const uint32_t count = CSF_AT(uint32_t, self, 0x48);
    if (count != 0)
    {
        C_00863ac0** entry = CSF_AT(C_00863ac0**, self, 0x40);
        for (uint32_t index = 0; index < count; ++index, entry += 2)
        {
            if (*entry == object)
                return index;
        }
    }

    const int collection = SceneLoader_GetWorldObjectCollection(object, nullptr);
    uint32_t* proxy = Gfx2d_CreateVisibilityProxy(collection, 0);

    const int colorMultiplier = CSF_AT(int, self, 0xE8);
    Rt2dObjectSetColorMultiplier(proxy, colorMultiplier);
    // The trailing arguments reuse the preceding call's still-stacked arguments.
    Rt2dObjectSetColorOffset(
        proxy, CSF_AT(int, self, 0xEC),
        reinterpret_cast<int>(proxy), colorMultiplier);

    g_VisibilityRenderCallback =
        Rt2dObjectGetRenderCallBack(reinterpret_cast<int>(proxy));
    Rt2dObjectSetRenderCallBack(
        reinterpret_cast<int>(proxy),
        (int)CSF_ADDR_FN(0x006D6390));
    RtDictAddEntry(
        CSF_AT(uint32_t*, self, 0x3C),
        reinterpret_cast<int>(proxy), 0);

    const uint32_t currentCount = CSF_AT(uint32_t, self, 0x48);
    const uint32_t capacity = CSF_AT(uint32_t, self, 0x44);
    auto* buffer = reinterpret_cast<C_00863ac0*>(
        reinterpret_cast<uint8_t*>(self) + 0x40);

    if (currentCount >= capacity)
        GrowPositionBuffer(buffer, nullptr, static_cast<int>(buffer->field_000c));

    const uint32_t insertionIndex = buffer->field_0008;
    auto** entry = reinterpret_cast<C_00863ac0**>(buffer->vtbl)
        + 2 * insertionIndex;
    const uint32_t result = insertionIndex + 1;
    buffer->field_0008 = result;
    entry[0] = object;
    entry[1] = reinterpret_cast<C_00863ac0*>(proxy);
    return result;
}

HOOK(0x006D6EE0, C_00863ac0_AttachVisibilityProxy, "auto");
