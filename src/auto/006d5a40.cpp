#include <cstdint>
#include "globals.h"
#include "hook.h"

// Per-element update call: vtable slot 7 (0x1C), ecx = element, one stack arg (a2).
// Hex-Rays labels it __stdcall, but the disassembly leaves ecx = element, so bind as thiscall-style.
static int __fastcall GfxScene_RunSubsystemUpdate(void* self, void* /*edx*/, void* a2)
{
    CSF_AT(uint8_t, self, 0x49) = 1;
    CSF_AT(uint8_t, self, 0x34) = 0;

    // a2->vtbl[4]() -> object; object->vtbl[4]() -> time source; read float at +4.
    void* stateObject = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, a2, 0)[4])(a2, nullptr);
    void* timeSource = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, stateObject, 0)[4])(stateObject, nullptr);
    if (CSF_AT(float, timeSource, 4) <= CSF_AT(float, self, 0x9C)) {
        CSF_AT(uint8_t, self, 0x34) = 1;
    }

    int returnValue = 0;
    if (CSF_AT(uint8_t, self, 1) != 0) {
        uint32_t count = CSF_AT(uint32_t, self, 0xC);
        CSF_AT(uint8_t, self, 1) = 0;
        CSF_AT(uint8_t, self, 0x8C) = 0;

        for (uint32_t index = 0; index < count; ++index) {
            void** elementArray = CSF_AT(void**, self, 4);
            void* element = elementArray[index];
            if (element != nullptr) {
                CSF_AT(int, element, 0xB4) = (CSF_AT(uint8_t, self, 0x34) != 0) ? 1 : 0;
                ((void (__fastcall*)(void*, void*, void*))CSF_AT(void**, element, 0)[7])(element, nullptr, a2);
                CSF_AT(uint8_t, self, 0x8C) = 1;
            }
            count = CSF_AT(uint32_t, self, 0xC);
        }
        returnValue = static_cast<int>(count);
        CSF_AT(uint8_t, self, 1) = 1;
    }

    CSF_AT(uint8_t, self, 0x49) = 0;
    return returnValue;
}

HOOK(0x6D5A40, GfxScene_RunSubsystemUpdate, "auto");
