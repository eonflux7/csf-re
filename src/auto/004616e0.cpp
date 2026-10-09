#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callee bindings (by original address).
CSF_FUNC(int, __cdecl, GetGlobal8afc7c, 0x0043E010, ());
CSF_FUNC(uint32_t*, __fastcall, sub_43E730, 0x0043E730, (void* self, void* edx, uint32_t* a2));

// Reads a descriptor chain: a3 -> owner, owner+4 -> block, block+28 = count.
// Inline payload at block+8 when count < 0x10, otherwise the pointer stored at block+8.
// The payload value is pushed as the stack argument of sub_43E730, which is invoked
// on the result of GetGlobal8afc7c. Callee pops 12 bytes (stdcall, 3 args).
static int __stdcall DispatchDescriptorPayload(int unusedFirst, int unusedSecond, int descriptorRef)
{
    (void)unusedFirst;
    (void)unusedSecond;

    uint32_t owner = *reinterpret_cast<uint32_t*>(descriptorRef);
    uint8_t* block = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, owner, 4));
    uint32_t entryCount = CSF_AT(uint32_t, block, 28);
    uint8_t* payloadSlot = block + 4;

    int payloadValue;
    if (entryCount < 0x10)
        payloadValue = reinterpret_cast<int>(payloadSlot + 4);
    else
        payloadValue = static_cast<int>(CSF_AT(uint32_t, payloadSlot, 4));

    uint32_t* globalObject = reinterpret_cast<uint32_t*>(GetGlobal8afc7c());
    return reinterpret_cast<int>(sub_43E730(globalObject, nullptr,
                                            reinterpret_cast<uint32_t*>(payloadValue)));
}

HOOK(0x004616E0, DispatchDescriptorPayload, "auto");
