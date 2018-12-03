function [errTrain, errTest] = main(nWL)
% 输入：nWL - 训练迭代的轮数
% 输出：errTrain - 训练集的错误率
%        errTest - 测试集的错误率

load('faces_small.mat');
if nargin == 0
    nWL = 500;
end
% 训练过程
CABoosted = adaBoost( faces_small, faces_label, nWL );

% 对于训练样本
classLabel = adaBoostClassify( faces_small, CABoosted );
errTrain = sum(classLabel ~= faces_label) / length(faces_label);

clear classLabel
% 对于测试样本
classLabel = adaBoostClassify( new_faces_small, CABoosted );
errTest = sum(classLabel ~= new_label) / length(new_label);