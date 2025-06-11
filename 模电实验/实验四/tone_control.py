import matplotlib.pyplot as plt
import numpy as np

plt.rcParams['font.sans-serif'] = ['SimHei']  # 设置中文字体为黑体
plt.rcParams['axes.unicode_minus'] = False    # 正确显示负号

# 频率
f = np.array([20, 50, 125, 500, 1000, 2000, 8000, 20000, 50000])

# 两组增益数据
Av1 = [16.99, 13.26, 7.89, 0.83, 0.09, -1.62, -7.54, -13.98, -17.72]
Av2 = [-19.17, -13.56, -6.56, -0.54, 1.06, 3.97, 13.44, 18.39, 20.21]

plt.figure(figsize=(8, 5))
plt.semilogx(f, Av1, 'o-', label='低音提升-高音衰减')
plt.semilogx(f, Av2, 's-', label='低音衰减-高音提升')

plt.xlabel('频率 f (Hz)')
plt.ylabel('电压增益 $A_v$ (dB)')
plt.title('音调控制特性曲线')
plt.grid(True, which='both', linestyle='--', alpha=0.7)
plt.legend()
plt.tight_layout()
plt.show()