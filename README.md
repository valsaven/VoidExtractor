# VoidExtractor

**Universal game archive extractor.**

Currently supports Pathologic (2005) and Pathologic Classic HD (2015) game archives (.vfs).

More games and archive formats are planned for the future.

![VoidExtractor Cover](VoidExtractorCover.png)

## Features

- Supports **both** Pathologic (2005) and Pathologic Classic HD (2015) VFS - extracts all archives except Textures.vfs (WIP, landing very soon)
- Preserves original folder hierarchy
- Zero dependencies - single static executable (~44 KB)
- Fast - written in pure C (C23)

## Why VoidExtractor?

- No Python, no runtime, no bloat
- Works on any Windows from XP to 11
- Perfect for modding, translation projects, asset extraction
- Actively developed and open source (GPLv3)

## Quick start (recommended)

Download the latest binary from **[Releases](https://github.com/valsaven/VoidExtractor/releases/latest)** and run:

```powershell
# Windows PowerShell / CMD
.\VoidExtractor.exe .\data\Actors.vfs

# Or specify output folder
.\VoidExtractor.exe .\data\Sounds.vfs extracted_sounds
```

## Usage

```text
VoidExtractor v1.0 - universal game archive extractor

Usage: VoidExtractor.exe <archive> [output_directory]

If output_directory is not specified, a folder named after the archive without extension will be created in the current directory.

Supported formats:
  - VFS v1 [Pathologic Classic HD (2015)]
  - VFS v2 [The Void (2016)] - coming soon

Examples:
  VoidExtractor.exe .\data\Actors.vfs
  VoidExtractor.exe Sounds.vfs extracted_sounds
  VoidExtractor.exe --help
```

## Build from source (Windows)

### Prerequisites

- GCC ≥ 13
- Meson ≥ 1.9.1
- Ninja ≥ 1.13.2

### Build instructions

```sh
# Clone and enter the repo
git clone https://github.com/valsaven/VoidExtractor.git
cd VoidExtractor

# Configure and build
meson setup build
meson compile -C build

# The binary will be at build/VoidExtractor.exe
```

### Optional: release build (smaller & faster)

```sh
meson setup -Dbuildtype=release build-release
meson compile -C build-release
```

### Run

```sh
build\VoidExtractor.exe  # Windows PowerShell
```

## Roadmap

- [x] VFS v1 support (Pathologic Classic HD, 2015)
- [ ] VFS v2 support (The Void, 2016)
- [ ] Linux & macOS support (maybe?)
- [ ] GUI version (maybe?)

## License

[GNU GPL v3](LICENSE)
