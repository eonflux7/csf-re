#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

CSF_FUNC(void*, __fastcall, GetMenuEventOwner, 0x006683F0,
         (void* self, void* edx));

static int __fastcall MenuMpPause_HandleMenuEvent(
    void* self, void* /*edx*/, std::uint32_t eventId, void* eventData)
{
    if (eventId == 1)
    {
        void* receiver = CSF_AT(void*, self, 0x30);
        void** vtable = CSF_AT(void**, receiver, 0);
        void* parent = reinterpret_cast<void*>(
            reinterpret_cast<std::uintptr_t>(self) - 4u);

        return reinterpret_cast<int (__fastcall*)(void*, void*, void*, int)>(
            vtable[0x60 / 4])(receiver, nullptr, parent, 0);
    }

    const std::uint32_t result = eventId - 50u;
    if (result != 0)
        return static_cast<std::int32_t>(result);

    if (CSF_AT(std::uint8_t, eventData, 0x38) == 0)
        return static_cast<std::int32_t>(
            reinterpret_cast<std::uintptr_t>(eventData));

    void* owner = GetMenuEventOwner(self, nullptr);
    void** vtable = CSF_AT(void**, owner, 0);
    return reinterpret_cast<int (__fastcall*)(void*, void*)>(
        vtable[0x1C / 4])(owner, nullptr);
}

HOOK(0x00533A30, MenuMpPause_HandleMenuEvent, "auto");
