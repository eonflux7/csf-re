#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee: sub_40B100, __fastcall with one implicit this argument.
CSF_FUNC(int, __fastcall, sub_40B100, 0x0040B100, (void* self, void* edx));

// Finds the last child in the owner's child array whose id (at +8) equals targetId.
// If no child matches, returns 1. Otherwise returns the result of processing that child.
static char __fastcall ProcessLastChildMatchingId(void* self, void* /*edx*/, int targetId)
{
    uint32_t remainingCount = CSF_AT(uint32_t, self, 0x24);
    void* matchedChild = nullptr;

    if (remainingCount == 0)
        return 1;

    uint32_t childArrayBase = CSF_AT(uint32_t, self, 0x1c);
    void** childSlot = reinterpret_cast<void**>(childArrayBase);

    do
    {
        void* child = *childSlot;
        if (CSF_AT(int, child, 0x08) == targetId)
            matchedChild = child;
        ++childSlot;
        --remainingCount;
    } while (remainingCount != 0);

    if (matchedChild == nullptr)
        return 1;

    return static_cast<char>(sub_40B100(matchedChild, nullptr));
}

HOOK(0x408500, ProcessLastChildMatchingId, "auto");
