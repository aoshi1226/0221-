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



//BMP�t�@�C���ǂݍ���
void ReadBmp(const char FileName[], BMP* bmp);
//�摜����������
void WriteBmp(const char FileName[], BMP* bmp);
//BMP�\���̂̂̃������J��
void Destruct_Bmp(BMP* bmp);
//BMP�\���̂̏���\��
void Show_BmpInfo(BMP* bmp);
//�O���[�X�P�[���ϊ�
void ConvertToGrey(BMP* bmp);
//�F�ϊ�
void ConvertColor(BMP* bmp);
//�F�ϊ�2
void ConvertColor2(BMP* bmp);
//�F�ϊ�3
void ConvertColor3(BMP* bmp);
//�F�ϊ�4
void ConvertColor4(BMP* bmp);
//�F�ϊ�5
void ConvertColor5(BMP* bmp);
//�F�ϊ�6
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




//BMP�t�@�C���ǂݍ���
void ReadBmp(const char FileName[], BMP* bmp)
{
	FILE* fp;



	fopen_s(&fp, "c:\\image\\sample.bmp", "rb");
	if (fp == NULL) {
		printf("Not Found : %s", FileName);
		exit(1);
	}



	//�w�b�_�[����ǂݍ���
	fread(bmp->FileHeader, sizeof(unsigned char), 14, fp);
	fread(&bmp->Size, sizeof(int), 1, fp);
	fread(&bmp->Width, sizeof(int), 1, fp);
	fread(&bmp->Height, sizeof(int), 1, fp);
	fread(bmp->InfoHeader, sizeof(unsigned char), 28, fp);




	//�摜�{�̂̓ǂݍ���
	bmp->img.red = (unsigned char*)malloc((bmp->Width) * (bmp->Height) * sizeof(unsigned char));
	bmp->img.blue = (unsigned char*)malloc((bmp->Width) * (bmp->Height) * sizeof(unsigned char));
	bmp->img.green = (unsigned char*)malloc((bmp->Width) * (bmp->Height) * sizeof(unsigned char));
	if (bmp->img.red == NULL) {
		printf("��؊m�ۂɎ��s\n");
		exit(1);
	}
	if (bmp->img.green == NULL) {
		printf("��؊m�ۂɎ��s\n");
		exit(1);
	}
	if (bmp->img.blue == NULL) {
		printf("��؊m�ۂɎ��s\n");
		exit(1);
	}
	//���4byte�P�ʂ̒����łȂ��Ƃ����Ȃ��̂�, padding�������s��
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



	//�ǂ݂��񂾃t�@�C�������
	fclose(fp);
}



//�摜����������
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



	//���4byte�P�ʂłȂ���΂Ȃ�Ȃ��̂ŁApadding�������s��
	int stride = (bmp->Width * 3 + 3) / 4 * 4;
	unsigned char padding = '00';



	//�摜�{�̂̏�������
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



//BMP�\���̂̃����������
void Destruct_Bmp(BMP * bmp)
{
	free(bmp->img.red);
	free(bmp->img.green);
	free(bmp->img.blue);
}



void Show_BmpInfo(BMP * bmp)
{
	printf("�� =%d\n", bmp->Width);
	printf("����=%d\n", bmp->Height);
	printf("�傫�� = %d\n", bmp->Height * bmp->Width);
}





//�O���[�X�P�[���ϊ�
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

//�F�ϊ�
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

//�F�ϊ�
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

//�F�ϊ�
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
