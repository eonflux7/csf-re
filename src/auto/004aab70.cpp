#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void, __stdcall, PrepareStaticAiState, 0x004DC3C0, (void* a1));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, C_DecrementCounterAt30, 0x0048CAC0, (void* self, void*, int));

static int __stdcall AiDecrementLinkedObjectCounter(void* a1)
{
    PrepareStaticAiState(a1);

    int linkedValue = GetLinkedObjectValue_4DB5C0(a1, nullptr);
    int argValue = linkedValue ? linkedValue - 4 : 0;

    int ownerValue = GetLinkedObjectValue_4DB5C0(a1, nullptr);
    int ownerAddr = ownerValue ? ownerValue - 4 : 0;

    void* ownerObject = reinterpret_cast<void*>(ownerAddr);
    void* counterTarget = CSF_AT(void*, CSF_AT(void*, ownerObject, 0x2C), 0x204);

    return C_DecrementCounterAt30(counterTarget, nullptr, argValue);
}

HOOK(0x004AAB70, AiDecrementLinkedObjectCounter, "auto");
