/**
 * VoidExtractor - universal game archive unpacker
 *
 * Copyright (C) 2025  valsaven
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "detect.h"
#include "vfs/vfs_v1.h"
// #include "vfs/vfs_v2.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const Extractor extractors[] = {
    {
        .name      = "VFS v1 [Pathologic Classic HD (2015)]",
        .magic     = (const uint8_t*)"LP1C\0\0\0\0",
        .magic_len = 8,
        .extract   = vfs_v1_extract,
    },
    { .name = NULL }
};

const Extractor *detect_extractor(const char *path) {
    FILE *f = fopen(path, "rb");
    if (!f) {
        perror("detect_extractor: fopen");
        return NULL;
    }

    uint8_t header[16] = {0};
    size_t read_bytes = fread(header, 1, sizeof(header), f);
    fclose(f);

    if (read_bytes < 4) {
        return NULL; // the file is too small
    }

    for (const Extractor *e = extractors; e->name != NULL; ++e) {
        if (e->magic_len <= read_bytes && memcmp(header, e->magic, e->magic_len) == 0) {
            return e;
        }
    }

    return NULL; // not found
}

void detect_print_supported(void) {
    printf("Supported formats:\n");
    for (const Extractor *e = extractors; e->name; ++e) {
        printf("  - %s\n", e->name);
    }
}
