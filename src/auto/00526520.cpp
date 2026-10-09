#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));

// Re-reads the positional emitter of a sound instance from its scene object's position.
static void __fastcall C_00840ff0_RefreshEmitterPosition(C_00840ff0* self, void* /*edx*/, int a2) {
    (void)a2;
    if (self->field_0004 != 5) {
        return;
    }

    // Global service object, then its virtual getter at slot 28 (+0x70) returns the sound object.
    void* globalService = reinterpret_cast<void*>(static_cast<intptr_t>(GetGlobalValue_70DDE0()));
    using GetSoundObjectFn = int(__fastcall*)(void*, void*);
    void* soundObj = reinterpret_cast<void*>(static_cast<intptr_t>(
        reinterpret_cast<GetSoundObjectFn>(CSF_AT(void**, globalService, 0)[28])(globalService, nullptr)));
    if (soundObj == nullptr) {
        return;
    }

    // Negative flag value (sign bit set) means no valid handle.
    if (static_cast<int32_t>(self->field_0030) < 0) {
        return;
    }

    // Handle is the address of the embedded handle member at +0x28.
    int handle = static_cast<int>(reinterpret_cast<intptr_t>(reinterpret_cast<uint8_t*>(self) + 0x28));
    int sceneObjectHandle = CHandleTable_Resolve(g_pCurrentScene, nullptr, handle);
    if (sceneObjectHandle == 0) {
        return;
    }
    C_008342e8* sceneObject = reinterpret_cast<C_008342e8*>(static_cast<intptr_t>(sceneObjectHandle));

    void** soundVtable = CSF_AT(void**, soundObj, 0);
    int position = GetObjectPositionVector(sceneObject, nullptr);
    using SetEmitterPositionFn = void(__fastcall*)(void*, void*, int);
    reinterpret_cast<SetEmitterPositionFn>(soundVtable[17])(soundObj, nullptr, position);
}

HOOK(0x00526520, C_00840ff0_RefreshEmitterPosition, "auto");
