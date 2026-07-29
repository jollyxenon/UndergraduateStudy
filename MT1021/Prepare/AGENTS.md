# AGENTS.md

## 项目目标

本目录用于陕西延安实践基地案头调研。核心交付物是 `docs/yanan_desk_research_report.md`，论文资料放在 `literature/`。

## 工作规范

- 优先使用 `pixi` 管理依赖和运行脚本。
- 原始 PDF 不修改，只在 `work/` 下生成抽取文本和 OCR 缓存。
- 年度汇编包含所有地区，分析时只使用陕西延安相关页段。
- 四份年度汇编为扫描版，引用时要保留原 PDF 名、物理页和报告页。
- 论文下载到 `literature/`，并在 `literature/README.md` 记录来源 URL 和用途。
- 正式报告与项目记忆放在 `docs/`。

## 常用命令

```bash
rtk proxy pixi run python scripts/extract_pdf_text.py
rtk proxy pixi run python scripts/ocr_pdf_pages.py '社会实践2025年报告汇编.pdf' --pages 204-223 --dpi 90 --workers 1 --timeout 60 --psm 11
rtk proxy pixi run python scripts/build_yanan_corpus.py
rtk proxy pixi run python scripts/extract_literature_text.py
```
