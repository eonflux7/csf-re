#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall NetCallbacks_SetRuleOption(
    void* self, void* /*edx*/, int option, int value)
{
    int result = static_cast<int>(static_cast<std::uint32_t>(option) - 13u);

    switch (option)
    {
    case 13:
        CSF_AT(int, self, 0x08) = value;
        result = value;
        break;
    case 14:
        CSF_AT(int, self, 0x38) = value;
        break;
    case 15:
        CSF_AT(int, self, 0x30) = value;
        result = value;
        break;
    case 16:
        CSF_AT(int, self, 0x28) = value;
        break;
    case 17:
        CSF_AT(int, self, 0x20) = value;
        result = value;
        break;
    case 18:
        CSF_AT(int, self, 0x18) = value;
        break;
    case 19:
        CSF_AT(int, self, 0x10) = value;
        result = value;
        break;
    case 22:
        CSF_AT(int, self, 0x04) = value;
        break;
    case 23:
        CSF_AT(int, self, 0x34) = value;
        result = value;
        break;
    case 24:
        CSF_AT(int, self, 0x2C) = value;
        break;
    case 25:
        CSF_AT(int, self, 0x24) = value;
        result = value;
        break;
    case 26:
        CSF_AT(int, self, 0x1C) = value;
        break;
    case 27:
        CSF_AT(int, self, 0x14) = value;
        result = value;
        break;
    case 28:
        CSF_AT(int, self, 0x0C) = value;
        break;
    default:
        break;
    }

    return result;
}

HOOK(0x00744150, NetCallbacks_SetRuleOption, "auto");
