K = 1000; % 频率采样点
w_index = linspace(-pi, pi, K); % 频率范围 [-π, π]

% 原始函数 H(w)
Hid = F(w_index);

% 绘制原始函数 H(w)
figure;
subplot(4, 1, 1);
plot(w_index, Hid, 'b', 'LineWidth', 1.5);
xlabel("w");
ylabel("H(w)");
title("原始函数 H(w)");

% 初始化不同 M 值的结果
M_values = [1, 5, 15, 25];
colors = ['r', 'g', 'm', 'c']; % 不同 M 值的颜色
legend_entries = cell(1, numel(M_values)); % 图例

% 遍历不同的 M 值
for m_idx = 1:numel(M_values)
    M = M_values(m_idx);
    HM = zeros(1, K); % 初始化 HM
    
    % 计算 HM(w) 的值
    for k = 1:K
        for i = -M:M
            if i == 0
                continue; % 避免分母为 0
            end
            HM(k) = HM(k) + sin(pi * i / 2) * exp(-1j * w_index(k) * i) / (pi * i);
        end
    end
    
    % 绘制 HM(w)
    subplot(4, 1, m_idx + 1);
    plot(w_index, abs(HM), colors(m_idx), 'LineWidth', 1.5); % 绘制幅值
    hold on;
    plot(w_index, Hid, 'b--', 'LineWidth', 1); % 绘制原始函数 H(w) 作为对比
    xlabel("w");
    ylabel("H_M(w)");
    title(sprintf("H_M(w) (M = %d)", M));
    legend_entries{m_idx} = sprintf("M = %d", M);
end

% 定义原始函数 F(w)
function H = F(w)
    H = zeros(size(w));
    for i = 1:numel(w)
        if w(i) <= (pi / 2) && w(i) >= (-pi / 2)
            H(i) = 1;
        else
            H(i) = 0;
        end
    end
end