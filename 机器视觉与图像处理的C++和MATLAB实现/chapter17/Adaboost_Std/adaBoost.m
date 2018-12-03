function CABoosted = adaBoost( TrainData, label, nIter )
% 训练 :迭代弱分类器
% 输入值: 
%       TrainData - 训练数据
%       label - 类标签
%       nIter - 弱分类器个数

fInd  = find(label == 1);
mInd = find(label == -1);
fP = length(fInd);
nP = length(mInd);

% 初始化权重向量，让正例和反例各自计算权重可以保证每次分类时都考虑正例和反例的贡献
% 否则如果正例远远大于反例，反例w比例太小，数量又少，其作用（贡献、代价）的不到体现
w(fInd) = 1 / (2 * fP);
w(mInd) = 1 / (2 * nP);

%初始化一个较小的错误率，若存在一弱分类器错误率小于该值，
%则该分类器所出结果可以接受成为最终结果
eps = 0.001;


% 建立nIter个弱分类器分量，组成一个强分类器
for iIt = 1:nIter
    % 归一化 w
    w = w / sum(w);
    %调用一次弱分类器，传入分布、训练集及标签数据
	WL = weakLearner(w,TrainData,label);
    
    CABoosted{iIt}.classifier = WL;
       
    
    mRes = TrainData(:, WL.iFeature) > WL.thres;
    fRes = TrainData(:, WL.iFeature) <= WL.thres;
    mRes = -1 * mRes;
    res = fRes + mRes;
    %若标记为-1，为结果集取反
    if WL.direction == -1
        res = -1 * res;
    end
   %构造a，根据弱分类器返回结果重新构造分布，减小正确结果的权重同时增加错误结果权重
   alfa(iIt) = (1/2) * log( (1-WL.error) / (WL.error + eps) ); 
   w = w .* exp( -alfa(iIt) * (label .* res) )';
   %为弱分类器该次结果加权重
    CABoosted{iIt}.alfa = alfa(iIt);    
    %如果该次所分类器的错误率小于eps，则可认为结果近似正确，可以接受
    if WL.error < eps
        break;
    end
end

