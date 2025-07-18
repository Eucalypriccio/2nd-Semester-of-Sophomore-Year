N = 500;                % 时间采样点
K = 1000;               % 频率采样点
t_index = linspace(-1, 1, N); % 时间范围 [-1, 1]
dt = t_index(2) - t_index(1); % 时间步长
w_index = linspace(-8*pi, 8*pi, K); % 频率范围 [-8π, 8π]

% 定义 f(t)
ft = f(t_index);

% 绘制 f(t)
figure;
subplot(5, 1, 1);
plot(t_index, ft);
xlabel("t");
ylabel("f(t)");
title("时域信号 f(t)");

% 方法 1：循环求和
tic;
Fw_loop = zeros(1, K);
for k = 1:K
    for n = 1:N
        Fw_loop(k) = Fw_loop(k) + ft(n) * exp(-1j * w_index(k) * t_index(n)) * dt;
    end
end
time_loop = toc;

% 绘制 F(w) (循环求和)
subplot(5, 1, 2);
plot(w_index, real(Fw_loop));
xlabel("w");
ylabel("|F(w)|");
title("频域信号 F(w) (循环求和)");

% 方法 2：向量运算
tic;
Fw_vector = zeros(1, K);
for k = 1:K
    Fw_vector(k) = sum(ft .* exp(-1j * w_index(k) * t_index) * dt);
end
time_vector = toc;

% 绘制 F(w) (向量运算)
subplot(5, 1, 3);
plot(w_index, real(Fw_vector));
xlabel("w");
ylabel("|F(w)|");
title("频域信号 F(w) (向量运算)");

% 方法 3：矩阵运算
tic;
Fw_matrix = (2/(N-1)) * (exp(-1j * (w_index.') * t_index) * ft.');
time_matrix = toc;

% 绘制 F(w) (矩阵运算)
subplot(5, 1, 4);
plot(w_index, real(Fw_matrix));
xlabel("w");
ylabel("|F(w)|");
title("频域信号 F(w) (矩阵运算)");

% 比较三种方法的时间
times = [time_loop, time_vector, time_matrix];
subplot(5, 1, 5);
bar(times);
set(gca, 'XTickLabel', {'循环求和', '向量运算', '矩阵运算'});
ylabel('时间 (秒)');
title('三种方法的计算时间比较');

% 定义 f(t) 函数
function y = f(t)
    y = zeros(size(t));
    for i = 1:numel(t)
        if t(i) <= 0.5 && t(i) >= -0.5
            y(i) = 1;
        else
            y(i) = 0;
        end
    end
end