#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Typed view of the object returned by the first virtual call (vtable slot 6).
struct C_008670f0;

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

CSF_FUNC(int*, __fastcall, GfxSkin_LookupByteField, 0x006C4960, (void* self, void* edx, int* a2, int a3));
CSF_FUNC(int*, __fastcall, GfxSkin_LookupFloatField, 0x006C48C0, (void* self, void* edx, int* a2, int a3));

// True when the byte field looked up with key 12 equals 1, or when the float field
// looked up with key 3 is below g_TransitionOne.
static int __cdecl GfxSkin_IsTransitionComplete(int a2)
{
    void* obj = reinterpret_cast<void*>(static_cast<uintptr_t>(a2));
    void* skinState = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, obj, 0)[6])(obj, nullptr);

    int* byteField = GfxSkin_LookupByteField(skinState, nullptr, &a2, 12);
    if (*reinterpret_cast<uint8_t*>(byteField) == 1) {
        return 1;
    }

    int floatKey = 0;
    int* floatField = GfxSkin_LookupFloatField(skinState, nullptr, &floatKey, 3);
    if (*reinterpret_cast<float*>(floatField) < g_TransitionOne) {
        return 1;
    }
    return 0;
}

HOOK(0x006C5990, GfxSkin_IsTransitionComplete, "auto");
