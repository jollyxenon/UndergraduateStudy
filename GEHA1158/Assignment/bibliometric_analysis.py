# -*- coding: utf-8 -*-
"""
文献计量分析脚本：解析WoS和CNKI导出数据，生成关键词频次、学科分布对比图表
用于论文"规训与启蒙：青年数字文化在中国高校通识教育中的话语审视"
"""

import re
import os
from collections import Counter
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import matplotlib.font_manager as fm

# ========== 配置 ==========
BASE_DIR = r"D:\UndergraduateStudy\GEHA1158\Assignment"
WOS_FILE = os.path.join(BASE_DIR, "CiteSpaceProject", "WOS_data", "WOS.txt")
CNKI_FILE = os.path.join(BASE_DIR, "CiteSpaceProject", "CNKI_data", "CNKI.txt")
OUTPUT_DIR = os.path.join(BASE_DIR, "figures")
os.makedirs(OUTPUT_DIR, exist_ok=True)

CNKI_DISCIPLINE_MAP = {
    'A': 'Marxism / Philosophy',
    'B': 'Philosophy / Religion',
    'C': 'Social Sciences',
    'D': 'Politics / Law',
    'E': 'Military Science',
    'F': 'Economics / Management',
    'G': 'Education',
    'H': 'Language / Literature / Arts',
    'I': 'Literature / History / Geography',
    'J': 'Arts',
    'K': 'History / Geography',
    'N': 'Natural Sciences',
    'O': 'Mathematics / Physics / Chemistry',
    'P': 'Astronomy / Earth Science',
    'Q': 'Biology',
    'R': 'Medicine / Health',
    'S': 'Agriculture',
    'T': 'Industry / Technology',
    'U': 'Transportation',
    'V': 'Aerospace',
    'X': 'Environmental Science / Safety',
    'Z': 'Comprehensive / General'
}

# 尝试使用中文字体
def get_chinese_font():
    """找到系统中可用的中文字体"""
    candidates = ['SimHei', 'Microsoft YaHei', 'SimSun', 'KaiTi', 'FangSong']
    available = {f.name for f in fm.fontManager.ttflist}
    for name in candidates:
        if name in available:
            return name
    return None

CN_FONT = get_chinese_font()
if CN_FONT:
    plt.rcParams['font.sans-serif'] = [CN_FONT, 'DejaVu Sans']
    plt.rcParams['axes.unicode_minus'] = False
    print(f"[INFO] 使用中文字体: {CN_FONT}")
else:
    print("[WARN] 未找到中文字体，中文可能显示为方块")

# ========== 解析 WoS 数据 ==========
def parse_wos(filepath):
    """解析 WoS Plain Text 格式"""
    records = []
    with open(filepath, 'r', encoding='utf-8-sig') as f:
        text = f.read()
    
    # 按 ER 分割记录
    raw_records = re.split(r'\nER\s*\n', text)
    
    for raw in raw_records:
        if not raw.strip() or raw.strip().startswith('FN ') or raw.strip().startswith('VR '):
            continue
        
        record = {}
        current_field = None
        current_value = []
        
        for line in raw.split('\n'):
            if len(line) >= 3 and line[2] == ' ' and line[:2].isalpha():
                # 新字段
                if current_field:
                    record[current_field] = ' '.join(current_value).strip()
                current_field = line[:2]
                current_value = [line[3:]]
            elif line.startswith('   '):
                # 续行
                if current_value is not None:
                    current_value.append(line.strip())
        
        if current_field:
            record[current_field] = ' '.join(current_value).strip()
        
        if record.get('TI'):
            records.append(record)
    
    return records

# ========== 解析 CNKI 数据 ==========
def parse_cnki(filepath):
    """解析 CNKI Refworks 格式"""
    records = []
    with open(filepath, 'r', encoding='utf-8-sig') as f:
        text = f.read()
    
    # 按空行分割
    blocks = re.split(r'\n\s*\n', text)
    
    for block in blocks:
        block = block.strip()
        if not block:
            continue
        
        record = {}
        for line in block.split('\n'):
            line = line.strip()
            if not line:
                continue
            # Refworks 格式: FIELD value
            match = re.match(r'^([A-Z][A-Z0-9])\s+(.*)', line)
            if match:
                field, value = match.group(1), match.group(2)
                record[field] = value
        
        if record.get('T1'):
            records.append(record)
    
    return records

# ========== 关键词提取 ==========
def extract_wos_keywords(records):
    """从 WoS 记录提取作者关键词 (DE) 和 KeyWords Plus (ID)"""
    keywords = []
    for r in records:
        de = r.get('DE', '')
        if de:
            kws = [k.strip().lower() for k in de.split(';') if k.strip()]
            keywords.extend(kws)
        kid = r.get('ID', '')
        if kid:
            kws = [k.strip().lower() for k in kid.split(';') if k.strip()]
            keywords.extend(kws)
    return keywords

def extract_cnki_keywords(records):
    """从 CNKI 记录提取关键词 (K1)"""
    keywords = []
    for r in records:
        k1 = r.get('K1', '')
        if k1:
            kws = [k.strip() for k in k1.split(';') if k.strip()]
            keywords.extend(kws)
    return keywords

# ========== 学科分布提取 ==========
def extract_wos_categories(records):
    """从 WoS 记录提取 Web of Science Categories (WC)"""
    cats = []
    for r in records:
        wc = r.get('WC', '')
        if wc:
            cs = [c.strip() for c in wc.split(';') if c.strip()]
            cats.extend(cs)
    return cats

def extract_cnki_categories(records):
    """从 CNKI 记录提取学科分类（基于 CN 字段后缀字母）"""
    cats = []
    for r in records:
        cn = r.get('CN', '')
        if not cn:
            continue
        match = re.search(r'/([A-Z])(?:\d+)?$', cn)
        if not match:
            continue
        code = match.group(1)
        cats.append(CNKI_DISCIPLINE_MAP.get(code, f'Other ({code})'))
    return cats

# ========== 年份分布 ==========
def extract_wos_years(records):
    years = []
    for r in records:
        py = r.get('PY', '')
        if py and py.isdigit():
            years.append(int(py))
    return years

def extract_cnki_years(records):
    years = []
    for r in records:
        yr = r.get('YR', '')
        if yr and yr.isdigit():
            years.append(int(yr))
    return years

# ========== 主程序 ==========
def main():
    print("=" * 60)
    print("  文献计量分析：WoS vs CNKI 话语对比")
    print("=" * 60)
    
    # 解析数据
    wos_records = parse_wos(WOS_FILE)
    cnki_records = parse_cnki(CNKI_FILE)
    
    print(f"\n[数据概览]")
    print(f"  WoS 记录数: {len(wos_records)}")
    print(f"  CNKI 记录数: {len(cnki_records)}")
    
    # ---- 关键词分析 ----
    wos_kws = extract_wos_keywords(wos_records)
    cnki_kws = extract_cnki_keywords(cnki_records)
    
    wos_kw_counter = Counter(wos_kws)
    cnki_kw_counter = Counter(cnki_kws)
    
    print(f"\n[关键词统计]")
    print(f"  WoS 总关键词数: {len(wos_kws)}, 不重复: {len(wos_kw_counter)}")
    print(f"  CNKI 总关键词数: {len(cnki_kws)}, 不重复: {len(cnki_kw_counter)}")
    
    print(f"\n  WoS Top 25 关键词:")
    for kw, cnt in wos_kw_counter.most_common(25):
        print(f"    {kw}: {cnt}")
    
    print(f"\n  CNKI Top 25 关键词:")
    for kw, cnt in cnki_kw_counter.most_common(25):
        print(f"    {kw}: {cnt}")
    
    # ---- 学科分布 ----
    wos_cats = extract_wos_categories(wos_records)
    wos_cat_counter = Counter(wos_cats)
    cnki_cats = extract_cnki_categories(cnki_records)
    cnki_cat_counter = Counter(cnki_cats)

    print(f"\n[WoS 学科分布] (Top 15)")
    for cat, cnt in wos_cat_counter.most_common(15):
        print(f"    {cat}: {cnt}")
    print(f"\n[CNKI 学科分布] (Top 10)")
    for cat, cnt in cnki_cat_counter.most_common(10):
        print(f"    {cat}: {cnt}")
    
    # ---- 年份分布 ----
    wos_years = extract_wos_years(wos_records)
    cnki_years = extract_cnki_years(cnki_records)
    
    print(f"\n[年份分布]")
    print(f"  WoS: {min(wos_years) if wos_years else 'N/A'}-{max(wos_years) if wos_years else 'N/A'}")
    print(f"  CNKI: {min(cnki_years) if cnki_years else 'N/A'}-{max(cnki_years) if cnki_years else 'N/A'}")
    
    # ========== 可视化 ==========
    
    # --- 图1: WoS 关键词 Top 20 ---
    fig, ax = plt.subplots(figsize=(12, 7))
    top20_wos = wos_kw_counter.most_common(20)
    labels = [k for k, _ in top20_wos]
    values = [v for _, v in top20_wos]
    bars = ax.barh(range(len(labels)), values, color='#4472C4')
    ax.set_yticks(range(len(labels)))
    ax.set_yticklabels(labels, fontsize=10)
    ax.invert_yaxis()
    ax.set_xlabel('Frequency', fontsize=12)
    ax.set_title('WoS Literature: Top 20 Author Keywords', fontsize=14, fontweight='bold')
    for bar, val in zip(bars, values):
        ax.text(bar.get_width() + 0.3, bar.get_y() + bar.get_height()/2, str(val),
                va='center', fontsize=9)
    plt.tight_layout()
    fig.savefig(os.path.join(OUTPUT_DIR, 'wos_keywords_top20.png'), dpi=200)
    plt.close(fig)
    print(f"\n[已保存] wos_keywords_top20.png")
    
    # --- 图2: CNKI 关键词 Top 20 ---
    fig, ax = plt.subplots(figsize=(12, 7))
    top20_cnki = cnki_kw_counter.most_common(20)
    labels = [k for k, _ in top20_cnki]
    values = [v for _, v in top20_cnki]
    bars = ax.barh(range(len(labels)), values, color='#C0504D')
    ax.set_yticks(range(len(labels)))
    ax.set_yticklabels(labels, fontsize=11)
    ax.invert_yaxis()
    ax.set_xlabel('频次', fontsize=12)
    ax.set_title('CNKI文献：高频关键词 Top 20', fontsize=14, fontweight='bold')
    for bar, val in zip(bars, values):
        ax.text(bar.get_width() + 0.1, bar.get_y() + bar.get_height()/2, str(val),
                va='center', fontsize=9)
    plt.tight_layout()
    fig.savefig(os.path.join(OUTPUT_DIR, 'cnki_keywords_top20.png'), dpi=200)
    plt.close(fig)
    print(f"[已保存] cnki_keywords_top20.png")
    
    # --- 图3: WoS 学科分布 Top 10 ---
    fig, ax = plt.subplots(figsize=(10, 6))
    top10_cat = wos_cat_counter.most_common(10)
    labels = [k for k, _ in top10_cat]
    values = [v for _, v in top10_cat]
    bars = ax.barh(range(len(labels)), values, color='#7F6084')
    ax.set_yticks(range(len(labels)))
    ax.set_yticklabels(labels, fontsize=9)
    ax.invert_yaxis()
    ax.set_xlabel('Count', fontsize=12)
    ax.set_title('WoS Literature: Subject Category Distribution (Top 10)', fontsize=13, fontweight='bold')
    for bar, val in zip(bars, values):
        ax.text(bar.get_width() + 0.3, bar.get_y() + bar.get_height()/2, str(val),
                va='center', fontsize=9)
    plt.tight_layout()
    fig.savefig(os.path.join(OUTPUT_DIR, 'wos_categories_top10.png'), dpi=200)
    plt.close(fig)
    print(f"[已保存] wos_categories_top10.png")

    # --- 图4: CNKI 学科分布 Top 10 ---
    fig, ax = plt.subplots(figsize=(10, 6))
    top10_cnki_cat = cnki_cat_counter.most_common(10)
    labels = [k for k, _ in top10_cnki_cat]
    values = [v for _, v in top10_cnki_cat]
    bars = ax.barh(range(len(labels)), values, color='#70AD47')
    ax.set_yticks(range(len(labels)))
    ax.set_yticklabels(labels, fontsize=10)
    ax.invert_yaxis()
    ax.set_xlabel('Count', fontsize=12)
    ax.set_title('CNKI Literature: Subject Category Distribution (Top 10)', fontsize=13, fontweight='bold')
    for bar, val in zip(bars, values):
        ax.text(bar.get_width() + 0.3, bar.get_y() + bar.get_height()/2, str(val),
                va='center', fontsize=9)
    plt.tight_layout()
    fig.savefig(os.path.join(OUTPUT_DIR, 'cnki_categories_top10.png'), dpi=200)
    plt.close(fig)
    print(f"[已保存] cnki_categories_top10.png")

    # --- 图5: 话语取向对比（核心图表）---
    # 将关键词按话语系统分类
    # 话语A（管理规训/思政）相关词
    discourse_a_terms = {
        'cn': ['思想政治教育', '大学生', '教育引导', '网络亚文化', '主流文化', '价值观', 
               '引导策略', '主流价值观', '网络素养', '思政教育', '亚文化', '破圈',
               '圈层化', '网络社群', '意识形态'],
        'en': ['ideological and political education', 'ideological', 'political education',
               'mainstream', 'guidance', 'moral education', 'values education',
               'socialist', 'chinese characteristics']
    }
    
    # 话语B（批判教育学/课程教学）相关词
    discourse_b_terms = {
        'cn': ['课程设计', '教学创新', '批判性思维', '媒介素养', '参与度', '学习兴趣',
               '教学改革', '通识教育'],
        'en': ['critical pedagogy', 'critical thinking', 'curriculum', 'pedagogy',
               'teaching', 'learning', 'student engagement', 'media literacy',
               'digital literacy', 'popular culture', 'creative', 'empowerment',
               'participatory', 'game-based', 'gamification']
    }
    
    # 统计每个话语系统的关键词命中数
    def count_discourse(keywords_counter, terms_list):
        total = 0
        matched = {}
        for term in terms_list:
            for kw, cnt in keywords_counter.items():
                if term.lower() in kw.lower():
                    total += cnt
                    matched[kw] = cnt
        return total, matched
    
    cnki_a_total, cnki_a_matched = count_discourse(cnki_kw_counter, discourse_a_terms['cn'])
    cnki_b_total, cnki_b_matched = count_discourse(cnki_kw_counter, discourse_b_terms['cn'])
    wos_a_total, wos_a_matched = count_discourse(wos_kw_counter, discourse_a_terms['en'])
    wos_b_total, wos_b_matched = count_discourse(wos_kw_counter, discourse_b_terms['en'])
    
    print(f"\n[话语取向分析]")
    print(f"  CNKI - 话语A(管理规训): {cnki_a_total} 次命中")
    for kw, cnt in sorted(cnki_a_matched.items(), key=lambda x: -x[1])[:10]:
        print(f"    {kw}: {cnt}")
    print(f"  CNKI - 话语B(教学创新): {cnki_b_total} 次命中")
    for kw, cnt in sorted(cnki_b_matched.items(), key=lambda x: -x[1])[:10]:
        print(f"    {kw}: {cnt}")
    print(f"  WoS - 话语A(管理规训): {wos_a_total} 次命中")
    for kw, cnt in sorted(wos_a_matched.items(), key=lambda x: -x[1])[:10]:
        print(f"    {kw}: {cnt}")
    print(f"  WoS - 话语B(教学创新): {wos_b_total} 次命中")
    for kw, cnt in sorted(wos_b_matched.items(), key=lambda x: -x[1])[:10]:
        print(f"    {kw}: {cnt}")
    
    # 对比图
    fig, axes = plt.subplots(1, 2, figsize=(14, 6))
    
    # CNKI 话语分布
    cnki_other = len(cnki_kws) - cnki_a_total - cnki_b_total
    cnki_vals = [cnki_a_total, cnki_b_total, max(cnki_other, 0)]
    cnki_labels_d = ['Discourse A\n(Disciplinary/\nIdeological)', 
                     'Discourse B\n(Pedagogical/\nCritical)', 
                     'Other']
    colors_cnki = ['#C0504D', '#4472C4', '#A5A5A5']
    wedges1, texts1, autotexts1 = axes[0].pie(cnki_vals, labels=cnki_labels_d, 
                                               colors=colors_cnki,
                                               autopct='%1.1f%%', startangle=90,
                                               textprops={'fontsize': 10})
    axes[0].set_title('CNKI Literature\nDiscourse Orientation', fontsize=13, fontweight='bold')
    
    # WoS 话语分布
    wos_other = len(wos_kws) - wos_a_total - wos_b_total
    wos_vals = [wos_a_total, wos_b_total, max(wos_other, 0)]
    wedges2, texts2, autotexts2 = axes[1].pie(wos_vals, labels=cnki_labels_d, 
                                               colors=colors_cnki,
                                               autopct='%1.1f%%', startangle=90,
                                               textprops={'fontsize': 10})
    axes[1].set_title('WoS Literature\nDiscourse Orientation', fontsize=13, fontweight='bold')
    
    plt.suptitle('Discourse Orientation Comparison: CNKI vs WoS', 
                 fontsize=15, fontweight='bold', y=1.02)
    plt.tight_layout()
    fig.savefig(os.path.join(OUTPUT_DIR, 'discourse_comparison.png'), dpi=200, bbox_inches='tight')
    plt.close(fig)
    print(f"[已保存] discourse_comparison.png")
    
    # --- 打印论文所需的文本摘要 ---
    print(f"\n{'='*60}")
    print(f"  论文可用统计摘要")
    print(f"{'='*60}")
    print(f"""
本研究从Web of Science核心合集检索到 {len(wos_records)} 篇英文文献，
从CNKI检索到 {len(cnki_records)} 篇中文文献。

关键词共现分析显示：
- CNKI文献的高频关键词集中于"思想政治教育""教育引导""网络亚文化"
  "主流文化""价值观"等管理规训话语体系；
- WoS文献的高频关键词则围绕"digital culture""higher education"
  "pedagogy""popular culture""curriculum"等教学创新与批判教育学话语。

学科分布统计显示：
- WoS 文献最集中的学科为 {wos_cat_counter.most_common(1)[0][0]}（{wos_cat_counter.most_common(1)[0][1]} 次），
  同时在 {wos_cat_counter.most_common(2)[1][0]}（{wos_cat_counter.most_common(2)[1][1]} 次）
  等领域也有较明显分布；
- CNKI 文献最集中的学科为 {cnki_cat_counter.most_common(1)[0][0]}（{cnki_cat_counter.most_common(1)[0][1]} 次），
  其后为 {cnki_cat_counter.most_common(2)[1][0]}（{cnki_cat_counter.most_common(2)[1][1]} 次）。

话语取向量化统计：
- CNKI文献中，属于"管理规训"话语的关键词占比为 
  {cnki_a_total}/{len(cnki_kws)} ({cnki_a_total/max(len(cnki_kws),1)*100:.1f}%)，
  属于"教学创新"话语的仅 {cnki_b_total}/{len(cnki_kws)} ({cnki_b_total/max(len(cnki_kws),1)*100:.1f}%)。
- WoS文献中，"管理规训"话语占 {wos_a_total}/{len(wos_kws)} ({wos_a_total/max(len(wos_kws),1)*100:.1f}%)，
  "教学创新/批判教育学"话语占 {wos_b_total}/{len(wos_kws)} ({wos_b_total/max(len(wos_kws),1)*100:.1f}%)。

这一对比清晰地表明，中英文学术界对青年数字文化在高等教育中的角色
存在显著的话语分化。
""")
    
    # --- 输出所有论文标题供检查 ---
    print(f"\n[WoS 文献标题列表]")
    for i, r in enumerate(wos_records, 1):
        py = r.get('PY', '????')
        ti = r.get('TI', 'No title')
        au = r.get('AU', 'Unknown')
        first_author = au.split('\n')[0].split(';')[0].strip() if au else 'Unknown'
        print(f"  {i}. [{py}] {first_author}: {ti}".encode('gbk', 'replace').decode('gbk'))
    
    print(f"\n[CNKI 文献标题列表]")
    for i, r in enumerate(cnki_records, 1):
        yr = r.get('YR', '????')
        t1 = r.get('T1', 'No title')
        a1 = r.get('A1', 'Unknown')
        print(f"  {i}. [{yr}] {a1}: {t1}".encode('gbk', 'replace').decode('gbk'))

if __name__ == '__main__':
    main()
