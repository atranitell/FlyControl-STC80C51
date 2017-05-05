%% 卡尔曼滤波
% 通过 jump_time[0, 50] 产生一个随机 [-15,15] 的阶跃信号
% 用来观察卡尔曼滤波效果
function test()
    N = 200;
    count = 0 ;
    jump_time = 10 ;
    count_last = 0 ;
    y(1)=0;
    figure(1);
    for i=2:N
        count(i) = i ;
        y(i) = rand(1)-0.5 ;
        % 产生随机信号
        if i == jump_time
            y(i) = 30*rand(1)-15 ;
            jump_time = i+round(rand(1)*50);
        end
    end    
    Bs = drawP(N, y) ;
    plot(count, y, 'g', count, Bs, 'r');
    hold on;
    drawnow;
    ylim([-20 20]);
end

% x_last 是上一次的真实值, 一般可用上一次滤波后的结果
% p_last 是估计值, 系统自迭代
% Q, R 是高斯白噪声
% y_now 是这一次的测量值
function [Bs, p_now] = KMfilter( x_last, p_last, Q, R, y_now )
    x_now = x_last ;
    p_now = p_last + Q ;
    Kg_now = p_now/(p_now+R) ;
    Bs = x_now + Kg_now * (y_now - x_now) ;
    p_now = p_now * (1 - Kg_now) ;
end

function Bs = drawP(N, y)
    w = 0.1 * randn(1, N);
    V = randn(1, N);
    % 以下是计算系统白噪声, 其分布应符合系统特性
    % Rvv 与 Qww 应该符合系统特性
    q1 = std(V);
    Rvv = q1.^ 2;
    q3 = std(w);
    Qww = q3.^ 2;
    
    % Bs是滤波后的效果
    % y 是实际观测值
    % x 是真实值, 现在将Bs的预测结果作为真实值
    p_now = 1;
    Bs(1) = 0;
    for t = 2 : N;
        [Bs(t),p_now] = KMfilter(Bs(t-1), p_now, Qww, Rvv, y(t));
    end
end