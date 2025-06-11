%% 参数设置
Ts = 0.1; % 采样周期
t_continuous = 0:0.001:4; % 连续时间向量
t_samples = 0:Ts:4; % 采样时间点
k_samples = 0:length(t_samples)-1;
x_continuous = exp(-0.5 * t_continuous); % 连续信号
x_samples = exp(-0.5 * t_samples); % 采样信号

%% 问题1：绘制x(t)和x(kTs)
figure;
subplot(6,1,1);
plot(t_continuous, x_continuous, 'b', 'LineWidth', 2.0);
hold on;
xlabel('Time (s)');
ylabel('Amplitude');
title('Original Signal x(t)');
grid on;

subplot(6,1,2);
stem(k_samples, x_samples, "filled", 'LineWidth',2.0);
xlabel('k');
ylabel('Amplitude');
title('Sampled Signal x(kT_s)');
grid on;

%% 问题2：绘制|X(jω)|
omega = linspace(-100, 100, 1000); % 频率向量
X_magnitude = 1 ./ sqrt(0.25 + omega.^2); % |X(jω)|

subplot(6,1,3);
plot(omega, X_magnitude, 'b', 'LineWidth', 2.0);
xlabel('Frequency (rad/s)');
ylabel('|X(jω)|');
title('Magnitude Spectrum of X(jω)');
grid on;

%% 问题3：绘制|X_p(jω)|
omega_s = 2 * pi / Ts; % 采样角频率
k_values = (-5:5)'; % 取k从-5到5，以覆盖足够的重复项
omega_p = linspace(-100, 100, 1000); % 频率向量

% 计算X_p(jω)
shifted_omega = omega_p - k_values * omega_s; % 11 x 1000矩阵，每行对应一个k值
X_terms = 1 ./ (0.5 + 1i * shifted_omega); % 计算每个k对应的X(j(ω -kω_s))
Xp_sum = sum(X_terms, 1); % 对每个ω求和
Xp = (1/Ts) .* Xp_sum; % 乘以1/Ts
Xp_magnitude = abs(Xp); % 幅度

subplot(6,1,4);
plot(omega_p, Xp_magnitude, 'b', 'LineWidth', 2.0);
xlabel('Frequency (rad/s)');
ylabel('|X_p(jω)|');
title('Magnitude Spectrum of X_p(jω)');
grid on;

%% 问题4：重构x_r(t)并绘制
t_recon = 0:0.001:4; % 重构时间点
xr = zeros(size(t_recon));

% 计算每个t_recon点的重构信号
for i = 1:length(t_recon)
    t = t_recon(i);
    % 计算所有采样点的sinc插值
    xr(i) = sum( x_samples .* sinc( (t - t_samples)/Ts ) );
end

subplot(6,1,5);
plot(t_recon, xr, 'b', 'LineWidth', 2.0);
xlabel('Time (s)');
ylabel('Amplitude');
title('Reconstructed Signal x_r(t)');
grid on;

%% 问题5：绘制插值误差
x_recon_continuous = exp(-0.5 * t_recon);
error = abs(x_recon_continuous - xr);

subplot(6,1,6);
plot(t_recon, error, 'r', 'LineWidth', 2.0);
xlabel('Time (s)');
ylabel('Error');
title('Interpolation Error |x(t) - x_r(t)|');
grid on;