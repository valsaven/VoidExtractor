/**
 * VoidExtractor - universal game archive unpacker
 *
 * Copyright (C) 2025  valsaven
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "extractor.h"

/**
 * Determines the archive type based on the first bytes of the file.
 * Returns a pointer to a static Extractor structure or NULL, if the format is not supported.
 */
const Extractor *detect_extractor(const char *path);

/* Helper function - prints a list of all supported formats */
void detect_print_supported(void);
