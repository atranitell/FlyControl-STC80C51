% 绘制图形
function drawPWM()
    global count ;
    global data ;
    data = zeros(100000,5) ;
    count = 1 ; 
    openSerial();
end

function openSerial()
    % 查找串口对象
    scoms = instrfind;
    % 尝试停止、关闭删除串口对象
    % 第一次使用时将其置为0
    % 以后使用时置为1
    % 因为程序仍然在占用端口
    if ( 1 )
        stopasync(scoms);
        fclose(scoms);
        delete(scoms);
    end
    
    % 决定一次读取多少数据
    ComQuerySize = 1 ;
    % 获取端口，从设备管理器中得到端口号
    port_id = 'com3' ;
    % 配置uart串口
    g = serial(port_id, 'Parity','none','DataBits',8,'StopBits',1) ;
    g.BaudRate = 2400 ;
    % 配置输入输出缓冲区大小
    g.InputBufferSize = 131072;
    g.OutputBufferSize = 5120;
    % 配置超时
    g.Timeout = 30 ;
    g.ReadAsyncMode='continuous';
    g.BytesAVailableFcnMode='byte';
    g.BytesAvailableFcnCount = ComQuerySize;
    % 设置回调函数，即如果收到新数据调用该函数
    g.BytesAvailableFcn = @drawB ;
    g.Terminator = 'CR';
    fopen(g);
end

function drawA(obj, BytesAvailable, event)
    global count ;
    global data ;
    % 设定plot的x轴的区间
    L = 300;
    % 获取接收到得字符
    out = fscanf(obj);
    % 将接收的char类型字符转换为num
    d1 = str2num(out);
    if ( size(d1,2) == 5 )
        data(count,1:4) = 1000 - d1(1,2:5);
        drawnow
        figure(1)
        if count > L
            plot(count-L:count, data(count-L:count,1),'g');
            hold on;
            plot(count-L:count, data(count-L:count,2),'b');
            hold on;
            plot(count-L:count, data(count-L:count,3),'r');
            hold on;
            plot(count-L:count, data(count-L:count,4),'y');
            hold on;
        else
            plot(1:count, data(1:count,1),'g');
            hold on;
            plot(1:count, data(1:count,2),'b');
            hold on;
            plot(1:count, data(1:count,3),'r');
            hold on;
            plot(1:count, data(1:count,4),'y');
            hold on;
        end
        ylim([-10 1000]);
        count = count + 1;
    end
end

function drawB(obj, BytesAvailable, event)
    global count ;
    global data ;
    % 设定plot的x轴的区间
    L = 300;
    % 获取接收到得字符
    out = fscanf(obj);
    % 将接收的char类型字符转换为num
    d1 = str2num(out);
    if ( size(d1,2) == 6 )
        data(count,1:5) = d1(1,2:6);
        drawnow
        figure(1)
        if count > L
            plot(count-L:count, data(count-L:count,1),'g');
            hold on;
            plot(count-L:count, data(count-L:count,2),'b');
            hold on;
            plot(count-L:count, data(count-L:count,3),'r');
            hold on;
            plot(count-L:count, data(count-L:count,4),'m');
            hold on;
            plot(count-L:count, data(count-L:count,5),'c');
            hold on;
        else
            plot(1:count, data(1:count,1),'g');
            hold on;
            plot(1:count, data(1:count,2),'b');
            hold on;
            plot(1:count, data(1:count,3),'r');
            hold on;
            plot(1:count, data(1:count,4),'m');
            hold on;
            plot(1:count, data(1:count,5),'c');
            hold on;
        end
        ylim([-10 10]);
        count = count + 1;
    end
end
    %{
    data(count,1:4) = d1(1,2:5);
    if count > 40
        plot(count-40:count, data(count-40:count));
    end
    count = count + 1;
    if count > 80
        count = 0 ;
    end
    %}