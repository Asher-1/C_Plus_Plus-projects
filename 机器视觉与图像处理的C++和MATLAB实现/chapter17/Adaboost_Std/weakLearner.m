function WL = weakLearner(w, TrainData, label)
% 输入值: w - 权重分布
%        TrainData - 待区分的数据集
%        label - 数据集的标签
% 输出值: WL - 结构体，保存弱分类器向量相关信息

%初始化，m为对象数，n为每个对象的特征数目
[m n] = size(TrainData);

% search the best discriminative feature exhaustly
fInd = (label == 1); % 正例的索引
mInd = (label == -1); % 反例的索引

for iFeature = 1:n
    % 以正例和反例在iFeature特征那一维上的均值作为分类阈值
    fMean = fInd' * TrainData(:, iFeature) / sum(fInd); % 正例的均值
    mMean = mInd' * TrainData(:, iFeature) / sum(mInd); % 反例的均值
    
    thres(iFeature) = (fMean + mMean) / 2; % 在iFeature那一维特征上的分类阈值
    %大于阈值的为男人，小于等于为女人
    mRes = TrainData(:, iFeature) > thres(iFeature);
    fRes = TrainData(:, iFeature) <= thres(iFeature);
    mRes = -1 * mRes;
    res = fRes + mRes;
    %通过标签与结果集的对比得出错误率
    error(iFeature) = w * ( label ~=  res);
end
%取得错误率最低的结果项，封装
[val, ind] = max(abs(error-0.5));
if error(ind) > 0.5
    % 应当将分类器反向判别
    error(ind) = 1 - error(ind);
    WL.direction = -1;
else
    WL.direction = 1;
end
%封装结果
WL.iFeature = ind;
WL.error = error(ind);
WL.thres = thres(ind);

    
    
    
    
    
    
    
    
    
    
    
    
    
    


