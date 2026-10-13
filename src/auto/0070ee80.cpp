#include <cstddef>
#include "globals.h"
#include <cstdint>

#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(bool, __fastcall, Audio_CreateCaptureBuffer, 0x007116F0, (void* self, void*, int32_t* parameters));
CSF_FUNC(uint32_t*, __fastcall, C_00863d18_Initialize, 0x007118A0, (C_00863d18* self, void*));

static uint32_t* __stdcall Audio_EnumCallbackCreateCaptureSlot(int a1)
{
    C_00863d18* slot = reinterpret_cast<C_00863d18*>(CoreMemory_AllocUntagged(12));
    uint32_t* object = nullptr;
    if (slot)
        object = C_00863d18_Initialize(slot, nullptr);

    if (Audio_CreateCaptureBuffer(object ? object : nullptr, nullptr, reinterpret_cast<int32_t*>(a1)) || !object)
        return object;

    ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
    return nullptr;
}

HOOK(0x0070EE80, Audio_EnumCallbackCreateCaptureSlot, "auto");
