#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));

static int __fastcall S_00523cf0_ProcessObjectReference(
    S_00523cf0* self, void* /*edx*/, void* object)
{
    const std::uint32_t savedContext = CSF_AT(std::uint32_t, object, 0x1C);

    bool failed = false;
    if (CSF_AT(std::uint8_t, object, 0) == 0)
    {
        void* owner = CSF_AT(void*, object, 4);
        failed = !((bool(__fastcall*)(void*, void*, int, void*))
            CSF_AT(void**, owner, 0)[0x80 / 4])(owner, nullptr, 0, object);
    }

    CSF_AT(std::uint8_t, object, 0) = failed;
    CSF_AT(std::uint32_t, object, 0x1C) = 0;

    if (!failed)
    {
        void* owner = CSF_AT(void*, object, 4);
        CSF_AT(std::uint8_t, object, 0x15) = 0;
        CSF_AT(std::uint8_t, object, 0x16) = 0;
        const int result = ((int(__fastcall*)(void*, void*, S_00523cf0*, void*))
            CSF_AT(void**, owner, 0)[0x48 / 4])(owner, nullptr, self, object);
        CSF_AT(std::uint8_t, object, 0) = result == -1;
    }

    const bool shouldSerialize = CSF_AT(std::uint8_t, object, 0) == 0;
    CSF_AT(std::uint32_t, object, 0x1C) = 0;
    if (shouldSerialize)
    {
        CSF_AT(std::uint8_t, object, 0x15) = 0;
        CSF_AT(std::uint8_t, object, 0x16) = 0;
        Archive_SerializeObjectRef(
            reinterpret_cast<std::uint8_t*>(self) + 4,
            reinterpret_cast<bool*>(object));
    }

    const bool shouldFinalize = CSF_AT(std::uint8_t, object, 0) == 0;
    CSF_AT(std::uint32_t, object, 0x1C) = savedContext;
    if (shouldFinalize)
    {
        void* owner = CSF_AT(void*, object, 4);
        // Original tests only al (test al, al) but returns the whole eax.
        const int result = ((int(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, owner, 0)[0x7C / 4])(owner, nullptr, object);
        if (static_cast<std::uint8_t>(result))
        {
            CSF_AT(std::uint8_t, object, 0) = 0;
            return result;
        }
    }

    CSF_AT(std::uint8_t, object, 0) = 1;
    return 1;
}

HOOK(0x00523CF0, S_00523cf0_ProcessObjectReference, "auto");
