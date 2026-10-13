#include <cstdint>
#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __stdcall, PrepareStaticAiState, 0x004DC3C0, (void* a1));
CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));

static int __stdcall AiOrder_PrepareAndDispatchLinkedObject(C_0085e760* a1)
{
    PrepareStaticAiState(a1);
    int entryValue = ResolveEntryLookupValue(a1, nullptr);
    int fieldResult = GetLinkedObjectField10(a1, nullptr);

    void* fieldObj = reinterpret_cast<void*>(fieldResult);
    if (CSF_AT(uint8_t, fieldObj, 0xB0) != 0)
    {
        int linkedValue = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(entryValue), nullptr);
        if (linkedValue != 0)
        {
            void* target = CSF_AT(void*, reinterpret_cast<void*>(linkedValue), 0x714);
            return ((int(__fastcall*)(void*, void*))CSF_AT(void**, target, 0)[160 / 4])(target, nullptr);
        }

        // Original reads the absolute address 0x718 (MEMORY[0x718]) with a zero base.
        void* zeroBase = static_cast<void*>(nullptr);
        void* fallback = CSF_AT(void*, zeroBase, 0x718);
        return ((int(__fastcall*)(void*, void*))CSF_AT(void**, fallback, 0)[160 / 4])(fallback, nullptr);
    }
    return fieldResult;
}

HOOK(0x004ACE70, AiOrder_PrepareAndDispatchLinkedObject, "auto");
