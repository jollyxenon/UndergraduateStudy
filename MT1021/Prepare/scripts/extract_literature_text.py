#!/usr/bin/env python3
"""Extract text from downloaded literature PDFs."""

from __future__ import annotations

from pathlib import Path

import pdfplumber


ROOT = Path(__file__).resolve().parents[1]
LITERATURE_DIR = ROOT / "literature"
OUTPUT_DIR = ROOT / "work" / "research_notes" / "literature_text"


def extract_pdf(pdf_path: Path, output_path: Path) -> tuple[int, int]:
    """Extract PDF text with page markers; return page and non-empty counts."""
    pages = 0
    nonempty = 0
    chunks: list[str] = []
    with pdfplumber.open(pdf_path) as pdf:
        for pages, page in enumerate(pdf.pages, start=1):
            text = page.extract_text() or ""
            if text.strip():
                nonempty += 1
            chunks.append(f"\n\n===== PAGE {pages} =====\n{text.strip()}")
    output_path.write_text("".join(chunks).strip() + "\n", encoding="utf-8")
    return pages, nonempty


def main() -> None:
    """Extract all literature PDFs."""
    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
    for pdf_path in sorted(LITERATURE_DIR.glob("*.pdf")):
        output_path = OUTPUT_DIR / f"{pdf_path.stem}.txt"
        pages, nonempty = extract_pdf(pdf_path, output_path)
        print(f"{pdf_path.name}: {pages} pages, {nonempty} text pages")


if __name__ == "__main__":
    main()
