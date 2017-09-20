void FilterImage(CImage* image, int nType)
{
	if (image->IsNull())
		return;
int smoothGauss[9] = {1,2,1,2,4,2,1,2,1}; // 高斯模板
 int sharpLaplacian[9] = {-1,-1,-1,-1,9,-1,-1,-1,-1}; // 拉普拉斯模板
 int opTemp[9];
 float aver; // 系数
 if ( nType > 1) nType = 0;
 switch( nType ){
	 case 0: // 高斯模板 平滑
	 aver = (float)(1.0/16.0);
	 memcpy( opTemp, smoothGauss, 9*sizeof(int));
	 break;
	 case 1: // 拉普拉斯模板 锐化
	 aver = 1.0;
	 memcpy( opTemp, sharpLaplacian, 9*sizeof(int));
	 break;
	 }
	 int i,j;
	 int nWidth = image->GetWidth();
	 int nHeight = image->GetHeight();
	 for (i = 1; i < nWidth-1; i++){
		 for (j = 1; j < nHeight-1; j++){
			 int rr = 0, gg = 0, bb = 0;
			 int index = 0;
			 for (int col = -1; col <= 1; col++){
				 for (int row = -1; row <= 1; row++){
					 COLORREF clr = image->GetPixel( i+row, j+col);
					 rr += GetRValue(clr) * opTemp[index];
					 gg += GetGValue(clr) * opTemp[index];
					 bb += GetBValue(clr) * opTemp[index];
					 index++;
					 }
					 }
					 rr = (int)(rr*aver);
					 gg = (int)(gg*aver);
					 bb = (int)(bb*aver);
   // 处理溢出点
   if ( rr > 255 ) rr = 255;
   else if ( rr < 0 ) rr = -rr;
   if ( gg > 255 ) gg = 255;
   else if ( gg < 0 ) gg = -gg;
   if ( bb > 255 ) bb = 255;
   else if ( bb < 0 ) bb = -bb;
   // 错位重写以避免前一个像素被新的像素覆盖
   image->SetPixel( i-1, j-1, RGB(rr,gg,bb));
  }
 }
 Invalidate(); // 强制调用OnDraw