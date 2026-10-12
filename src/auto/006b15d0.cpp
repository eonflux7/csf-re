#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original 0x6E0480: __cdecl, returns a new page runtime object (vtable-bearing), or null.
CSF_FUNC(uint8_t*, __cdecl, CreatePageRuntimeObject, 0x006E0480, ());

// Sets the current menu page runtime object from a page index.
// Releases the previous page object (vtable slot 0 with arg 1), creates a new one,
// and asks it (vtable slot 1, three args) whether it accepts the page. On failure the
// new object is released and the function returns 0.
static char __fastcall MenuRuntime_SetPageByIndex(C_0082d410* self, void* /*edx*/, int a2, int a3, int a4)
{
    uint8_t* previousPage = CSF_AT(uint8_t*, self, 0x8C);
    if (previousPage != nullptr)
    {
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, previousPage, 0)[0])(previousPage, nullptr, 1);
        CSF_AT(uint8_t*, self, 0x8C) = nullptr;
    }

    uint8_t* newPage = CreatePageRuntimeObject();
    CSF_AT(uint8_t*, self, 0x8C) = newPage;
    if (newPage != nullptr)
    {
        uint8_t accepted = ((uint8_t(__fastcall*)(void*, void*, int, int, int))CSF_AT(void**, newPage, 0)[1])(
            newPage, nullptr, a2, a3, a4);
        if (accepted != 0)
        {
            return 1;
        }

        uint8_t* currentPage = CSF_AT(uint8_t*, self, 0x8C);
        if (currentPage != nullptr)
        {
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, currentPage, 0)[0])(currentPage, nullptr, 1);
            CSF_AT(uint8_t*, self, 0x8C) = nullptr;
        }
    }
    return 0;
}

HOOK(0x006B15D0, MenuRuntime_SetPageByIndex, "auto");
