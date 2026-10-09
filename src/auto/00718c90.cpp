#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static std::uint64_t __cdecl GuiControl_RoundToNearestInteger(float value)
{
    double adjusted = value;
    if (!(value >= g_fZero))
        adjusted -= g_HalfDurationScale;
    else
        adjusted += g_HalfDurationScale;

    return static_cast<std::uint64_t>(static_cast<std::int64_t>(adjusted));
}

HOOK(0x00718C90, GuiControl_RoundToNearestInteger, "auto");
