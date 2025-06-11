% 定义函数 h(t)
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