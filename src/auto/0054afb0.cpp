#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __cdecl TypeRegistry_CompareNames(const char* left, const char* right)
{
    const auto* a = reinterpret_cast<const unsigned char*>(left);
    const auto* b = reinterpret_cast<const unsigned char*>(right);
    while (*a == *b)
    {
        if (*a == 0)
            return 0;
        ++a;
        ++b;
    }
    return *a < *b ? -1 : 1;
}

static int* __fastcall TypeRegistry_FindNameLowerBound(
    S_0054afb0* self, void* /*edx*/, const char** name)
{
    int* candidate = reinterpret_cast<int*>(self->field_0004);
    int* node = CSF_AT(int*, candidate, 0x04);

    if (CSF_AT(uint8_t, node, 0x19) != 0)
        return candidate;

    const char* searchName = *name;
    do
    {
        if (TypeRegistry_CompareNames(
                CSF_AT(const char*, node, 0x0C), searchName) >= 0)
        {
            candidate = node;
            node = CSF_AT(int*, node, 0x00);
        }
        else
        {
            node = CSF_AT(int*, node, 0x08);
        }
    } while (CSF_AT(uint8_t, node, 0x19) == 0);

    return candidate;
}

HOOK(0x0054AFB0, TypeRegistry_FindNameLowerBound, "auto");
