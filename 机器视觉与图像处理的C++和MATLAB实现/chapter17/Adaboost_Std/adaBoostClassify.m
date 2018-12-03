
function [classLabel, sum] = adaBoostClassify( Data, CABoosted )
% 输入值:
%      Data - 待分类数据矩阵，每行一个样本
%      CABoosted - CellArray 类型，记录着每个若分类器的相关信息
%      nWL - 弱分类器的数量
% 输出值:
%      classLabel - Data的类标号

%初始化m，n
[m n] = size(Data);
%初始化sum为51列1行的全0数组
sum = zeros(m, 1);

%做弱分类器返回结果数次的循环，
nWL = length(CABoosted);
for iWL = 1:nWL
    %取得权重和结果集
    WL = CABoosted{iWL}.classifier;
    alfa = CABoosted{iWL}.alfa;
    mRes = Data(:, WL.iFeature) > WL.thres;
    fRes = Data(:, WL.iFeature) <= WL.thres;
    mRes = -1 * mRes;
    res = fRes + mRes;
    %若标记为-1，为结果集取反
    if WL.direction == -1
        res = -1 * res;
    end
    %求得加权综合后的结果集
    sum = sum + alfa * res;
end
%将sum转换为和标签同形状的数组
classLabel = -1* ones(m, 1);
ind = find(sum >= 0);
classLabel(ind) = 1;


