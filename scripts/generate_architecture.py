#!/usr/bin/env python3
"""
Generate architecture diagrams and module maps from repository structure.
Run: python scripts/generate_architecture.py
Output: docs/architecture/module-map-generated.md and Mermaid diagram
"""

import os
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parent.parent
SRC_DIR = REPO_ROOT / "src"
DOCS_DIR = REPO_ROOT / "docs"
ARCH_DIR = DOCS_DIR / "architecture"


def collect_modules():
    """Collect HAL and service modules from src/."""
    hal_modules = []
    service_modules = []
    mock_drivers = []

    hal_dir = SRC_DIR / "hal"
    if hal_dir.exists():
        for f in sorted(hal_dir.glob("*.hpp")):
            if "hal" in f.name and not f.name.startswith("mock"):
                hal_modules.append(f.stem)

    services_dir = SRC_DIR / "services"
    if services_dir.exists():
        for f in sorted(services_dir.glob("*.hpp")):
            if "service" in f.name:
                service_modules.append(f.stem)

    mock_dir = SRC_DIR / "drivers" / "mock"
    if mock_dir.exists():
        for f in sorted(mock_dir.glob("*.hpp")):
            mock_drivers.append(f.stem)

    return hal_modules, service_modules, mock_drivers


def generate_mermaid(hal_modules, service_modules, mock_drivers):
    """Generate Mermaid diagram of module structure."""
    lines = [
        "graph TB",
        "    subgraph HAL",
    ]
    for m in hal_modules:
        node = m.replace("-", "_").replace(".", "_")
        lines.append(f"        {node}[{m}]")
    lines.append("    end")
    lines.append("")
    lines.append("    subgraph Services")
    for m in service_modules:
        node = m.replace("-", "_").replace(".", "_")
        lines.append(f"        {node}[{m}]")
    lines.append("    end")
    lines.append("")
    lines.append("    subgraph MockDrivers")
    for m in mock_drivers:
        node = m.replace("-", "_").replace(".", "_")
        lines.append(f"        {node}[{m}]")
    lines.append("    end")
    return "\n".join(lines)


def generate_module_map_md(hal_modules, service_modules, mock_drivers):
    """Generate Markdown module map."""
    lines = [
        "# Module Map (Auto-Generated)",
        "",
        "> Regenerate with: `python scripts/generate_architecture.py`",
        "",
        "## Mermaid Diagram",
        "",
        "```mermaid",
        generate_mermaid(hal_modules, service_modules, mock_drivers),
        "```",
        "",
        "## HAL Modules",
        "",
        "| Module | File |",
        "|--------|------|",
    ]
    for m in hal_modules:
        lines.append(f"| {m} | {m}.hpp |")

    lines.extend([
        "",
        "## Services",
        "",
        "| Service |",
        "|---------|",
    ])
    for m in service_modules:
        lines.append(f"| {m} |")

    lines.extend([
        "",
        "## Mock Drivers",
        "",
        "| Driver |",
        "|--------|",
    ])
    for m in mock_drivers:
        lines.append(f"| {m} |")

    return "\n".join(lines)


def main():
    ARCH_DIR.mkdir(parents=True, exist_ok=True)
    hal, svc, mock = collect_modules()

    content = generate_module_map_md(hal, svc, mock)
    out_path = ARCH_DIR / "module-map-generated.md"
    out_path.write_text(content, encoding="utf-8")
    print(f"Wrote {out_path}")

    # Also write raw Mermaid for inclusion elsewhere
    mermaid_path = ARCH_DIR / "architecture-generated.mmd"
    mermaid_path.write_text(generate_mermaid(hal, svc, mock), encoding="utf-8")
    print(f"Wrote {mermaid_path}")


if __name__ == "__main__":
    main()
