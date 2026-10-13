#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, sub_6DC750, 0x006DC750, (void* self, void* edx, uint32_t* a2));

static int* __fastcall S_006dc750_QueryVideoOptions(
    S_006dc750* self, void* /*edx*/, int* options, uint32_t* capabilities)
{
    // The packet also lists the callee address as a vtable reference.
    (void)CSF_VTBL(0x006DC750);

    int qualityLevel = -1;
    int shaderOption = -1;
    int performanceLevel = -1;

    if (sub_6DC750(self, nullptr, capabilities))
    {
        const uint32_t speed = capabilities[0];
        shaderOption = 0;
        performanceLevel = 2;

        if (speed < 0xBB8 ||
            capabilities[1] < 0x400 ||
            capabilities[2] < 0x100 ||
            capabilities[3] < 0xFFFE0200 ||
            capabilities[4] < 0xFFFE0200)
        {
            if (speed < 0x708 ||
                capabilities[1] < 0x200 ||
                capabilities[2] < 0x80 ||
                capabilities[3] < 0xFFFE0110 ||
                capabilities[4] < 0xFFFE0130)
            {
                qualityLevel = 0;
            }
            else
            {
                qualityLevel = 1;
            }
        }
        else
        {
            qualityLevel = 2;
        }

        if (capabilities[4] >= 0xFFFE0200 &&
            capabilities[3] >= 0xFFFE0200)
        {
            shaderOption = 1;
        }

        if (speed < 0xBB8)
            performanceLevel = speed >= 0x640;
    }

    options[0] = qualityLevel;
    options[1] = shaderOption;
    options[2] = performanceLevel;
    return options;
}

HOOK(0x006DC7E0, S_006dc750_QueryVideoOptions, "auto");
