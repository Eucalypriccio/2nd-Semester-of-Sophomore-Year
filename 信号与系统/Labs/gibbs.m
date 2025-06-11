% Parameters
wc = pi/2;          % 截止频率
n_values = [1, 5, 15, 25];  % 截断长度列表
w = linspace(-pi, pi, 1000); % 频率范围
colors = ['b', 'g', 'm', 'c']; % 颜色配置

%% 创建4x1子图布局
figure('Position', [100, 100, 1000, 1200]);

% 循环处理每个截断长度
for i = 1:length(n_values)
    % ========================
    % 计算冲激响应和频率响应
    % ========================
    n_current = -n_values(i):n_values(i);
    h_current = sin(wc*n_current)./(pi*n_current);
    h_current(n_current==0) = wc/pi; % 处理n=0
    
    % 计算DTFT
    H = zeros(size(w));
    for k = 1:length(w)
        H(k) = sum(h_current .* exp(-1j*w(k)*n_current));
    end
    
    % ========================
    % 绘制子图
    % ========================
    subplot(4,1,i);
    
    % 绘制实际频率响应
    plot(w, abs(H), 'Color', colors(i), 'LineWidth', 1.5);
    hold on;
    
    % 绘制理想频率响应
    plot([-wc -wc], [0 1], 'r--', 'LineWidth', 1.5);
    plot([wc wc], [0 1], 'r--', 'LineWidth', 1.5);
    plot([-pi -wc], [0 0], 'r--', 'LineWidth', 1.5);
    plot([-wc wc], [1 1], 'r--', 'LineWidth', 1.5);
    plot([wc pi], [0 0], 'r--', 'LineWidth', 1.5);
    
    % 设置图形属性
    title(sprintf('N = %d', n_values(i)));
    xlabel('w');
    ylabel('H');
    legend('reality', 'ideal', 'Location', 'best');
    xlim([-pi pi]);
    ylim([-0.1 1.1]);
    grid on;
    
end