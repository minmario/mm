
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<thread>
#include <Windows.h>
#include<time.h>

#define MAX_ROW 100
#define MAX_COL 100
#define MAX_THREAD 1500
#define MAX_LINE 10000
int A[MAX_ROW][MAX_COL];
int B[MAX_ROW][MAX_COL];
int C[MAX_ROW][MAX_COL];
#define DELIM		" \r\t\n"
int Ar, Ac;
int Br, Bc;
int Cr, Cc;

class MyInfo{
public:
	int r;
	int k;
	int c;
	MyInfo() { r = k = c=0; }
	~MyInfo(){}

};
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
//확실
DWORD WINAPI InstanceThread(LPVOID lpvParam) {
	MyInfo* info;
	int k;


	info = (MyInfo*)lpvParam;
	for (k = 0; k < info->k; k++) C[info->r][info->c] += A[info->r][k] * B[k][info->c];
	delete info;
	return 1;
}

void calculate_multiplication_with_thread() {
	int r, c, count, i, max;
	MyInfo* info;
	DWORD dwThreadId[MAX_THREAD] = { 0 };
	HANDLE hThread[MAX_THREAD] = { NULL };
	
	Cr = Ar;
	Cc = Bc;
	count = i = 0;
	printf("(%5dx%-5d) x (%5dx%-5d) = (%5dx%-5d)\n", Ar, Ac, Br, Bc, Cr, Cc);
	Sleep(1000);
	memset(C, 0, sizeof(int) * MAX_ROW * MAX_COL);
	for (r = 0; r < Cr; r++) {
		for (c = 0; c < Cc; c++) {
			info = new MyInfo();
			info->r = r;
			info->k = Ac;
			info->c = c;

			hThread[i] = CreateThread(NULL,0,InstanceThread,(LPVOID) info,0,&dwThreadId[i]);
			if (hThread[i] == NULL) {
				printf("ERROR: CreateThread failed %5d: %3d x %3d\n", count, r, c);
				exit(0);
			}
			i++;
			count++;
			if (i == MAX_THREAD) {
				for (i = 0; i < MAX_THREAD; i++) {
					WaitForSingleObject(hThread[i], INFINITE);
					CloseHandle(hThread[i]);
				}
				i = 0;
			}
		
		}
		max = i;
		for (i = 0; i < max; i++) {
			WaitForSingleObject(hThread[i], INFINITE);
			CloseHandle(hThread[i]);

		}
	}

}
//확실
void copy_matrix() {
	int r, c;
	Ar = Cr;
	Ac = Cc;
	for (r = 0; r < Cr; r++) {
		for (c = 0; c < Cc; c++) {
			A[r][c] = C[r][c];
		}
	}
}
//확실
void write_matrix(char* filename) {
	int r, c;
	FILE* fp;
	if (!(fp = fopen(filename, "w"))) { printf("ERROR: file open\n"); exit(0); }
	for (r = 0; r < Ar; r++) {
		for (c = 0; c < Ac; c++)
			fprintf(fp, "%10d", A[r][c]);
		fprintf(fp, "\n");
	}
	fclose(fp);
}
int main(int argc,char** argv) {
	
	int i;
	DWORD initTime, lastTime;
	double processingTime;

	initTime = GetTickCount();

	printf("\n");
	if (argc < 3) {
		printf("ERROR: input sequence!...\n"); return 0;
	}

	memset(A, 0, sizeof(int) * MAX_ROW * MAX_COL);
	memset(B, 0, sizeof(int) * MAX_ROW * MAX_COL);
	memset(C, 0, sizeof(int) * MAX_ROW * MAX_COL);
	Ar = Ac = Br = Bc = Cr = Cc = 0;

	read_matrix(A, Ar, Ac, argv[1]);

	for (i = 2; i < argc; i++) {
		read_matrix(B, Br, Bc,argv[i]);

		calculate_multiplication_with_thread();

		copy_matrix();
	}
	char result[256];
	strcpy(result,"resultsol.txt");
	write_matrix(result);

	lastTime = GetTickCount();
	processingTime = lastTime > initTime ? (lastTime - initTime) / 1000.0 : (0xFFFFFFFF - lastTime + 1 + initTime) / 1000.0;

	printf("\n");
	printf("Processing Time: %9.3f sec\n", processingTime);

	return 1;

}
