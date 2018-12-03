function WL = weakLearner(w, TrainData, label)
% input: w - Sample Weight
%        TrainData - data to be classified
%        label - the class label of TrainData
% output: WL - 结构体，保存弱分类器向量相关信息
%               WL.


[m n] = size(TrainData);





% search the best discriminative feature exhaustly
pInd = (label == 1); % 正例的索引
nInd = (label == -1); % 反例的索引

for iFeature = 1:n
    % 以正例和反例在iFeature特征那一维上的均值作为分类域值
    pMean = pInd' * TrainData(:, iFeature) / sum(pInd); % 正例的均值
    nMean = nInd' * TrainData(:, iFeature) / sum(nInd); % 反例的均值
    
    thres(iFeature) = (pMean + nMean) / 2; % 在iFeature那一维特征上的分类阈值
    
    nRes = TrainData(:, iFeature) >= thres(iFeature);
    pRes = TrainData(:, iFeature) < thres(iFeature);
    nRes = -1 * nRes;
    res = pRes + nRes;
    
    error(iFeature) = w * ( label ~=  res);
end

[val, ind] = max(abs(error-0.5));
if error(ind) > 0.5
    % 应当将分类器反向判别
    error(ind) = 1 - error(ind);
    WL.direction = -1;
else
    WL.direction = 1;
end

WL.iFeature = ind;
WL.error = error(ind);
WL.thres = thres(ind);

    
    
    
    
    
    
    
    
    
    
    
    
    
    


