#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
void InverseImage(BYTE* Img, BYTE *Out, int W, int H)
{
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++)
	{
		Out[i] = 255 - Img[i];
	}
}
void BrightnessAdj(BYTE* Img, BYTE* Out, int W, int H, int Val)
{
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++)
	{
		if (Img[i] + Val > 255)
		{
			Out[i] = 255;
		}
		else if (Img[i] + Val < 0)
		{
			Out[i] = 0;
		}
		else 	Out[i] =Img[i] + Val;
	}
}
void ContrastAdj(BYTE* Img, BYTE* Out, int W, int H, double Val)
{
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++)
	{
		if (Img[i] * Val > 255.0)
		{
			Out[i] = 255;
		}
		else 	Out[i] = (BYTE)(Img[i] * Val);
	}
}

void ObtainHistogram(BYTE* Img, int* Histo, int W, int H)
{
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++) {
		Histo[Img[i]]++;
	}
}

void ObtainAHistogram(int* Histo, int* AHisto)
{
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j <= i; j++) {
			AHisto[i] += Histo[j];
		}
	}
}

void HistogramStretching(BYTE * Img, BYTE * Out, int * Histo, int W, int H)
{
	int ImgSize = W * H;
	BYTE Low, High;
	for (int i = 0; i < 256; i++) {
		if (Histo[i] != 0) {
			Low = i;
			break;
		}
	}
	for (int i = 255; i >= 0; i--) {
		if (Histo[i] != 0) {
			High = i;
			break;
		}
	}
	for (int i = 0; i < ImgSize; i++) {
		Out[i] = (BYTE)((Img[i] - Low) / (double)(High - Low) * 255.0);
	}
}
void HistogramEqualization(BYTE* Img, BYTE* Out, int* AHisto, int W, int H)
{
	int ImgSize = W * H;
	int Nt = W * H, Gmax = 255;
	double Ratio = Gmax / (double)Nt;
	BYTE NormSum[256];
	for (int i = 0; i < 256; i++) {
		NormSum[i] = (BYTE)(Ratio * AHisto[i]);
	}
	for (int i = 0; i < ImgSize; i++)
	{
		Out[i] = NormSum[Img[i]];
	}
}

void Binarization(BYTE * Img, BYTE * Out, int W, int H, BYTE Threshold)
{
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++) {
		if (Img[i] < Threshold) Out[i] = 0;
		else Out[i] = 255;
	}
}

//int GozalezBinThresh()
//{
//
//}

void AverageConv(BYTE* Img, BYTE* Out, int W, int H) // 박스평활화
{
	double Kernel[3][3] = {0.11111, 0.11111, 0.11111,
										0.11111, 0.11111, 0.11111,
										0.11111, 0.11111, 0.11111	};
	double SumProduct = 0.0;
	for (int i = 1; i < H-1; i++) { // Y좌표 (행)
		for (int j = 1; j < W-1; j++) { // X좌표 (열)
			for (int m = -1; m <= 1; m++) { // Kernel 행
				for (int n = -1; n <= 1; n++) { // Kernel 열
					SumProduct += Img[(i+m)*W + (j+n)] * Kernel[m+1][n+1];
				}
			}
			Out[i * W + j] = (BYTE)SumProduct;
			SumProduct = 0.0;
		}
	}
}

void GaussAvrConv(BYTE* Img, BYTE* Out, int W, int H) // 가우시안평활화
{
	double Kernel[3][3] = {0.0625, 0.125, 0.0625,
										0.125, 0.25, 0.125,
										0.0625, 0.125, 0.0625 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y좌표 (행)
		for (int j = 1; j < W - 1; j++) { // X좌표 (열)
			for (int m = -1; m <= 1; m++) { // Kernel 행
				for (int n = -1; n <= 1; n++) { // Kernel 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			Out[i * W + j] = (BYTE)SumProduct;
			SumProduct = 0.0;
		}
	}
}

void Prewitt_X_Conv(BYTE* Img, BYTE* Out, int W, int H) // Prewitt 마스크 X
{
	double Kernel[3][3] = { -1.0, 0.0, 1.0,
										-1.0, 0.0, 1.0,
										-1.0, 0.0, 1.0 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y좌표 (행)
		for (int j = 1; j < W - 1; j++) { // X좌표 (열)
			for (int m = -1; m <= 1; m++) { // Kernel 행
				for (int n = -1; n <= 1; n++) { // Kernel 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 765  =====> 0 ~ 255
			Out[i * W + j] = abs((long)SumProduct) / 3;
			SumProduct = 0.0;
		}
	}
}

void Prewitt_Y_Conv(BYTE* Img, BYTE* Out, int W, int H) // Prewitt 마스크 X
{
	double Kernel[3][3] = { -1.0, -1.0, -1.0,
										0.0, 0.0, 0.0,
										1.0, 1.0, 1.0 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y좌표 (행)
		for (int j = 1; j < W - 1; j++) { // X좌표 (열)
			for (int m = -1; m <= 1; m++) { // Kernel 행
				for (int n = -1; n <= 1; n++) { // Kernel 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 765  =====> 0 ~ 255
			Out[i * W + j] = abs((long)SumProduct) / 3;
			SumProduct = 0.0;
		}
	}
}

void Sobel_X_Conv(BYTE* Img, BYTE* Out, int W, int H) // Prewitt 마스크 X
{
	double Kernel[3][3] = { -1.0, 0.0, 1.0,
										-2.0, 0.0, 2.0,
										-1.0, 0.0, 1.0 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y좌표 (행)
		for (int j = 1; j < W - 1; j++) { // X좌표 (열)
			for (int m = -1; m <= 1; m++) { // Kernel 행
				for (int n = -1; n <= 1; n++) { // Kernel 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 1020  =====> 0 ~ 255
			Out[i * W + j] = abs((long)SumProduct) / 4;
			SumProduct = 0.0;
		}
	}
}

void Sobel_Y_Conv(BYTE* Img, BYTE* Out, int W, int H) // Prewitt 마스크 X
{
	double Kernel[3][3] = { -1.0, -2.0, -1.0,
										0.0, 0.0, 0.0,
										1.0, 2.0, 1.0 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y좌표 (행)
		for (int j = 1; j < W - 1; j++) { // X좌표 (열)
			for (int m = -1; m <= 1; m++) { // Kernel 행
				for (int n = -1; n <= 1; n++) { // Kernel 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 765  =====> 0 ~ 255
			Out[i * W + j] = abs((long)SumProduct) / 4;
			SumProduct = 0.0;
		}
	}
}

void Laplace_Conv(BYTE* Img, BYTE* Out, int W, int H) // Prewitt 마스크 X
{
	double Kernel[3][3] = { -1.0, -1.0, -1.0,
										-1.0, 8.0, -1.0,
										-1.0, -1.0, -1.0 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y좌표 (행)
		for (int j = 1; j < W - 1; j++) { // X좌표 (열)
			for (int m = -1; m <= 1; m++) { // Kernel 행
				for (int n = -1; n <= 1; n++) { // Kernel 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 2040  =====> 0 ~ 255
			Out[i * W + j] = abs((long)SumProduct) / 8;
			SumProduct = 0.0;
		}
	}
}

void Laplace_Conv_DC(BYTE* Img, BYTE* Out, int W, int H) // Prewitt 마스크 X
{
	double Kernel[3][3] = { -1.0, -1.0, -1.0,
										-1.0, 9.0, -1.0,
										-1.0, -1.0, -1.0 };
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++) { // Y좌표 (행)
		for (int j = 1; j < W - 1; j++) { // X좌표 (열)
			for (int m = -1; m <= 1; m++) { // Kernel 행
				for (int n = -1; n <= 1; n++) { // Kernel 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			//Out[i * W + j] = abs((long)SumProduct) / 8;
			if (SumProduct > 255.0) Out[i * W + j] = 255;
			else if (SumProduct < 0.0) Out[i * W + j] = 0;
			else Out[i * W + j] = (BYTE)SumProduct;
			SumProduct = 0.0;
		}
	}
}

void SaveBMPFile(BITMAPFILEHEADER hf, BITMAPINFOHEADER hInfo, 
	RGBQUAD* hRGB, BYTE* Output, int W, int H, const char* FileName)
{
	FILE * fp = fopen(FileName, "wb");
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
	fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output, sizeof(BYTE), W*H, fp);
	fclose(fp);
}

void swap(BYTE* a, BYTE* b) 
{
	BYTE temp = *a;
	*a = *b;
	*b = temp;
}

BYTE Median(BYTE* arr, int size)
{
	// 오름차순 정렬
	const int S = size;
	for (int i = 0; i < size - 1; i++) // pivot index
	{
		for (int j = i + 1; j < size; j++) // 비교대상 index
		{
			if (arr[i] > arr[j]) 	swap(&arr[i], &arr[j]);
		}
	}
	return arr[S/2];
}

BYTE MaxPooling(BYTE* arr, int size)
{
	// 오름차순 정렬
	const int S = size;
	for (int i = 0; i < size - 1; i++) // pivot index
	{
		for (int j = i + 1; j < size; j++) // 비교대상 index
		{
			if (arr[i] > arr[j]) 	swap(&arr[i], &arr[j]);
		}
	}
	return arr[S-1];
}

BYTE MinPooling(BYTE* arr, int size)
{
	// 오름차순 정렬
	const int S = size;
	for (int i = 0; i < size - 1; i++) // pivot index
	{
		for (int j = i + 1; j < size; j++) // 비교대상 index
		{
			if (arr[i] > arr[j]) 	swap(&arr[i], &arr[j]);
		}
	}
	return arr[0];
}

int main()
{
	BITMAPFILEHEADER hf; // 14바이트
	BITMAPINFOHEADER hInfo; // 40바이트
	RGBQUAD hRGB[256]; // 1024바이트
	FILE* fp;
	fp = fopen("lenna_impulse.bmp", "rb");
	if (fp == NULL) {
		printf("File not found!\n");
		return -1;
	}
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fread(hRGB, sizeof(RGBQUAD), 256, fp);
	int ImgSize = hInfo.biWidth * hInfo.biHeight;
	BYTE * Image = (BYTE *)malloc(ImgSize);
	BYTE * Temp = (BYTE*)malloc(ImgSize); // 임시배열
	BYTE* Output = (BYTE*)malloc(ImgSize);
	fread(Image, sizeof(BYTE), ImgSize, fp);
	fclose(fp);

	int Histo[256] = { 0 };
	int AHisto[256] = { 0 };

	/*ObtainHistogram(Image, Histo, hInfo.biWidth, hInfo.biHeight);
	ObtainAHistogram(Histo, AHisto);
	HistogramEqualization(Image, Output, AHisto, hInfo.biWidth, hInfo.biHeight);*/
	/*int Thres = GozalezBinThresh();
	Binarization(Image, Output, hInfo.biWidth, hInfo.biHeight, Thres);*/

	//GaussAvrConv(Image, Output, hInfo.biWidth, hInfo.biHeight);
	
	/*Sobel_X_Conv(Image, Temp, hInfo.biWidth, hInfo.biHeight);
	Sobel_Y_Conv(Image, Output, hInfo.biWidth, hInfo.biHeight);
	for (int i = 0; i < ImgSize; i++) {
		if (Temp[i] > Output[i]) 	Output[i] = Temp[i];
	}
	Binarization(Output, Output, hInfo.biWidth, hInfo.biHeight, 40);*/
	/*GaussAvrConv(Image, Temp, hInfo.biWidth, hInfo.biHeight);
	Laplace_Conv_DC(Temp, Output, hInfo.biWidth, hInfo.biHeight);*/
	
	//HistogramStretching(Image, Output, Histo, hInfo.biWidth, hInfo.biHeight);
	//InverseImage(Image, Output, hInfo.biWidth, hInfo.biHeight);
	//BrightnessAdj(Image, Output, hInfo.biWidth, hInfo.biHeight, 70);
	//ContrastAdj(Image, Output, hInfo.biWidth, hInfo.biHeight, 0.5);




	/*5x5 bmp*/

	/* Median filtering */
	int Length = 5;  // 마스크의 한 변의 길이
	int Margin = Length / 2; // Margin의 길이, margin은 영상 영역 밖의 처리를 할 상황을 막아줌, 3X3일때 1, 5x5일 때 2, 7X7일 때 3.. 의 방식으로 증가
	int WSize = Length * Length; // Window size이며 커널의 사이즈를 의미한다.
	BYTE* temp = (BYTE*)malloc(sizeof(BYTE) * WSize); // 일시적으로 커널에 값을 저장해줄 temp를 window size만큼 동적할당 해준다.
	int W = hInfo.biWidth, H = hInfo.biHeight; // W는 Image의 Width를 의미하며, H는 Image의 Height를 의미한다. 
	int i, j, m, n; // 추후에 사용할 i,j,m,n의 변수 선언이다.

	for (i = Margin; i < H - Margin; i++) {  // 영상 영역 밖의 처리를 막기 위해 마진부터 i를 시작하며 Image의 열 값을 시작으로 1차 반복문을 연다.
		for (j = Margin; j < W - Margin; j++) { //영상 영역 밖의 처리를 막기위해 마진부터 j를 시작하며 Image의 행 값을 시작으로 2차 반복문을 연다.
			for (m = -Margin; m <= Margin; m++) { // m은 Kernel의 열을 의미해주고 -margin부터 시작함으로써 kernel의 첫 열부터 3차 반복문을 연다.
				for (n = -Margin; n <= Margin; n++) { //n은 Kernel의 행을 의미해주고 -margin부터 시작함으로써 kernel의 첫 행부터 3차 반복문을 연다.

					temp[(m + Margin) * Length + (n + Margin)] = Image[(i + m) * W + j + n]; //2차원 배열의 원소를 1차원 배열의 곱과 합으로 픽셀 위치를 나타내며 그 값을 임의로 동적할당한 temp에 9개의 값을 넣어준다.
				}
			}
			Output[i * W + j] = Median(temp, WSize); // temp에 저장된 값을 Sorting해주고 그 중간 값을 return하는 median함수를 사용해 Output의 해당 인덱스에 return 값을 저장한다.
		}
	}
	free(temp); //동적할당한 temp를 free선언해줘 할당한 메모리를 해제해준다.

	/* Median filtering */
	
	//AverageConv(Image, Output, hInfo.biWidth, hInfo.biHeight); // 평활화를 해준다.
		
	SaveBMPFile(hf, hInfo, hRGB, Output, hInfo.biWidth, hInfo.biHeight, "median_5.bmp"); // 파일 저장하는 것을 별도의 함수로 짧게 표현했다.

	/*5x5 bmp*/

	/*7x7 bmp*/
	/* Median filtering */
	Length = 7;  // 마스크의 한 변의 길이
	Margin = Length / 2; // Margin의 길이, margin은 영상 영역 밖의 처리를 할 상황을 막아줌, 3X3일때 1, 5x5일 때 2, 7X7일 때 3.. 의 방식으로 증가
	WSize = Length * Length; // Window size이며 커널의 사이즈를 의미한다.
	BYTE* temp1 = (BYTE*)malloc(sizeof(BYTE) * WSize); // 일시적으로 커널에 값을 저장해줄 temp를 window size만큼 동적할당 해준다.
	W = hInfo.biWidth, H = hInfo.biHeight; // W는 Image의 Width를 의미하며, H는 Image의 Height를 의미한다. 

	for (i = Margin; i < H - Margin; i++) {  // 영상 영역 밖의 처리를 막기 위해 마진부터 i를 시작하며 Image의 열 값을 시작으로 1차 반복문을 연다.
		for (j = Margin; j < W - Margin; j++) { //영상 영역 밖의 처리를 막기위해 마진부터 j를 시작하며 Image의 행 값을 시작으로 2차 반복문을 연다.
			for (m = -Margin; m <= Margin; m++) { // m은 Kernel의 열을 의미해주고 -margin부터 시작함으로써 kernel의 첫 열부터 3차 반복문을 연다.
				for (n = -Margin; n <= Margin; n++) { //n은 Kernel의 행을 의미해주고 -margin부터 시작함으로써 kernel의 첫 행부터 3차 반복문을 연다.

					temp1[(m + Margin) * Length + (n + Margin)] = Image[(i + m) * W + j + n]; //2차원 배열의 원소를 1차원 배열의 곱과 합으로 픽셀 위치를 나타내며 그 값을 임의로 동적할당한 temp에 9개의 값을 넣어준다.
				}
			}
			Output[i * W + j] = Median(temp1, WSize); // temp에 저장된 값을 Sorting해주고 그 중간 값을 return하는 median함수를 사용해 Output의 해당 인덱스에 return 값을 저장한다.
		}
	}
	free(temp1); //동적할당한 temp를 free선언해줘 할당한 메모리를 해제해준다.

	/* Median filtering */

	//AverageConv(Image, Output, hInfo.biWidth, hInfo.biHeight); // 평활화를 해준다.

	SaveBMPFile(hf, hInfo, hRGB, Output, hInfo.biWidth, hInfo.biHeight, "median_7.bmp"); // 파일 저장하는 것을 별도의 함수로 짧게 표현했다.

	/*7x7 bmp*/
	/*9x9 bmp*/
		/* Median filtering */
	Length = 9;  // 마스크의 한 변의 길이
	Margin = Length / 2; // Margin의 길이, margin은 영상 영역 밖의 처리를 할 상황을 막아줌, 3X3일때 1, 5x5일 때 2, 7X7일 때 3.. 의 방식으로 증가
	WSize = Length * Length; // Window size이며 커널의 사이즈를 의미한다.
	BYTE* temp2 = (BYTE*)malloc(sizeof(BYTE) * WSize); // 일시적으로 커널에 값을 저장해줄 temp를 window size만큼 동적할당 해준다.
	W = hInfo.biWidth, H = hInfo.biHeight; // W는 Image의 Width를 의미하며, H는 Image의 Height를 의미한다. 
	

	for (i = Margin; i < H - Margin; i++) {  // 영상 영역 밖의 처리를 막기 위해 마진부터 i를 시작하며 Image의 열 값을 시작으로 1차 반복문을 연다.
		for (j = Margin; j < W - Margin; j++) { //영상 영역 밖의 처리를 막기위해 마진부터 j를 시작하며 Image의 행 값을 시작으로 2차 반복문을 연다.
			for (m = -Margin; m <= Margin; m++) { // m은 Kernel의 열을 의미해주고 -margin부터 시작함으로써 kernel의 첫 열부터 3차 반복문을 연다.
				for (n = -Margin; n <= Margin; n++) { //n은 Kernel의 행을 의미해주고 -margin부터 시작함으로써 kernel의 첫 행부터 3차 반복문을 연다.

					temp2[(m + Margin) * Length + (n + Margin)] = Image[(i + m) * W + j + n]; //2차원 배열의 원소를 1차원 배열의 곱과 합으로 픽셀 위치를 나타내며 그 값을 임의로 동적할당한 temp에 9개의 값을 넣어준다.
				}
			}
			Output[i * W + j] = Median(temp2, WSize); // temp에 저장된 값을 Sorting해주고 그 중간 값을 return하는 median함수를 사용해 Output의 해당 인덱스에 return 값을 저장한다.
		}
	}
	free(temp2); //동적할당한 temp를 free선언해줘 할당한 메모리를 해제해준다.

	/* Median filtering */

	//AverageConv(Image, Output, hInfo.biWidth, hInfo.biHeight); // 평활화를 해준다.

	SaveBMPFile(hf, hInfo, hRGB, Output, hInfo.biWidth, hInfo.biHeight, "median_9.bmp"); // 파일 저장하는 것을 별도의 함수로 짧게 표현했다.

	/*9x9 bmp*/

	
	free(Image);
	free(Output);
	free(Temp);


	return 0;
}