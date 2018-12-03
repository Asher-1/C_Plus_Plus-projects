function [classLabel, sum] = adaBoostClassify( Data, CABoosted )
% Input:
%      Data - 待分类数据矩阵，每行一个样本
%      CABoosted - CellArray 类型，记录着每个若分类器的相关信息

% Output:
%      classLabel - Data的类标号
%      sum - 可以作为分类置信度信息

[m n] = size(Data);

sum = zeros(m, 1);

nWL = length(CABoosted);

for iWL = 1:nWL
    WL = CABoosted{iWL}.classifier;
    alfa = CABoosted{iWL}.alfa;
    pRes = Data(:, WL.iFeature) >= WL.thres;
    nRes = Data(:, WL.iFeature) < WL.thres;
    nRes = -1 * nRes;
    res = pRes + nRes;
    
    if WL.direction == -1
        res = -1 * res;
    end

    sum = sum + alfa * res;
end

classLabel =  ones(m, 1);
ind = find(sum >= 0);

classLabel(ind) = -1;


