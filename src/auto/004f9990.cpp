#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MenuControls_ResetSessionChild, 0x00712CE0, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, MenuControls_ApplyChildIndex, 0x00712D90, (void* self, void* edx, int index));

CSF_GLOBAL(int, g_Value8B055C, 0x008B055C);

static int __fastcall C_0085ec18_ResetSession(C_0085ec18* self, void* /*edx*/)
{
    const int sessionChild = CSF_AT(int, self, 0x0C);
    void* referencedObject = GetReferencedObject(self, nullptr);
    MenuControls_ResetSessionChild(referencedObject, nullptr, sessionChild);

    const int childIndex = g_Value8B055C;
    referencedObject = GetReferencedObject(self, nullptr);
    MenuControls_ApplyChildIndex(referencedObject, nullptr, childIndex);

    CSF_AT(int, self, 0x0C) = 0;
    CSF_AT(uint8_t, self, 0x10) = 0;
    return 0;
}

HOOK(0x004F9990, C_0085ec18_ResetSession, "auto");
