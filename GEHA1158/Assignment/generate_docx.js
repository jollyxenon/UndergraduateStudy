const fs = require("fs");
const path = require("path");
const {
  AlignmentType,
  BorderStyle,
  Document,
  Footer,
  HeadingLevel,
  ImageRun,
  Packer,
  PageNumber,
  Paragraph,
  TextRun,
} = require("docx");

const INPUT = "paper_final.md";
const OUTPUT = "paper_final.docx";
const TITLE = "规训与启蒙：青年数字文化在中国高校通识教育中的话语审视";
const AUTHOR = "2025521024 姚鉴轩";

const FONT_BODY = { eastAsia: "SimSun", ascii: "Times New Roman", hAnsi: "Times New Roman" };
const FONT_HEADING = { eastAsia: "SimHei", ascii: "Times New Roman", hAnsi: "Times New Roman" };
const CONTENT_WIDTH_PX = 540;

// Create a consistently styled text run with Chinese and Latin font fallbacks.
function run(text, options = {}) {
  return new TextRun({
    text,
    font: options.font || FONT_BODY,
    size: options.size || 24,
    bold: options.bold || false,
    italics: options.italics || false,
    superScript: options.superScript || false,
  });
}

// Convert simple Markdown inline syntax into editable Word text runs.
function inlineRuns(text, options = {}) {
  const runs = [];
  const pattern = /(\[\^(\d+)\]|_([^_]+)_)/g;
  let cursor = 0;
  let match;

  while ((match = pattern.exec(text)) !== null) {
    if (match.index > cursor) {
      runs.push(run(text.slice(cursor, match.index), options));
    }

    if (match[2]) {
      runs.push(run(`[${match[2]}]`, { ...options, size: 18, superScript: true }));
    } else if (match[3]) {
      runs.push(run(match[3], { ...options, italics: true }));
    }

    cursor = pattern.lastIndex;
  }

  if (cursor < text.length) {
    runs.push(run(text.slice(cursor), options));
  }

  return runs;
}

// Build a normal body paragraph following common Chinese academic-paper layout.
function bodyParagraph(text) {
  return new Paragraph({
    children: inlineRuns(text),
    alignment: AlignmentType.JUSTIFIED,
    indent: { firstLine: 480 },
    spacing: { line: 360, before: 0, after: 120 },
  });
}

// Build an abstract or keyword paragraph without first-line indentation.
function frontMatterParagraph(label, text) {
  return new Paragraph({
    children: [
      run(`${label}：`, { bold: true }),
      ...inlineRuns(text),
    ],
    alignment: AlignmentType.JUSTIFIED,
    spacing: { line: 360, before: 0, after: 120 },
  });
}

// Build a first-level numbered heading with Word outline metadata.
function heading1(text) {
  return new Paragraph({
    heading: HeadingLevel.HEADING_1,
    children: [run(text, { font: FONT_HEADING, size: 28, bold: true })],
    spacing: { before: 240, after: 180 },
  });
}

// Build a second-level numbered heading with Word outline metadata.
function heading2(text) {
  return new Paragraph({
    heading: HeadingLevel.HEADING_2,
    children: [run(text, { font: FONT_HEADING, size: 24, bold: true })],
    spacing: { before: 180, after: 120 },
  });
}

// Read PNG dimensions directly from the file header so images keep their ratio.
function pngSize(imagePath) {
  const buffer = fs.readFileSync(imagePath);
  return {
    width: buffer.readUInt32BE(16),
    height: buffer.readUInt32BE(20),
  };
}

// Build a centered figure paragraph with bounded width and accessible alt text.
function figureParagraph(imagePath, caption) {
  const { width, height } = pngSize(imagePath);
  const renderedWidth = Math.min(CONTENT_WIDTH_PX, width);
  const renderedHeight = Math.round((height / width) * renderedWidth);

  return new Paragraph({
    alignment: AlignmentType.CENTER,
    children: [
      new ImageRun({
        type: "png",
        data: fs.readFileSync(imagePath),
        transformation: { width: renderedWidth, height: renderedHeight },
        altText: {
          title: caption || path.basename(imagePath),
          description: caption || path.basename(imagePath),
          name: caption || path.basename(imagePath),
        },
      }),
    ],
    spacing: { before: 120, after: 60 },
  });
}

// Build a figure caption in a smaller centered style.
function captionParagraph(text) {
  return new Paragraph({
    children: inlineRuns(text, { size: 21 }),
    alignment: AlignmentType.CENTER,
    spacing: { before: 0, after: 180 },
  });
}

// Convert a Markdown footnote bibliography entry into a GB/T-like numbered entry.
function referenceParagraph(text) {
  const normalized = text.replace(/^\[\^(\d+)\]:\s*/, "[$1] ");
  return new Paragraph({
    children: inlineRuns(normalized, { size: 21 }),
    alignment: AlignmentType.JUSTIFIED,
    indent: { left: 420, hanging: 420 },
    spacing: { line: 300, before: 0, after: 80 },
  });
}

// Consume consecutive non-empty Markdown lines as a single paragraph block.
function readParagraph(lines, start) {
  const parts = [];
  let index = start;

  while (index < lines.length) {
    const line = lines[index].trim();
    if (!line || line.startsWith("#") || line.startsWith("![")) {
      break;
    }
    parts.push(line);
    index += 1;
  }

  return { text: parts.join(" "), next: index };
}

// Parse the Markdown manuscript into document children.
function parseMarkdown(markdown) {
  const children = [];
  const lines = markdown.replace(/\r\n/g, "\n").split("\n");

  for (let i = 0; i < lines.length;) {
    const raw = lines[i];
    const line = raw.trim();

    if (!line || line === `# ${TITLE}`) {
      i += 1;
      continue;
    }

    if (line.startsWith("### ")) {
      children.push(heading2(line.replace(/^###\s+/, "")));
      i += 1;
      continue;
    }

    if (line.startsWith("## ")) {
      children.push(heading1(line.replace(/^##\s+/, "")));
      i += 1;
      continue;
    }

    const imageMatch = line.match(/^!\[[^\]]*\]\(([^)]+)\)$/);
    if (imageMatch) {
      const imagePath = imageMatch[1].replace(/\\/g, "/");
      let caption = "";
      let next = i + 1;
      while (next < lines.length && !lines[next].trim()) {
        next += 1;
      }
      if (next < lines.length && /^图\d+[：:]/.test(lines[next].trim())) {
        caption = lines[next].trim();
        next += 1;
      }
      children.push(figureParagraph(imagePath, caption));
      if (caption) {
        children.push(captionParagraph(caption));
      }
      i = next;
      continue;
    }

    if (/^\[\^\d+\]:/.test(line)) {
      children.push(referenceParagraph(line));
      i += 1;
      continue;
    }

    const paragraph = readParagraph(lines, i);
    if (paragraph.text) {
      children.push(bodyParagraph(paragraph.text));
    }
    i = paragraph.next || i + 1;
  }

  return children;
}

// Build the full Word document with academic paper page and style settings.
function buildDocument(children) {
  return new Document({
    creator: "Codex",
    title: TITLE,
    description: "Formatted academic paper generated from paper_final.md",
    styles: {
      default: {
        document: {
          run: { font: FONT_BODY, size: 24 },
          paragraph: { spacing: { line: 360 } },
        },
      },
      paragraphStyles: [
        {
          id: "Heading1",
          name: "Heading 1",
          basedOn: "Normal",
          next: "Normal",
          quickFormat: true,
          run: { size: 28, bold: true, font: FONT_HEADING },
          paragraph: { spacing: { before: 240, after: 180 }, outlineLevel: 0 },
        },
        {
          id: "Heading2",
          name: "Heading 2",
          basedOn: "Normal",
          next: "Normal",
          quickFormat: true,
          run: { size: 24, bold: true, font: FONT_HEADING },
          paragraph: { spacing: { before: 180, after: 120 }, outlineLevel: 1 },
        },
      ],
    },
    sections: [
      {
        properties: {
          page: {
            size: { width: 11906, height: 16838 },
            margin: { top: 1440, right: 1800, bottom: 1440, left: 1800 },
          },
        },
        footers: {
          default: new Footer({
            children: [
              new Paragraph({
                alignment: AlignmentType.CENTER,
                children: [run("", { size: 20 }), new TextRun({ children: [PageNumber.CURRENT], size: 20 })],
              }),
            ],
          }),
        },
        children: [
          new Paragraph({
            children: [run(TITLE, { font: FONT_HEADING, size: 36, bold: true })],
            alignment: AlignmentType.CENTER,
            spacing: { before: 240, after: 240 },
          }),
          new Paragraph({
            children: [run(AUTHOR, { size: 24 })],
            alignment: AlignmentType.CENTER,
            spacing: { before: 0, after: 360 },
          }),
          frontMatterParagraph(
            "摘要",
            "本文以青年数字文化在中国高校通识教育中的边缘位置为问题意识，结合中外文献的关键词、学科分布与话语框架比较，讨论通识教育经典化倾向、文化资本再生产与管理性研究话语之间的关系。文章指出，青年数字文化并非仅是需要治理的外部现象，也可以作为理解学生现实文化经验、培养媒介素养和批判性思维的课程文本。将其分析性地纳入通识教育，并不意味着替代经典文本，而是要求通识教育在经典训练之外回应学生已经身处其中的数字文化现实。"
          ),
          frontMatterParagraph(
            "关键词",
            "青年数字文化；通识教育；文化资本；文献计量；批判性教育学"
          ),
          new Paragraph({
            border: { bottom: { style: BorderStyle.SINGLE, size: 4, color: "999999", space: 1 } },
            spacing: { before: 120, after: 240 },
          }),
          ...children,
        ],
      },
    ],
  });
}

async function main() {
  const markdown = fs.readFileSync(INPUT, "utf8");
  const children = parseMarkdown(markdown);
  const doc = buildDocument(children);
  const buffer = await Packer.toBuffer(doc);
  fs.writeFileSync(OUTPUT, buffer);
  console.log(`Document created successfully at ${OUTPUT}`);
}

main().catch((error) => {
  console.error(error);
  process.exit(1);
});
