#!/usr/bin/env python3
"""Extract page-level text from source PDFs for evidence-based desk research."""

from __future__ import annotations

from pathlib import Path

import pdfplumber


ROOT = Path(__file__).resolve().parents[1]
SOURCE_DIR = ROOT
OUTPUT_DIR = ROOT / "work" / "pdf_text"


def extract_pdf(pdf_path: Path, output_path: Path) -> tuple[int, int]:
    """Extract one PDF into a page-delimited text file."""
    page_count = 0
    nonempty_pages = 0
    lines: list[str] = []

    with pdfplumber.open(pdf_path) as pdf:
        for page_count, page in enumerate(pdf.pages, start=1):
            text = page.extract_text(x_tolerance=1, y_tolerance=3) or ""
            if text.strip():
                nonempty_pages += 1
            lines.append(f"\n\n===== PAGE {page_count} =====\n")
            lines.append(text)

    output_path.write_text("".join(lines).strip() + "\n", encoding="utf-8")
    return page_count, nonempty_pages


def main() -> None:
    """Extract all root-level PDFs and print a compact summary."""
    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
    pdf_paths = sorted(SOURCE_DIR.glob("*.pdf"))
    if not pdf_paths:
        raise SystemExit("No root-level PDF files found.")

    for pdf_path in pdf_paths:
        output_path = OUTPUT_DIR / f"{pdf_path.stem}.txt"
        page_count, nonempty_pages = extract_pdf(pdf_path, output_path)
        print(f"{pdf_path.name}: {page_count} pages, {nonempty_pages} pages with text")


if __name__ == "__main__":
    main()
