#include "hook.h"
#include "globals.h"
#include "core/memory.h"

static bool __fastcall Geometry_ContainsPointInTriangle(
    float* self, void* /*edx*/, float* point)
{
    bool inside = false;

    if (CSF_AT(float, self, 0x0C) != CSF_AT(float, self, 0x04)
        && ((CSF_AT(float, self, 0x04) >= CSF_AT(float, point, 0x04)
             && CSF_AT(float, self, 0x0C) <= CSF_AT(float, point, 0x04))
            || (CSF_AT(float, self, 0x04) <= CSF_AT(float, point, 0x04)
                && CSF_AT(float, self, 0x0C) >= CSF_AT(float, point, 0x04)))
        && (CSF_AT(float, point, 0x04) - CSF_AT(float, self, 0x04))
               * (CSF_AT(float, self, 0x08) - CSF_AT(float, self, 0x00))
               / (CSF_AT(float, self, 0x0C) - CSF_AT(float, self, 0x04))
               + CSF_AT(float, self, 0x00)
           > CSF_AT(float, point, 0x00))
    {
        inside = true;
    }

    if (CSF_AT(float, self, 0x14) != CSF_AT(float, self, 0x0C)
        && ((CSF_AT(float, self, 0x0C) >= CSF_AT(float, point, 0x04)
             && CSF_AT(float, self, 0x14) <= CSF_AT(float, point, 0x04))
            || (CSF_AT(float, self, 0x0C) <= CSF_AT(float, point, 0x04)
                && CSF_AT(float, self, 0x14) >= CSF_AT(float, point, 0x04)))
        && (CSF_AT(float, point, 0x04) - CSF_AT(float, self, 0x0C))
               * (CSF_AT(float, self, 0x10) - CSF_AT(float, self, 0x08))
               / (CSF_AT(float, self, 0x14) - CSF_AT(float, self, 0x0C))
               + CSF_AT(float, self, 0x08)
           > CSF_AT(float, point, 0x00))
    {
        inside = !inside;
    }

    if (CSF_AT(float, self, 0x04) != CSF_AT(float, self, 0x14)
        && ((CSF_AT(float, self, 0x14) >= CSF_AT(float, point, 0x04)
             && CSF_AT(float, self, 0x04) <= CSF_AT(float, point, 0x04))
            || (CSF_AT(float, self, 0x14) <= CSF_AT(float, point, 0x04)
                && CSF_AT(float, self, 0x04) >= CSF_AT(float, point, 0x04)))
        && (CSF_AT(float, point, 0x04) - CSF_AT(float, self, 0x14))
               * (CSF_AT(float, self, 0x00) - CSF_AT(float, self, 0x10))
               / (CSF_AT(float, self, 0x04) - CSF_AT(float, self, 0x14))
               + CSF_AT(float, self, 0x10)
           > CSF_AT(float, point, 0x00))
    {
        return !inside;
    }

    return inside;
}

HOOK(0x006AFCA0, Geometry_ContainsPointInTriangle, "auto");
