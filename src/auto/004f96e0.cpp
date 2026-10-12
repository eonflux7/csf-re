#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SetConsoleControlState, 0x00712CE0, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, MenuControls_ApplyChildIndex, 0x00712D90, (void* self, void* edx, int index));
CSF_GLOBAL(int, g_Value8B055C, 0x008B055C);

static void __fastcall C_0085ec18_ReleaseConsoleResources(
    C_0085ec18* self, void* /*edx*/)
{
    if (CSF_AT(uint8_t, self, 0x10))
    {
        const int state = CSF_AT(int, self, 0x0C);
        void* referencedObject = GetReferencedObject(self, nullptr);
        SetConsoleControlState(referencedObject, nullptr, state);

        const int index = g_Value8B055C;
        referencedObject = GetReferencedObject(self, nullptr);
        MenuControls_ApplyChildIndex(referencedObject, nullptr, index);
        CSF_AT(int, self, 0x0C) = 0;
    }

    void* resource = CSF_AT(void*, self, 0xF0);
    if (resource)
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, resource, 0)[0])(resource, nullptr, 1);
        CSF_AT(void*, self, 0xF0) = nullptr;
    }

    resource = CSF_AT(void*, self, 0xF4);
    if (resource)
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, resource, 0)[0])(resource, nullptr, 1);
        CSF_AT(void*, self, 0xF4) = nullptr;
    }
}

HOOK(0x004F96E0, C_0085ec18_ReleaseConsoleResources, "auto");
