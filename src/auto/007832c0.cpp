#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(const char*, g_pszKey_LISTADATOS_8A9BFC, 0x008A9BFC);

CSF_FUNC(S_00692b80*, __fastcall, LookupNamedElement, 0x00693360, (S_00692b80* self, void* , char* String1));
CSF_FUNC(int, __fastcall, S_00692b80_GetChildCount, 0x00692B80, (S_00692b80* self, void*));
CSF_FUNC(S_00692b80*, __fastcall, S_00692b80_GetComponent, 0x00692BD0, (S_00692b80* self, void* , unsigned int index));
CSF_FUNC(char, __fastcall, ProcessListaDatosComponent, 0x007830E0, (void* self, void* edx, void* a2));

static char __fastcall ProcessListaDatosChildren(S_00782f90* self, void* /*edx*/, S_00692b80* a2)
{
    // Reference to the child-list class vtable (documentation only, no runtime effect).
    (void)CSF_VTBL(0x00692B80);

    S_00692b80* listElement = LookupNamedElement(a2, nullptr, const_cast<char*>(g_pszKey_LISTADATOS_8A9BFC));
    unsigned int childCount = S_00692b80_GetChildCount(listElement, nullptr);
    for (unsigned int i = 0; i < childCount; ++i)
    {
        S_00692b80* component = S_00692b80_GetComponent(listElement, nullptr, i);
        ProcessListaDatosComponent(self, nullptr, component);
    }
    return 1;
}

HOOK(0x007832C0, ProcessListaDatosChildren, "auto");
