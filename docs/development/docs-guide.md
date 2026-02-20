# Documentation Guide

This section describes how the documentation is organized and how to build or extend it.

## Structure

| Path | Purpose |
|------|---------|
| `index.md` | Documentation home |
| `guides/` | Quick start, build, HDMI setup |
| `architecture/` | System design, diagrams, module map |
| `api/` | HAL and service API overview |
| `development/` | Project structure, contributing |
| `testing/` | Test strategy and execution |
| `*.md` (root) | Reference documents (Implementation, PRD, etc.) |

## Building

```bash
pip install -r docs/requirements-docs.txt
mkdocs build
```

## Serving Locally

```bash
mkdocs serve
# Open http://127.0.0.1:8000
```

## Regenerating Architecture

```bash
python scripts/generate_architecture.py
```

Updates `architecture/module-map-generated.md` from the `src/` directory structure.

## Mermaid Diagrams

Diagrams use [Mermaid](https://mermaid.js.org/) syntax. They render in:

- MkDocs Material (when built)
- GitHub (native Markdown preview)
- Most modern Markdown viewers
