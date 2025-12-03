/**
 * VoidExtractor - universal game archive unpacker
 *
 * Copyright (C) 2025  valsaven
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "detect.h"

static void print_usage(const char *prog) {
    printf("VoidExtractor v1.0 - universal game archive extractor\n\n");
    printf("Usage: %s <archive> [output_folder]\n\n", prog);

    printf("If output_directory is not specified, a folder named after the archive without extension will be created in the current directory.\n\n");

    detect_print_supported();

    printf("\nExamples:\n");
    printf("  %s .\\tests\\data\\Actors.vfs\n", prog);
    printf("  %s Sounds.vfs extracted_sounds\n", prog);
    printf("  %s --help\n", prog);
}

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 3 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        print_usage(argv[0]);

        return (argc < 2 || argc > 3) ? 1 : 0;
    }

    const char *input = argv[1];
    const char *output = (argc == 3) ? argv[2] : NULL;

    const Extractor *ext = detect_extractor(input);
    if (!ext) {
        fprintf(stderr, "Error: Unknown or unsupported archive format\n");
        detect_print_supported();
        return EXTRACTOR_ERROR_UNSUPPORTED;
    }

    printf("Format detected: %s\n", ext->name);

    // Forming a default output folder
    char default_out[512];
    if (!output) {
        const char *base = strrchr(input, '\\');

        if (!base) {
            base = input;
        } else {
            ++base;
        }

        snprintf(
            default_out,
            sizeof(default_out),
            "%.*s",
            (int)(strcspn(base, ".")),
            base
        );

        output = default_out;
    }

    ExtractorResult res = ext->extract(input, output);

    if (res == EXTRACTOR_SUCCESS) {
        printf("Unpacking complete: %s\n", output);
        return 0;
    } else {
        fprintf(stderr, "Unpacking error: code %d\n", res);
        return 1;
    }
}
