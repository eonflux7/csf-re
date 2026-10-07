#include <cstdint>

#include "globals.h"
#include "hook.h"

static void __cdecl ClearChildAssociation(void* child)
{
    if (!child)
        return;

    void* related =
        ((void* (__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[68])(
            child, nullptr);

    if (related)
    {
        ((void (__fastcall*)(void*, void*, const char*))
            CSF_AT(void**, related, 0)[15])(
                related, nullptr, CSF_STR(0x0082E65C, ""));
    }
}

static void __fastcall ClearChildAssociations(void* self, void* /*edx*/)
{
    // The thunk adjusts this by -4 before entering the shared body.
    std::uint8_t* adjusted = static_cast<std::uint8_t*>(self) - 4;

    ClearChildAssociation(CSF_AT(void*, adjusted, 0x44));
    ClearChildAssociation(CSF_AT(void*, adjusted, 0x48));
    ClearChildAssociation(CSF_AT(void*, adjusted, 0x4C));
    ClearChildAssociation(CSF_AT(void*, adjusted, 0x50));
    ClearChildAssociation(CSF_AT(void*, adjusted, 0x54));
    ClearChildAssociation(CSF_AT(void*, adjusted, 0x58));
    ClearChildAssociation(CSF_AT(void*, adjusted, 0x5C));

    std::uint8_t* childSlot = adjusted + 0x60;
    int remaining = 3;
    do
    {
        ClearChildAssociation(CSF_AT(void*, childSlot, 0));
        childSlot += 4;
        --remaining;
    }
    while (remaining);
}

HOOK(0x0052AED0, ClearChildAssociations, "auto");
