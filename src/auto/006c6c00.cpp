#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Binds to original functions by address.
CSF_FUNC(int, __fastcall, RenderHandle_FromObject, 0x006B2D50, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SkinGeometry_ApplyHandleEntry, 0x006C68F0, (void* self, void* edx, int a1, int a2, void* a4));

// Returns the number of render handles attached to a2 (vtable slot 0x84).
static uint32_t __fastcall GetRenderHandleCount(void* renderObject)
{
    return ((uint32_t(__fastcall*)(void*, void*))CSF_AT(void**, renderObject, 0)[0x84 / 4])(renderObject, nullptr);
}

// Returns the render handle at the given index (vtable slot 0x80).
static void* __fastcall GetRenderHandleAt(void* renderObject, uint32_t index)
{
    return ((void* (__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, renderObject, 0)[0x80 / 4])(renderObject, nullptr, index);
}

static unsigned int __fastcall C_00860288_ProcessAllRenderHandles(C_00860288* self, void* /*edx*/, C_0085e7b8* renderObject)
{
    RenderHandle_FromObject(renderObject, nullptr);

    uint32_t index = 0;
    uint32_t result = GetRenderHandleCount(renderObject);
    if (result != 0)
    {
        do
        {
            void* handle = GetRenderHandleAt(renderObject, index);
            int valuePointer = Handle_GetValuePointer(handle, nullptr);
            SkinGeometry_ApplyHandleEntry(self, nullptr,
                static_cast<int>(CSF_AT(uint32_t, handle, 0x0C)),
                valuePointer,
                renderObject);
            ++index;
            result = GetRenderHandleCount(renderObject);
        } while (index < result);
    }
    return result;
}

HOOK(0x006C6C00, C_00860288_ProcessAllRenderHandles, "auto");
