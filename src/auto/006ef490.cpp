#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_fZero, 0x82D4CC);

// Slot table starts at +0x24 with a dword stride. Each active entry i holds a
// child object pointer; the float scale for that entry is read from the dword
// three slots further on (+0x0C from the slot), exactly as the original does.
static unsigned int __fastcall GfxScene_UpdatePrimaryObject(void* self, void* /*edx*/, float a2) {
    unsigned int result = CSF_AT(uint32_t, self, 0x20);
    unsigned int index = 0;

    if (result) {
        do {
            const uint32_t slotOffset = 0x24 + index * 4;
            const uint32_t element = CSF_AT(uint32_t, self, slotOffset);

            if (element) {
                if (CSF_AT(float, self, slotOffset + 12) != g_fZero) {
                    void* obj = reinterpret_cast<void*>(static_cast<uintptr_t>(element));
                    void* child = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, obj, 0)[6])(obj, nullptr);

                    float scaled = a2 * CSF_AT(float, self, slotOffset + 12);
                    ((void (__fastcall*)(void*, void*, float))CSF_AT(void**, child, 0)[17])(child, nullptr, scaled);
                }
            }

            result = CSF_AT(uint32_t, self, 0x20);
            ++index;
        } while (index < result);
    }

    return result;
}

HOOK(0x006EF490, GfxScene_UpdatePrimaryObject, "auto");
