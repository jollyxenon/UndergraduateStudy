#!/usr/bin/env python3
"""Build a local Yan'an-only OCR corpus from cached report pages."""

from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
OCR_ROOT = ROOT / "work" / "ocr_pages"
OUTPUT_PATH = ROOT / "work" / "research_notes" / "yanan_local_ocr_corpus.txt"


@dataclass(frozen=True)
class Section:
    """Describe one Yan'an-related local source section."""

    title: str
    pdf_stem: str
    physical_start: int
    physical_end: int
    report_page_offset: int


SECTIONS = [
    Section("陕西延安课题简介", "陕西延安课题简介", 1, 3, 0),
    Section("2022 延安市退耕还林三十年的绩效评价", "社会实践2022年报告汇编", 175, 186, -4),
    Section("2022 立足黄土高原，追寻文化记忆", "社会实践2022年报告汇编", 187, 215, -4),
    Section("2023 延安红色文化资源的问题与开发路径", "社会实践2023年报告汇编", 211, 270, -4),
    Section("2024 延安文旅新潮：传承与创新", "社会实践2024年报告汇编", 195, 217, -6),
    Section("2025 延安的数字化创新之路", "社会实践2025年报告汇编", 204, 223, -6),
]


def page_text(pdf_stem: str, page_number: int) -> str:
    """Read cached OCR text for one physical PDF page."""
    page_path = OCR_ROOT / pdf_stem / f"page_{page_number:04d}.txt"
    if not page_path.exists():
        return "[MISSING_OCR_PAGE]"
    return page_path.read_text(encoding="utf-8").strip()


def build_section(section: Section) -> str:
    """Format one section with stable page markers."""
    chunks = [f"\n\n# {section.title}\n"]
    for physical_page in range(section.physical_start, section.physical_end + 1):
        report_page = physical_page + section.report_page_offset
        if section.report_page_offset:
            marker = f"physical_page={physical_page}; report_page={report_page}"
        else:
            marker = f"physical_page={physical_page}"
        chunks.append(f"\n\n===== {section.pdf_stem}.pdf; {marker} =====\n")
        chunks.append(page_text(section.pdf_stem, physical_page))
    return "".join(chunks)


def main() -> None:
    """Write the Yan'an-only corpus file."""
    OUTPUT_PATH.parent.mkdir(parents=True, exist_ok=True)
    content = ["延安本地资料 OCR 语料\n"]
    content.extend(build_section(section) for section in SECTIONS)
    OUTPUT_PATH.write_text("".join(content).strip() + "\n", encoding="utf-8")
    print(OUTPUT_PATH)


if __name__ == "__main__":
    main()
