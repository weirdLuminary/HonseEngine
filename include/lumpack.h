#pragma once
#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define LUMPACK_LIB_VERSION 3 // Version for the library
#define LUM_VERSION 1 // Version of the packing format; used internally

#ifdef LUM_VERBOSE
    #define LOG(...) printf(__VA_ARGS__)
#else
    #define LOG(...) ((void)0)
#endif

#ifdef __cplusplus
extern "C" {
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
void lum_write_file(const char* outputFile, const char inputAssetPaths[][256], uint16_t numAssetCount);

// Used to retrieve singular assets from the LUM asset packs.
//
// Takes in the input asset pack path, and the asset path hash string.
//
// Returns a LumAsset struct with the respective asset file stream and
// extension. If an error occurs, returns a { NULL, NULL } struct. Check
// validity via lum_is_valid().
//
LumAsset lum_extract_asset(const char* inputAssetPack, uint64_t id);

// Deduces whether a LumAsset is valid for further use.
bool lum_is_valid(LumAsset* asset);

// Automatically frees the memory of a LumAsset.
void lum_free(LumAsset* asset);

// Hashes a string with FNV1A for use with lumpack resource hash IDs.
uint64_t lum_hash(const char* str);

#ifdef __cplusplus
}
#endif

#ifdef LUMPACK_IMPLEMENTATION

////////////////// Structure //////////////////

struct _lum_code {
        uint32_t code;
        uint8_t length;
};

struct _lum_header {
        char magic[3];
        uint8_t version;
        uint16_t numAssets;
        struct _lum_code byteCodes[256];
};

struct _lum_AssetInfo {
        uint64_t hashName;
        uint64_t offset;
        bool compressed;
        char extension[12];
        uint64_t size;
        uint64_t originalSize;
};

////////////////// Endian conversion //////////////////

bool _lm_write_u16(FILE* file, uint16_t value) {
    uint8_t bytes[2] = { (uint8_t)(value), (uint8_t)(value >> 8) };

    return fwrite(bytes, 1, sizeof(bytes), file) == sizeof(bytes);
}

bool _lm_write_u32(FILE* file, uint32_t value) {
    uint8_t bytes[4] = {
        (uint8_t)(value), (uint8_t)(value >> 8), (uint8_t)(value >> 16), (uint8_t)(value >> 24)
    };

    return fwrite(bytes, 1, sizeof(bytes), file) == sizeof(bytes);
}

bool _lm_write_u64(FILE* file, uint64_t value) {
    uint8_t bytes[8] = { (uint8_t)(value),
        (uint8_t)(value >> 8),
        (uint8_t)(value >> 16),
        (uint8_t)(value >> 24),
        (uint8_t)(value >> 32),
        (uint8_t)(value >> 40),
        (uint8_t)(value >> 48),
        (uint8_t)(value >> 56) };

    return fwrite(bytes, 1, sizeof(bytes), file) == sizeof(bytes);
}

bool _lm_read_u16(FILE* file, uint16_t* value) {
    uint8_t bytes[2];

    if (fread(bytes, 1, sizeof(bytes), file) != sizeof(bytes))
        return false;

    *value = ((uint16_t)bytes[0]) | ((uint16_t)bytes[1] << 8);

    return true;
}

bool _lm_read_u32(FILE* file, uint32_t* value) {
    uint8_t bytes[4];

    if (fread(bytes, 1, sizeof(bytes), file) != sizeof(bytes))
        return false;

    *value = ((uint32_t)bytes[0]) | ((uint32_t)bytes[1] << 8) | ((uint32_t)bytes[2] << 16) |
             ((uint32_t)bytes[3] << 24);

    return true;
}

bool _lm_read_u64(FILE* file, uint64_t* value) {
    uint8_t bytes[8];

    if (fread(bytes, 1, sizeof(bytes), file) != sizeof(bytes))
        return false;

    *value = ((uint64_t)bytes[0]) | ((uint64_t)bytes[1] << 8) | ((uint64_t)bytes[2] << 16) |
             ((uint64_t)bytes[3] << 24) | ((uint64_t)bytes[4] << 32) | ((uint64_t)bytes[5] << 40) |
             ((uint64_t)bytes[6] << 48) | ((uint64_t)bytes[7] << 56);

    return true;
}

bool _lm_write_code(FILE* file, const struct _lum_code* code) {
    return _lm_write_u32(file, code->code) && fputc(code->length, file) != EOF;
}

bool _lm_read_code(FILE* file, struct _lum_code* code) {
    int length;

    if (!_lm_read_u32(file, &code->code))
        return false;

    length = fgetc(file);

    if (length == EOF)
        return false;

    code->length = (uint8_t)length;

    return true;
}

bool _lm_write_header(FILE* file, const struct _lum_header* header) {
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

bool _lm_read_header(FILE* file, struct _lum_header* header) {
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

bool _lm_write_asset_info(FILE* file, const struct _lum_AssetInfo* info) {
    if (!_lm_write_u64(file, info->hashName))
        return false;

    if (!_lm_write_u64(file, info->offset))
        return false;

    if (fputc(info->compressed ? 1 : 0, file) == EOF)
        return false;

    if (fwrite(info->extension, 1, sizeof(info->extension), file) != sizeof(info->extension))
        return false;

    if (!_lm_write_u64(file, info->size))
        return false;

    if (!_lm_write_u64(file, info->originalSize))
        return false;

    return true;
}

bool _lm_read_asset_info(FILE* file, struct _lum_AssetInfo* info) {
    if (!_lm_read_u64(file, &info->hashName))
        return false;

    if (!_lm_read_u64(file, &info->offset))
        return false;

    int compressed = fgetc(file);

    if (compressed == EOF)
        return false;

    info->compressed = compressed != 0;

    if (fread(info->extension, 1, sizeof(info->extension), file) != sizeof(info->extension))
        return false;

    if (!_lm_read_u64(file, &info->size))
        return false;

    if (!_lm_read_u64(file, &info->originalSize))
        return false;

    return true;
}

////////////////// Directories //////////////////

#ifdef _WIN32
    #include <direct.h>
#else
    #include <sys/stat.h>
#endif

bool _lm_create_tmp_directory(void) {
    #ifdef _WIN32
    return _mkdir("tmp") == 0 || errno == EEXIST;
    #else
    return mkdir("tmp", 0755) == 0 || errno == EEXIST;
    #endif
}

////////////////// Bitwriter //////////////////

struct _lum_bitwriter {
        FILE* file;
        uint8_t buffer;
        uint8_t bitCount;
};

void _lm_write_bit(struct _lum_bitwriter* w, uint8_t bit) {
    w->buffer = (w->buffer << 1) | bit;
    w->bitCount++;

    if (w->bitCount == 8) {
        fputc(w->buffer, w->file);
        w->buffer = 0;
        w->bitCount = 0;
    }
}

void _lm_bit_write_code(struct _lum_bitwriter* w, struct _lum_code code) {
    for (int i = code.length - 1; i >= 0; i--) {
        uint8_t bit = (code.code >> i) & 1;
        _lm_write_bit(w, bit);
    }
}

int _lm_read_bit(struct _lum_bitwriter* r) {
    if (r->bitCount == 0) {
        int c = fgetc(r->file);

        if (c == EOF)
            return -1;

        r->buffer = (uint8_t)c;
        r->bitCount = 8;
    }

    int bit = (r->buffer >> 7) & 1;

    r->buffer <<= 1;
    r->bitCount--;

    return bit;
}

void _lm_flush_bits(struct _lum_bitwriter* w) {
    if (w->bitCount == 0)
        return;

    w->buffer <<= (8 - w->bitCount);

    fputc(w->buffer, w->file);

    w->buffer = 0;
    w->bitCount = 0;
}

////////////////// Huffman tree //////////////////

struct _lum_huffman_node {

        uint8_t byte;
        uint32_t frequency;

        struct _lum_huffman_node* left;
        struct _lum_huffman_node* right;
};

struct _lum_heap {

        struct _lum_huffman_node** array;
        size_t size;
};

struct _lum_heap* _lm_create_heap() {
    struct _lum_heap* heap = (struct _lum_heap*)malloc(sizeof(struct _lum_heap));
    heap->size = 0;
    heap->array = (struct _lum_huffman_node**)malloc(sizeof(struct _lum_huffman_node*) * (UINT8_MAX + 1));
    return heap;
}

struct _lum_huffman_node* _lm_create_node(uint8_t byte, uint32_t frequency) {
    struct _lum_huffman_node* node = (struct _lum_huffman_node*)malloc(sizeof(struct _lum_huffman_node));
    node->byte = byte;
    node->frequency = frequency;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void _lm_swap_node(struct _lum_heap* heap, size_t a, size_t b) {
    struct _lum_huffman_node* temp = heap->array[a];
    heap->array[a] = heap->array[b];
    heap->array[b] = temp;
}

void _lm_insert_node(struct _lum_heap* heap, struct _lum_huffman_node* value) {
    struct _lum_huffman_node** array = heap->array;

    int i = heap->size;
    array[i] = value;
    heap->size++;

    while (i > 0) {
        int parent = (i - 1) / 2;

        if (array[parent]->frequency <= array[i]->frequency)
            break;

        _lm_swap_node(heap, parent, i);

        i = parent;
    }
}

struct _lum_huffman_node* _lm_extract_min(struct _lum_heap* heap) {
    struct _lum_huffman_node** array = heap->array;
    size_t* size = &heap->size;

    struct _lum_huffman_node* result = array[0];

    heap->size--;

    if (*size == 0)
        return result;

    array[0] = array[heap->size];

    int i = 0;

    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;

        if (left < *size && array[left]->frequency < array[smallest]->frequency)
            smallest = left;

        if (right < *size && array[right]->frequency < array[smallest]->frequency)
            smallest = right;

        if (smallest == i)
            break;

        _lm_swap_node(heap, i, smallest);

        i = smallest;
    }

    return result;
}

struct _lum_huffman_node* _lm_huffman_merge(struct _lum_heap* heap) {
    while (heap->size > 1) {
        struct _lum_huffman_node* left = _lm_extract_min(heap);
        struct _lum_huffman_node* right = _lm_extract_min(heap);

        struct _lum_huffman_node* parent =
            (struct _lum_huffman_node*)malloc(sizeof(struct _lum_huffman_node));

        parent->byte = 0;
        parent->frequency = left->frequency + right->frequency;
        parent->left = left;
        parent->right = right;

        _lm_insert_node(heap, parent);
    }

    return _lm_extract_min(heap); // Root
}

void _lm_generate_codes(
    struct _lum_huffman_node* root, struct _lum_code codes[256], uint64_t code, uint8_t depth) {
    if (root == NULL)
        return;

    if (root->left == NULL && root->right == NULL) {
        codes[root->byte].code = code;
        codes[root->byte].length = depth;
        return;
    }

    _lm_generate_codes(root->left, codes, code << 1, depth + 1);

    _lm_generate_codes(root->right, codes, (code << 1) | 1, depth + 1);
}

////////////////// Public implementations //////////////////

uint64_t lum_hash(const char* str) {
    uint64_t hash = 0xcbf29ce484222325ULL;
    size_t length = strlen(str);
    for (int i = 0; i < length; i++) {
        char c = str[i];
        hash ^= (uint64_t)c;
        hash *= 0x00000100000001B3ULL;
    }
    return hash;
}

void lum_write_file(const char* outputFile, const char inputAssetPaths[][256], uint16_t numAssetCount) {

    FILE* file = fopen(outputFile, "wb"); // Output file

    if (file == NULL) {
        fprintf(stderr, "Failed to create/open output file!\n");
    }

    LOG("PACKING STARTED\n");
    LOG("---------------\n");

    struct _lum_AssetInfo info[numAssetCount]; // Pre-allocated asset information

    LOG("Generating bytecodes...");

    uint32_t frequencies[UINT8_MAX + 1] = { 0 }; // Frequency of bytes in all of the assets

    // Calculate byte frequency
    for (int i = 0; i < numAssetCount; i++) {

        FILE* asset = fopen(inputAssetPaths[i], "rb");
        if (asset == NULL) {
            fprintf(stderr, "Failed to open asset file \'%s\'\n", inputAssetPaths[i]);
        };

        int c;
        while ((c = getc(asset)) != EOF) {
            frequencies[(uint8_t)c]++;
        }

        fclose(asset);
    }

    // Build Huffman tree

    struct _lum_heap* heap = _lm_create_heap();

    for (int i = 0; i < UINT8_MAX + 1; i++) {
        if (frequencies[i] <= 0)
            continue;
        _lm_insert_node(heap, _lm_create_node(i, frequencies[i]));
    }

    struct _lum_code codes[256] = { 0 };
    _lm_generate_codes(_lm_huffman_merge(heap), codes, 0, 0);

    struct _lum_header head = { { 'L', 'U', 'M' }, LUM_VERSION, numAssetCount };
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
    fseek(file, sizeof(struct _lum_AssetInfo) * numAssetCount, SEEK_CUR);

    uint64_t totalOriginalAssetSize = 0;
    uint64_t totalCompressedAssetSize = 0;

    struct _lum_bitwriter writer = { file, 0, 0 };

    for (int i = 0; i < numAssetCount; i++) {

        FILE* asset = fopen(inputAssetPaths[i], "rb");
        if (asset == NULL) {
            fprintf(stderr, "Failed to open asset file \'%s\'\n", inputAssetPaths[i]);
        };

        const char* extension = strrchr(inputAssetPaths[i], '.');

        if (extension == NULL) {
            extension = "\0";
        }
        else
            extension++; // Skip dot

        uint64_t offset = ftell(file);

        LOG("Asset %d:\n", i);

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

        bool compressed = compressedSize < originalSize; // Whether the file should be compressed or not

        info[i] = (struct _lum_AssetInfo) {
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

            LOG(" offset=%lu bytes | size=%lu bytes\n", offset, compressedSize, originalSize);

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
                offset,
                compressedSize,
                originalSize);

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
    
    double efficiency =
        (1.0 - ((double)totalCompressedAssetSize /
                (double)totalOriginalAssetSize)) * 100.0;

    LOG("Compression efficiency: %.2f%%\n\n", efficiency);

    printf("Wrote %ld bytes to %s\n", finalSize, outputFile);

    fclose(file);
}

LumAsset lum_extract_asset(const char* inputAssetPack, uint64_t id) {

    FILE* file = fopen(inputAssetPack, "rb");

    if (file == NULL) {
        fprintf(stderr, "Failed to open input file!\n");
        return (LumAsset) { NULL, NULL };
    }

    LOG("READING STARTED\n");
    LOG("---------------\n");

    LOG("Reading header...");

    struct _lum_header head;

    if (!_lm_read_header(file, &head)) {
        fprintf(stderr, "Failed to read LUM header!\n");
        fclose(file);
        return (LumAsset) { NULL, NULL };
    }

    if (memcmp(head.magic, "LUM", 3) != 0) {
        fprintf(stderr, "File format not recognized.\n");
        fclose(file);
        return (LumAsset) { NULL, NULL };
    }

    if (head.version != LUM_VERSION) {
        fprintf(stderr, ".lum file version mismatch!\n");
        fclose(file);
        return (LumAsset) { NULL, NULL };
    }

    LOG(" Done!\n");

    LOG("Reading assets...");

    struct _lum_AssetInfo* info =
        (struct _lum_AssetInfo*)malloc(sizeof(struct _lum_AssetInfo) * head.numAssets);

    for (size_t i = 0; i < head.numAssets; i++) {
        if (!_lm_read_asset_info(file, &info[i])) {
            fprintf(stderr, "Failed to read asset metadata!\n");
            free(info);
            fclose(file);
            return (LumAsset) { NULL, NULL };
        }
    }

    LOG(" Done!\n");

    size_t found = -1;
    char* ext;
    char* path = (char*)malloc(128);
    FILE* asset = NULL;

    for (int i = 0; i < head.numAssets; i++) {
        if (info[i].hashName != id)
            continue;

        found = i;

        const char* src = info[found].extension;
        size_t len = strlen(src) + 1;

        ext = (char*)malloc(len);
        if (ext != NULL) {
            memcpy(ext, src, len);
        }

        if (!_lm_create_tmp_directory()) {
            fprintf(stderr, "Failed to create/find tmp directory.\n");
            return (LumAsset) { NULL, NULL };
        }

        snprintf(path, 128, "tmp/%" PRIx64 ".%s", id, ext);

        free(ext);

        asset = fopen(path, "wb+");
        if (asset == NULL) {
            fprintf(stderr, "Failed to create temp asset file.\n");
            return (LumAsset) { NULL, NULL };
        }

        printf("Extracting asset: offset=%lu size=%lu original=%lu\n",
            info[i].offset,
            info[i].size,
            info[i].originalSize);

        fseek(file, info[i].offset, SEEK_SET);

        if (info[i].compressed) {

            struct _lum_bitwriter reader = { file, 0, 0 };

            uint32_t code = 0;
            uint8_t length = 0;
            uint64_t readSize = 0;
            while (readSize < info[i].originalSize) { // Read until end

                int bit = _lm_read_bit(&reader);

                if (bit < 0)
                    break;

                code = (code << 1) | bit;
                length++;

                for (int byte = 0; byte < 256; byte++) { // Go through header bytecodes
                    if (head.byteCodes[byte].length == length && head.byteCodes[byte].code == code) {

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
        return (LumAsset) { NULL, NULL };
    }

    free(info);
    fclose(file);

    printf("\nRead asset %lu!\n", id);

    rewind(asset);

    return (LumAsset) { asset, path };
}

bool lum_is_valid(LumAsset* asset) { return asset->data != NULL && asset->path != NULL; }

void lum_free(LumAsset* asset) {
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

#endif