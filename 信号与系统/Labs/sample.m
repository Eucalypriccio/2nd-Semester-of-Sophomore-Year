t_index = 0:0.001:4;
w_index = -100:0.001:100;

Ts = 0.1;
xt = x(t_index);

figure;
plot(t_index, xt);






function y = x(t)
    y = zeros(size(t));
    for i = 1:numel(t)
        if t(i)>=0
            y(i) = exp(-0.5*t(i));
        else 
            y(i) = 0;
        end
    end
end