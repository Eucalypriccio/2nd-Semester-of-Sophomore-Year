import matplotlib.pyplot as plt
import numpy as np

# 数据整理
texts_cn = ['war_and_peace_cn.txt', 'zhaoxia_cn.txt', 'quantum_cn.txt']
texts_en = ['proust_en.txt', 'alphabetia_en.txt', 'Steve_Jobs_Speech_en.txt']
methods = ['Shannon', 'Binary Huffman', 'Ternary Huffman', 'Quaternary Huffman']

# 中文数据
entropy_cn = [
    [7.9203, 8.0812, 8.1755],  # Shannon
    [7.9203, 8.0812, 8.1755],  # Binary Huffman
    [4.9972, 5.0987, 5.1582],  # Ternary Huffman
    [3.9602, 4.0406, 4.0878],  # Quaternary Huffman
]
avglen_cn = [
    [8.3680, 8.4022, 8.7180],  # Shannon
    [7.9582, 8.1043, 8.2099],  # Binary Huffman
    [5.0384, 5.1511, 5.1963],  # Ternary Huffman
    [4.0226, 4.0870, 4.1429],  # Quaternary Huffman
]
eff_cn = [
    [94.65, 96.18, 93.78],     # Shannon
    [99.52, 99.71, 99.58],     # Binary Huffman
    [99.18, 98.98, 99.27],     # Ternary Huffman
    [98.45, 98.87, 98.67],     # Quaternary Huffman
]

# 英文数据
entropy_en = [
    [4.2701, 4.6123, 4.3870],  # Shannon
    [4.2701, 4.6123, 4.3870],  # Binary Huffman
    [2.6941, 2.9100, 2.7679],  # Ternary Huffman
    [2.1351, 2.3062, 2.1935],  # Quaternary Huffman
]
avglen_en = [
    [4.8120, 5.0929, 4.7736],  # Shannon
    [4.3133, 4.6480, 4.4281],  # Binary Huffman
    [2.7681, 2.9626, 2.8320],  # Ternary Huffman
    [2.1795, 2.3745, 2.2443],  # Quaternary Huffman
]
eff_en = [
    [88.74, 90.56, 91.90],     # Shannon
    [99.00, 99.23, 99.07],     # Binary Huffman
    [97.33, 98.23, 97.74],     # Ternary Huffman
    [97.96, 97.12, 97.74],     # Quaternary Huffman
]

x = np.arange(len(texts_cn))
bar_width = 0.18

def plot_compare(title, texts, entropy, avglen, eff):
    plt.figure(figsize=(12, 8))
    # 信源熵
    plt.subplot(3, 1, 1)
    for i, label in enumerate(methods):
        bars = plt.bar(x + i*bar_width, entropy[i], width=bar_width, label=label)
        for bar, value in zip(bars, entropy[i]):
            plt.text(bar.get_x() + bar.get_width()/2, bar.get_height(), f'{value:.2f}',
                     ha='center', va='bottom', fontsize=8)
    plt.xticks(x + 1.5*bar_width, texts)
    plt.ylabel('Entropy (bits)')
    plt.title(title + ' - Entropy')
    plt.legend()

    # 平均码长
    plt.subplot(3, 1, 2)
    for i, label in enumerate(methods):
        bars = plt.bar(x + i*bar_width, avglen[i], width=bar_width, label=label)
        for bar, value in zip(bars, avglen[i]):
            plt.text(bar.get_x() + bar.get_width()/2, bar.get_height(), f'{value:.2f}',
                     ha='center', va='bottom', fontsize=8)
    plt.xticks(x + 1.5*bar_width, texts)
    plt.ylabel('Average Codeword Length (bits/symbol)')
    plt.title(title + ' - Average Codeword Length')

    # 编码效率
    plt.subplot(3, 1, 3)
    for i, label in enumerate(methods):
        bars = plt.bar(x + i*bar_width, eff[i], width=bar_width, label=label)
        for bar, value in zip(bars, eff[i]):
            plt.text(bar.get_x() + bar.get_width()/2, bar.get_height(), f'{value:.2f}',
                     ha='center', va='bottom', fontsize=8)
    plt.xticks(x + 1.5*bar_width, texts)
    plt.ylabel('Encoding Efficiency (%)')
    plt.title(title + ' - Encoding Efficiency')
    plt.ylim(85, 101)

    plt.tight_layout()
    plt.show()

# 中文图表
plot_compare('Chinese Texts', texts_cn, entropy_cn, avglen_cn, eff_cn)

# 英文图表
plot_compare('English Texts', texts_en, entropy_en, avglen_en, eff_en)