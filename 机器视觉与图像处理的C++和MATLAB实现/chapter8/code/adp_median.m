function II = adp_median(I, Smax)
%   自适应中值滤波函数
%   example:
%   F = adpmedian(g, Smax) 对图像g执行自适应中值滤波
%   从3*3模板开始，一直迭代值Smax*Smax,Smax是比1大的奇数

if (Smax <= 1) || (Smax/2 == round(Smax/2)) || (Smax ~= round(Smax))
   error('SMAX must be an odd integer > 1.')
end

% 初始化.
II = I;
II(:) = 0;
alreadyProcessed = false(size(I));

% 迭代.
for k = 3:2:Smax
   zmin = ordfilt2(I, 1, ones(k, k), 'symmetric');
   zmax = ordfilt2(I, k * k, ones(k, k), 'symmetric');
   zmed = medfilt2(I, [k k], 'symmetric');
   
   processUsingLevelB = (zmed > zmin) & (zmax > zmed) & ...
       ~alreadyProcessed; 
   zB = (I > zmin) & (zmax > I);
   outputZxy  = processUsingLevelB & zB;
   outputZmed = processUsingLevelB & ~zB;
   II(outputZxy) = I(outputZxy);
   II(outputZmed) = zmed(outputZmed);
   
   alreadyProcessed = alreadyProcessed | processUsingLevelB;
   if all(alreadyProcessed(:))
      break;
   end
end

II(~alreadyProcessed) = zmed(~alreadyProcessed);
