const path = require("path");
const pptxgen = require("pptxgenjs");

const pptx = new pptxgen();
pptx.layout = "LAYOUT_WIDE";
pptx.author = "姚鉴轩";
pptx.subject = "GEHA1158 classroom report";
pptx.title = "规训与启蒙：青年数字文化在中国高校通识教育中的话语审视";
pptx.company = "ShanghaiTech University";
pptx.lang = "zh-CN";
pptx.theme = {
  headFontFace: "Microsoft YaHei",
  bodyFontFace: "Microsoft YaHei",
  lang: "zh-CN",
};
pptx.defineLayout({ name: "CUSTOM_WIDE", width: 13.333, height: 7.5 });
pptx.layout = "CUSTOM_WIDE";
pptx.margin = 0;

const root = path.resolve(__dirname, "..");
const outPath = path.join(__dirname, "GEHA1158_final_report.pptx");

const C = {
  white: "FFFFFF",
  ink: "111111",
  blue: "1F5E99",
  paleBlue: "EAF2FA",
  lineBlue: "7CA7D8",
  gray: "5F6368",
  lightGray: "E6EAF0",
};

const FONT = "Microsoft YaHei";
const SLIDE_W = 13.333;
const SLIDE_H = 7.5;

function text(slide, value, x, y, w, h, opts = {}) {
  slide.addText(value, {
    x,
    y,
    w,
    h,
    fontFace: FONT,
    color: opts.color || C.ink,
    fontSize: opts.fontSize || 20,
    bold: opts.bold || false,
    margin: opts.margin ?? [2, 4, 2, 4],
    breakLine: false,
    fit: "shrink",
    valign: opts.valign || "mid",
    align: opts.align || "left",
    paraSpaceAfterPt: opts.paraSpaceAfterPt || 0,
    lineSpacingMultiple: opts.lineSpacingMultiple || 0.95,
    rotate: opts.rotate,
  });
}

function richText(slide, runs, x, y, w, h, opts = {}) {
  slide.addText(runs, {
    x,
    y,
    w,
    h,
    fontFace: FONT,
    fontSize: opts.fontSize || 20,
    color: C.ink,
    margin: opts.margin ?? [2, 4, 2, 4],
    fit: "shrink",
    valign: opts.valign || "mid",
    lineSpacingMultiple: opts.lineSpacingMultiple || 0.95,
  });
}

function shape(slide, shapeType, opts) {
  slide.addShape(shapeType, opts);
}

function header(slide, section, title) {
  shape(slide, pptx.ShapeType.rect, {
    x: 0,
    y: 0,
    w: SLIDE_W,
    h: 0.16,
    fill: { color: C.blue },
    line: { color: C.blue, transparency: 100 },
  });
  text(slide, section, 0.55, 0.36, 1.2, 0.26, {
    fontSize: 10,
    bold: true,
    color: C.blue,
  });
  text(slide, title, 0.55, 0.66, 11.7, 0.52, {
    fontSize: 25,
    bold: true,
  });
}

function footer(slide, idx) {
  shape(slide, pptx.ShapeType.line, {
    x: 0.55,
    y: 7.05,
    w: 12.1,
    h: 0,
    line: { color: C.lightGray, width: 0.8 },
  });
  text(slide, "GEHA1158 期末课堂报告", 0.55, 7.12, 3.4, 0.22, {
    fontSize: 8,
    color: C.gray,
  });
  text(slide, String(idx).padStart(2, "0"), 12.0, 7.08, 0.65, 0.28, {
    fontSize: 10,
    color: C.blue,
    bold: true,
    align: "right",
  });
}

function bulletList(slide, items, x, y, w, opts = {}) {
  const fs = opts.fontSize || 19;
  const gap = opts.gap || 0.55;
  const box = opts.box || 0.12;
  items.forEach((item, i) => {
    const yy = y + i * gap;
    shape(slide, pptx.ShapeType.rect, {
      x,
      y: yy + 0.15,
      w: box,
      h: box,
      fill: { color: C.blue },
      line: { color: C.blue },
    });
    text(slide, item, x + 0.25, yy, w - 0.25, gap - 0.02, {
      fontSize: fs,
      color: opts.color || C.ink,
      lineSpacingMultiple: 0.9,
      valign: "mid",
    });
  });
}

function label(slide, value, x, y, w, h, opts = {}) {
  shape(slide, pptx.ShapeType.roundRect, {
    x,
    y,
    w,
    h,
    rectRadius: 0.08,
    fill: { color: opts.fill || C.paleBlue },
    line: { color: opts.line || C.lineBlue, width: 0.8 },
  });
  text(slide, value, x + 0.1, y + 0.02, w - 0.2, h - 0.04, {
    fontSize: opts.fontSize || 17,
    bold: opts.bold || false,
    color: opts.color || C.ink,
    align: opts.align || "center",
  });
}

function sectionTag(slide, value, x, y, w = 1.0) {
  shape(slide, pptx.ShapeType.rect, {
    x,
    y,
    w,
    h: 0.26,
    fill: { color: C.blue },
    line: { color: C.blue },
  });
  text(slide, value, x + 0.04, y + 0.02, w - 0.08, 0.2, {
    fontSize: 9,
    bold: true,
    color: C.white,
    align: "center",
  });
}

function arrow(slide, x1, y1, x2, y2) {
  shape(slide, pptx.ShapeType.line, {
    x: x1,
    y: y1,
    w: x2 - x1,
    h: y2 - y1,
    line: { color: C.blue, width: 1.2, beginArrowType: "none", endArrowType: "triangle" },
  });
}

function addImage(slide, relPath, x, y, w, h) {
  slide.addImage({
    path: path.join(root, relPath),
    x,
    y,
    w,
    h,
    transparency: 0,
  });
}

function barChart(slide, title, data, x, y, w, h, opts = {}) {
  const maxValue = Math.max(...data.map((d) => d.value));
  const labelW = opts.labelW || 2.05;
  const valueW = 0.36;
  const barX = x + labelW;
  const barW = w - labelW - valueW - 0.12;
  const rowH = (h - 0.52) / data.length;
  text(slide, title, x, y, w, 0.32, {
    fontSize: opts.titleSize || 14,
    bold: true,
    color: C.blue,
  });
  shape(slide, pptx.ShapeType.line, {
    x: barX,
    y: y + 0.48,
    w: 0,
    h: h - 0.48,
    line: { color: C.lightGray, width: 0.8 },
  });
  data.forEach((item, i) => {
    const yy = y + 0.52 + i * rowH;
    const bw = (item.value / maxValue) * barW;
    text(slide, item.label, x, yy, labelW - 0.08, rowH * 0.86, {
      fontSize: opts.fontSize || 9.5,
      color: C.ink,
      align: "right",
    });
    shape(slide, pptx.ShapeType.rect, {
      x: barX,
      y: yy + rowH * 0.2,
      w: bw,
      h: rowH * 0.5,
      fill: { color: C.blue },
      line: { color: C.blue, transparency: 100 },
    });
    text(slide, String(item.value), barX + bw + 0.05, yy, valueW, rowH * 0.86, {
      fontSize: opts.fontSize || 9.5,
      color: C.gray,
    });
  });
}

const wosKeywords = [
  { label: "digital culture", value: 12 },
  { label: "education", value: 12 },
  { label: "higher education", value: 10 },
  { label: "popular culture", value: 7 },
  { label: "digital transformation", value: 5 },
  { label: "university", value: 3 },
  { label: "engagement", value: 3 },
  { label: "digital humanities", value: 3 },
  { label: "pedagogy", value: 3 },
  { label: "ict", value: 3 },
];

const cnkiKeywords = [
  { label: "网络亚文化", value: 71 },
  { label: "大学生", value: 50 },
  { label: "思想政治教育", value: 30 },
  { label: "高校", value: 17 },
  { label: "对策", value: 10 },
  { label: "影响", value: 9 },
  { label: "亚文化", value: 7 },
  { label: "大学生价值观", value: 7 },
  { label: "价值观", value: 6 },
  { label: "新媒体", value: 6 },
];

const slides = [
  {
    section: "00",
    title: "规训与启蒙",
    subtitle: "青年数字文化在中国高校通识教育中的话语审视",
    notes:
      "大家好，我今天汇报的论文题目是《规训与启蒙：青年数字文化在中国高校通识教育中的话语审视》。中心论点是：青年数字文化不应取代经典文本，但应作为可分析、可批判、可反思的文化文本进入通识教育讨论。",
    draw(slide) {
      shape(slide, pptx.ShapeType.rect, {
        x: 0,
        y: 0,
        w: SLIDE_W,
        h: SLIDE_H,
        fill: { color: C.white },
        line: { color: C.white },
      });
      shape(slide, pptx.ShapeType.rect, {
        x: 0,
        y: 0,
        w: 0.24,
        h: SLIDE_H,
        fill: { color: C.blue },
        line: { color: C.blue },
      });
      text(slide, "规训与启蒙", 0.8, 1.36, 7.2, 0.78, {
        fontSize: 40,
        bold: true,
      });
      text(slide, "青年数字文化在中国高校通识教育中的话语审视", 0.83, 2.15, 9.8, 0.45, {
        fontSize: 22,
        color: C.blue,
        bold: true,
      });
      shape(slide, pptx.ShapeType.line, {
        x: 0.83,
        y: 2.85,
        w: 6.2,
        h: 0,
        line: { color: C.blue, width: 1.6 },
      });
      richText(
        slide,
        [
          { text: "中心论点：", options: { bold: true, color: C.ink } },
          { text: "不替代经典，而是把青年数字文化作为", options: { color: C.ink } },
          { text: "可批判分析的文化文本", options: { bold: true, color: C.blue } },
          { text: "纳入通识教育。", options: { color: C.ink } },
        ],
        0.83,
        3.4,
        9.8,
        0.75,
        { fontSize: 24 }
      );
      text(slide, "姚鉴轩 2025521024", 0.86, 6.55, 3.5, 0.3, {
        fontSize: 13,
        color: C.gray,
      });
    },
  },
  {
    section: "ROUTE",
    title: "原论文结构与 10 分钟汇报路线",
    notes:
      "这次汇报严格按照原论文的大结构展开：引言、界定、经典化与文化区隔、国内外话语比较、边缘化争议、限度与意义、结语。我会把重点放在中心论点和论证过程上。",
    draw(slide) {
      header(slide, "ROUTE", "原论文结构与 10 分钟汇报路线");
      const items = [
        ["1 引言", "生活现实进入教育问题"],
        ["2 界定", "青年数字文化是什么"],
        ["3 经典化", "文化区隔为何形成"],
        ["4 比较", "国内外研究话语差异"],
        ["5 争议", "是否挤压经典文本"],
        ["6 意义", "分析性纳入的限度"],
        ["7 结语", "通识教育如何启蒙"],
      ];
      const leftX = 1.05;
      const rightX = 4.0;
      const topY = 1.42;
      const rowGap = 0.72;
      items.forEach((it, i) => {
        const y = topY + i * rowGap;
        label(slide, it[0], leftX, y, 2.25, 0.45, { bold: true, fontSize: 15 });
        text(slide, it[1], rightX, y + 0.02, 4.15, 0.38, { fontSize: 16 });
        if (i < items.length - 1) {
          shape(slide, pptx.ShapeType.line, {
            x: leftX + 1.12,
            y: y + 0.45,
            w: 0,
            h: rowGap - 0.45,
            line: { color: C.lineBlue, width: 1 },
          });
        }
      });
      shape(slide, pptx.ShapeType.line, {
        x: 8.78,
        y: 1.42,
        w: 0,
        h: 4.85,
        line: { color: C.lightGray, width: 1 },
      });
      text(slide, "汇报重心", 9.25, 1.55, 1.4, 0.28, {
        fontSize: 14,
        color: C.blue,
        bold: true,
      });
      bulletList(
        slide,
        ["第 3-5 页：解释问题为什么形成", "第 6-10 页：用话语比较提供证据", "第 11-13 页：回应争议并回到结论"],
        9.25,
        2.18,
        3.2,
        { fontSize: 14.5, gap: 0.58, box: 0.09 }
      );
      text(slide, "结构跟随原论文，篇幅按课堂讲解重新分配。", 9.25, 5.45, 3.0, 0.55, {
        fontSize: 15.5,
        bold: true,
        color: C.blue,
      });
      footer(slide, 2);
    },
  },
  {
    section: "01",
    title: "引言：从生活现实到教育问题",
    notes:
      "论文首先从现实背景出发：游戏文化、二次元文化、网络社群等已经参与大学生的娱乐、社交、表达和身份认同。问题是，它们在通识教育中更多被当作需要治理的对象，而不是可以进入课堂分析的文化材料。",
    draw(slide) {
      header(slide, "01", "引言：从生活现实到教育问题");
      label(slide, "学生的真实文化经验", 0.8, 1.7, 3.1, 0.62, { bold: true });
      label(slide, "通识教育的正式课程", 5.1, 1.7, 3.1, 0.62, { bold: true });
      label(slide, "结构性忽视", 9.4, 1.7, 2.45, 0.62, { bold: true });
      arrow(slide, 3.95, 2.01, 4.9, 2.01);
      arrow(slide, 8.25, 2.01, 9.2, 2.01);
      bulletList(slide, ["游戏、二次元、网络社群成为日常经验", "研究中常被放入引导、规范、治理语境", "论文要追问：通识教育能否回应这些经验"], 1.0, 3.15, 10.6, { fontSize: 21, gap: 0.7 });
      text(slide, "问题意识", 0.95, 5.82, 1.2, 0.32, { fontSize: 13, bold: true, color: C.blue });
      text(slide, "这不是把流行文化简单搬进课堂，而是讨论哪些文化经验有资格被通识教育认真分析。", 2.02, 5.72, 9.8, 0.5, { fontSize: 19, bold: true });
      footer(slide, 3);
    },
  },
  {
    section: "02",
    title: "青年数字文化的界定：讨论对象是什么",
    notes:
      "论文第二部分限定概念。这里的青年数字文化不是所有数字媒介活动，而是在高校学生中广泛参与、具有身份认同功能、带有亚文化色彩，并与正式教育边界关系紧张的实践。代表案例是动画、漫画、游戏等。",
    draw(slide) {
      header(slide, "02", "青年数字文化的界定：讨论对象是什么");
      const tags = ["广泛参与", "身份认同", "情感表达", "社群关系", "边界紧张"];
      tags.forEach((tag, i) => label(slide, tag, 0.85 + i * 2.35, 1.55, 1.72, 0.48, { bold: true, fontSize: 14 }));
      text(slide, "代表案例", 0.95, 2.75, 1.4, 0.3, { fontSize: 14, bold: true, color: C.blue });
      ["动画", "漫画", "游戏", "网络社群"].forEach((tag, i) => {
        label(slide, tag, 1.0 + i * 2.8, 3.18, 2.15, 0.72, { bold: true, fontSize: 21 });
      });
      bulletList(slide, ["不只是一组娱乐消费", "也是理解自我、表达情绪、建立社群的媒介", "因此不能只被概括为“不务正业”"], 1.0, 4.75, 10.7, { fontSize: 21, gap: 0.62 });
      footer(slide, 4);
    },
  },
  {
    section: "03",
    title: "经典化与文化区隔：为什么它难进入通识教育",
    notes:
      "第三部分是理论支点。通识教育偏好经典文本、传统文化、科学精神和制度化知识，这有正当性。但经典化也会确认哪些文化经验更有价值。文化资本与隐性课程视角提示我们，课程体系本身也在进行文化筛选。",
    draw(slide) {
      header(slide, "03", "经典化与文化区隔：为什么它难进入通识教育");
      label(slide, "经典文本与主流知识", 0.88, 1.65, 3.0, 0.65, { bold: true });
      label(slide, "文化资本再生产", 5.0, 1.65, 3.0, 0.65, { bold: true });
      label(slide, "隐性课程与品味规训", 9.1, 1.65, 3.0, 0.65, { bold: true });
      arrow(slide, 3.9, 1.98, 4.85, 1.98);
      arrow(slide, 8.02, 1.98, 8.95, 1.98);
      text(slide, "经典化的双重性", 0.95, 3.2, 2.1, 0.32, { fontSize: 16, bold: true, color: C.blue });
      bulletList(slide, ["正面：传承重要文化遗产，训练复杂文本理解", "风险：把某些文化自然化为更高价值的知识对象", "后果：青年数字文化缺少课程合法性"], 1.0, 3.72, 10.4, { fontSize: 21, gap: 0.64 });
      text(slide, "论文的关键判断：排除并不总是显性的，它也可能通过“什么值得学习”的标准发生。", 1.0, 6.05, 10.8, 0.45, { fontSize: 20, bold: true });
      footer(slide, 5);
    },
  },
  {
    section: "04.1",
    title: "材料与方法：探索性的话语比较",
    notes:
      "第四部分用材料来观察问题。论文参考 Web of Science 和 CNKI，围绕青年数字文化与高等教育、通识教育相关主题，整理标题、摘要、关键词和学科分类。这里做的是话语比较，不是对所有课堂实践的直接测量。",
    draw(slide) {
      header(slide, "04.1", "材料与方法：探索性的话语比较");
      label(slide, "Web of Science", 1.1, 1.65, 4.7, 0.65, { bold: true, fontSize: 22 });
      label(slide, "CNKI", 7.55, 1.65, 4.2, 0.65, { bold: true, fontSize: 22 });
      bulletList(slide, ["digital culture", "higher education", "popular culture", "pedagogy / literacy"], 1.25, 2.75, 4.25, { fontSize: 18, gap: 0.5 });
      bulletList(slide, ["网络亚文化", "大学生", "思想政治教育", "对策 / 治理"], 7.7, 2.75, 3.85, { fontSize: 18, gap: 0.5 });
      shape(slide, pptx.ShapeType.line, {
        x: 6.65,
        y: 1.62,
        w: 0,
        h: 3.9,
        line: { color: C.lightGray, width: 1 },
      });
      text(slide, "方法边界", 1.05, 5.85, 1.1, 0.28, { fontSize: 13, bold: true, color: C.blue });
      text(slide, "探索研究话语的倾向，不能替代对具体课堂实践的直接考察。", 2.0, 5.72, 8.9, 0.45, { fontSize: 20, bold: true });
      footer(slide, 6);
    },
  },
  {
    section: "04.2",
    title: "WoS：青年数字文化更容易进入教学转化框架",
    notes:
      "先看 WoS。高频关键词集中在 digital culture、education、higher education、popular culture，同时也出现 pedagogy、engagement、digital humanities 等词。这说明国际相关研究更容易把青年数字文化放入教学法、媒介素养、学习参与和文化理解的框架中。",
    draw(slide) {
      header(slide, "04.2", "WoS：青年数字文化更容易进入教学转化框架");
      barChart(slide, "Web of Science 高频关键词 Top 10", wosKeywords, 0.75, 1.55, 5.65, 4.85, {
        labelW: 2.15,
        fontSize: 9.2,
      });
      text(slide, "图表美化说明", 0.78, 6.46, 1.35, 0.22, { fontSize: 9, bold: true, color: C.blue });
      text(slide, "按原始关键词频次重绘，统一为黑白蓝横向条形图。", 2.0, 6.42, 4.25, 0.28, {
        fontSize: 10.5,
        color: C.gray,
      });
      text(slide, "观察重点", 7.0, 1.58, 1.35, 0.28, { fontSize: 14, bold: true, color: C.blue });
      bulletList(
        slide,
        [
          "关键词围绕教育、高等教育和流行文化展开",
          "pedagogy、engagement 指向课堂方法与学习参与",
          "digital humanities、media literacy 一类表达把数字文化视为可分析材料",
          "它不只是被当成风险，而是更容易被转化为教学资源",
        ],
        7.0,
        2.08,
        5.3,
        { fontSize: 18, gap: 0.68 }
      );
      text(slide, "简化结论：WoS 话语更接近“教学转化”。", 7.0, 5.92, 5.0, 0.42, {
        fontSize: 21,
        bold: true,
        color: C.blue,
      });
      footer(slide, 7);
    },
  },
  {
    section: "04.2",
    title: "CNKI：青年数字文化更常进入管理规训框架",
    notes:
      "再看 CNKI。高频关键词明显集中在网络亚文化、大学生、思想政治教育、高校、对策、影响等词。这并不说明国内研究不讨论青年数字文化，而是说明它更常以治理、引导和价值教育问题的身份进入教育讨论。",
    draw(slide) {
      header(slide, "04.2", "CNKI：青年数字文化更常进入管理规训框架");
      barChart(slide, "CNKI 高频关键词 Top 10", cnkiKeywords, 0.75, 1.55, 5.65, 4.85, {
        labelW: 1.75,
        fontSize: 10.2,
      });
      text(slide, "图表美化说明", 0.78, 6.46, 1.35, 0.22, { fontSize: 9, bold: true, color: C.blue });
      text(slide, "按原始关键词频次重绘，去除拥挤坐标与非主色系。", 2.0, 6.42, 4.25, 0.28, {
        fontSize: 10.5,
        color: C.gray,
      });
      text(slide, "观察重点", 7.0, 1.58, 1.35, 0.28, { fontSize: 14, bold: true, color: C.blue });
      bulletList(
        slide,
        [
          "网络亚文化、大学生、思政教育形成明显核心",
          "对策、影响、新媒体等词强化问题回应导向",
          "青年数字文化更常被理解为价值教育与治理对象",
          "它可以被讨论，但较少直接作为通识课程文本出现",
        ],
        7.0,
        2.08,
        5.3,
        { fontSize: 18, gap: 0.68 }
      );
      text(slide, "简化结论：CNKI 话语更接近“管理规训”。", 7.0, 5.92, 5.0, 0.42, {
        fontSize: 21,
        bold: true,
        color: C.blue,
      });
      footer(slide, 8);
    },
  },
  {
    section: "04.3",
    title: "两种框架：管理规训 vs. 教学转化",
    notes:
      "论文把差异概括为两种框架。管理规训框架把青年数字文化视为需要引导和规范的对象；教学转化框架把它看作可以进入课程、被批判分析的资源。这不是绝对二分，而是帮助理解研究重心的差异。",
    draw(slide) {
      header(slide, "04.3", "两种框架：管理规训 vs. 教学转化");
      label(slide, "管理规训", 1.0, 1.55, 4.8, 0.68, { bold: true, fontSize: 24 });
      label(slide, "教学转化", 7.55, 1.55, 4.8, 0.68, { bold: true, fontSize: 24 });
      bulletList(slide, ["对象：偏离主流价值的风险", "语言：引导、规范、治理、对策", "目标：降低不确定性"], 1.15, 2.75, 4.25, { fontSize: 19, gap: 0.58 });
      bulletList(slide, ["对象：可分析的文化文本", "语言：教学法、媒介素养、批判思维", "目标：扩展理解与反思"], 7.72, 2.75, 4.25, { fontSize: 19, gap: 0.58 });
      shape(slide, pptx.ShapeType.line, {
        x: 6.65,
        y: 1.55,
        w: 0,
        h: 4.1,
        line: { color: C.blue, width: 1.4, dash: "dash" },
      });
      text(slide, "同一文化经验，在不同话语框架下会获得不同教育意义。", 1.55, 6.25, 10.1, 0.35, { fontSize: 20, bold: true, align: "center" });
      footer(slide, 9);
    },
  },
  {
    section: "04.4",
    title: "教育含义：话语如何影响课程合法性",
    notes:
      "研究话语不等于课堂实践，但会影响课程想象。当一种文化长期出现在管理、规范和对策语言中，它就更难被看作值得进入课堂讨论的文本。因此，论文提出的是一个谨慎判断：国内研究中的管理导向提示了潜在的边缘化倾向。",
    draw(slide) {
      header(slide, "04.4", "教育含义：话语如何影响课程合法性");
      const steps = [
        "研究话语",
        "课程想象",
        "合法性判断",
        "课堂能见度",
      ];
      steps.forEach((s, i) => {
        label(slide, s, 1.0 + i * 2.95, 1.75, 2.1, 0.62, { bold: true });
        if (i < steps.length - 1) arrow(slide, 3.13 + i * 2.95, 2.06, 3.78 + i * 2.95, 2.06);
      });
      text(slide, "谨慎结论", 1.02, 3.3, 1.3, 0.3, { fontSize: 14, bold: true, color: C.blue });
      bulletList(slide, ["不是证明课堂实践中存在全面排斥", "而是提示研究话语中的管理导向", "青年数字文化更多作为“问题对象”出现"], 1.05, 3.82, 10.6, { fontSize: 22, gap: 0.65 });
      text(slide, "关键差别：作为问题被管理，还是作为文本被分析。", 1.1, 6.15, 10.7, 0.44, { fontSize: 23, bold: true, color: C.blue, align: "center" });
      footer(slide, 10);
    },
  },
  {
    section: "05",
    title: "边缘化及其争议：会不会挤压经典？",
    notes:
      "第五部分回应重要反驳：如果通识教育课程空间有限，纳入青年数字文化会不会挤压经典？论文承认这个担心有道理，但回应是：纳入并不等于替代。青年数字文化可以作为案例、文本或媒介现象进入课堂。",
    draw(slide) {
      header(slide, "05", "边缘化及其争议：会不会挤压经典？");
      label(slide, "核心异议", 0.95, 1.7, 2.4, 0.58, { bold: true });
      text(slide, "课程空间有限，青年数字文化是否会削弱经典训练？", 3.55, 1.7, 7.8, 0.58, { fontSize: 24, bold: true });
      shape(slide, pptx.ShapeType.line, {
        x: 1.0,
        y: 2.76,
        w: 10.9,
        h: 0,
        line: { color: C.lightGray, width: 1 },
      });
      label(slide, "不是替代", 1.2, 3.4, 2.4, 0.72, { bold: true, fontSize: 23 });
      label(slide, "不是迎合", 5.25, 3.4, 2.4, 0.72, { bold: true, fontSize: 23 });
      label(slide, "而是分析", 9.3, 3.4, 2.4, 0.72, { bold: true, fontSize: 23 });
      arrow(slide, 3.7, 3.76, 5.0, 3.76);
      arrow(slide, 7.75, 3.76, 9.05, 3.76);
      text(slide, "目标不是降低通识教育的深度，而是让它能够处理当代学生真实面对的文化问题。", 1.05, 5.72, 10.95, 0.55, { fontSize: 21, bold: true });
      footer(slide, 11);
    },
  },
  {
    section: "06",
    title: "限度与意义：分析性纳入，而非迎合式肯定",
    notes:
      "第六部分说明限度。论文不是因为青年数字文化流行，就认为它天然正确；也不是因为学生喜欢，就要求课堂迎合。所谓批判性纳入，是把它当作文化文本，分析其身份、社群、价值、平台和制度关系。",
    draw(slide) {
      header(slide, "06", "限度与意义：分析性纳入，而非迎合式肯定");
      text(slide, "青年数字文化值得进入通识教育，不是因为它流行，而是因为它已经影响学生理解世界与理解自身的方式。", 0.98, 1.52, 11.5, 0.72, {
        fontSize: 24,
        bold: true,
        color: C.blue,
        align: "center",
      });
      const cells = [
        ["身份", "如何塑造自我理解"],
        ["社群", "如何形成归属与边界"],
        ["价值", "如何传播判断与欲望"],
        ["平台", "如何受到商业机制塑造"],
      ];
      cells.forEach((cell, i) => {
        const x = 0.9 + i * 3.05;
        label(slide, cell[0], x, 3.0, 2.35, 0.62, { bold: true, fontSize: 22 });
        text(slide, cell[1], x + 0.1, 3.85, 2.15, 0.5, { fontSize: 16, color: C.gray, align: "center" });
      });
      text(slide, "批判性纳入 = 作为文化文本来分析，而不是无条件赞美。", 1.05, 5.78, 10.95, 0.42, { fontSize: 22, bold: true });
      footer(slide, 12);
    },
  },
  {
    section: "07",
    title: "结语：通识教育如何完成启蒙任务",
    notes:
      "最后回到规训与启蒙。当通识教育只承认制度化经典文化，而把青年数字文化主要放入管理框架，它就在无形中规训文化品味。相反，如果能将这些经验转化为可讨论、可反思、可分析的材料，通识教育就更接近启蒙任务。",
    draw(slide) {
      header(slide, "07", "结语：通识教育如何完成启蒙任务");
      label(slide, "规训", 1.25, 1.65, 3.8, 0.72, { bold: true, fontSize: 27 });
      label(slide, "启蒙", 8.2, 1.65, 3.8, 0.72, { bold: true, fontSize: 27 });
      bulletList(slide, ["只承认制度化经典文化", "把青年数字文化视为管理对象", "暗中规定“什么值得学习”"], 1.35, 2.82, 3.6, { fontSize: 17, gap: 0.55 });
      bulletList(slide, ["面对学生现实文化经验", "转化为可分析的课程材料", "训练批判性理解能力"], 8.3, 2.82, 3.6, { fontSize: 17, gap: 0.55 });
      arrow(slide, 5.3, 3.55, 7.85, 3.55);
      text(slide, "论文最终主张：通识教育的深度不只来自经典本身，也来自它能否帮助学生理解自己实际生活其中的文化世界。", 1.05, 5.82, 11.1, 0.6, {
        fontSize: 22,
        bold: true,
        color: C.blue,
        align: "center",
      });
      footer(slide, 13);
    },
  },
  {
    section: "END",
    title: "谢谢",
    notes:
      "以上就是我的汇报，谢谢大家。欢迎老师和同学提出问题。",
    draw(slide) {
      shape(slide, pptx.ShapeType.rect, {
        x: 0,
        y: 0,
        w: SLIDE_W,
        h: SLIDE_H,
        fill: { color: C.white },
        line: { color: C.white },
      });
      shape(slide, pptx.ShapeType.rect, {
        x: 0,
        y: 0,
        w: SLIDE_W,
        h: 0.16,
        fill: { color: C.blue },
        line: { color: C.blue },
      });
      text(slide, "谢谢", 0.9, 2.2, 4.2, 0.8, {
        fontSize: 44,
        bold: true,
      });
      shape(slide, pptx.ShapeType.line, {
        x: 0.95,
        y: 3.25,
        w: 4.5,
        h: 0,
        line: { color: C.blue, width: 1.6 },
      });
      text(slide, "欢迎老师和同学批评指正", 0.95, 3.72, 5.6, 0.42, {
        fontSize: 23,
        color: C.blue,
        bold: true,
      });
      text(slide, "规训与启蒙：青年数字文化在中国高校通识教育中的话语审视", 0.95, 6.35, 8.5, 0.32, {
        fontSize: 13,
        color: C.gray,
      });
    },
  },
];

function validateSlideTexts() {
  const sourceTexts = [];
  for (const slide of slides) {
    sourceTexts.push(slide.title, slide.subtitle || "", slide.notes || "");
  }
  const badBreak = sourceTexts.filter((s) => /(^|\n)[。；，、：！？]/.test(s));
  if (badBreak.length > 0) {
    throw new Error("存在以标点开头的手工换行，请先修正。");
  }
}

async function buildDeck() {
  validateSlideTexts();
  slides.forEach((entry) => {
    const slide = pptx.addSlide();
    slide.background = { color: C.white };
    entry.draw(slide);
    slide.addNotes(entry.notes);
  });
  await pptx.writeFile({ fileName: outPath });
  console.log(`Wrote ${outPath}`);
}

buildDeck().catch((error) => {
  console.error(error);
  process.exit(1);
});
