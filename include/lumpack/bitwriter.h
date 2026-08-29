#pragma once
#include "compression.h"
#include <stdint.h>
#include <stdio.h>

struct _LM_BitWriter {
    FILE* file;
    uint8_t buffer;
    uint8_t bitCount;
};

static void _lm_write_bit(struct _LM_BitWriter* w, uint8_t bit) {
    w->buffer = (w->buffer << 1) | bit;
    w->bitCount++;

    if (w->bitCount == 8) {
        fputc(w->buffer, w->file);
        w->buffer = 0;
        w->bitCount = 0;
    }
}

static void _lm_bit_write_code(struct _LM_BitWriter* w, struct _LM_Code code) {
    for (int i = code.length - 1; i >= 0; i--) {
        uint8_t bit = (code.code >> i) & 1;
        _lm_write_bit(w, bit);
    }
}

static int _lm_read_bit(struct _LM_BitWriter* r) {
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

static void _lm_flush_bits(struct _LM_BitWriter* w) {
    if (w->bitCount == 0)
        return;

    w->buffer <<= (8 - w->bitCount);

    fputc(w->buffer, w->file);

    w->buffer = 0;
    w->bitCount = 0;
}