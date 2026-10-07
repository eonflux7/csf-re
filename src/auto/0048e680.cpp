#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall HasNonzeroLinkedValueDifferentFrom(
    void* self, void* /*edx*/, int comparisonValue)
{
    void* linkedObject = CSF_AT(void*, self, 0x48);
    if (((int(__fastcall*)(void*, void*))
             CSF_AT(void**, linkedObject, 0)[16])(linkedObject, nullptr) == 0)
    {
        return 0;
    }

    linkedObject = CSF_AT(void*, self, 0x48);
    return ((int(__fastcall*)(void*, void*))
                CSF_AT(void**, linkedObject, 0)[16])(linkedObject, nullptr)
        != comparisonValue;
}

HOOK(0x0048E680, HasNonzeroLinkedValueDifferentFrom, "auto");
