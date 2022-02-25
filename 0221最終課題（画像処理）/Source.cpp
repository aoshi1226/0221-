#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>



typedef struct 
{
	unsigned char* red;
	unsigned char* blue;
	unsigned char* green;
}ImageData;



typedef struct
{
	unsigned char FileHeader[14];
	unsigned int Size;
	int Width, Height;
	unsigned char InfoHeader[28];
	ImageData img;
}BMP;



//BMPファイル読み込み
void ReadBmp(const char FileName[], BMP* bmp);
//画像を書き込む
void WriteBmp(const char FileName[], BMP* bmp);
//BMP構造体ののメモリ開放
void Destruct_Bmp(BMP* bmp);
//BMP構造体の情報を表示
void Show_BmpInfo(BMP* bmp);
//グレースケール変換
void ConvertToGrey(BMP* bmp);
//色変換
void ConvertColor(BMP* bmp);
//色変換2
void ConvertColor2(BMP* bmp);
//色変換3
void ConvertColor3(BMP* bmp);
//色変換4
void ConvertColor4(BMP* bmp);
//色変換5
void ConvertColor5(BMP* bmp);
//色変換6
void ConvertColor6(BMP* bmp);

int main()
	{
			BMP bmp, bmp_forGrey, bmp_forColor, bmp_forColor2, bmp_forColor3, bmp_forColor4, bmp_forColor5, bmp_forColor6;

	ReadBmp("lenna.bmp", &bmp);
	ReadBmp("lenna_out.bmp", &bmp_forGrey);
	ReadBmp("lenna_out.bmp", &bmp_forColor);
	ReadBmp("lenna_out.bmp", &bmp_forColor2);
	ReadBmp("lenna_out.bmp", &bmp_forColor3);
	ReadBmp("lenna_out.bmp", &bmp_forColor4);
	ReadBmp("lenna_out.bmp", &bmp_forColor5);
	ReadBmp("lenna_out.bmp", &bmp_forColor6);

	ConvertToGrey(&bmp_forGrey);
	ConvertColor(&bmp_forColor);
	ConvertColor2(&bmp_forColor2);
	ConvertColor3(&bmp_forColor3);
	ConvertColor4(&bmp_forColor4);
	ConvertColor5(&bmp_forColor5);
	ConvertColor6(&bmp_forColor6);

	//Show_BmpInfo(&bmp);
	Show_BmpInfo(&bmp_forGrey);
	Show_BmpInfo(&bmp_forColor);
	Show_BmpInfo(&bmp_forColor2);
	Show_BmpInfo(&bmp_forColor3);
	Show_BmpInfo(&bmp_forColor4);
	Show_BmpInfo(&bmp_forColor5);
	Show_BmpInfo(&bmp_forColor6);

	WriteBmp("lenna_out.bmp", &bmp);
	WriteBmp("lenna_out_Grey.bmp", &bmp_forGrey);
	WriteBmp("lenna_out_Color.bmp", &bmp_forColor);
	WriteBmp("lenna_out_Color2.bmp", &bmp_forColor2);
	WriteBmp("lenna_out_Color3.bmp", &bmp_forColor3);
	WriteBmp("lenna_out_Color4.bmp", &bmp_forColor4);
	WriteBmp("lenna_out_Color5.bmp", &bmp_forColor5);
	WriteBmp("lenna_out_Color6.bmp", &bmp_forColor6);

	Destruct_Bmp(&bmp);
	Destruct_Bmp(&bmp_forGrey);
	Destruct_Bmp(&bmp_forColor);
	Destruct_Bmp(&bmp_forColor2);
	Destruct_Bmp(&bmp_forColor3);
	Destruct_Bmp(&bmp_forColor4);
	Destruct_Bmp(&bmp_forColor5);
	Destruct_Bmp(&bmp_forColor6);

	return 0;
}




//BMPファイル読み込み
void ReadBmp(const char FileName[], BMP* bmp)
{
	FILE* fp;



	fopen_s(&fp, "c:\\image\\sample.bmp", "rb");
	if (fp == NULL) {
		printf("Not Found : %s", FileName);
		exit(1);
	}



	//ヘッダー情報を読み込み
	fread(bmp->FileHeader, sizeof(unsigned char), 14, fp);
	fread(&bmp->Size, sizeof(int), 1, fp);
	fread(&bmp->Width, sizeof(int), 1, fp);
	fread(&bmp->Height, sizeof(int), 1, fp);
	fread(bmp->InfoHeader, sizeof(unsigned char), 28, fp);




	//画像本体の読み込み
	bmp->img.red = (unsigned char*)malloc((bmp->Width) * (bmp->Height) * sizeof(unsigned char));
	bmp->img.blue = (unsigned char*)malloc((bmp->Width) * (bmp->Height) * sizeof(unsigned char));
	bmp->img.green = (unsigned char*)malloc((bmp->Width) * (bmp->Height) * sizeof(unsigned char));
	if (bmp->img.red == NULL) {
		printf("ﾒﾓﾘ確保に失敗\n");
		exit(1);
	}
	if (bmp->img.green == NULL) {
		printf("ﾒﾓﾘ確保に失敗\n");
		exit(1);
	}
	if (bmp->img.blue == NULL) {
		printf("ﾒﾓﾘ確保に失敗\n");
		exit(1);
	}
	//一列4byte単位の長さでないといけないので, padding処理を行う
	int stride = (bmp->Width * 3 + 3) / 4 * 4;
	unsigned char padding;



	for (int h = 0; h < bmp->Height; h++) {
		for (int w = 0; w < bmp->Width; w++) {
			fread(&bmp->img.red[w + h * bmp->Width], sizeof(unsigned char), 1, fp);
			fread(&bmp->img.green[w + h * bmp->Width], sizeof(unsigned char), 1, fp);
			fread(&bmp->img.blue[w + h * bmp->Width], sizeof(unsigned char), 1, fp);
		}
		for (int i = 0; i < stride - bmp->Width * 3; i++) {
			fread(&padding, sizeof(unsigned char), 1, fp);
		}
	}



	//読みこんだファイルを閉じる
	fclose(fp);
}



//画像を書き込む
void WriteBmp(const char FileName[], BMP * bmp)
{
	FILE* fp;
	fopen_s(&fp, FileName, "wb");
	if (fp == NULL) {
		printf("Not Found : %s\n", FileName);
		exit(1);
	}



	fwrite(bmp->FileHeader, sizeof(unsigned char), 14, fp);
	fwrite(&bmp->Size, sizeof(int), 1, fp);
	fwrite(&bmp->Width, sizeof(int), 1, fp);
	fwrite(&bmp->Height, sizeof(int), 1, fp);
	fwrite(bmp->InfoHeader, sizeof(unsigned char), 28, fp);



	//一列4byte単位でなければならないので、padding処理を行う
	int stride = (bmp->Width * 3 + 3) / 4 * 4;
	unsigned char padding = '00';



	//画像本体の書き込み
	for (int h = 0; h < bmp->Height; h++) {
		for (int w = 0; w < bmp->Width; w++) {
			fwrite(&bmp->img.red[w + h * bmp->Width], sizeof(unsigned char), 1, fp);
			fwrite(&bmp->img.green[w + h * bmp->Width], sizeof(unsigned char), 1, fp);
			fwrite(&bmp->img.blue[w + h * bmp->Width], sizeof(unsigned char), 1, fp);
		}
		for (int i = 0; i < stride - bmp->Width * 3; i++) {
			fwrite(&padding, sizeof(unsigned char), 1, fp);
		}



	}



	fclose(fp);
}



//BMP構造体のメモリを解放
void Destruct_Bmp(BMP * bmp)
{
	free(bmp->img.red);
	free(bmp->img.green);
	free(bmp->img.blue);
}



void Show_BmpInfo(BMP * bmp)
{
	printf("幅 =%d\n", bmp->Width);
	printf("高さ=%d\n", bmp->Height);
	printf("大きさ = %d\n", bmp->Height * bmp->Width);
}





//グレースケール変換
void ConvertToGrey(BMP * bmp)
{
	for (int h = 0; h < bmp->Height; h++) 
	{
		for (int w = 0; w < bmp->Width; w++)
		{
			unsigned char Average = '00';
			Average = (bmp->img.red[w + h * bmp->Width]
				+ bmp->img.green[w + h * bmp->Width]
				+ bmp->img.blue[w + h * bmp->Width]) / 3;

			bmp->img.red[w + h * bmp->Width] = Average;
			bmp->img.green[w + h * bmp->Width] = Average;
			bmp->img.blue[w + h * bmp->Width] = Average;
		}
	}
	printf("\n");
}

//色変換
void ConvertColor(BMP* bmp)
{
	for (int h = 0; h < bmp->Height; h++)
	{
		for (int w = 0; w < bmp->Width; w++)
		{
			unsigned char iro='0' ;
			
			iro = bmp->img.blue[w + h * bmp->Width];
			bmp->img.red[w + h * bmp->Width] = bmp->img.green[w + h * bmp->Width];
			bmp->img.green[w + h * bmp->Width] = iro;
			bmp->img.blue[w + h * bmp->Width] = bmp->img.red[w + h * bmp->Width];
			
		}
	}
	printf("\n");
}

//色変換
void ConvertColor2(BMP* bmp)
{
	for (int h = 0; h < bmp->Height; h++)
	{
		for (int w = 0; w < bmp->Width; w++)
		{
			unsigned char iro = '0';

			iro = bmp->img.red[w + h * bmp->Width];
			bmp->img.red[w + h * bmp->Width] = bmp->img.green[w + h * bmp->Width];
			bmp->img.green[w + h * bmp->Width] = bmp->img.blue[w + h * bmp->Width];
			bmp->img.blue[w + h * bmp->Width] = iro;
			
		}
	}
	printf("\n");
}

//色変換
void ConvertColor3(BMP* bmp)
{
	for (int h = 0; h < bmp->Height; h++)
	{
		for (int w = 0; w < bmp->Width; w++)
		{
			unsigned char iro = '0';

			iro = bmp->img.green[w + h * bmp->Width];
			bmp->img.red[w + h * bmp->Width] = bmp->img.blue[w + h * bmp->Width];
			bmp->img.green[w + h * bmp->Width] = iro;
			bmp->img.blue[w + h * bmp->Width] = bmp->img.red[w + h * bmp->Width];

		}
	}
	printf("\n");
}

void ConvertColor4(BMP* bmp)
{
	for (int h = 0; h < bmp->Height; h++)
	{
		for (int w = 0; w < bmp->Width; w++)
		{
			unsigned char iro = '0';

			iro = bmp->img.blue[w + h * bmp->Width];
			bmp->img.red[w + h * bmp->Width] = iro;
			bmp->img.green[w + h * bmp->Width] = bmp->img.red[w + h * bmp->Width];
			bmp->img.blue[w + h * bmp->Width] = bmp->img.green[w + h * bmp->Width];

		}
	}
	printf("\n");
}

void ConvertColor5(BMP* bmp)
{
	for (int h = 0; h < bmp->Height; h++)
	{
		for (int w = 0; w < bmp->Width; w++)
		{
			unsigned char iro = '0';

			iro = bmp->img.blue[w + h * bmp->Width];
			bmp->img.red[w + h * bmp->Width] = iro;
			bmp->img.green[w + h * bmp->Width] = bmp->img.red[w + h * bmp->Width];
			bmp->img.blue[w + h * bmp->Width] = bmp->img.green[w + h * bmp->Width];

		}
	}
	printf("\n");
}
void ConvertColor6(BMP* bmp)
{
	for (int h = 0; h < bmp->Height; h++)
	{
		for (int w = 0; w < bmp->Width; w++)
		{
			unsigned char iro = '0';

			iro = bmp->img.red[w + h * bmp->Width];
			bmp->img.red[w + h * bmp->Width] = bmp->img.red[w + h * bmp->Width];
			bmp->img.green[w + h * bmp->Width] = iro;
			bmp->img.blue[w + h * bmp->Width] = bmp->img.green[w + h * bmp->Width];

		}
	}
	printf("\n");
}
