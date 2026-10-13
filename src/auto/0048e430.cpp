#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Gets the object's position vector: asks the owner for its component, then
// walks two virtual getters (slot +0x18 on the component, then the result's
// slot +0x10 is tail-jumped to with the same stack arguments).
static hook::Tail __fastcall C_008342e8_GetObjectPositionVector(void* self, const uint32_t* /*args*/)
{
    C_008342e8* owner = reinterpret_cast<C_008342e8*>(self);

    // mov eax, [ecx] ; call [eax+0x28]  -> typed wrapper vGetField016c()
    void* firstComponent = reinterpret_cast<void*>(static_cast<uintptr_t>(owner->vGetField016c()));

    // mov edx, [eax] ; mov ecx, eax ; call [edx+0x18]
    void* (__fastcall* getSecond)(void*, void*) =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(CSF_AT(void**, firstComponent, 0)[0x18 / 4]);
    void* secondObject = getSecond(firstComponent, nullptr);

    // mov edx, [eax] ; mov ecx, eax ; jmp [edx+0x10]
    return hook::tail_jump(secondObject, (*reinterpret_cast<void***>(secondObject))[0x10 / 4]);
}

HOOK_TAIL(0x0048E430, C_008342e8_GetObjectPositionVector, "auto");
