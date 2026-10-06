#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "legacy/string.h"

// Returns the literal token "TMirar" used by the "look" command parser.
static const char* GetMirarToken()
{
  return CSF_STR(0x00835794, "TMirar");
}

HOOK(0x004AA0C0, GetMirarToken, "auto");
