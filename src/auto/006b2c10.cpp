#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Looks up element a2 in this object's table (count at +0x24, pointer array at +0x20)
// and calls its slot-4 virtual method (vtable +0x10). Returns -1 if the index is out
// of range or the element is null; otherwise returns the slot-4 result.
static int __fastcall C_00860288_QueryIndexedElement(C_00860288* self, void* /*edx*/, unsigned int index)
{
    int result = -1;
    if (index < static_cast<unsigned int>(self->field_0024))
    {
        void* element = reinterpret_cast<void*>(reinterpret_cast<uint32_t*>(self->field_0020)[index]);
        if (element)
        {
            result = ((int(__fastcall*)(void*, void*))CSF_AT(void**, element, 0)[4])(element, nullptr);
        }
    }
    return result;
}

HOOK(0x006B2C10, C_00860288_QueryIndexedElement, "auto");
