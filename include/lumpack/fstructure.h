#pragma once
#include "compression.h"
#include <stdbool.h>
#include <errno.h>

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif

const uint8_t LUM_VERSION = 1;

struct _LM_Header {
    char magic[3];
    uint8_t version;
    uint16_t numAssets;
    struct _LM_Code byteCodes[256];
};

struct _LM_AssetInfo {
    uint64_t hashName;
    uint64_t offset;
    bool compressed;
    char extension[12];
    uint64_t size;
    uint64_t originalSize;
};

static bool _lm_create_tmp_directory(void)
{
#ifdef _WIN32
    return _mkdir("tmp") == 0 || errno == EEXIST;
#else
    return mkdir("tmp", 0755) == 0 || errno == EEXIST;
#endif
}