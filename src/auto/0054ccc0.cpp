#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __stdcall TypeRegistry_SerializeAABox(void* box, void* context)
{
    bool failed = false;
    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* serializer = CSF_AT(void*, context, 4);
        failed = !((bool(__fastcall*)(void*, void*, const char*, void*))
            CSF_AT(void**, serializer, 0)[128 / 4])(
                serializer, nullptr, CSF_STR(0x00844388, "CAABox"), context);
    }
    CSF_AT(uint8_t, context, 0) = failed;

    // Serialize the second corner first, then the first corner.
    constexpr int componentOffsets[] = {12, 16, 20, 0, 4, 8};
    for (int component = 0; component < 6; ++component)
    {
        if (component != 0)
            failed = CSF_AT(uint8_t, context, 0) != 0;

        CSF_AT(uint32_t, context, 28) = 0;
        if (!failed)
        {
            void* serializer = CSF_AT(void*, context, 4);
            CSF_AT(uint8_t, context, 21) = 0;
            CSF_AT(uint8_t, context, 22) = 0;

            failed = ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, serializer, 0)[84 / 4])(
                    serializer, nullptr,
                    static_cast<uint8_t*>(box) + componentOffsets[component],
                    context) == -1;
            CSF_AT(uint8_t, context, 0) = failed;
        }
    }

    if (!failed)
    {
        void* serializer = CSF_AT(void*, context, 4);
        if (((bool(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, serializer, 0)[124 / 4])(
                serializer, nullptr, context))
        {
            CSF_AT(uint8_t, context, 0) = 0;
            return 0;
        }
    }

    CSF_AT(uint8_t, context, 0) = 1;
    return -1;
}

HOOK(0x0054CCC0, TypeRegistry_SerializeAABox, "auto");
