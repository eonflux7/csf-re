#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee bindings (exact signatures from the packet)
CSF_FUNC(signed char, __cdecl, RpAnisotGetMaxSupportedMaxAnisotropy, 0x00566520, ());
CSF_FUNC(int, __cdecl, RpAnisotTextureSetMaxAnisotropy, 0x00566530, (int a1, char a2));

// Sets the anisotropy filtering level of a texture object from the
// (filter mode, sample count, max anisotropy request) parameter block.
// a1: pointer to a slot whose first dword is the target object pointer.
// a2: parameter block { uint8 flag @+0, int mode @+4, int requested @+8 }.
// Returns the RpAnisotTextureSetMaxAnisotropy result when applied, otherwise
// the value of a1 (or the last capability query result when no call is made).
static int __cdecl SceneLoader_SetObjectPlacement(int a1, int a2) {
    int result = a1;
    int objectPtr = *reinterpret_cast<int*>(a1);

    if (objectPtr != 0) {
        uint8_t* object = reinterpret_cast<uint8_t*>(objectPtr);
        uint8_t* params = reinterpret_cast<uint8_t*>(a2);

        // Choose the filter-mode byte at object+0x50 from the mode field.
        int mode = CSF_AT(int, params, 4);
        if (mode == 0) {
            CSF_AT(uint8_t, object, 0x50) = 2;
        } else if (mode == 1) {
            CSF_AT(uint8_t, object, 0x50) = static_cast<uint8_t>(2 * (CSF_AT(uint8_t, params, 0) == 0) + 2);
        } else if (mode == 2) {
            CSF_AT(uint8_t, object, 0x50) = static_cast<uint8_t>(4 * (CSF_AT(uint8_t, params, 0) == 0) + 2);
        }

        signed char hardwareMaxAniso = RpAnisotGetMaxSupportedMaxAnisotropy();
        result = hardwareMaxAniso;

        if (hardwareMaxAniso > 0) {
            int requested = CSF_AT(int, params, 8);
            int levelToApply = 1;

            if (requested > 1) {
                signed char capability = RpAnisotGetMaxSupportedMaxAnisotropy();
                // Re-read the requested value after the call.
                int requestedAfterQuery = CSF_AT(int, params, 8);
                levelToApply = (requestedAfterQuery < capability) ? requestedAfterQuery : capability;
            }

            return RpAnisotTextureSetMaxAnisotropy(objectPtr, static_cast<char>(levelToApply));
        }
    }

    return result;
}

HOOK(0x006C4EB0, SceneLoader_SetObjectPlacement, "auto");
