#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(int, __fastcall, ReleaseSubObjectResources, 0x006DA9A0, (void* self, void* edx));

// Releases the matrix owned by this object and clears its active flag.
// Only does work when the flag at +0x000 is set.
static int __fastcall S_006d24a0_ReleaseMatrix(S_006d24a0* self, void* /*edx*/)
{
    int result = 0;

    if (self->field_0000)
    {
        ReleaseSubObjectResources(self, nullptr);

        result = static_cast<int>(self->field_0040);
        if (result)
        {
            result = RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(self->field_0040));
            self->field_0040 = 0;
        }

        self->field_0000 = 0;
    }

    return result;
}

HOOK(0x006DAD40, S_006d24a0_ReleaseMatrix, "auto");
