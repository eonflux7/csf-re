#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Copies two words from the source object into this object, asks the source
// for a child object, checks the child's slot-36 predicate, then passes this
// object's slot-10 result into the child's slot-40 method.
static int __fastcall S_0049cfa0_SyncPositionAndNotifyChild(S_0049cfa0* self, void* /*edx*/, uint32_t* a2)
{
    self->field_01ac = CSF_AT(uint32_t, a2, 0x20);
    self->field_01b0 = CSF_AT(uint32_t, a2, 0x24);

    // a2 slot 10 (+0x28): returns the child object pointer.
    uint32_t child = ((uint32_t(__fastcall*)(void*, void*))CSF_AT(void**, a2, 0)[10])(a2, nullptr);
    if (child == 0)
        return static_cast<int>(child);

    void* childObj = reinterpret_cast<void*>(child);

    // child slot 36 (+0x90): predicate; zero means nothing to do.
    int hasPart = ((int(__fastcall*)(void*, void*))CSF_AT(void**, childObj, 0)[36])(childObj, nullptr);
    if (hasPart == 0)
        return hasPart;

    // Read the child's vtable before calling into self, as the original does.
    void** childVtbl = CSF_AT(void**, childObj, 0);

    // self slot 10 (+0x28): called with (-1, 1).
    int targetValue = ((int(__fastcall*)(void*, void*, int, int))CSF_AT(void**, self, 0)[10])(self, nullptr, -1, 1);

    // child slot 40 (+0xA0): one argument, the value from above.
    return ((int(__fastcall*)(void*, void*, int))childVtbl[40])(childObj, nullptr, targetValue);
}

HOOK(0x49CFA0, S_0049cfa0_SyncPositionAndNotifyChild, "auto");
