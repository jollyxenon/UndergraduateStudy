from copy import deepcopy
from pathlib import Path
from zipfile import ZIP_DEFLATED, ZipFile
import xml.etree.ElementTree as ET

SOURCE = Path(r"D:\UndergraduateStudy\MT1021\Personal_Report\个人报告\延安社会实践个人报告_初稿.docx")
OUTPUT = Path(r"D:\UndergraduateStudy\MT1021\Personal_Report\个人报告\延安社会实践个人报告_修订版.docx")
W_NS = "http://schemas.openxmlformats.org/wordprocessingml/2006/main"
XML_NS = "http://www.w3.org/XML/1998/namespace"
NS = {"w": W_NS}
ET.register_namespace("w", W_NS)


def paragraph_text(paragraph):
    return "".join(node.text or "" for node in paragraph.findall(".//w:t", NS))


def set_paragraph_text(paragraph, value):
    nodes = paragraph.findall(".//w:t", NS)
    if not nodes:
        run = ET.SubElement(paragraph, f"{{{W_NS}}}r")
        nodes = [ET.SubElement(run, f"{{{W_NS}}}t")]
    nodes[0].text = value
    nodes[0].set(f"{{{XML_NS}}}space", "preserve")
    for node in nodes[1:]:
        node.text = ""


def find_one(paragraphs, marker):
    matches = [p for p in paragraphs if marker in paragraph_text(p)]
    if len(matches) != 1:
        raise RuntimeError(f"Expected one paragraph containing {marker!r}, found {len(matches)}")
    return matches[0]


with ZipFile(SOURCE, "r") as source_zip:
    document_xml = source_zip.read("word/document.xml")
    root = ET.fromstring(document_xml)
    body = root.find(".//w:body", NS)
    if body is None:
        raise RuntimeError("Document body not found")

    paragraphs = body.findall("w:p", NS)

    survey = find_one(paragraphs, "这段经历也磨砺了我的胆量")
    set_paragraph_text(
        survey,
        "问卷调查还让我注意到，实践中的问题有时不在“敢不敢开口”，而在团队是否真正形成了有效分工。最初我们六人一组发放问卷，但每次真正走上前询问路人的往往只有我，其他同学则围在旁边。我觉得这种局面既奇异又有些好笑：六个人聚在一起，看似声势很大，实际效率反而不高，还可能给受访者造成压力。后来我在会上直接提出这个问题，把自己在街头搭话时总结的“不要脸经验”分享给大家：先自然说明身份和来意，被拒绝也不纠缠，更不必把一次拒绝当成难堪；同时建议把调查组缩小到三人以下，让每个人都必须承担提问、记录或观察的任务。调整之后，问卷发放效率果然提高了很多。这让我认识到，实践中的主动性不只是自己愿意多做，也包括看见协作失灵后敢于指出，并推动团队改变做法。",
    )

    diplomacy = deepcopy(survey)
    set_paragraph_text(
        diplomacy,
        "实践中期，我又承担了一项更偏向组织协调的工作。行程中常会出现一些看似细小、却足以打乱节奏的问题，比如临时找不到开会教室，或与原定参访地点没有对接好。组长因此让我牵头成立一个被大家称作“外交部”的小组，专门负责打电话、确认场地和处理临时沟通。我需要快速判断该联系谁、怎样把情况说明白，以及如何把结果同步给队友。这个机制建立后，许多原本会拖住全队的小问题得以及时解决，后续行程顺利了很多。我也由此体会到，团队运转并不只依赖宏观计划，还依赖有人愿意接住琐碎却关键的沟通工作。",
    )
    survey_index = list(body).index(survey)
    body.insert(survey_index + 1, diplomacy)

    paragraphs = body.findall("w:p", NS)
    recap = find_one(paragraphs, "回顾这次实践，我的收获并不是得到几个关于延安的固定结论")
    set_paragraph_text(
        recap,
        "回顾这次实践，我的收获并不是得到几个关于延安的固定结论，而是形成了一种更具体的认识方式。在窑洞里，我理解了信念需要在困难中落实为行动；在街头采访中，我学会放下预设、从人民的经验中寻找问题；在调整问卷分组和“外交部”的工作中，我学会发现协作问题、提出方案并承担协调责任；在南泥湾的纺车旁，我重新认识劳动的重量；在思考苹果销售和产业转型时，我也开始把自己的专业学习与真实社会需求联系起来。",
    )

    suggestions = find_one(paragraphs, "对项目今后的开展，我有三点建议")
    set_paragraph_text(
        suggestions,
        "对项目今后的开展，我有三点建议。第一，在出发前增加调查方法训练，并控制调查小组规模，明确提问、记录和观察等职责，避免出现多人围观、一人工作的情况。第二，建立稳定的对外联络机制，提前确认场地、联系人与备用方案，并指定同学负责临时沟通。第三，在每天行程结束后安排固定复盘，把当天出现的协作问题、意外信息和待核实事实及时整理，并逐步形成跨年度项目资料库，让下一届同学能在前期资料上继续深入，也让调研成果有机会反馈给受访者和合作单位。",
    )

    conclusion = find_one(paragraphs, "离开延安后，那块生铁条")
    set_paragraph_text(
        conclusion,
        "离开延安后，那块生铁条、商户的讲述和手中不断断开的棉线仍常常出现在我的回忆里。它们提醒我，宏大的精神并不悬浮在日常生活之外，而是体现在每一次认真倾听、每一次发现问题后的调整和每一次把知识用于实际问题的努力之中。重走长征路不是对历史的简单复刻，而是在今天找到自己的责任和方向。今后面对学习与生活中的困难，我希望自己少一些抱怨和退缩，多一些调查、行动与坚持，走好属于这一代青年的“个人长征”。",
    )

    revised_xml = ET.tostring(root, encoding="utf-8", xml_declaration=True)

    with ZipFile(OUTPUT, "w", ZIP_DEFLATED) as output_zip:
        for item in source_zip.infolist():
            data = revised_xml if item.filename == "word/document.xml" else source_zip.read(item.filename)
            output_zip.writestr(item, data)

print(OUTPUT)
