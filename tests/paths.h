// Extracted retail data for the optional real-asset tests (never checked in): $CSF_PAKS, else
// $CSF_DATA_ROOT/csf-paks, else /mnt/e/dev/csf-paks. Same convention as tools/paths.py.
#pragma once
#include <cstdlib>
#include <string>

inline std::string paksPath(const char* rel) {
    if(const char* p=std::getenv("CSF_PAKS")) return std::string(p)+"/"+rel;
    const char* root=std::getenv("CSF_DATA_ROOT");
    return std::string(root?root:"/mnt/e/dev")+"/csf-paks/"+rel;
}
