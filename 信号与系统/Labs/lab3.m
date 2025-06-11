% 主脚本
dt = 0.01;                  % 时间步长
t = -1:dt:1;                % 时间范围
y = f(t);                   % 生成信号

% 计算 FFT
N = numel(t);
Fs = 1/dt;                  % 采样频率
df = Fs/N;                  % 频率分辨率
freq = (-Fs/2 : df : Fs/2 - df); % 频率轴

F_fft = fftshift(fft(y)) * dt; % FFT 并调整幅度

figure;
plot(freq, abs(F_fft));
xlabel('w');
ylabel('|F(\omega)|');
xlim([-50, 50]);

function y = f(t)
    y = (abs(t) <= 0.5) .* (1 - 2*abs(t));
end