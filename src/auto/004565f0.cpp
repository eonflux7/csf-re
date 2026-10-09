#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, sub_456360, 0x00456360, (void* self, void* edx, int a3));
CSF_FUNC(int, __fastcall, sub_4A0FF0, 0x004A0FF0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, sub_4A10A0, 0x004A10A0, (void* self, void* edx, char* a2));

// Returns the number of 48-byte entries in the range [field_00cc, field_00d0),
// after forwarding a2 into the sub-object at +0xC8 via sub_456360.
static int __fastcall S_00452980_GetElementCount(S_00452980* self, void* /*edx*/, char* a2)
{
    uint32_t rangeBegin = self->field_00cc;
    S_00455ed0* entryList = reinterpret_cast<S_00455ed0*>(reinterpret_cast<uint8_t*>(self) + 0xC8);

    int elementCount;
    if (rangeBegin != 0) {
        int rangeBytes = (int)(self->field_00d0 - rangeBegin);
        elementCount = rangeBytes / 48;
    } else {
        elementCount = 0;
    }

    C_00831278 tempBuffer;
    sub_4A10A0(&tempBuffer, nullptr, a2);
    sub_456360(entryList, nullptr, reinterpret_cast<int>(&tempBuffer));
    sub_4A0FF0(&tempBuffer, nullptr);

    return elementCount;
}

HOOK(0x004565F0, S_00452980_GetElementCount, "auto");
