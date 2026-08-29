#pragma once
#include "compression.h"
#include "fstructure.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// This header has functions for converting everything to little-endian
// representations.

static bool _lm_write_u16(FILE* file, uint16_t value) {
    uint8_t bytes[2] = {(uint8_t)(value), (uint8_t)(value >> 8)};

    return fwrite(bytes, 1, sizeof(bytes), file) == sizeof(bytes);
}

static bool _lm_write_u32(FILE* file, uint32_t value) {
    uint8_t bytes[4] = {(uint8_t)(value), (uint8_t)(value >> 8),
                        (uint8_t)(value >> 16), (uint8_t)(value >> 24)};

    return fwrite(bytes, 1, sizeof(bytes), file) == sizeof(bytes);
}

static bool _lm_write_u64(FILE* file, uint64_t value) {
    uint8_t bytes[8] = {(uint8_t)(value),       (uint8_t)(value >> 8),
                        (uint8_t)(value >> 16), (uint8_t)(value >> 24),
                        (uint8_t)(value >> 32), (uint8_t)(value >> 40),
                        (uint8_t)(value >> 48), (uint8_t)(value >> 56)};

    return fwrite(bytes, 1, sizeof(bytes), file) == sizeof(bytes);
}

static bool _lm_read_u16(FILE* file, uint16_t* value) {
    uint8_t bytes[2];

    if (fread(bytes, 1, sizeof(bytes), file) != sizeof(bytes))
        return false;

    *value = ((uint16_t)bytes[0]) | ((uint16_t)bytes[1] << 8);

    return true;
}

static bool _lm_read_u32(FILE* file, uint32_t* value) {
    uint8_t bytes[4];

    if (fread(bytes, 1, sizeof(bytes), file) != sizeof(bytes))
        return false;

    *value = ((uint32_t)bytes[0]) | ((uint32_t)bytes[1] << 8) |
             ((uint32_t)bytes[2] << 16) | ((uint32_t)bytes[3] << 24);

    return true;
}

static bool _lm_read_u64(FILE* file, uint64_t* value) {
    uint8_t bytes[8];

    if (fread(bytes, 1, sizeof(bytes), file) != sizeof(bytes))
        return false;

    *value = ((uint64_t)bytes[0]) | ((uint64_t)bytes[1] << 8) |
             ((uint64_t)bytes[2] << 16) | ((uint64_t)bytes[3] << 24) |
             ((uint64_t)bytes[4] << 32) | ((uint64_t)bytes[5] << 40) |
             ((uint64_t)bytes[6] << 48) | ((uint64_t)bytes[7] << 56);

    return true;
}

static bool _lm_write_code(FILE* file, const struct _LM_Code* code) {
    return _lm_write_u32(file, code->code) && fputc(code->length, file) != EOF;
}

static bool _lm_read_code(FILE* file, struct _LM_Code* code) {
    int length;

    if (!_lm_read_u32(file, &code->code))
        return false;

    length = fgetc(file);

    if (length == EOF)
        return false;

    code->length = (uint8_t)length;

    return true;
}

static bool _lm_write_header(FILE* file, const struct _LM_Header* header) {
    if (fwrite(header->magic, 1, 3, file) != 3)
        return false;

    if (fputc(header->version, file) == EOF)
        return false;

    if (!_lm_write_u16(file, header->numAssets))
        return false;

    for (size_t i = 0; i < 256; i++) {
        if (!_lm_write_code(file, &header->byteCodes[i]))
            return false;
    }

    return true;
}

static bool _lm_read_header(FILE* file, struct _LM_Header* header) {
    if (fread(header->magic, 1, 3, file) != 3)
        return false;

    int version = fgetc(file);

    if (version == EOF)
        return false;

    header->version = (uint8_t)version;

    if (!_lm_read_u16(file, &header->numAssets))
        return false;

    for (size_t i = 0; i < 256; i++) {
        if (!_lm_read_code(file, &header->byteCodes[i]))
            return false;
    }

    return true;
}

static bool _lm_write_asset_info(FILE* file, const struct _LM_AssetInfo* info) {
    if (!_lm_write_u64(file, info->hashName))
        return false;

    if (!_lm_write_u64(file, info->offset))
        return false;

    if (fputc(info->compressed ? 1 : 0, file) == EOF)
        return false;

    if (fwrite(info->extension, 1, sizeof(info->extension), file) !=
        sizeof(info->extension))
        return false;

    if (!_lm_write_u64(file, info->size))
        return false;

    if (!_lm_write_u64(file, info->originalSize))
        return false;

    return true;
}

static bool _lm_read_asset_info(FILE* file, struct _LM_AssetInfo* info) {
    if (!_lm_read_u64(file, &info->hashName))
        return false;

    if (!_lm_read_u64(file, &info->offset))
        return false;

    int compressed = fgetc(file);

    if (compressed == EOF)
        return false;

    info->compressed = compressed != 0;

    if (fread(info->extension, 1, sizeof(info->extension), file) !=
        sizeof(info->extension))
        return false;

    if (!_lm_read_u64(file, &info->size))
        return false;

    if (!_lm_read_u64(file, &info->originalSize))
        return false;

    return true;
}