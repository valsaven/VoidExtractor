/**
 * VoidExtractor - universal game archive unpacker
 *
 * Copyright (C) 2025  valsaven
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include <stddef.h>
#include <stdint.h>

typedef enum {
    EXTRACTOR_SUCCESS = 0,
    EXTRACTOR_ERROR_OPEN,
    EXTRACTOR_ERROR_UNSUPPORTED,
    EXTRACTOR_ERROR_CORRUPTED,
    EXTRACTOR_ERROR_IO,
    EXTRACTOR_ERROR_NOMEM,
} ExtractorResult;

/* Pointer to extracting function - a single interface for all formats */
typedef ExtractorResult (*extract_func_t)(
    const char *input_path,
    const char *output_dir
);

/* Description of supported format */
typedef struct Extractor {
    const char     *name;       // Name for logs, for example "VFS v1", "VFS v2", etc.
    const uint8_t  *magic;      // Pointer to signature bytessignature string (may be NULL)
    size_t          magic_len;  // Signature length
    extract_func_t  extract;    // Extracting function
} Extractor;
