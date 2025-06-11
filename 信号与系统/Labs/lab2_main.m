t = -1:0.01:4;
result1 = x(t);

figure;
plot(t, result1);
xlabel('t');
ylabel('x(t)');

result2 = h(t);
figure;
plot(t, result2);
xlabel('t');
ylabel('h(t)');

result3 = conv(result1, result2) * 0.01;
t_conv = (t(1)+t(1)):0.01:(t(end)+t(end));

figure;
plot(t_conv, result3);
xlabel('t');
ylabel('y(t)');

function y1 = x(t)
    y1 = zeros(size(t));
    for i = 1:numel(t)
        if t(i) >= -0.5 && t(i) <= 1
            y1(i) = 1;
        else
            y1(i) = 0;
        end
    end
end

function y2 = h(t)
    y2 = zeros(size(t));
    for i = 1:numel(t)
        if t(i) >= 0 && t(i) <= 2
            y2(i) = t(i) / 2;
        else
            y2(i) = 0;
        end
    end
end