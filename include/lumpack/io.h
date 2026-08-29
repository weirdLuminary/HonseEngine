#pragma once
#include "bitwriter.h"
#include "endian_conv.h"
#include "fstructure.h"
#include "hash.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#ifdef LUM_VERBOSE
#define LOG(...) printf(__VA_ARGS__)
#else
#define LOG(...) ((void)0)
#endif

// Return type for reading assets. Stores a temporary FILE* stream and the file
// extension that should be associated.
typedef struct LumAsset {
    FILE* data;
    char* path;
} LumAsset;

// Used to write LUM asset packs. Automatically reads asset files, compresses
// them, and writes the respective header and final data. If compressed file
// size is calculated to be bigger than the initial, compression is cancelled
// for the asset.
//
// Takes in the input asset pack path, the asset paths (files only), and the
// count of such.
//
static void lum_write_file(const char* outputFile, const char inputAssetPaths[][256],
                    size_t numAssetCount) {

    FILE* file = fopen(outputFile, "wb"); // Output file

    if (file == NULL) {
        fprintf(stderr, "Failed to create/open output file!\n");
    }

    LOG("PACKING STARTED\n");
    LOG("---------------\n");

    struct _LM_AssetInfo info[numAssetCount]; // Pre-allocated asset information

    LOG("Generating bytecodes...");

    uint32_t frequencies[UINT8_MAX + 1] = {
        0}; // Frequency of bytes in all of the assets

    // Calculate byte frequency
    for (int i = 0; i < numAssetCount; i++) {

        FILE* asset = fopen(inputAssetPaths[i], "rb");
        if (asset == NULL) {
            fprintf(stderr, "Failed to open asset file \'%s\'\n",
                    inputAssetPaths[i]);
        };

        int c;
        while ((c = getc(asset)) != EOF) {
            frequencies[(uint8_t)c]++;
        }

        fclose(asset);
    }

    // Build Huffman tree

    struct _LM_Heap* heap = _lm_create_heap();

    for (int i = 0; i < UINT8_MAX + 1; i++) {
        if (frequencies[i] <= 0)
            continue;
        _lm_insert_node(heap, _lm_create_node(i, frequencies[i]));
    }

    struct _LM_Code codes[256] = {0};
    _lm_generate_codes(_lm_huffman_merge(heap), codes, 0, 0);

    struct _LM_Header head = { {'L', 'U', 'M'}, LUM_VERSION, numAssetCount};
    memcpy(head.byteCodes, codes, sizeof codes);

    free(heap->array);
    free(heap);

    LOG(" Done!\n");
    LOG("Writing header metadata...");

    if (!_lm_write_header(file, &head)) {
        fprintf(stderr, "Failed to write LUM header!\n");
        fclose(file);
        return;
    }

    LOG(" Done!\n\n");
    LOG("Writing compressed asset data...\n");

    uint64_t metadataOffset = ftell(file);
    fseek(file, sizeof(struct _LM_AssetInfo) * numAssetCount, SEEK_CUR);

    uint64_t totalOriginalAssetSize = 0;
    uint64_t totalCompressedAssetSize = 0;

    struct _LM_BitWriter writer = {file, 0, 0};

    for (int i = 0; i < numAssetCount; i++) {

        FILE* asset = fopen(inputAssetPaths[i], "rb");
        if (asset == NULL) {
            fprintf(stderr, "Failed to open asset file \'%s\'\n",
                    inputAssetPaths[i]);
        };

        const char* extension = strrchr(inputAssetPaths[i], '.');

        if (extension == NULL) {
            extension = "\0";
        }

        if (extension != NULL)
            extension++; // Skip dot

        uint64_t offset = ftell(file);

        printf("Asset %d:\n", i);

        fseek(asset, 0, SEEK_END);
        uint64_t originalSize = ftell(asset);
        fseek(asset, 0, SEEK_SET);

        // Calculate theoretical compressed file size

        uint64_t compressedBits = 0;

        int c;
        while ((c = getc(asset)) != EOF) {
            compressedBits += codes[(uint8_t)c].length;
        }

        uint64_t compressedSize = (compressedBits + 7) / 8;

        fseek(asset, 0, SEEK_SET);
        totalOriginalAssetSize += originalSize;

        bool compressed =
            compressedSize <
            originalSize; // Whether the file should be compressed or not

        info[i] = (struct _LM_AssetInfo){
            lum_hash(inputAssetPaths[i]),
            offset,
            compressed,
            size : compressedSize,
            originalSize : originalSize
        }; // Write asset info
        strcpy(info[i].extension, extension);

        if (!compressed) { // Write initial file data

            LOG("Compressed file size bigger than original; writing initial "
                "data\n");

            int c;
            while ((c = getc(asset)) != EOF) {
                fputc(c, file);
            }

            LOG(" offset=%lu bytes | size=%lu bytes\n", offset, compressedSize,
                originalSize);

            totalCompressedAssetSize += originalSize;
        }
        else { // Compress

            int c;
            while ((c = getc(asset)) != EOF) {
                _lm_bit_write_code(&writer, codes[(uint8_t)c]);
            }
            _lm_flush_bits(&writer);

            LOG(" offset=%lu bytes | compressed=%lu bytes | original=%lu "
                "bytes\n",
                offset, compressedSize, originalSize);

            totalCompressedAssetSize += compressedSize;
        }
    }

    LOG("Done!\n");
    LOG("Writing asset metadata...");

    fseek(file, metadataOffset, SEEK_SET);
    for (size_t i = 0; i < numAssetCount; i++) {
        if (!_lm_write_asset_info(file, &info[i])) {
            fprintf(stderr, "Failed to write asset metadata!\n");
            fclose(file);
            return;
        }
    }

    LOG(" Done!\n");

    fseek(file, 0, SEEK_END);
    uint64_t finalSize = ftell(file);

    LOG("\nOriginal asset size: %ld\n", totalOriginalAssetSize);
    LOG("Compressed asset size: %ld\n", totalCompressedAssetSize);
    LOG("Compression efficiency: %f%\n\n",
        ((double)totalCompressedAssetSize / totalOriginalAssetSize) * 100.0);

    printf("Wrote %ld bytes to %s\n", finalSize, outputFile);

    fclose(file);
}

// Used to retrieve singular assets from the LUM asset packs.
//
// Takes in the input asset pack path, and the asset path hash string.
//
// Returns a LumAsset struct with the respective asset file stream and
// extension. If an error occurs, returns a { NULL, NULL } struct. Check
// validity via lum_is_valid().
//
static LumAsset lum_extract_asset(const char* inputAssetPack, uint64_t id) {

    FILE* file = fopen(inputAssetPack, "rb");

    if (file == NULL) {
        fprintf(stderr, "Failed to open input file!\n");
        return (LumAsset){NULL, NULL};
    }

    LOG("READING STARTED\n");
    LOG("---------------\n");

    LOG("Reading header...");

    struct _LM_Header head;

    if (!_lm_read_header(file, &head)) {
        fprintf(stderr, "Failed to read LUM header!\n");
        fclose(file);
        return (LumAsset){NULL, NULL};
    }

    if (memcmp(head.magic, "LUM", 3) != 0) {
        fprintf(stderr, "File format not recognized.\n");
        fclose(file);
        return (LumAsset){NULL, NULL};
    }

    if (head.version != LUM_VERSION) {
        fprintf(stderr, ".lum file version mismatch!\n");
        fclose(file);
        return (LumAsset){NULL, NULL};
    }

    LOG(" Done!\n");

    LOG("Reading assets...");

    struct _LM_AssetInfo* info =
        (struct _LM_AssetInfo*)malloc(sizeof(struct _LM_AssetInfo) * head.numAssets);

    for (size_t i = 0; i < head.numAssets; i++) {
        if (!_lm_read_asset_info(file, &info[i])) {
            fprintf(stderr, "Failed to read asset metadata!\n");
            free(info);
            fclose(file);
            return (LumAsset){NULL, NULL};
        }
    }

    printf(" Done!\n");

    size_t found = -1;
    char* ext;
    char* path = (char*)malloc(128);
    FILE* asset = NULL;

    for (int i = 0; i < head.numAssets; i++) {
        if (info[i].hashName != id)
            continue;

        found = i;

        ext = (char*)malloc(strlen(info[found].extension) + 1);
        strncpy(ext, info[found].extension, 11);
        ext[strlen(info[found].extension)] = '\0';

        if(!_lm_create_tmp_directory()) {
            fprintf(stderr, "Failed to create/find tmp directory.\n");
            return (LumAsset){NULL, NULL};
        }

        snprintf(path, 128, "tmp/%" PRIx64 ".%s", id, ext);

        asset = fopen(path, "wb+");
        if (asset == NULL) {
            fprintf(stderr, "Failed to create temp asset file.\n");
            return (LumAsset){NULL, NULL};
        }

        printf("Extracting asset: offset=%lu size=%lu original=%lu\n",
               info[i].offset, info[i].size, info[i].originalSize);

        fseek(file, info[i].offset, SEEK_SET);

        if (info[i].compressed) {

            struct _LM_BitWriter reader = {file, 0, 0};

            uint32_t code = 0;
            uint8_t length = 0;
            uint64_t readSize = 0;
            while (readSize < info[i].originalSize) { // Read until end

                int bit = _lm_read_bit(&reader);

                if (bit < 0)
                    break;

                code = (code << 1) | bit;
                length++;

                for (int byte = 0; byte < 256;
                     byte++) { // Go through header bytecodes
                    if (head.byteCodes[byte].length == length &&
                        head.byteCodes[byte].code == code) {

                        fputc(byte, asset);

                        readSize++;

                        code = 0;
                        length = 0;

                        break;
                    }
                }
            }
        }
        else {
            for (uint64_t j = 0; j < info[i].size; j++) {
                int c = getc(file);

                if (c == EOF)
                    break;

                fputc(c, asset);
            }
        }
        break;
    }

    if (found == -1) {
        fprintf(stderr, "Failed to find asset %lu.\n", id);
        free(info);
        return (LumAsset){NULL, NULL};
    }

    free(info);
    fclose(file);

    printf("\nRead asset %lu!\n", id);

    rewind(asset);

    return (LumAsset){asset, path};
}

// LumAsset

static bool lum_is_valid(LumAsset* asset) {
    return asset->data != NULL && asset->path != NULL;
}

static void lum_free(LumAsset* asset)
{
    if (!asset)
        return;

    if (asset->data)
        fclose(asset->data);

    if (asset->path) {
        remove(asset->path);
        free(asset->path);
    }

    asset->data = NULL;
    asset->path = NULL;
}
