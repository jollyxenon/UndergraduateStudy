#!/usr/bin/env python3
"""OCR selected PDF pages and cache page-level text for later citation."""

from __future__ import annotations

import argparse
from concurrent.futures import ThreadPoolExecutor, as_completed
from pathlib import Path

import pytesseract
from pdf2image import convert_from_path


ROOT = Path(__file__).resolve().parents[1]
OUTPUT_ROOT = ROOT / "work" / "ocr_pages"


def parse_ranges(value: str) -> list[int]:
    """Parse comma-separated page ranges such as 1-3,8,10-12."""
    pages: set[int] = set()
    for part in value.split(","):
        part = part.strip()
        if not part:
            continue
        if "-" in part:
            start_text, end_text = part.split("-", 1)
            start = int(start_text)
            end = int(end_text)
            if start > end:
                raise ValueError(f"Invalid page range: {part}")
            pages.update(range(start, end + 1))
        else:
            pages.add(int(part))
    return sorted(pages)


def ocr_page(
    pdf_path: Path,
    page_number: int,
    output_dir: Path,
    dpi: int,
    lang: str,
    psm: int,
    timeout: int,
) -> tuple[int, str]:
    """OCR one page unless cached text already exists."""
    output_path = output_dir / f"page_{page_number:04d}.txt"
    if output_path.exists() and output_path.read_text(encoding="utf-8").strip():
        return page_number, "cached"

    images = convert_from_path(
        str(pdf_path),
        dpi=dpi,
        first_page=page_number,
        last_page=page_number,
        fmt="png",
        thread_count=1,
    )
    if not images:
        output_path.write_text("", encoding="utf-8")
        return page_number, "empty"

    image = images[0].convert("L")
    try:
        text = pytesseract.image_to_string(
            image,
            lang=lang,
            config=f"--psm {psm} -c preserve_interword_spaces=1",
            timeout=timeout,
        )
    except RuntimeError as exc:
        output_path.write_text(f"[OCR_TIMEOUT_OR_ERROR] {exc}\n", encoding="utf-8")
        return page_number, "error"

    output_path.write_text(text.strip() + "\n", encoding="utf-8")
    return page_number, "ocr"


def write_combined(pdf_path: Path, output_dir: Path, pages: list[int]) -> Path:
    """Write a combined page-delimited text file for quick search and reading."""
    combined_path = output_dir / "combined_selected_pages.txt"
    chunks: list[str] = []
    for page_number in pages:
        page_path = output_dir / f"page_{page_number:04d}.txt"
        text = page_path.read_text(encoding="utf-8") if page_path.exists() else ""
        chunks.append(f"\n\n===== {pdf_path.name} PAGE {page_number} =====\n{text.strip()}")
    combined_path.write_text("".join(chunks).strip() + "\n", encoding="utf-8")
    return combined_path


def main() -> None:
    """Run OCR for requested pages and print compact progress."""
    parser = argparse.ArgumentParser()
    parser.add_argument("pdf", type=Path)
    parser.add_argument("--pages", required=True, help="Comma-separated pages/ranges, e.g. 1-20,35")
    parser.add_argument("--dpi", type=int, default=180)
    parser.add_argument("--lang", default="chi_sim+eng")
    parser.add_argument("--psm", type=int, default=6)
    parser.add_argument("--timeout", type=int, default=90)
    parser.add_argument("--workers", type=int, default=2)
    args = parser.parse_args()

    pdf_path = args.pdf.resolve()
    if not pdf_path.exists():
        raise SystemExit(f"PDF not found: {pdf_path}")

    pages = parse_ranges(args.pages)
    output_dir = OUTPUT_ROOT / pdf_path.stem
    output_dir.mkdir(parents=True, exist_ok=True)

    with ThreadPoolExecutor(max_workers=max(1, args.workers)) as pool:
        futures = [
            pool.submit(
                ocr_page,
                pdf_path,
                page_number,
                output_dir,
                args.dpi,
                args.lang,
                args.psm,
                args.timeout,
            )
            for page_number in pages
        ]
        for future in as_completed(futures):
            page_number, status = future.result()
            print(f"{pdf_path.name} page {page_number}: {status}")

    combined_path = write_combined(pdf_path, output_dir, pages)
    print(f"combined: {combined_path}")


if __name__ == "__main__":
    main()
