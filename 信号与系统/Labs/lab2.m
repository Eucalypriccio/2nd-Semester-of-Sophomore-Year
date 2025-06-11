% 定义函数 x(t)
function y1 = x(t)
    y1 = zeros(size(t));
    for i = 1:numel(t)
        if t(i) >= 0.5 && t(i) <= 1
            y1(i) = 1;
        else
            y1(i) = 0;
        end
    end
end

