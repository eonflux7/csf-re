#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// Returns max(0, global+2820), clamped below by global+2828. Each read calls
// GetGlobalValue_8BC1E0 again, matching the original's call sequence.
static double GetInitialStateValue()
{
    float initialValue = 0.0f;

    // if (0.0 < global->field_0B04) initialValue = global->field_0B04;
    {
        uint8_t* globals = reinterpret_cast<uint8_t*>(GetGlobalValue_8BC1E0());
        if (g_fZero < CSF_AT(float, globals, 0xB04)) {
            globals = reinterpret_cast<uint8_t*>(GetGlobalValue_8BC1E0());
            initialValue = CSF_AT(float, globals, 0xB04);
        }
    }

    // if (!(initialValue < global->field_0B0C)) return initialValue; else return global->field_0B0C;
    {
        uint8_t* globals = reinterpret_cast<uint8_t*>(GetGlobalValue_8BC1E0());
        if (!(initialValue < CSF_AT(float, globals, 0xB0C))) {
            return static_cast<double>(initialValue);
        }
        globals = reinterpret_cast<uint8_t*>(GetGlobalValue_8BC1E0());
        return static_cast<double>(CSF_AT(float, globals, 0xB0C));
    }
}

HOOK(0x493CF0, GetInitialStateValue, "auto");
