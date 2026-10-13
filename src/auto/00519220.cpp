#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(void*, __fastcall, C_00830438_Construct, 0x00712CC0, (void* self, void* edx));

CSF_GLOBAL(int32_t, g_Value_008B0664, 0x008B0664);

// Destroys the previous singleton instance (if any), then allocates and constructs a new one.
// Returns the constructed object, or nullptr when the allocation fails.
static C_00830438* C_00830438_ReplaceSingletonInstance()
{
    if (g_Value_008B0664)
    {
        void* previousInstance = reinterpret_cast<void*>(g_Value_008B0664);
        // Slot 0 of the vtable is the deleting destructor; the argument 1 requests the free.
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, previousInstance, 0)[0])(previousInstance, nullptr, 1);
        g_Value_008B0664 = 0;
    }

    int allocatedAddress = CoreMemory_AllocUntagged(16);
    if (allocatedAddress != 0)
    {
        void* instance = reinterpret_cast<void*>(allocatedAddress);
        void* result = C_00830438_Construct(instance, nullptr);
        CSF_AT(uint8_t, instance, 0xC) = 0;
        CSF_AT(uint8_t, instance, 0xD) = 0;
        CSF_AT(void**, instance, 0) = CSF_VTBL(0x00840C58);
        g_Value_008B0664 = allocatedAddress;
        return reinterpret_cast<C_00830438*>(result);
    }

    g_Value_008B0664 = 0;
    return nullptr;
}

HOOK(0x519220, C_00830438_ReplaceSingletonInstance, "auto");
