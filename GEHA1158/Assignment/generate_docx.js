const fs = require('fs');
const { Document, Packer, Paragraph, TextRun, ImageRun, HeadingLevel, AlignmentType, WidthType } = require('docx');

// Helper to create paragraphs
function createParagraph(text, indent = true) {
    return new Paragraph({
        children: [new TextRun({ text: text, font: "SimSun", size: 24 })],
        alignment: AlignmentType.JUSTIFIED,
        indent: indent ? { firstLine: 480 } : undefined,
        spacing: { line: 360 }
    });
}

function createHeading1(text) {
    return new Paragraph({
        heading: HeadingLevel.HEADING_1,
        children: [new TextRun({ text: text, font: "SimHei", size: 32, bold: true })],
        spacing: { before: 240, after: 240 },
        outlineLevel: 0
    });
}

function createHeading2(text) {
    return new Paragraph({
        heading: HeadingLevel.HEADING_2,
        children: [new TextRun({ text: text, font: "SimHei", size: 28, bold: true })],
        spacing: { before: 180, after: 180 },
        outlineLevel: 1
    });
}

function createImage(imagePath, width, height) {
    return new Paragraph({
        alignment: AlignmentType.CENTER,
        children: [
            new ImageRun({
                type: "png",
                data: fs.readFileSync(imagePath),
                transformation: { width: width, height: height },
                altText: { title: "Figure", description: "Figure", name: "Figure" }
            })
        ],
        spacing: { before: 120, after: 120 }
    });
}

function createCaption(text) {
    return new Paragraph({
        alignment: AlignmentType.CENTER,
        children: [new TextRun({ text: text, font: "SimSun", size: 20 })],
        spacing: { before: 0, after: 240 }
    });
}

const doc = new Document({
    styles: {
        default: { document: { run: { font: "SimSun", size: 24 } } },
        paragraphStyles: [
            { id: "Heading1", name: "Heading 1", basedOn: "Normal", next: "Normal", quickFormat: true,
              run: { size: 32, bold: true, font: "SimHei" },
              paragraph: { spacing: { before: 240, after: 240 }, outlineLevel: 0 } },
            { id: "Heading2", name: "Heading 2", basedOn: "Normal", next: "Normal", quickFormat: true,
              run: { size: 28, bold: true, font: "SimHei" },
              paragraph: { spacing: { before: 180, after: 180 }, outlineLevel: 1 } },
        ]
    },
    sections: [{
        properties: {
            page: {
                size: { width: 11906, height: 16838 }, // A4
                margin: { top: 1440, right: 1440, bottom: 1440, left: 1440 }
            }
        },
        children: [
            new Paragraph({
                alignment: AlignmentType.CENTER,
                children: [new TextRun({ text: "规训与启蒙：青年数字文化在中国高校通识教育中的话语审视", font: "SimHei", size: 40, bold: true })],
                spacing: { before: 240, after: 480 }
            }),
            
            createHeading1("1. 引言"),
            createParagraph("近年来，随着数字技术的普及与青年群体的代际更迭，青年数字文化（如游戏文化、二次元文化、创客文化等）已成为大学生日常生活中不可或缺的组成部分。然而，在当前中国高校的通识教育体系中，这类非主流文化往往处于边缘化甚至被排斥的地位。通识教育本应致力于培养具有批判性思维、广阔视野和健全人格的现代公民，但其在实践中却常常表现出对传统经典和主流文化的过度偏好，从而在无形中形成了一种文化规训。"),
            createParagraph("现有研究多从思想政治教育或学生管理的角度探讨青年亚文化，将其视为需要被“引导”或“规范”的对象（韩建萌，2022；管桂翠，2025）。相对而言，从通识教育的文化包容性与批判性启蒙视角出发，审视青年数字文化在教育场域中遭遇的排斥与规训的研究仍显不足。本文旨在通过文献计量分析，系统梳理国内外关于青年数字文化与高等教育的研究脉络，揭示中国高校通识教育中非主流文化缺位的深层原因，并探讨如何通过批判性教育学视角，实现从“文化规训”向“批判性启蒙”的转变。"),

            createHeading1("2. 概念界定：青年数字文化"),
            createParagraph("本文所探讨的“青年数字文化”，涵盖了当代大学生群体中具有广泛影响力的数字文化实践。这不仅包括以动画、漫画、游戏（ACG）为代表的二次元文化和强调技术共享的创客（Geek/Maker）文化，也广泛涉及社交媒体互动、短视频创作与网络流行语等日常数字表达。本文以ACG和创客文化作为典型案例进行深入剖析，旨在揭示这些带有亚文化色彩的实践在教育场域中的境遇。"),
            createParagraph("这些文化形态不仅是青年群体娱乐休闲的方式，更是他们进行身份认同、情感表达和社会交往的重要媒介。然而，在传统的教育评价体系中，这些文化往往被贴上“不务正业”或“沉迷”的标签，难以进入通识教育的正式课程体系。这种文化区隔不仅限制了通识教育的视野，也削弱了其与学生真实生活经验的联系。"),

            createHeading1("3. 通识教育的经典化与文化资本的再生产"),
            createParagraph("中国高校的通识教育经历了从建国初期的苏联模式（强调专业化与政治教育）到近年来向博雅教育（Liberal Arts）回归的演变（Jiang, 2024；Sargent & Xiao, 2018）。在这一过程中，通识教育逐渐确立了以中外经典文献、传统文化和科学精神为核心的课程体系。这种“经典化”的倾向虽然有助于传承人类优秀文化遗产，但也隐含着一种文化霸权。"),
            createParagraph("借用布迪厄（Bourdieu）的文化资本理论，通识教育在传递主流文化的同时，也在进行着文化资本的再生产（陈泽星，2024；徐瑞 & 郭兴举，2011）。然而，数字时代的“数字资本”在获取门槛、传播方式和评价机制上与传统精英文化资本存在本质差异，其去中心化特征对传统教育的权威性构成了挑战。诚然，任何教育系统都必然包含一定程度的规范设定和价值引导，规训并非完全负面；但当通识教育对非主流文化采取系统性排斥时，这种必要的规范便异化为一种隐蔽的文化霸权，通过确立“何为有价值的知识”的单一标准，压抑了数字文化资本的多元潜力。"),

            createHeading1("4. 国内外研究话语的分化（基于文献计量的实证分析）"),
            createParagraph("为了更客观地揭示青年数字文化在高等教育场域中的境遇，本文采用文献计量学方法，对国内外相关研究进行了系统梳理与对比分析。实验过程与数据分析是本文立论的重要支撑。"),

            createHeading2("4.1 研究方法与数据收集"),
            createParagraph("本研究的数据来源于两个主要的学术数据库：代表国际研究前沿的Web of Science (WoS) 核心合集，以及代表国内研究现状的中国知网 (CNKI)。"),
            createParagraph("在数据收集阶段，针对WoS数据库，我们构建了检索式：TS=(\"digital culture\" OR \"youth culture\" OR \"subculture\" OR \"gaming\" OR \"ACG\") AND TS=(\"higher education\" OR \"general education\" OR \"undergraduate\")。经过筛选，共获取相关文献82篇。针对CNKI数据库，我们采用类似的中文检索词（如“网络亚文化”、“大学生”、“通识教育”等）进行高级检索，获取有效文献135篇。"),
            createParagraph("在数据处理与分析阶段，我们利用自然语言处理技术对文献进行了词频统计与主题聚类。特别地，我们构建了“管理规训”与“教学创新”两个自定义话语词典来量化分析价值取向差异。需要承认的是，自定义词典具有一定主观性，未来研究可引入多位编码员进行一致性检验以提升效度。此外，词频统计难以完全捕捉深层语义，后续研究应结合批判性话语分析（CDA）以验证关键词在具体语境中的实际含义。同时，受限于CNKI有效文献总数（135篇），本研究的结论主要反映当前特定数据库内的学术话语倾向，在向宏观教育现状外推时需保持谨慎。"),

            createHeading2("4.2 文献计量发现"),
            createParagraph("通过对提取的关键词进行词频分析，我们发现了国内外研究在关注点上的显著差异。"),
            
            createImage("figures/wos_keywords_top20.png", 600, 400),
            createCaption("图1：Web of Science 数据库高频关键词 Top 20"),
            
            createParagraph("如图1所示，在WoS数据库中，高频关键词主要集中在“digital culture”（12次）、“education”（12次）、“higher education”（10次）、“popular culture”（7次）以及“digital transformation”（5次）。值得注意的是，“pedagogy”（3次）、“critical thinking”（2次）和“media literacy”（2次）等词汇的出现，表明国际研究更倾向于将数字文化视为一种教育资源或教学媒介，探讨其在培养学生批判性思维和媒介素养方面的潜力（Clinton-Lisell, 2025；Jubas, 2023）。"),

            createImage("figures/cnki_keywords_top20.png", 600, 400),
            createCaption("图2：CNKI 数据库高频关键词 Top 20"),

            createParagraph("相比之下，图2展示了CNKI数据库的高频关键词分布。国内研究的核心词汇高度集中于“网络亚文化”（71次）、“大学生”（50次）和“思想政治教育”（30次）。此外，“对策”（10次）、“负面影响”（4次）等词汇的频繁出现，反映出国内学界更多地将青年数字文化视为一种需要被审视、管理和引导的外部冲击，而非通识教育的内在组成部分。"),

            createImage("figures/wos_categories_top10.png", 600, 400),
            createCaption("图3：Web of Science 数据库学科分类 Top 10"),

            createParagraph("从学科分布来看（图3），WoS文献主要集中在“Education & Educational Research”（46篇）和“Education Scientific Disciplines”（13篇），同时也有大量文献分布在计算机科学的跨学科应用（12篇）和人工智能（7篇）领域。这表明国际上对该议题的研究具有较强的跨学科特征，注重技术与教育的深度融合。而CNKI的数据显示，国内研究主要集中在“高等教育”、“新闻与传媒”和“思想政治教育”等领域，跨学科的教学实践研究相对匮乏。"),

            createHeading2("4.3 两种话语范式：管理规训与教学创新"),
            createParagraph("基于自定义话语词典的量化分析，本研究进一步揭示了国内外研究中存在的两种截然不同的话语范式。"),

            createImage("figures/discourse_comparison.png", 600, 400),
            createCaption("图4：国内外研究话语范式对比（管理规训 vs. 教学创新）"),

            createParagraph("如图4所示，在CNKI的文献样本中，“管理规训”话语的出现频率极高（占比达65.4%），而“教学创新”话语的占比仅为0.2%。这印证了前文的观察：国内高校在面对青年数字文化时，往往采取防御性的姿态，强调通过思想政治教育等手段对其进行规范和引导（张蓓，2022；刘治金，2026）。这种话语范式本质上是传统教育权力对新兴文化形态的规训。"),
            createParagraph("相反，在WoS的文献样本中，“管理规训”话语几乎缺位（仅占0.9%），而“教学创新”话语的占比达到13.2%。这表明国际研究中展现出了更多教学创新的视角，更多地探讨如何将游戏化学习（Gamification）、数字叙事等青年文化元素融入高等教育，以激发学生的学习动机，培养其批判性意识（Su Tao, 2025；Alberti, 2021）。这种话语范式体现了对非主流文化的包容与赋权，为实现教育的批判性启蒙提供了参考。"),

            createHeading1("5. 批判性评述与未来研究展望"),
            createParagraph("通过上述文献计量与话语分析，我们可以清晰地看到，中国高校通识教育在面对青年数字文化时，普遍存在着“防范规训”重于“批判性启蒙”的倾向。这种倾向不仅导致了非主流文化在通识教育课程体系中的缺位，也加剧了教育内容与学生真实生活经验的脱节。"),
            createParagraph("通识教育不应仅仅是主流文化资本的传递带，更应成为不同文化形态对话与碰撞的公共空间。未来的研究与实践应当探索如何将青年数字文化有机地融入通识教育之中。需要明确的是，这并非意味着所有亚文化都具有进入课堂的价值，关键在于提取其中的“批判性”或“素养”元素。例如，在课程设计中，教师可以引导学生批判性地分析游戏机制背后的算法偏见，或利用创客文化培养跨学科解决问题的能力。同时，考虑到中国高校在课程设置与教学评估等方面的现实制度约束，这种融合应当是渐进式的。教育者需在现有体制下寻找弹性空间，采取批判性教育学（Critical Pedagogy）的立场（Li Kaiqing, 2022），引导学生在反思自身文化实践的过程中培养数字素养。只有在规范与包容之间找到平衡，通识教育才能真正完成其启蒙的使命。"),

            createHeading1("参考文献"),
            createParagraph("[1] 韩建萌. (2022). 新时代大学生网络亚文化现象的审视与引导. 思想理论教育导刊.", false),
            createParagraph("[2] Jiang, Y. (2024). The evolution of general education in Chinese universities. Higher Education.", false),
            createParagraph("[3] Sargent, T. C., & Xiao, Y. (2018). Liberal arts education in China: A critical review. International Journal of Educational Development.", false),
            createParagraph("[4] 肖蓉, 梁晓波. (2021). 高校通识教育的本土化困境与突破. 高等教育研究.", false),
            createParagraph("[5] 陈泽星. (2024). 文化资本视角下的高校通识教育公平问题研究. 教育发展研究.", false),
            createParagraph("[6] 徐瑞, 郭兴举. (2011). 教育的文化再生产功能探析. 教育研究.", false),
            createParagraph("[7] Li, K. (2022). Critical pedagogy in the digital age. Journal of Critical Education Policy Studies.", false),
            createParagraph("[8] 管桂翠. (2025). 大学生网络亚文化的思政教育应对策略. 学校党建与思想教育.", false),
            createParagraph("[9] 张蓓. (2022). 青年亚文化对高校主流意识形态安全的挑战及对策. 思想教育研究.", false),
            createParagraph("[10] Su, T. (2025). Gamification and critical thinking in higher education. Computers & Education.", false),
            createParagraph("[11] Jubas, K. (2023). Popular culture and critical pedagogy. Adult Education Quarterly.", false),
            createParagraph("[12] Hikmat, A. (2024). Digital culture and student engagement. Journal of Educational Technology.", false),
            createParagraph("[13] Clinton-Lisell, V. (2025). Media literacy in the age of digital culture. Educational Psychologist.", false),
            createParagraph("[14] 刘治金. (2026). 新媒体时代高校网络文化治理研究. 中国高等教育.", false),
            createParagraph("[15] Alberti, J. (2021). Freirean pedagogy and game-based learning. Pedagogy, Culture & Society.", false)
        ]
    }]
});

Packer.toBuffer(doc).then((buffer) => {
    fs.writeFileSync("paper_final.docx", buffer);
    console.log("Document created successfully at paper_final.docx");
}).catch((err) => {
    console.error("Error creating document:", err);
});
