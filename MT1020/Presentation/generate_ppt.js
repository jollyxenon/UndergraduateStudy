const fs = require("fs");
const path = require("path");
const pptxgen = require("pptxgenjs");
const { imageSize } = require("image-size");

const pptx = new pptxgen();
pptx.layout = "LAYOUT_WIDE";
pptx.author = "Codex";
pptx.subject = "铸就社会主义文化新辉煌";
pptx.title = "铸就社会主义文化新辉煌";
pptx.company = "ShanghaiTech University";
pptx.lang = "zh-CN";
pptx.theme = {
  headFontFace: "Microsoft YaHei",
  bodyFontFace: "Microsoft YaHei",
  lang: "zh-CN",
};
pptx.defineLayout({ name: "CUSTOM_WIDE", width: 13.333, height: 7.5 });
pptx.layout = "CUSTOM_WIDE";

const OUT = "MT1020_culture_presentation.pptx";
const W = 13.333;
const H = 7.5;
const C = {
  black: "111111",
  white: "FFFFFF",
  red: "B21D2A",
  redDark: "7E1019",
  gray: "6B6F76",
  light: "F4F4F4",
  line: "D9D9D9",
};

// 为函数统一默认字体、字号和安全的文本框内边距。
function text(slide, value, x, y, w, h, opt = {}) {
  slide.addText(value, {
    x,
    y,
    w,
    h,
    fontFace: "Microsoft YaHei",
    fontSize: opt.fontSize ?? 18,
    color: opt.color ?? C.black,
    bold: opt.bold ?? false,
    breakLine: opt.breakLine ?? false,
    valign: opt.valign ?? "mid",
    align: opt.align ?? "left",
    margin: opt.margin ?? 0.08,
    fit: "shrink",
    paraSpaceAfterPt: opt.paraSpaceAfterPt ?? 0,
    breakLine: opt.breakLine ?? false,
    ...opt,
  });
}

// 统一页眉页脚，保证每页都能看出所属章节。
function chrome(slide, section, page) {
  slide.background = { color: C.white };
  slide.addShape(pptx.ShapeType.rect, { x: 0, y: 0, w: W, h: 0.11, fill: { color: C.red }, line: { color: C.red } });
  text(slide, section, 0.55, 0.22, 7.4, 0.28, { fontSize: 8.5, color: C.gray, bold: true, margin: 0 });
  slide.addShape(pptx.ShapeType.line, { x: 0.55, y: 7.05, w: 11.45, h: 0, line: { color: C.line, width: 0.6 } });
  text(slide, String(page).padStart(2, "0"), 12.1, 6.88, 0.7, 0.28, { fontSize: 8.5, color: C.gray, align: "right", margin: 0 });
}

// 标准标题区域，避免标题因动态长度压到正文。
function title(slide, main, kicker = "") {
  if (kicker) text(slide, kicker, 0.72, 0.82, 3.5, 0.28, { fontSize: 10, color: C.red, bold: true, margin: 0 });
  text(slide, main, 0.68, 1.0, 8.9, 0.7, { fontSize: 28, bold: true, margin: 0 });
  slide.addShape(pptx.ShapeType.line, { x: 0.72, y: 1.82, w: 1.2, h: 0, line: { color: C.red, width: 2.2 } });
}

// 插入图片时按比例完整显示，避免图片越界或被不当拉伸。
function containImage(slide, imagePath, x, y, w, h) {
  const dim = imageSize(fs.readFileSync(imagePath));
  const ratio = Math.min(w / dim.width, h / dim.height);
  const iw = dim.width * ratio;
  const ih = dim.height * ratio;
  slide.addShape(pptx.ShapeType.rect, { x, y, w, h, fill: { color: "F8F8F8" }, line: { color: C.line, width: 0.7 } });
  slide.addImage({ path: imagePath, x: x + (w - iw) / 2, y: y + (h - ih) / 2, w: iw, h: ih });
}

// 章节页使用大号编号和短句，保持课堂切换节奏。
function sectionSlide(no, heading, sub, page) {
  const slide = pptx.addSlide();
  chrome(slide, "章节切换", page);
  text(slide, no, 0.72, 1.25, 1.4, 0.75, { fontSize: 40, bold: true, color: C.red, margin: 0 });
  text(slide, heading, 2.0, 1.27, 9.6, 0.78, { fontSize: 32, bold: true, margin: 0 });
  slide.addShape(pptx.ShapeType.line, { x: 0.75, y: 2.4, w: 11.0, h: 0, line: { color: C.line, width: 1 } });
  text(slide, sub, 0.78, 2.95, 10.7, 1.2, { fontSize: 22, color: C.gray, bold: true, breakLine: true, margin: 0 });
  return slide;
}

// 数据卡片用于突出文稿案例中的关键事实。
function statCard(slide, label, value, x, y, w, h) {
  slide.addShape(pptx.ShapeType.roundRect, { x, y, w, h, rectRadius: 0.08, fill: { color: C.white }, line: { color: C.line, width: 0.8 } });
  text(slide, value, x + 0.12, y + 0.18, w - 0.24, 0.34, { fontSize: 15.5, color: C.red, bold: true, align: "center", margin: 0 });
  text(slide, label, x + 0.18, y + 0.68, w - 0.36, 0.36, { fontSize: 10.5, color: C.gray, margin: 0 });
}

// 案例页统一为左图右文，保证“一页最多一个案例”。
function caseSlide({ page, section, heading, source, image, lead, points, stats = [] }) {
  const slide = pptx.addSlide();
  chrome(slide, section, page);
  title(slide, heading, "案例");
  containImage(slide, path.join("assets", image), 0.72, 2.05, 6.25, 4.55);
  text(slide, source, 0.78, 6.68, 6.0, 0.22, { fontSize: 7.5, color: C.gray, margin: 0 });
  text(slide, lead, 7.36, 2.04, 5.05, 0.72, { fontSize: 21, bold: true, color: C.black, margin: 0.02, breakLine: true });
  points.forEach((p, i) => {
    const y = 2.95 + i * 0.72;
    slide.addShape(pptx.ShapeType.rect, { x: 7.38, y: y + 0.1, w: 0.1, h: 0.3, fill: { color: C.red }, line: { color: C.red } });
    text(slide, p, 7.6, y, 4.75, 0.46, { fontSize: 15, color: C.black, margin: 0, breakLine: true });
  });
  stats.forEach((s, i) => statCard(slide, s.label, s.value, 7.38 + i * 1.72, 5.72, 1.5, 1.05));
}

// 双图案例页用于同一案例内的两个并列视觉证据。
function palaceCaseSlide() {
  const slide = pptx.addSlide();
  chrome(slide, "二、传承中华优秀传统文化", 7);
  title(slide, "故宫 AI 文创：从宫猫到“万吉骦”", "案例");
  containImage(slide, path.join("assets", "palace_ai.png"), 0.72, 2.05, 3.05, 4.08);
  containImage(slide, path.join("assets", "horse_sina_2.jpg"), 3.95, 2.05, 3.05, 4.08);
  text(slide, "宫猫福墩 AI 玩偶", 0.78, 6.22, 2.9, 0.22, { fontSize: 9, color: C.gray, align: "center", margin: 0 });
  text(slide, "“马到成功”AI 玩偶“万吉骦”", 4.0, 6.22, 2.9, 0.22, { fontSize: 9, color: C.gray, align: "center", margin: 0 });
  text(slide, "图源：央视网 / 新浪新闻", 0.78, 6.68, 6.0, 0.22, { fontSize: 7.5, color: C.gray, margin: 0 });
  text(slide, "同一逻辑：\n传统符号转为智能交互产品", 7.36, 2.04, 5.05, 0.72, { fontSize: 21, bold: true, color: C.black, margin: 0.02, breakLine: true });
  [
    "宫猫福墩：以宫猫为原型，承载双语对话、文化讲解与情绪陪伴",
    "“马到成功”AI 玩偶：以《万吉骦轴》为原型，让古画进入文创场景",
    "从静态藏品到可互动产品，完成创造性转化",
    "从图文授权到 AI 文创，体现创新性发展",
  ].forEach((p, i) => {
    const y = 2.95 + i * 0.72;
    slide.addShape(pptx.ShapeType.rect, { x: 7.38, y: y + 0.1, w: 0.1, h: 0.3, fill: { color: C.red }, line: { color: C.red } });
    text(slide, p, 7.6, y, 4.75, 0.46, { fontSize: 15, color: C.black, margin: 0, breakLine: true });
  });
  [
    { value: "AI+IP", label: "科技赋能文创" },
    { value: "古画原型", label: "《万吉骦轴》" },
    { value: "文化出海", label: "现代表达传播" },
  ].forEach((s, i) => statCard(slide, s.label, s.value, 7.38 + i * 1.72, 5.72, 1.5, 1.05));
}

// 新增国际传播案例页：两个媒介产品并列，强调真实记录与去中介化表达。
function addEverydayChinaCase() {
  const slide = pptx.addSlide();
  chrome(slide, "四、提升国家文化软实力和中华文化影响力", 21);
  title(slide, "真实记录：让多元主体共同讲中国故事", "案例");
  containImage(slide, path.join("assets", "solar_terms_poster.jpg"), 0.72, 2.03, 3.0, 4.65);
  containImage(slide, path.join("assets", "cycling_zhongshan.jpg"), 3.98, 2.03, 3.05, 4.65);
  text(slide, "二十四节气开机海报", 0.78, 6.72, 2.9, 0.2, { fontSize: 8, color: C.gray, align: "center", margin: 0 });
  text(slide, "《从中山出发，骑行看中国》", 3.98, 6.72, 3.05, 0.2, { fontSize: 8, color: C.gray, align: "center", margin: 0 });
  text(slide, "图源：云南日报网 / 中山网", 0.78, 6.93, 6.0, 0.12, { fontSize: 6.6, color: C.gray, margin: 0 });
  text(slide, "不是宏大叙事，\n而是可感、可见、可转发的中国", 7.36, 2.04, 5.05, 0.72, { fontSize: 20, bold: true, color: C.black, margin: 0.02, breakLine: true });
  [
    "二十四节气海报：把东方时间美学转化成海外用户能直接理解的视觉语言",
    "骑行看中国：以个人旅程记录城市、乡村与边境，降低国际受众理解门槛",
    "两者共同点：用具体生活场景替代抽象口号",
    "案例意义：让多元主体共同参与中国形象的建构",
  ].forEach((p, i) => {
    const y = 2.95 + i * 0.72;
    slide.addShape(pptx.ShapeType.rect, { x: 7.38, y: y + 0.1, w: 0.1, h: 0.3, fill: { color: C.red }, line: { color: C.red } });
    text(slide, p, 7.6, y, 4.75, 0.46, { fontSize: 14.4, color: C.black, margin: 0, breakLine: true });
  });
  [
    { value: "24节气", label: "东方美学视觉化" },
    { value: "2811km", label: "骑行路线长度" },
    { value: "去中介化", label: "真实记录传播" },
  ].forEach((s, i) => statCard(slide, s.label, s.value, 7.38 + i * 1.72, 5.72, 1.5, 1.05));
}

// 普通理论页按原文逻辑给出“少文字、大关系”。
function theorySlide({ page, section, heading, kicker, blocks, foot }) {
  const slide = pptx.addSlide();
  chrome(slide, section, page);
  title(slide, heading, kicker);
  blocks.forEach((b, i) => {
    const x = 0.78 + (i % 2) * 6.05;
    const y = 2.2 + Math.floor(i / 2) * 1.55;
    slide.addShape(pptx.ShapeType.roundRect, { x, y, w: 5.45, h: 1.1, rectRadius: 0.08, fill: { color: C.light }, line: { color: C.line, width: 0.6 } });
    text(slide, b.h, x + 0.24, y + 0.15, 5.0, 0.28, { fontSize: 17, bold: true, color: C.black, margin: 0 });
    text(slide, b.t, x + 0.24, y + 0.5, 4.95, 0.38, { fontSize: 11.8, color: C.gray, margin: 0, breakLine: true });
  });
  if (foot) text(slide, foot, 0.82, 6.55, 11.3, 0.48, { fontSize: 15, bold: true, color: C.red, margin: 0 });
}

// 首页突出主题，不承载过多正文。
function addCover() {
  const slide = pptx.addSlide();
  slide.background = { color: C.white };
  slide.addShape(pptx.ShapeType.rect, { x: 0, y: 0, w: W, h: 0.16, fill: { color: C.red }, line: { color: C.red } });
  text(slide, "MT1020 课堂汇报", 0.78, 0.78, 3.0, 0.3, { fontSize: 11, color: C.gray, bold: true, margin: 0 });
  text(slide, "铸就社会主义\n文化新辉煌", 0.78, 1.45, 7.5, 1.65, { fontSize: 42, bold: true, color: C.black, margin: 0, breakLine: true });
  slide.addShape(pptx.ShapeType.line, { x: 0.82, y: 3.45, w: 1.35, h: 0, line: { color: C.red, width: 3 } });
  text(slide, "从中华文明突出特性出发\n以典型案例说明新时代文化建设路径", 0.82, 3.85, 7.0, 0.82, { fontSize: 18, color: C.gray, margin: 0, breakLine: true });
  ["中华文明特性", "传统文化传承", "文化事业产业", "文化软实力"].forEach((v, i) => {
    slide.addShape(pptx.ShapeType.roundRect, { x: 0.82 + i * 2.25, y: 5.45, w: 1.85, h: 0.55, rectRadius: 0.08, fill: { color: C.light }, line: { color: C.line } });
    text(slide, v, 0.92 + i * 2.25, 5.61, 1.65, 0.18, { fontSize: 10, align: "center", color: i === 0 ? C.red : C.black, bold: true, margin: 0 });
  });
}

// 第二页用原文稿条理作为目录，强调严格按文稿展开。
function addRoadmap() {
  const slide = pptx.addSlide();
  chrome(slide, "开场：按原文稿条理展开", 2);
  title(slide, "从“五大特性”到“三条实践路径”", "汇报路线");
  const items = [
    ["一", "中华文明的突出特性", "连续性、创新性、统一性、包容性、和平性"],
    ["二", "传承中华优秀传统文化", "创造性转化、创新性发展、文化遗产保护"],
    ["三", "繁荣文化事业与文化产业", "文艺使命、公共文化服务、文旅融合"],
    ["四", "提升国家文化软实力", "落差意识、讲好中国故事、国际传播能力"],
    ["五", "总结", "坚定文化自信，铸就文化新辉煌"],
  ];
  items.forEach((it, i) => {
    const y = 2.05 + i * 0.92;
    text(slide, it[0], 0.9, y, 0.35, 0.34, { fontSize: 16, color: C.red, bold: true, margin: 0 });
    text(slide, it[1], 1.45, y - 0.02, 3.7, 0.36, { fontSize: 16.5, bold: true, margin: 0 });
    text(slide, it[2], 5.35, y, 6.3, 0.32, { fontSize: 13, color: C.gray, margin: 0 });
    slide.addShape(pptx.ShapeType.line, { x: 0.9, y: y + 0.55, w: 10.9, h: 0, line: { color: C.line, width: 0.5 } });
  });
}

// 五大突出特性以五个短定义呈现，承接原文总起。
function addFiveTraits() {
  const slide = pptx.addSlide();
  chrome(slide, "一、中华文明的突出特性", 3);
  title(slide, "文化自信的底气：五大突出特性", "理论基础");
  const traits = [
    ["连续性", "决定中华民族必然走自己的路"],
    ["创新性", "守正不守旧，尊古不复古"],
    ["统一性", "国土不可分、国家不可乱、文明不可断"],
    ["包容性", "交往交流交融，兼收并蓄"],
    ["和平性", "不搞文化霸权，不强加价值观"],
  ];
  traits.forEach((t, i) => {
    const x = 0.78 + (i % 3) * 4.05;
    const y = 2.28 + Math.floor(i / 3) * 1.55;
    slide.addShape(pptx.ShapeType.roundRect, { x, y, w: 3.55, h: 1.1, rectRadius: 0.08, fill: { color: C.light }, line: { color: C.line } });
    text(slide, t[0], x + 0.18, y + 0.16, 3.1, 0.3, { fontSize: 20, bold: true, color: C.black, margin: 0 });
    text(slide, t[1], x + 0.18, y + 0.58, 3.08, 0.32, { fontSize: 10.5, color: C.gray, margin: 0, breakLine: true });
  });
  text(slide, "这些特性塑造中华民族精神品格，也是新时代文化建设的根本遵循。", 0.82, 6.25, 10.6, 0.38, { fontSize: 16.5, color: C.red, bold: true, margin: 0 });
}

// 过渡页把原文中的三大展开方向压成清晰路径。
function addTransition() {
  const slide = pptx.addSlide();
  chrome(slide, "一、中华文明的突出特性", 4);
  title(slide, "接下来：从三条路径展开案例", "由理论进入实践");
  const steps = [
    ["传承中华优秀传统文化", "让古老文明在新时代焕发新生"],
    ["繁荣文化事业与文化产业", "满足人民精神需求，增强文化整体实力"],
    ["提升国家文化软实力", "塑造可信、可爱、可敬的中国形象"],
  ];
  steps.forEach((s, i) => {
    const x = 1.0 + i * 4.05;
    slide.addShape(pptx.ShapeType.line, { x: x + 0.25, y: 3.1, w: 3.0, h: 0, line: { color: C.line, width: 1.1 } });
    slide.addShape(pptx.ShapeType.ellipse, { x, y: 2.45, w: 0.78, h: 0.78, fill: { color: C.red }, line: { color: C.red } });
    text(slide, `${i + 1}`, x + 0.2, 2.55, 0.36, 0.34, { fontSize: 24, color: C.white, bold: true, margin: 0, align: "center" });
    text(slide, s[0], x, 3.45, 3.35, 0.55, { fontSize: 18, bold: true, align: "center", margin: 0, breakLine: true });
    text(slide, s[1], x, 4.22, 3.35, 0.52, { fontSize: 12.5, color: C.gray, align: "center", margin: 0, breakLine: true });
  });
}

addCover();
addRoadmap();
addFiveTraits();
addTransition();

sectionSlide("二", "传承中华优秀传统文化", "从“根和魂”出发：创造性转化、创新性发展、文化遗产保护", 5);

theorySlide({
  page: 6,
  section: "二、传承中华优秀传统文化",
  heading: "创造性转化 + 创新性发展",
  kicker: "理论压缩",
  blocks: [
    { h: "创造性转化", t: "按时代特点改造仍有价值的内涵与旧形式", red: true },
    { h: "创新性发展", t: "补充、拓展、完善传统文化内涵" },
    { h: "目标", t: "让文化基因同当代中国、现代社会、现实文化相融通" },
    { h: "边界", t: "传承不是故步自封，而是开放交流、取长补短" },
  ],
  foot: "关键判断：守正不守旧，尊古不复古",
});

palaceCaseSlide();

caseSlide({
  page: 8,
  section: "二、传承中华优秀传统文化",
  heading: "数字敦煌：让脆弱文物获得“数字永生”",
  source: "图源：搜狐 / 数字敦煌报道",
  image: "dunhuang.jpeg",
  lead: "保护优先\n用科技提高文化遗产保护水平",
  points: [
    "壁画数字化、彩塑与大遗址三维重建同步推进",
    "高精度石窟数字档案把不可逆损耗转化为可长期保存的数据",
    "云端共享让全球观众不远赴戈壁也能观看千年文明",
    "案例意义：保护、传承、共享在同一套数字工程中统一",
  ],
  stats: [
    { value: "600TB+", label: "项目数据量" },
    { value: "三维重建", label: "彩塑与遗址" },
    { value: "全球共享", label: "文化遗产云端传播" },
  ],
});

sectionSlide("三", "繁荣文化事业与文化产业", "既满足人民精神需求，也增强文化整体实力", 9);

theorySlide({
  page: 10,
  section: "三、繁荣文化事业与文化产业",
  heading: "文艺的使命与导向",
  kicker: "社会主义文艺是人民的文艺",
  blocks: [
    { h: "灵魂", t: "中国精神是社会主义文艺的灵魂", red: true },
    { h: "使命", t: "举精神旗帜、立精神支柱、建精神家园" },
    { h: "创作标准", t: "思想性、艺术性、观赏性有机统一" },
    { h: "落点", t: "满足人民精神文化需求，增强人民精神力量" },
  ],
  foot: "文艺不是单纯娱乐，而是民族精神的表达方式",
});

caseSlide({
  page: 11,
  section: "三、繁荣文化事业与文化产业",
  heading: "“剧美中国”：把中国精神故事化",
  source: "图源：国家广播电视总局",
  image: "juemei.jpg",
  lead: "精品电视剧片单\n让主流价值进入具体故事",
  points: [
    "国家广电总局联合多部门启动精品创作计划",
    "从优秀传统文化、革命文化、社会主义先进文化中提炼题材",
    "把中国精神具象化、情感化，降低观众理解门槛",
    "案例意义：以人民可感的作品凝聚中国价值与中国力量",
  ],
  stats: [
    { value: "2025.10", label: "计划启动" },
    { value: "近30部", label: "首批精品剧片单" },
    { value: "三类文化", label: "传统 / 革命 / 先进" },
  ],
});

theorySlide({
  page: 12,
  section: "三、繁荣文化事业与文化产业",
  heading: "公共文化服务：把文化成果送到基层",
  kicker: "保障人民文化权益",
  blocks: [
    { h: "目标", t: "改善人民生活品质，补齐文化发展短板", red: true },
    { h: "原则", t: "政府主导、社会参与、重心下移、共建共享" },
    { h: "重点", t: "城乡一体化、数字化战略、文化惠民工程" },
    { h: "效果", t: "缩小城乡差距，让文化成果公平惠及全体人民" },
  ],
  foot: "公共文化的关键，不是“有活动”，而是“到达人民”",
});

caseSlide({
  page: 13,
  section: "三、繁荣文化事业与文化产业",
  heading: "上海市民文化节：破解“最后一公里”",
  source: "图源：上观新闻",
  image: "shanghai_culture.jpg",
  lead: "一年 365 天不落幕\n公共文化配送直达基层",
  points: [
    "线上线下活动全年覆盖，形成高频公共文化供给",
    "配送活动、文艺辅导、线上内容进入社区、乡镇和基层末梢",
    "文化服务从“集中展示”转向“精准抵达”",
    "案例意义：以人民为中心的公共文化服务能激发基层文化活力",
  ],
  stats: [
    { value: "4万+", label: "全年活动数量" },
    { value: "近2000万", label: "参与人次" },
    { value: "9.3万场", label: "公共文化配送" },
  ],
});

theorySlide({
  page: 14,
  section: "三、繁荣文化事业与文化产业",
  heading: "文化产业发展：社会效益与经济效益统一",
  kicker: "以文塑旅，以旅彰文",
  blocks: [
    { h: "衡量标准", t: "能否提供更多满足人民文化需求的产品", red: true },
    { h: "发展趋势", t: "数字产业化、产业数字化，新型文化业态" },
    { h: "融合路径", t: "文化内涵成为旅游体验的灵魂" },
    { h: "传播效果", t: "旅游载体成为文化传播的通道" },
  ],
  foot: "文旅融合的重点，是让游客在体验中理解文化",
});

caseSlide({
  page: 15,
  section: "三、繁荣文化事业与文化产业",
  heading: "“天下嘉峪关”夜游：长城文化的沉浸式转化",
  source: "图源：国际在线 / 澎湃新闻相关报道",
  image: "jiayuguan_paper_1.jpg",
  lead: "数字技术 + 长城原址\n把历史场景变为夜游体验",
  points: [
    "项目以嘉峪关关城为天然舞台，融合激光投影、裸眼 3D、实景演艺",
    "四大篇章串联边关、河西、长城、中国脊梁等历史叙事",
    "入选 2025 中国旅游产业影响力案例",
    "案例意义：文化内涵成为旅游体验的核心，而非附属装饰",
  ],
  stats: [
    { value: "4171万", label: "项目总投资" },
    { value: "21.16万", label: "累计接待游客" },
    { value: "2025", label: "影响力案例" },
  ],
});

sectionSlide("四", "提升国家文化软实力和中华文化影响力", "面对逆差、反差、落差：用真实案例塑造中国形象", 16);

theorySlide({
  page: 17,
  section: "四、提升国家文化软实力和中华文化影响力",
  heading: "软实力的重要性及现存“落差”",
  kicker: "软实力与硬实力相得益彰",
  blocks: [
    { h: "逆差", t: "信息流进流出存在不平衡", red: true },
    { h: "反差", t: "中国真实形象与西方主观印象不一致" },
    { h: "落差", t: "软实力与硬实力还不完全匹配" },
    { h: "任务", t: "提升文化传播力、文明影响力和国际影响力" },
  ],
  foot: "软实力建设不是口号，而要靠可被国际理解的实践成果",
});

caseSlide({
  page: 18,
  section: "四、提升国家文化软实力和中华文化影响力",
  heading: "字节跳动古籍保护：数字保护获得国际认可",
  source: "图源：字节跳动公益 / UNESCO ICCSD",
  image: "unesco_case.png",
  lead: "数字化 + 活化利用\n让古籍保护进入国际示范案例",
  points: [
    "项目入选“2025 数字环境下保护与促进文化表现形式多样性示范案例”",
    "“识典古籍”平台用智能技术提升古籍数字化整理效率",
    "公益、技术、传播结合，降低古籍知识获取门槛",
    "案例意义：以创新成果主动参与全球文化治理",
  ],
  stats: [
    { value: "9800+", label: "已整理古籍数量" },
    { value: "40项", label: "全球示范案例" },
    { value: "2026.3", label: "论坛发布" },
  ],
});

theorySlide({
  page: 19,
  section: "四、提升国家文化软实力和中华文化影响力",
  heading: "讲好中国故事：真实、立体、全面",
  kicker: "核心价值观的生命力与感召力",
  blocks: [
    { h: "立场", t: "坚守中华文化立场，提炼精神标识和文化精髓", red: true },
    { h: "内容", t: "全面阐述发展观、文明观、安全观、人权观、生态观" },
    { h: "方式", t: "让多元主体共同参与中国形象建构" },
    { h: "形象", t: "努力塑造可信、可爱、可敬的中国形象" },
  ],
  foot: "好的国际传播，是让海外观众在真实相遇中建立理解",
});

caseSlide({
  page: 20,
  section: "四、提升国家文化软实力和中华文化影响力",
  heading: "普宁英歌赴莫斯科：从“送出去”到“走进去”",
  source: "图源：南方+",
  image: "yingge_moscow_2.png",
  lead: "冰雪莫斯科 + 英歌气势\n形成强烈视觉与情感反差",
  points: [
    "2026 年农历除夕，广东普宁富美青年英歌队赴俄罗斯莫斯科表演",
    "传统英歌的力量感与莫斯科冬夜形成跨文化现场冲击",
    "海外观众在真实表演中感受中华文化生命力",
    "案例意义：文明对话不是静态展品输出，而是主动进入对方场景",
  ],
  stats: [
    { value: "2026", label: "农历除夕" },
    { value: "莫斯科", label: "海外现场" },
    { value: "英歌", label: "非遗文化表达" },
  ],
});

addEverydayChinaCase();

theorySlide({
  page: 22,
  section: "四、提升国家文化软实力和中华文化影响力",
  heading: "国际传播能力：科技赋能，构建中国话语体系",
  kicker: "传播好中国声音",
  blocks: [
    { h: "根本目标", t: "形成同综合国力和国际地位相匹配的国际话语权", red: true },
    { h: "传播理念", t: "以我为主，以理服人，以情动人" },
    { h: "能力建设", t: "提升影响力、感召力、亲和力、说服力、引导力" },
    { h: "传播策略", t: "全球化表达、区域化表达、分众化表达" },
  ],
  foot: "科技赋能的重点，是构建融通中外的叙事体系",
});

// 总结页回扣原文最后一节，并给出青年行动落点。
function addSummary() {
  const slide = pptx.addSlide();
  chrome(slide, "五、总结：坚定文化自信，铸就文化新辉煌", 23);
  title(slide, "从文化自信走向文化新辉煌", "总结");
  text(slide, "五大突出特性", 0.9, 2.1, 2.2, 0.35, { fontSize: 19, color: C.red, bold: true, margin: 0 });
  text(slide, "连续性 / 创新性 / 统一性 / 包容性 / 和平性", 3.2, 2.12, 7.5, 0.32, { fontSize: 16, color: C.black, bold: true, margin: 0 });
  slide.addShape(pptx.ShapeType.line, { x: 1.05, y: 3.08, w: 10.5, h: 0, line: { color: C.line, width: 1 } });
  const actions = [
    ["传承者", "传承中华文脉"],
    ["建设者", "参与社会主义文化建设"],
    ["传播者", "讲好真实、立体、全面的中国故事"],
  ];
  actions.forEach((a, i) => {
    slide.addShape(pptx.ShapeType.roundRect, { x: 1.05 + i * 3.8, y: 3.65, w: 3.05, h: 1.35, rectRadius: 0.08, fill: { color: C.light }, line: { color: C.line } });
    text(slide, a[0], 1.27 + i * 3.8, 3.98, 2.6, 0.32, { fontSize: 21, bold: true, color: C.black, align: "center", margin: 0 });
    text(slide, a[1], 1.25 + i * 3.8, 4.45, 2.6, 0.32, { fontSize: 11.5, color: C.gray, align: "center", margin: 0 });
  });
}

addSummary();

// 结束页单独承载致谢，避免总结页信息层级混杂。
function addThanks() {
  const slide = pptx.addSlide();
  slide.background = { color: C.white };
  slide.addShape(pptx.ShapeType.rect, { x: 0, y: 0, w: W, h: 0.14, fill: { color: C.red }, line: { color: C.red } });
  text(slide, "谢谢大家", 0, 2.85, W, 0.8, { fontSize: 44, bold: true, color: C.black, align: "center", margin: 0 });
  slide.addShape(pptx.ShapeType.line, { x: 5.85, y: 3.95, w: 1.6, h: 0, line: { color: C.red, width: 3 } });
}

addThanks();

pptx.writeFile({ fileName: OUT }).then(() => {
  console.log(`Generated ${OUT}`);
});
