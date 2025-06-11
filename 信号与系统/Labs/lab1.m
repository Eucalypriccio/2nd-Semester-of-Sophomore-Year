x = [0, 0, 0, 0, 1, 4, 3, 5, 1, 2, 3, 2, 0, 0, 0];
x_index = -8:6;
h = [0, 0, 0, 0, 0, 3, 2, 4, 1, 3, 2, 0, 0, 0, 0];
h_index = -8:6;

figure;
stem(x_index, x, "filled");
xlabel('n');
ylabel('x[n]');

figure;
stem(h_index, h, "filled");
xlabel('n');
ylabel('h[n]');

result = conv(x, h);
result_index = -16:12;

figure;
stem(result_index, result, 'filled');
xlabel('n');
ylabel('y[n]');