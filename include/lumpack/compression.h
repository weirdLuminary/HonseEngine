#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct _LM_Node {

    uint8_t byte;
    uint32_t frequency;

    struct _LM_Node* left;
    struct _LM_Node* right;
};
struct _LM_Heap {

    struct _LM_Node** array;
    size_t size;
};
struct _LM_Code {
    uint32_t code;
    uint8_t length;
};

static struct _LM_Heap* _lm_create_heap() {
    struct _LM_Heap* heap = (struct _LM_Heap*)malloc(sizeof(struct _LM_Heap));
    heap->size = 0;
    heap->array =
        (struct _LM_Node**)malloc(sizeof(struct _LM_Node*) * (UINT8_MAX + 1));
    return heap;
}

static struct _LM_Node* _lm_create_node(uint8_t byte, uint32_t frequency) {
    struct _LM_Node* node = (struct _LM_Node*)malloc(sizeof(struct _LM_Node));
    node->byte = byte;
    node->frequency = frequency;
    node->left = NULL;
    node->right = NULL;
    return node;
}

static void _lm_swap_node(struct _LM_Heap* heap, size_t a, size_t b) {
    struct _LM_Node* temp = heap->array[a];
    heap->array[a] = heap->array[b];
    heap->array[b] = temp;
}

static void _lm_insert_node(struct _LM_Heap* heap, struct _LM_Node* value) {
    struct _LM_Node** array = heap->array;

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

static struct _LM_Node* _lm_extract_min(struct _LM_Heap* heap) {
    struct _LM_Node** array = heap->array;
    size_t* size = &heap->size;

    struct _LM_Node* result = array[0];

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

        if (right < *size &&
            array[right]->frequency < array[smallest]->frequency)
            smallest = right;

        if (smallest == i)
            break;

        _lm_swap_node(heap, i, smallest);

        i = smallest;
    }

    return result;
}

static struct _LM_Node* _lm_huffman_merge(struct _LM_Heap* heap) {
    while (heap->size > 1) {
        struct _LM_Node* left = _lm_extract_min(heap);
        struct _LM_Node* right = _lm_extract_min(heap);

        struct _LM_Node* parent =
            (struct _LM_Node*)malloc(sizeof(struct _LM_Node));

        parent->byte = 0;
        parent->frequency = left->frequency + right->frequency;
        parent->left = left;
        parent->right = right;

        _lm_insert_node(heap, parent);
    }

    return _lm_extract_min(heap); // Root
}

/////////////////////////

static void _lm_generate_codes(struct _LM_Node* root, struct _LM_Code codes[256],
                           uint64_t code, uint8_t depth) {
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