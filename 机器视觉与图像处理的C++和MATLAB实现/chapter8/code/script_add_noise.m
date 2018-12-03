%script_add_noise
I=imread('square.bmp');
J1 = add_noise(I, 'gaussian', 0, 10);
subplot(321)
hist(double(J1(:)), 100)
title('高斯');
subplot(322)
J2 = add_noise(I, 'uniform', -20, 20);
hist(double(J2(:)), 100)
title('均匀');
subplot(323)
J3 = add_noise(I, 'salt & pepper',0.02);
hist(double(J3(:)), 100)
title('椒盐');
subplot(324)
J4 = add_noise(I, 'rayleigh',30);
hist(double(J4(:)), 100)
title('瑞利');
subplot(325)
J5 = add_noise(I, 'exp',15);
hist(double(J5(:)), 100)
title('指数');
subplot(326)
J6 = add_noise(I, 'gamma',2,10);
hist(double(J6(:)), 100)
title('伽马');
