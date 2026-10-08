#include <cstdint>
#include <cstring>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0,
         (void* self, void* edx));

// Handles an event delivered to a control embedded (at -4) inside a menu widget.
// eventCode 1/14 -> forward to the owner's virtual handler (vtbl slot 0x4C/4).
// eventCode 50   -> if the supplied record has a byte set at +0x38, resolve the
//                   referenced object and invoke its virtual method at slot 0x1C/4.
static int __fastcall CMenuAccountEdit_HandleEmbeddedControlEvent(
    void* self, void* /*edx*/, int eventCode, int eventData)
{
    using HandleEvent = int (__fastcall*)(void*, void*);

    if (eventCode == 1 || eventCode == 14)
    {
        void* owner = static_cast<std::uint8_t*>(self) - 4;
        return reinterpret_cast<HandleEvent>(
            CSF_AT(void**, owner, 0)[0x4C / 4])(
                owner, nullptr);
    }

    if (eventCode == 50)
    {
        void* data = reinterpret_cast<void*>(
            static_cast<std::uint32_t>(eventData));
        if (CSF_AT(std::uint8_t, data, 0x38))
        {
            void* object = GetReferencedObject(self, nullptr);
            return reinterpret_cast<HandleEvent>(
                CSF_AT(void**, object, 0)[0x1C / 4])(
                    object, nullptr);
        }
        return eventData;
    }

    std::uint32_t resultBits =
        static_cast<std::uint32_t>(eventCode) - 50u;
    int result;
    std::memcpy(&result, &resultBits, sizeof(result));
    return result;
}

HOOK(0x0055DE90, CMenuAccountEdit_HandleEmbeddedControlEvent, "auto");
