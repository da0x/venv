# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

VENV is a C++ CLI tool for file stashing and configuration management. It creates local file repositories ("venvs") and swaps entire working copies between them — unlike git stash, it doesn't diff file contents, avoiding merge conflicts in binary/XML files.

Author: Daher Alfawares. License: GPL v3.

## Build Commands

```bash
make              # Build the 'venv' binary (g++ with C++20)
make clean        # Remove .obj/ build artifacts
make install      # Copy binary to /usr/local/bin/venv
make uninstall    # Remove from /usr/local/bin/venv
```

**Dependencies:** None beyond the C++ standard library. Only requires g++ with C++20 support.

No test suite exists.

## Architecture

### Namespaces

- `v::` — Domain classes (repository, venv, item)
- `x::` — Utilities (options, shell, table, process)
- `uuid::` — UUID v4 generation

### Core Classes

- **`repository`** (`repository.hpp/cpp`) — Central manager. Holds a `std::map<string, v::venv>` of all venvs and tracks the active one. State auto-loads in the constructor and auto-saves in the destructor via line-based text serialization to `.venv/venv`.
- **`venv`** (`venv.hpp/cpp`) — A named collection of file items with a UUID identifier.
- **`item`** (`venv_item.hpp/cpp`) — A tracked file, storing the external filename and an internal UUID. Files are stored at `.venv/<venv_name>/<item_uuid>`.
- **`main.cpp`** — Entry point. Maps CLI flags to operations using `x::options::map_to()`. Uses function-try-block (`int main(...) try { ... } catch(...) {}`) for error handling.

### Storage Layout

```
.venv/                    # Root (defined in static.hpp as ".venv")
  venv                    # Serialized repository state (line-based text format)
  <venv_name>/            # Per-venv directory
    <item_uuid>           # Stored file copy (identified by UUID, not filename)
```

### Utilities

- **`x::shell`** — Wraps shell commands (cp, rm, mv, diff, mkdir) via `::system()`. All paths are quoted for safety.
- **`x::options`** — Declarative CLI argument parser with help generation.
- **`x::table`** — ASCII table formatter for status display.
- **`uuid::v4`** (`x_uuid.hpp`) — UUID v4 generation using `<random>`, no external dependencies.

### Key Operations

- **push:** copies working files → internal `.venv/` storage
- **pull:** copies internal storage → working files
- **select/switch:** pushes current venv, then pulls the target venv
