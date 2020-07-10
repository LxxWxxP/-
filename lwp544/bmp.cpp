#include "stdafx.h"
#include <math.h>
#include <complex>
using namespace std;

BITMAPINFO* lpBitsInfo = NULL;

//快速傅立叶正变换：
BITMAPINFO *lpDIB_FFT=NULL;
BITMAPINFO* lpDIB_IFFT = NULL;

//读取bmp文件
BOOL LoadBmpFile(char* BmpFileName)
{
	FILE* fp;
	if(NULL==(fp=fopen(BmpFileName,"rb")))
		return FALSE;
	BITMAPFILEHEADER bf;
	BITMAPINFOHEADER bi;

	//读文件头到bf
	//fread(&bf, 14, 1, fp);
	fread(&bf, sizeof(BITMAPFILEHEADER), 1, fp);

	//读文件头到bi
	//fread(&bi, 40, 1, fp);
	fread(&bi, sizeof(BITMAPINFOHEADER), 1, fp);

	DWORD NumColors;
	if (bi.biClrUsed > 0)//biClrUsed表示调色板的长度；若biClrUsed=0，表示调色板长度要根据biBitCount计算
		NumColors = bi.biClrUsed;
	else 
	{
		switch(bi.biBitCount)
		{	
		case 1://二值图像，一个二进制位可以描述，纯白或纯黑
			NumColors = 2;
			break;
		case 4://16色
			NumColors = 16;
			break;
		case 8://256图像或灰度图像
			NumColors = 256;
			break;
		case 24://24位真彩没有调色板
			NumColors = 0;
			break;
		}
	}

	DWORD LineBytes = (bi.biWidth * bi.biBitCount + 31) / 32 * 4;
	DWORD ImgSize = LineBytes * bi.biHeight;
	DWORD size = 40 + NumColors * 4 + ImgSize;
	
	//分配内存
	if (NULL == (lpBitsInfo = (BITMAPINFO*)malloc(size)))	
		return FALSE;

	//指针偏移到信息头
	fseek(fp, 14, SEEK_SET);
	fread((char*)lpBitsInfo, size, 1, fp);

	//保证内存中调色板的长度是正确的NumColors
	lpBitsInfo->bmiHeader.biClrUsed = NumColors;

	return TRUE;
}

//灰度化
void gray()
{
	if (NULL == lpBitsInfo)
		return;
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];//调色板在越界之后的第一个字节
	
	int LineBytes_gray = (w * 8 + 31) / 32 * 4;
	LONG size = 40 + 1024 + LineBytes_gray * h;

	LPBITMAPINFO lpBitsInfo_gray = (LPBITMAPINFO) malloc(size);

	memcpy(lpBitsInfo_gray, lpBitsInfo, 40);
	lpBitsInfo_gray->bmiHeader.biBitCount = 8;
	lpBitsInfo_gray->bmiHeader.biClrUsed = 256;


	int i, j;
	for (i = 0; i < 256; i++)
	{
		lpBitsInfo_gray->bmiColors[i].rgbRed = i;
		lpBitsInfo_gray->bmiColors[i].rgbGreen = i;
		lpBitsInfo_gray->bmiColors[i].rgbBlue = i;
		lpBitsInfo_gray->bmiColors[i].rgbReserved = 0;
	}

	BYTE* lpBits_gray = (BYTE*)&lpBitsInfo_gray->bmiColors[256];
	BYTE *R, *G, *B, avg, *pixel;

	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
			//lpBits是实际位图数据的首地址，倒着存放，先拿到的数据是B，然后是G，最后是R
			B = lpBits + LineBytes * (h - 1 - i) + j * 3;
			G = B + 1;
			R = G + 1;
			avg = (*R + *G + *B) / 3;
			
			pixel = lpBits_gray + LineBytes_gray * (h - 1 - i) + j;
			*pixel = avg;
		}
	}
	free(lpBitsInfo);
	lpBitsInfo = lpBitsInfo_gray;
}

//是否灰度图像
BOOL IsGray()
{
	int r,g,b;
	if (lpBitsInfo->bmiHeader.biBitCount == 8)
	{
		r = lpBitsInfo->bmiColors[128].rgbRed;
		g = lpBitsInfo->bmiColors[128].rgbGreen;
		b = lpBitsInfo->bmiColors[128].rgbBlue;
		if (r == b && b == g)
			return TRUE;
	}
	return FALSE;
}

//访问像素点
void pixel(int i, int j, char* str)
{
	if (NULL == lpBitsInfo)
		return;
	
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	if (i >= h || j >= w)
		return;

	BYTE *pixel, bv;
	int r,g,b;

	switch (lpBitsInfo->bmiHeader.biBitCount)
	{
	case 8:
		pixel = lpBits + LineBytes * (h - 1 - i) + j;
		if (IsGray())
			sprintf(str, "灰度值:%d", *pixel);
		else
		{
			r = lpBitsInfo->bmiColors[*pixel].rgbRed;
			g = lpBitsInfo->bmiColors[*pixel].rgbGreen;
			b = lpBitsInfo->bmiColors[*pixel].rgbBlue;
			sprintf(str, "RGB(%d%d%d)", r, g, b);
		}
	case 24:
		pixel = lpBits + LineBytes * (h - 1 - i) + j * 3;
		b = *pixel;
		g = *(pixel + 1);
		r = *(pixel + 2);
		sprintf(str, "RGB(%d%d%d)", r, g, b);
		break;
	case 1:
		bv = *(lpBits + LineBytes * (h - 1 - i) + j / 8) & (1 << (7-j%8));
		if (0 == bv)
			strcpy(str, "背景点");
		else 
			strcpy(str, "前景点");
		break;
	case 4:
		bv = *(lpBits + LineBytes * (h - 1 - i) + j / 2);
		if (0 == j % 2)
			bv = bv >> 4;
		else
			bv = bv & 15;
		r = lpBitsInfo->bmiColors[bv].rgbRed;
		g = lpBitsInfo->bmiColors[bv].rgbGreen;
		b = lpBitsInfo->bmiColors[bv].rgbBlue;
		sprintf(str, "RGB(%d,%d,%d)", r, g, b);
		break;
	}
}

//直方图
DWORD H[256];
void Histogram()
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	int i, j;
	BYTE* pixel;

	for (i = 0; i < 256; i++)
		H[i] = 0;

	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
			pixel = lpBits + LineBytes * (h - 1 - i) + j;
			H[*pixel]++;
		}
	}
}

//线性化
void Linetrans(float a,float b)
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	int i,j;
	BYTE *pixel;
	float temp;
	for(i=0; i<h; i++)
	{
		for(j=0; j<w; j++)
		{
			pixel= lpBits + LineBytes * (h - 1 - i) + j;
			temp = a * (*pixel) + b;
			if(temp > 255)
				*pixel=255;
			else if (temp < 0 )
				*pixel=0;
			else
				*pixel=(BYTE)(temp+0.5);
		}
	}
}


//均衡化
void Equalize()
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	int i,j;
	BYTE *pixel;
	DWORD temp;
	BYTE Map[256];

	Histogram();

	for(i = 0 ; i < 256 ; i++)
	{
		temp=0;
		for(j=0; j<=i;  j++)
			temp+=H[j];
		Map[i]=(BYTE)(temp*255/w/h);
	}

	for(i=0; i<h; i++)
		for(j=0; j<w; j++)
		{
			pixel=lpBits + LineBytes * (h-1-i) + j;
			*pixel = Map[*pixel];
		}
}


//傅里叶正变换
#define PI 3.1415926335
void FT(complex<double>* TD,complex<double>* FD,int m )
{
	int x,u;
	double angle;
	for(u=0;u<m;u++)
	{
		FD[u]=0;
		for(x=0;x<m;x++)
		{
			angle=-2*PI*u*x/m;
			FD[u] +=TD[x] * complex<double>(cos(angle),sin(angle));
		
		}
		FD[u]/=m;
	}
}


//傅里叶反变换
void IFT(complex<double>* FD,complex<double>* TD,int m)
{
	int u,x;
	double angle;

	for(x=0;x<m;x++)
	{
		TD[x]=0;
		for(u=0;u<m;u++)
		{
			angle=2*PI*u*x/m;
			TD[x]+=FD[u]*complex<double>(cos(angle),sin(angle));
		}
	}
}


complex<double>* globalFD=NULL ;
void Fourier()
{
	int width=lpBitsInfo->bmiHeader.biWidth;
	int height=lpBitsInfo->bmiHeader.biHeight;
	int LineBytes=(width*lpBitsInfo->bmiHeader.biBitCount+31)/32 * 4;
	BYTE* lpBits =(BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];//实际位图数据地址
	int i,j;
	BYTE* pixel;
	complex<double>* TD =new complex<double>[width*height];
	complex<double>* FD =new complex<double>[width*height];

	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{
		pixel=lpBits+LineBytes*(height-1-i)+j;
		TD[width*i+j]=complex<double>(*pixel*pow(-1,i+j),0);
		}
	}
	for(i=0;i<height;i++)
	{
		FT(&TD[width*i],&FD[width*i],width);
	}

	for(i=0;i<height;i++)
		for(j=0;j<width;j++)
			TD[height*j+i]=FD[width*i+j];
	
	for(j=0;j<width;j++)
		FT(&TD[height*j],&FD[height*j],height);

	LONG size=40+1024+LineBytes*height;
	LPBITMAPINFO lpDIB_FT=(LPBITMAPINFO)malloc(size);
	memcpy(lpDIB_FT,lpBitsInfo,size);

	lpBits=(BYTE*)&lpDIB_FT->bmiColors[256];
	double temp;
	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{
			pixel=lpBits +LineBytes*(height-1-i)+j;
			temp=sqrt(FD[j*height+i].real()*FD[j*height+i].real()+FD[j*height+i].imag()*FD[j*height+i].imag())*2000;
			if(temp>255)
				temp=255;
			*pixel=(BYTE)(temp);
		}
	}

	delete TD;
	globalFD=FD;
//	delete FD;

	free(lpBitsInfo);
	lpBitsInfo=lpDIB_FT;
}


BOOL globalFDvalid()
{
	return globalFD!=NULL;
}
void IFourier()
{
	int width=lpBitsInfo->bmiHeader.biWidth;
	int height=lpBitsInfo->bmiHeader.biHeight;
	int LineBytes=(width*lpBitsInfo->bmiHeader.biBitCount+31)/32 * 4;
	BYTE* lpBits =(BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];//实际位图数据地址
	int i,j;
	BYTE* pixel;
	complex<double>* TD =new complex<double>[width*height];
	complex<double>* FD =new complex<double>[width*height];
	
	for(i=0;i<height;i++)
		for(j=0;j<width;j++)
			FD[width*i+j]=globalFD[height*j+i];
	
	for(i=0;i<height;i++)
		IFT(&FD[width*i],&TD[width*i],width);
	for(i=0;i<height;i++)
		for(j=0;j<width;j++)
			FD[height*j+i]=TD[width*i+j];
	for(j=0;j<width;j++)
		IFT(&FD[j*height],&TD[j*height],height);
	
	LONG size =40 +1024+LineBytes*height;

	LPBITMAPINFO lpDIB_IFT=(LPBITMAPINFO)malloc(size);
	memcpy(lpDIB_IFT,lpBitsInfo,size);
	lpBits=(BYTE*)&lpDIB_IFT->bmiColors[256];


	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{
			pixel=lpBits+LineBytes*(height-1-i)+j;
			*pixel =(BYTE)(TD[j*height+i].real()/pow(-1,i+j));
		}
	}

	delete TD;
	delete FD;
//	delete globalFD;
//	globalFD =NULL;
	free(lpBitsInfo);
	lpBitsInfo=lpDIB_IFT;
}

//快速傅里叶正变换
void FFT(complex<double> *TD,complex<double> *FD,int r)
{
	//计算傅立叶变换系数(2次幂)
	LONG count =1<<r;
	//计算加权系数
	int i;
	double angle;
	complex<double> *W=new complex<double>[count/2];
	for(i=0;i<count/2;i++)
	{
		angle=-i*PI*2/count;
		W[i]=complex<double>(cos(angle),sin(angle));
	}
	//将时域点写入X1；
	complex<double> *X1=new complex<double>[count];
	memcpy(X1,TD,sizeof(complex<double>)*count);

	//采用碟算法进行快速傅立叶变换，输出为频域值X2
	complex<double> *X2=new complex<double>[count];

	int k,j,p,size;
	complex<double > *temp;
	for(k=0;k<r;k++)
	{
		for(j=0;j<1<<k;j++)
		{
			size=1<<(r-k);
			for(i=0;i<size/2;i++)
			{
				p=j*size;
				X2[i+p]=X1[i+p] +X1[i+p+size/2];
				X2[i+p+size/2]=(X1[i+p]-X1[i+p+size/2])*W[i*(1<<k)];
			}
		}
		temp=X1;
		X1=X2;
		X2=temp;
	
	}
	//重新排序(码位倒序排列)
	for(j=0;j<count;j++)
	{
		p=0;
		for(i=0;i<r;i++)
		{
			if(j&(1<<i))
			{
				p+=1<<(r-i-1);
			}
		}
		FD[j]=X1[p];
		FD[j]/=count;
	}
	delete W;
	delete X1;
	delete X2;
}
void FFourier()
{
	int width=lpBitsInfo->bmiHeader.biWidth;
	int height=lpBitsInfo->bmiHeader.biHeight;
	int LineBytes=(width*lpBitsInfo->bmiHeader.biBitCount+31)/32 * 4;
	BYTE* lpBits =(BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];//实际位图数据地址

	//FFT宽度(必须为2的整数次幂)
	int FFT_w=1;
	//FFT宽度的幂数，即迭代次数
	int wp=0;
	while(FFT_w*2<=width)
	{
		FFT_w*=2;
		wp++;
	}
	int FFT_h=1;
	int hp=0;
	while(FFT_h*2<=height)
	{
		FFT_h*=2;
		hp++;
	}

	//分配内存
	complex<double>*TD=new complex<double>[FFT_w*FFT_h];
	complex<double>*FD=new complex<double>[FFT_w*FFT_h];
	
	int i,j;
	BYTE* pixel;
	for(i=0;i<FFT_h;i++)
	{
		for(j=0;j<FFT_w;j++)
		{
			//指向DIB第i行，第j个像素的指针
			pixel=lpBits+LineBytes*(height-1-i)+j;
			TD[j+FFT_w*i]=complex<double>(*pixel*pow(-1,i+j),0);
		}
	}
	for(i=0;i<FFT_h;i++)
	{
		//对y方向进行快速傅立叶变换
		FFT(&TD[FFT_w*i],&FD[FFT_w*i],wp);
	}

	//保存中间变换结果
	for(i=0;i<FFT_h;i++)
	{
		for(j=0;j<FFT_w;j++)
		{
			TD[i+FFT_h*j]=FD[j+FFT_w*i];
		}
	}
	for(i = 0; i < FFT_w; i++)
	{
		// 对x方向进行快速付立叶变换
		FFT(&TD[i * FFT_h], &FD[i * FFT_h], hp);
	}

	// 删除临时变量
	delete TD;

	//生成频谱图像
	//为频域图像分配内存
	LONG size = 40 + 1024 + LineBytes * height;
	lpDIB_FFT = (LPBITMAPINFO) malloc(size);
	if (NULL == lpDIB_FFT)
		return;
	memcpy(lpDIB_FFT, lpBitsInfo, size);

	//指向频域图像数据指针
	lpBits = (BYTE*)&lpDIB_FFT->bmiColors[256];

	double temp;
	for(i = 0; i < FFT_h; i++) // 行
	{
		for(j = 0; j < FFT_w; j++) // 列
		{
			// 计算频谱幅度
			temp = sqrt(FD[j * FFT_h + i].real() * FD[j * FFT_h + i].real() + 
				        FD[j * FFT_h + i].imag() * FD[j * FFT_h + i].imag()) *2000;
			
			// 判断是否超过255
			if (temp > 255)
			{
				// 对于超过的，直接设置为255
				temp = 255;
			}
			
			pixel = lpBits + LineBytes * (height - 1 - i) + j;

			// 更新源图像
			*pixel = (BYTE)(temp);
		}
	}

	globalFD=FD;
}


//快速傅里叶反变换
void IFFT(complex<double> * FD, complex<double> * TD, int r)
{
	// 付立叶变换点数
	LONG	count;
	// 计算付立叶变换点数
	count = 1 << r;

	// 分配运算所需存储器
	complex<double> * X = new complex<double>[count];
	// 将频域点写入X
	memcpy(X, FD, sizeof(complex<double>) * count);
	
	// 求共轭
	for(int i = 0; i < count; i++)
		X[i] = complex<double> (X[i].real(), -X[i].imag());
	
	// 调用快速付立叶变换
	FFT(X, TD, r);
	
	// 求时域点的共轭
	for(i = 0; i < count; i++)
		TD[i] = complex<double> (TD[i].real() * count, -TD[i].imag() * count);
	
	// 释放内存
	delete X;
}

void IFFourier()
{
	//图像的宽度和高度
	int width = lpBitsInfo->bmiHeader.biWidth;
	int height = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (width * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;

	// FFT宽度（必须为2的整数次方）
	int FFT_w = 1;
	// FFT宽度的幂数，即迭代次数
	int wp = 0;
	while(FFT_w * 2 <= width)
	{
		FFT_w *= 2;
		wp ++;
	}

	// FFT高度（必须为2的整数次方）
	int FFT_h = 1;
	// FFT高度的幂数，即迭代次数
	int hp = 0;
	while(FFT_h * 2 <= height)
	{
		FFT_h *= 2;
		hp ++;
	}

	// 分配内存
	complex<double>* TD = new complex<double>[FFT_w * FFT_h];
	complex<double>* FD = new complex<double>[FFT_w * FFT_h];
	
	int i, j;
	for(i = 0; i < FFT_h; i++)  // 行
		for(j = 0; j < FFT_w; j++)  // 列
			FD[j + FFT_w * i] = globalFD[i + FFT_h*j];
	
	// 沿水平方向进行快速付立叶变换
	for(i = 0; i < FFT_h; i++)
		IFFT(&FD[FFT_w * i], &TD[FFT_w * i], wp);
	
	// 保存中间变换结果
	for(i = 0; i < FFT_h; i++)
		for(j = 0; j < FFT_w; j++)
			FD[i + FFT_h * j] = TD[j + FFT_w * i];
	
	// 沿垂直方向进行快速付立叶变换
	for(i = 0; i < FFT_w; i++)
		IFFT(&FD[i * FFT_h], &TD[i * FFT_h], hp);

	//为反变换图像分配内存
	LONG size = 40 + 1024 + LineBytes * height;

	lpDIB_IFFT = (LPBITMAPINFO) malloc(size);
	if (NULL == lpDIB_IFFT)
		return;
	memcpy(lpDIB_IFFT, lpBitsInfo, size);

	//指向反变换图像数据指针
	BYTE* lpBits = (BYTE*)&lpDIB_IFFT->bmiColors[256];
	BYTE* pixel;
	double temp;
	for(i = 0; i < FFT_h; i++) // 行
	{
		for(j = 0; j < FFT_w; j++) // 列
		{
			pixel = lpBits + LineBytes * (height - 1 - i) + j;
			temp= (TD[j*FFT_h + i].real() / pow(-1, i+j));
			if (temp < 0)
				temp = 0;
			else if (temp >255)
				temp = 255;
			*pixel = (BYTE)temp;
		}
	}
	
	// 删除临时变量
	delete FD;
	delete TD;
	//delete globalFD;
}

//模板运算
void Template(int *Array,float coef)  
{
	int w=lpBitsInfo->bmiHeader.biWidth;
	int h=lpBitsInfo->bmiHeader.biHeight;
	int LineBytes=(w*lpBitsInfo->bmiHeader.biBitCount+31)/32 * 4;
	BYTE* lpBits =(BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];//实际位图数据地址
	
	BITMAPINFO* new_lpBitsInfo;
	LONG size =sizeof(BITMAPINFOHEADER) +256 *sizeof(RGBQUAD) +h*LineBytes;
	if(NULL==(new_lpBitsInfo =(LPBITMAPINFO)malloc(size)))
		return;
	//复制bmp
	memcpy(new_lpBitsInfo,lpBitsInfo,size);
	//找到新图像像素起始位置
	BYTE* lpNewBits =(BYTE*)&new_lpBitsInfo->bmiColors[new_lpBitsInfo->bmiHeader.biClrUsed];
	
	int i,j,k,l;
	BYTE *pixel,*new_pixel;
	float result;
	//行(除去边缘几行)
	for(i=1;i<h;i++)
	{	//列(除去边缘几列)
		for(j=1;j<w-1;j++)
		{
			//指向新图像第i行,第j列像素的指针
			new_pixel=lpNewBits+LineBytes*(h-1-i)+j;
			result=0;
			//计算3*3模板内像素灰度值的和
			for(k=0;k<3;k++)
			{
				for(l=0;l<3;l++)
				{
					//指向原图在模板内每个像素点的灰度值,第i-1+k行,第j-1+l个像素的指针;
					pixel=lpBits+LineBytes*(h-i-k)+j-1+l;
					//灰度值乘上模板系数后累加 
					result+=(*pixel) *Array[k*3+l];
				}
				
			}
			//乘上系数
			result *=coef;
			//保护性代码,防止灰度值超过正常范围
			if(result<0)
				*new_pixel=0;
			else if(result>255)
				*new_pixel=255;
			else
				//四舍五入
				*new_pixel=(BYTE)(result+0.5);
		}
	
	}
	free(lpBitsInfo);
	lpBitsInfo=new_lpBitsInfo;
}

//均值平滑滤波器
void AverageFilter()
{
	int Array[9];
//均值滤波
	Array[0]=1;
	Array[1]=1;
	Array[2]=1;
	Array[3]=1;
	Array[4]=1;
	Array[5]=1;
	Array[6]=1;
	Array[7]=1;
	Array[8]=1;
		
	Template(Array,(float)1/9);
/*	

*/
 }
void GossFilter()
{
	int Array[9];

	//高斯滤波
	Array[0]=1;
	Array[1]=2;
	Array[2]=1;
	Array[3]=2;
	Array[4]=4;
	Array[5]=2;
	Array[6]=1;
	Array[7]=2;
	Array[8]=1;
		
	Template(Array,(float)1/16);
}


//冒泡法排序获得中值函数
BYTE WINAPI GetMedianNum(BYTE *Array)
{
	int i,j;
	BYTE temp;
	
	for(j=0;j<8;j++)
	{
		for(i=0;i<8-j;i++)
			if(Array[i]>Array[i+1])
			{
				temp=Array[i];
				Array[i]=Array[i+1];
				Array[i+1]=temp;
			}
	}
	return Array[4];

}

//中值平滑滤波器
void MedianFilter()
{	
	int w=lpBitsInfo->bmiHeader.biWidth;
	int h=lpBitsInfo->bmiHeader.biHeight;
	int LineBytes=(w*lpBitsInfo->bmiHeader.biBitCount+31)/32 * 4;
	BYTE *lpBits =(BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];//实际位图数据地址
	
	BITMAPINFO* new_lpBitsInfo;
	LONG size =sizeof(BITMAPINFOHEADER) +256 *sizeof(RGBQUAD) +h*LineBytes;
	if(NULL==(new_lpBitsInfo =(LPBITMAPINFO)malloc(size)))
		return;
	//复制bmp
	memcpy(new_lpBitsInfo,lpBitsInfo,size);
	//找到新图像像素起始位置
	BYTE* lpNewBits =(BYTE*)&new_lpBitsInfo->bmiColors[new_lpBitsInfo->bmiHeader.biClrUsed];
	
	int i,j,k,l;
	BYTE *pixel,*new_pixel;
	BYTE Value[9];//3*3模板
	
	for(i=1;i<h-1;i++)
	{
		for(j=1;j<w-1;j++)
		{
			//指向新图第i行，第j个像素的指针
			new_pixel =lpNewBits +LineBytes *(h-1-i)+j;
			for(k=0;k<3;k++)
			{
				for(l=0;l<3;l++)
				{
					//指向原图第i-1+k行，第j-1+l个像素的指针
					pixel=lpBits+LineBytes*(h-i-k)+j-1+l;
					Value[k*3+l]=*pixel;
				}
			}
			//获得中值
			*new_pixel=GetMedianNum(Value);
		}
	}
	free(lpBitsInfo);
	lpBitsInfo=new_lpBitsInfo;
}

//梯度锐化
void GradSharp()
{
	
	int w=lpBitsInfo->bmiHeader.biWidth;
	int h=lpBitsInfo->bmiHeader.biHeight;
	int LineBytes=(w*lpBitsInfo->bmiHeader.biBitCount+31)/32 * 4;
	BYTE* lpBits =(BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];//实际位图数据地址
	
	//指向原图像的指针
	BYTE *lpSrc,*lpSrc1,*lpSrc2;

	int i,j;
	BYTE temp;

	//每行
	for(i=0;i<h-1;i++)
	{
		for(j=0;j<w-1;j++)
		{
			//指向图像第i行，第j个像素
			lpSrc = lpBits+LineBytes*(h-1-i)+j;
			//指向图像第i+1行，第j个像素
			lpSrc1 = lpBits+LineBytes*(h-2-i)+j;
			//指向图像第i行，第j+1个像素
			lpSrc2 = lpBits+LineBytes*(h-1-i)+j+1;
			//梯度算子
			temp=abs((*lpSrc)-(*lpSrc1))+abs((*lpSrc)-(*lpSrc2));

			if(temp>255)
				*lpSrc=255;
			else 
				*lpSrc=temp;
		}
	}
}

//拉普拉斯锐化
void RaplasSharpen()
{
	int  Array[9]; //3*3模板
	//拉普拉斯锐化
	Array[0]=-1;
	Array[1]=-1;
	Array[2]=-1;
	Array[3]=-1;

	Array[4]=9;

	Array[5]=-1;
	Array[6]=-1;
	Array[7]=-1;
	Array[8]=-1;

	Template(Array,(float)1);
}


//理想低-高通滤波
void Ideal_Filter_FFT(int D)
{
	//图像的宽度和高度
	int width = lpBitsInfo->bmiHeader.biWidth;
	int height = lpBitsInfo->bmiHeader.biHeight;
	int FFT_w = 1;
	while(FFT_w * 2 <= width)
		FFT_w *= 2;
	int FFT_h = 1;
	while(FFT_h * 2 <= height)
		FFT_h *= 2;

	//备份原始频域数据
	complex<double>* origin_FD = new complex<double>[FFT_w * FFT_h];
	for(int n = 0; n < FFT_w * FFT_h; n++)
		origin_FD[n] = globalFD[n];

	//频率滤波（理想高/低通滤波）
	int i, j;
	double dis;
	for(i = 0; i < FFT_h; i++)
	{
		for(j = 0; j < FFT_w; j++)
		{
			dis = sqrt((i - FFT_h/2) * (i - FFT_h/2) + (j - FFT_w/2)  * (j - FFT_w/2) + 1);
			if (D > 0) //低通
			{
				if (dis > D) 
					globalFD[i * FFT_h + j] = 0; //理想低通，截断高频
			}
			else { //高通
				if (dis <= -D) 
					globalFD[i * FFT_h + j] = 0; //理想高通，截断低频
			}
		}
	}

	//生成新的频谱图像
	int LineBytes = (width * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	LONG size = 40 + 1024 + LineBytes * height;
	BITMAPINFO* new_lpDIB_FFT = (LPBITMAPINFO) malloc(size);
	memcpy(new_lpDIB_FFT, lpDIB_FFT, size);
	BYTE* lpBits = (BYTE*)&new_lpDIB_FFT->bmiColors[new_lpDIB_FFT->bmiHeader.biClrUsed];
	double temp;
	BYTE* pixel;
	for(i = 0; i < FFT_h; i++)
	{
		for(j = 0; j < FFT_w; j++)
		{
			temp = sqrt(globalFD[j * FFT_h + i].real() * globalFD[j * FFT_h + i].real() + 
				        globalFD[j * FFT_h + i].imag() * globalFD[j * FFT_h + i].imag())*2000 ;
			if (temp > 255)
				temp = 255;
			pixel = lpBits + LineBytes * (height - 1 - i) + j;
			*pixel = (BYTE)(temp);
		}
	}
	//释放原频谱图像
	if (lpDIB_FFT)
		free(lpDIB_FFT);
	//更新新的频谱图像
	lpDIB_FFT = new_lpDIB_FFT;

	//快速傅里叶反变换
	IFFourier();

	//恢复到原始频域数据
	delete globalFD;
	globalFD = origin_FD;
}