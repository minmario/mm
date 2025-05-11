

// 해당 파일은 os.reader에서 조금 변형시킨 코드다
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<time.h>
#include<windows.h>
#include<process.h>
//최대 행과 열을 지정한다
#define MAX_ROW 100
#define MAX_COL 100
//계산에 사용할 이중 배열들을 지정한다
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
// thread를 생성해준다 
HANDLE pthread[100][100] = { NULL };
DWORD dwthreadID = NULL;

unsigned int WINAPI ThreadFunction(void* arg) {
	return 0;
}
// 이곳은 기존의 파일을 읽어주는 sample 함수이다 
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
	//결과물을 출력한다
	char file[15] = { "result.txt" };
	FILE* fp = stdout;
	fp = fopen(file, "w");
	//파일이 하나일시
	if (argc < 3) {
		printf("파일을 두개 이상 입력하시오\n");
		return 0;
	}
	clock_t start, end;
	start = clock();

	printf("\n");
	memset(A, 0, sizeof(int) * MAX_ROW * MAX_COL);
	Ar = Ac = 0;
	memset(B, 0, sizeof(int) * MAX_ROW * MAX_COL);
	Br = Bc = 0;
	// 전부 0으로 초기화를 시켜준 다음에 
	//첫 파일을 A로 두번쨰 파일을 B로 할당한다
	read_matrix(A, Ar, Ac, argv[1]);
	read_matrix(B, Br, Bc, argv[2]);
	ABr = ABc = 0;

	//행렬곱, pthread를 사용했다.
	for (i = 0; i < Ar; i++) 
	for (j = 0; j < Bc; j++) {
			AB[i][j] = 0;
			for (k = 0; k < Ac; k++) {
				AB[i][j] = AB[i][j] + A[i][k] * B[k][j];
				
			}
			pthread[i][j] = (HANDLE)_beginthreadex(NULL, 0, ThreadFunction, NULL, 0, (unsigned*)&dwthreadID);



	}
	
	//한계산이 끝날시 1초의 지연시간을 갖는다
	Sleep(1000);
	printf("( %dx%d ) X ( %dx%d ) = (%dx%d)\n", Ar, Ac, Br, Bc, Ar, Bc);
	//파일을 2개만 곱할시 
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
		CloseHandle(pthread);//핸들 종료
		end = clock();
		printf("Processing Time\t: %.3lf Sec\n", (end - start) / (double)1000);

	}
	//행렬의 곱셈이 3개 이상일시 
	else if (argc >= 4) {
	
		for (a = 3; a < argc; a++) {
			read_matrix(C, Cr[a - 3], Cc[a - 3], argv[a]);
			// 수행과정을 보여주는 문장이다
			Sleep(1000);
			printf("( %dx%d ) X ( %dx%d ) = (%dx%d)\n", Ar, Cr[a - 3], Cr[a - 3], Cc[a - 3], Ar, Cc[a - 3]);
			//여기서의 a는 파일 개수라 봐도 무방하다
			if (a == 3) {
				for (i = 0; i < Ar; i++)
				for (j = 0; j < Cc[a - 3]; j++) {
						DD[i][j] = 0;
						for (k = 0; k < Cr[a - 3]; k++) {
							DD[i][j] = DD[i][j] + AB[i][k] * C[k][j];

						}
						pthread[i][j] = (HANDLE)_beginthreadex(NULL, 0, ThreadFunction, NULL, 0, (unsigned*)&dwthreadID);

				}
				//마지막 행렬만 보여준다
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
			//파일이 4개일시
			//여기서 부터 패턴이 정형화 되어있다.
			//ppt에서 명시한 부분을 뺴면 값이 같다.

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
			//파일에 5개 일시
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
			//파일이 6개일시
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
			//파일이 7개일시
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
			//파일이 8개일시
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
			//파일이 9개일시
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
			//파일이 10개일시
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