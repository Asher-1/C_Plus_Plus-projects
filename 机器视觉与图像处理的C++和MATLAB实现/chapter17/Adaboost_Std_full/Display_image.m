function display_image(v,row,col)
% [a] = vec2matrix(vector,ROW,COL)
% input is  a column vector of size N and integers ROW and COL
% output a is a matrix of size ROW by COL
% Note N must be equal to ROW*COL
[size1 size2] = size(v);

if size1 ~= 1
   error('first parameter must be a row vector');
end;
if  row*col ~= size2
   error('row*col must be equal to vector length');
end;
 
a=zeros(row,col);
a(:)=v(:);


figure;
imagesc(a);
colormap(gray);
axis image;

