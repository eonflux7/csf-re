#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Virtual slot 0x48 / 4 = 18 on the embedded sub-object at this+0x10 (its first dword is the vtable).
// The slot returns a pointer to three dwords (a 3-component vector).
using GetEmbeddedVectorFn = uint32_t* (__fastcall*)(void* self, void* edx);

static uint32_t* __fastcall CLightObject_CopyEmbeddedVectorOut(CLightObject* self, void* /*edx*/, uint32_t* a2)
{
    uint8_t* embeddedObject = reinterpret_cast<uint8_t*>(self) + 0x10;
    void** embeddedVtable = CSF_AT(void**, self, 0x10);
    GetEmbeddedVectorFn getVector = reinterpret_cast<GetEmbeddedVectorFn>(embeddedVtable[0x48 / 4]);

    uint32_t* source = getVector(embeddedObject, nullptr);
    a2[0] = source[0];
    a2[1] = source[1];
    a2[2] = source[2];
    return a2;
}

HOOK(0x6FD1B0, CLightObject_CopyEmbeddedVectorOut, "auto");
