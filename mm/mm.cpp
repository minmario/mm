

// �ش� ������ os.reader���� ���� ������Ų �ڵ��
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<time.h>
#include<windows.h>
#include<process.h>
//�ִ� ��� ���� �����Ѵ�
#define MAX_ROW 100
#define MAX_COL 100
//��꿡 ����� ���� �迭���� �����Ѵ�
int A[MAX_ROW][MAX_COL];
int Ar, Ac;
int B[MAX_ROW][MAX_COL];
int Br, Bc;
int AB[MAX_ROW][MAX_COL];
int ABr, ABc;
int C[MAX_ROW][MAX_COL];
int Cr[50] = { 0 };
int Cc[50] = { 0 };

int DD[MAX_ROW][MAX_COL];
int FF[MAX_ROW][MAX_COL];
int GG[MAX_ROW][MAX_COL];
int HH[MAX_ROW][MAX_COL];
int II[MAX_ROW][MAX_COL];
int JJ[MAX_ROW][MAX_COL];
int KK[MAX_ROW][MAX_COL];
int LL[MAX_ROW][MAX_COL];

int a, i, j, k;
// thread�� �������ش� 
HANDLE pthread[100][100] = { NULL };
DWORD dwthreadID = NULL;

unsigned int WINAPI ThreadFunction(void* arg) {
	return 0;
}
// �̰��� ������ ������ �о��ִ� sample �Լ��̴� 
#define DELIM		" \r\t\n"
#define MAX_LINE	1000
void read_matrix(int A[MAX_ROW][MAX_COL], int& Ar, int& Ac, char* filename)
{
	char line[MAX_LINE], * tok;
	FILE* fp;

	if (!(fp = fopen(filename, "r")))
	{
		printf("ERROR: file open\n");
		exit(0);
	}
	Ar = 0;
	while (fgets(line, MAX_LINE, fp))
	{

		tok = strtok(line, DELIM);
		Ac = 0;
		do
		{
			A[Ar][Ac++] = atoi(tok);

		} while (tok = strtok(NULL, DELIM));
		Ar++;
	}
	fclose(fp);
}



int main(int argc, char** argv) {
	//������� ����Ѵ�
	char file[15] = { "result.txt" };
	FILE* fp = stdout;
	fp = fopen(file, "w");
	//������ �ϳ��Ͻ�
	if (argc < 3) {
		printf("������ �ΰ� �̻� �Է��Ͻÿ�\n");
		return 0;
	}
	clock_t start, end;
	start = clock();

	printf("\n");
	memset(A, 0, sizeof(int) * MAX_ROW * MAX_COL);
	Ar = Ac = 0;
	memset(B, 0, sizeof(int) * MAX_ROW * MAX_COL);
	Br = Bc = 0;
	// ���� 0���� �ʱ�ȭ�� ������ ������ 
	//ù ������ A�� �ι��� ������ B�� �Ҵ��Ѵ�
	read_matrix(A, Ar, Ac, argv[1]);
	read_matrix(B, Br, Bc, argv[2]);
	ABr = ABc = 0;

	//��İ�, pthread�� ����ߴ�.
	for (i = 0; i < Ar; i++) 
	for (j = 0; j < Bc; j++) {
			AB[i][j] = 0;
			for (k = 0; k < Ac; k++) {
				AB[i][j] = AB[i][j] + A[i][k] * B[k][j];
				
			}
			pthread[i][j] = (HANDLE)_beginthreadex(NULL, 0, ThreadFunction, NULL, 0, (unsigned*)&dwthreadID);



	}
	
	//�Ѱ���� ������ 1���� �����ð��� ���´�
	Sleep(1000);
	printf("( %dx%d ) X ( %dx%d ) = (%dx%d)\n", Ar, Ac, Br, Bc, Ar, Bc);
	//������ 2���� ���ҽ� 
	if (argc == 3) {
		printf("\n");
		for (i = 0; i < Ar; i++) {
			for (j = 0; j < Bc; j++) {
				printf("%d\t", AB[i][j]);
				fprintf(fp, "%5d", AB[i][j]);
			}
			printf("\n");
			fprintf(fp, "\n");
		}
		printf("\n");
		CloseHandle(pthread);//�ڵ� ����
		end = clock();
		printf("Processing Time\t: %.3lf Sec\n", (end - start) / (double)1000);

	}
	//����� ������ 3�� �̻��Ͻ� 
	else if (argc >= 4) {
	
		for (a = 3; a < argc; a++) {
			read_matrix(C, Cr[a - 3], Cc[a - 3], argv[a]);
			// ��������� �����ִ� �����̴�
			Sleep(1000);
			printf("( %dx%d ) X ( %dx%d ) = (%dx%d)\n", Ar, Cr[a - 3], Cr[a - 3], Cc[a - 3], Ar, Cc[a - 3]);
			//���⼭�� a�� ���� ������ ���� �����ϴ�
			if (a == 3) {
				for (i = 0; i < Ar; i++)
				for (j = 0; j < Cc[a - 3]; j++) {
						DD[i][j] = 0;
						for (k = 0; k < Cr[a - 3]; k++) {
							DD[i][j] = DD[i][j] + AB[i][k] * C[k][j];

						}
						pthread[i][j] = (HANDLE)_beginthreadex(NULL, 0, ThreadFunction, NULL, 0, (unsigned*)&dwthreadID);

				}
				//������ ��ĸ� �����ش�
				if (a == argc - 1) {
					printf("\n");
					for (i = 0; i < Ar; i++) {
						for (j = 0; j < Cc[a - 3]; j++) {
							printf("%d\t", DD[i][j]);
							fprintf(fp, "%5d", DD[i][j]);
						}
						printf("\n");
						fprintf(fp, "\n");
					}
					printf("\n");
					CloseHandle(pthread);
					end = clock();
					printf("Processing Time\t: %.3lf Sec\n", (end - start) / (double)1000);
				}
			}
			//������ 4���Ͻ�
			//���⼭ ���� ������ ����ȭ �Ǿ��ִ�.
			//ppt���� ����� �κ��� ���� ���� ����.

			else if (a == 4) {
				for (i = 0; i < Cr[a - 4]; i++) 
				for (j = 0; j < Cc[a - 3]; j++) {
						FF[i][j] = 0;
						for (k = 0; k < Cr[a - 3]; k++) {
							FF[i][j] = FF[i][j] + DD[i][k] * C[k][j];
							
						}
						pthread[i][j] = (HANDLE)_beginthreadex(NULL, 0, ThreadFunction, NULL, 0, (unsigned*)&dwthreadID);
					
				}
				if (a == argc - 1) {
					printf("\n");
					for (i = 0; i < Ar; i++) {
						for (j = 0; j < Cc[a - 3]; j++) {
							printf("%d\t", FF[i][j]);
							fprintf(fp, "%5d", FF[i][j]);
						}
						printf("\n");
						fprintf(fp, "\n");

					}
					printf("\n");
					CloseHandle(pthread);
					end = clock();
					printf("Processing Time\t: %.3lf Sec\n", (end - start) / (double)1000);
				}
			}
			//���Ͽ� 5�� �Ͻ�
			else if (a == 5) {
				for (i = 0; i < Cr[a - 5]; i++) 
				for (j = 0; j < Cc[a - 4]; j++) {
						GG[i][j] = 0;
						for (k = 0; k < Cr[a - 4]; k++) {
							GG[i][j] = GG[i][j] + FF[i][k] * C[k][j];
							
						}
						pthread[i][j] = (HANDLE)_beginthreadex(NULL, 0, ThreadFunction, NULL, 0, (unsigned*)&dwthreadID);
					
				}
				if (a == argc - 1) {
					printf("\n");
					for (i = 0; i < Ar; i++) {
						for (j = 0; j < Cc[a - 3]; j++) {
							printf("%d\t", GG[i][j]);
							fprintf(fp, "%5d", GG[i][j]);
						}
						printf("\n");
						fprintf(fp, "\n");

					}
					printf("\n");
					CloseHandle(pthread);
					end = clock();
					printf("Processing Time\t: %.3lf Sec\n", (end - start) / (double)1000);
				}
			}
			//������ 6���Ͻ�
			else if (a == 6) {
				for (i = 0; i < Cr[a - 6]; i++) 
				for (j = 0; j < Cc[a - 5]; j++) {
						HH[i][j] = 0;
						for (k = 0; k < Cr[a - 5]; k++) {
							HH[i][j] = HH[i][j] + GG[i][k] * C[k][j];
							
						}
						pthread[i][j] = (HANDLE)_beginthreadex(NULL, 0, ThreadFunction, NULL, 0, (unsigned*)&dwthreadID);
				}
				
				if (a == argc - 1) {
					printf("\n");
					for (i = 0; i < Ar; i++) {
						for (j = 0; j < Cc[a - 3]; j++) {
							printf("%d\t", HH[i][j]);
							fprintf(fp, "%5d", HH[i][j]);
						}
						printf("\n");
						fprintf(fp, "\n");

					}
					printf("\n");
					CloseHandle(pthread);
					end = clock();
					printf("Processing Time\t: %.3lf Sec\n", (end - start) / (double)1000);
				}
			}
			//������ 7���Ͻ�
			else if (a == 7) {
				for (i = 0; i < Cr[a - 7]; i++) 
				for (j = 0; j < Cc[a - 6]; j++) {
						II[i][j] = 0;
						for (k = 0; k < Cr[a - 6]; k++) {
							II[i][j] = II[i][j] + HH[i][k] * C[k][j];
							
						}
						pthread[i][j] = (HANDLE)_beginthreadex(NULL, 0, ThreadFunction, NULL, 0, (unsigned*)&dwthreadID);
					
				}
				if (a == argc - 1) {
					printf("\n");
					for (i = 0; i < Ar; i++) {
						for (j = 0; j < Cc[a - 3]; j++) {
							printf("%d\t", II[i][j]);
							fprintf(fp, "%5d", II[i][j]);
						}
						printf("\n");
						fprintf(fp, "\n");

					}
					printf("\n");
					CloseHandle(pthread);
					end = clock();
					printf("Processing Time\t: %.3lf Sec\n", (end - start) / (double)1000);
				}
			}
			//������ 8���Ͻ�
			else if (a == 8) {
				for (i = 0; i < Cr[a - 8]; i++) 
				for (j = 0; j < Cc[a - 7]; j++) {
						JJ[i][j] = 0;
						for (k = 0; k < Cr[a - 7]; k++) {
							JJ[i][j] = JJ[i][j] + II[i][k] * C[k][j];
							
						}
						pthread[i][j] = (HANDLE)_beginthreadex(NULL, 0, ThreadFunction, NULL, 0, (unsigned*)&dwthreadID);
					
				}
				if (a == argc - 1) {
					printf("\n");
					for (i = 0; i < Ar; i++) {
						for (j = 0; j < Cc[a - 3]; j++) {
							printf("%d\t", JJ[i][j]);
							fprintf(fp, "%5d", JJ[i][j]);
						}
						printf("\n");
						fprintf(fp, "\n");

					}
					printf("\n");
					CloseHandle(pthread);
					end = clock();
					printf("Processing Time\t: %.3lf Sec\n", (end - start) / (double)1000);
				}
			}
			//������ 9���Ͻ�
			else if (a == 9) {
				for (i = 0; i < Cr[a - 9]; i++) 
				for (j = 0; j < Cc[a - 8]; j++) {
						KK[i][j] = 0;
						for (k = 0; k < Cr[a - 8]; k++) {
							KK[i][j] = KK[i][j] + JJ[i][k] * C[k][j];
							
						}
						pthread[i][j] = (HANDLE)_beginthreadex(NULL, 0, ThreadFunction, NULL, 0, (unsigned*)&dwthreadID);
					
				}
				if (a == argc - 1) {
					printf("\n");
					for (i = 0; i < Ar; i++) {
						for (j = 0; j < Cc[a - 3]; j++) {
							printf("%d\t", KK[i][j]);
							fprintf(fp, "%5d", KK[i][j]);
						}
						printf("\n");
						fprintf(fp, "\n");

					}
					printf("\n");
					CloseHandle(pthread);
					end = clock();
					printf("Processing Time\t: %.3lf Sec\n", (end - start) / (double)1000);
				}
			}
			//������ 10���Ͻ�
			else if (a == 10) {
				for (i = 0; i < Cr[a - 10]; i++) 
				for (j = 0; j < Cc[a - 9]; j++) {
						LL[i][j] = 0;
						for (k = 0; k < Cr[a - 9]; k++) {
							LL[i][j] = LL[i][j] + KK[i][k] * C[k][j];
							
						}
						pthread[i][j] = (HANDLE)_beginthreadex(NULL, 0, ThreadFunction, NULL, 0, (unsigned*)&dwthreadID);
					
				}
				if (a == argc - 1) {
					printf("\n");
					for (i = 0; i < Ar; i++) {
						for (j = 0; j < Cc[a - 3]; j++) {
							printf("%d\t", LL[i][j]);
							fprintf(fp, "%5d", LL[i][j]);
						}
						printf("\n");
						fprintf(fp, "\n");

					}
					printf("\n");
					CloseHandle(pthread);
					end = clock();
					printf("Processing Time\t: %.3lf Sec\n", (end - start) / (double)1000);
				}
			}
			


		}


	}




}