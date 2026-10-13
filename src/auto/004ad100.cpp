#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));

static int __stdcall AiStateFactory_ApplyRequestArguments(int a1, int a2, int a3, int a4)
{
    void* owner = reinterpret_cast<void*>(static_cast<uintptr_t>(a1));

    // First block: request fields written at offsets 0x378..0x380 on the linked object (base = value - 4).
    int linkedValue = GetLinkedObjectValue_4DB5C0(owner, nullptr);
    void* requestBase = linkedValue != 0
        ? reinterpret_cast<void*>(static_cast<uintptr_t>(linkedValue - 4))
        : static_cast<void*>(nullptr);
    CSF_AT(int, requestBase, 0x378) = a2;
    CSF_AT(int, requestBase, 0x380) = a4;
    CSF_AT(int, requestBase, 0x368) = 1;
    CSF_AT(int, requestBase, 0x370) = 0;
    CSF_AT(int, requestBase, 0x374) = 0;
    CSF_AT(int, requestBase, 0x37C) = a3;

    // Second block: two float timers (0.1f bit pattern) at 0x370 / 0x374.
    linkedValue = GetLinkedObjectValue_4DB5C0(owner, nullptr);
    void* timerBase = linkedValue != 0
        ? reinterpret_cast<void*>(static_cast<uintptr_t>(linkedValue - 4))
        : static_cast<void*>(nullptr);
    CSF_AT(int, timerBase, 0x370) = 0x3DCCCCCD;
    CSF_AT(int, timerBase, 0x374) = 0x3DCCCCCD;

    // Third block: clear flag at 0x368 on the raw linked value; otherwise clear 0x36C at null base and return 0.
    int result = GetLinkedObjectValue_4DB5C0(owner, nullptr);
    if (result != 0)
    {
        void* flagBase = reinterpret_cast<void*>(static_cast<uintptr_t>(result));
        CSF_AT(int, flagBase, 0x368) = 0;
        return result;
    }

    CSF_AT(int, static_cast<void*>(nullptr), 0x36C) = 0;
    return 0;
}

HOOK(0x004AD100, AiStateFactory_ApplyRequestArguments, "auto");
