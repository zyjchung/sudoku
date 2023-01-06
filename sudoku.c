#include <stdio.h>
//#include <mem.h>


#define	BOARD_SIZE		(9)
#define	GRAPH_UNIT_SIZE	(3)
#define GRAPH_SIZE		(BOARD_SIZE * GRAPH_UNIT_SIZE)

char	baseBoard[BOARD_SIZE][BOARD_SIZE];
int 	candidateBoard[BOARD_SIZE][BOARD_SIZE][BOARD_SIZE];
char	graphBoard[GRAPH_SIZE][GRAPH_SIZE];



void DrawGraphBoard(void)
{
	int i,j;

	for(i=0;i<GRAPH_SIZE;i++)
	{
		printf("\n");
		if(i%9 == 0)
		{
			printf("========================================\n");
		}
		else if(i%3 == 0)
		{
			printf("----------------------------------------\n");
		}
		for(j=0;j<GRAPH_SIZE;j++)
		{
			if(j%9 == 0)
			{
				printf("||");
			}
			else if(j%3 == 0)
			{
				printf("|");
			}

			printf("%c",graphBoard[i][j]);
		}
	}
	printf("\n");
}



void CalcGraphBoard(int candiBoard[][BOARD_SIZE][BOARD_SIZE])
{
	int i,j,k;

	for(i=0;i<BOARD_SIZE;i++)
	{
		for(j=0;j<BOARD_SIZE;j++)
		{
			for(k=0;k<BOARD_SIZE;k++)
			{
				int x,y;

				y = i * GRAPH_UNIT_SIZE + k / GRAPH_UNIT_SIZE;
				x = j * GRAPH_UNIT_SIZE + k % GRAPH_UNIT_SIZE;
				if( candiBoard[i][j][k] == 1)
				{
						graphBoard[y][x] = k + '1';
				}
				else
				{
						graphBoard[y][x] = ' ';
				}
			}
		}
	}
}

void InitCandidateBoard(void)
{
	int i,j,k;

	memset(graphBoard,' ',sizeof(graphBoard));

	for(i=0;i<BOARD_SIZE;i++)
	{
		for(j=0;j<BOARD_SIZE;j++)
		{
			for(k=0;k<BOARD_SIZE;k++)
			{
				candidateBoard[i][j][k] = 1;
			}
		}
	}
}

int InitCalcCandidateBoard(void)
{
	int i,j,k;

	for(i=0;i<BOARD_SIZE;i++)
	{
		for(j=0;j<BOARD_SIZE;j++)
		{
			if(baseBoard[i][j] != ' ')
			{
				for(k=0;k<BOARD_SIZE;k++)
				{
					candidateBoard[i][j][k] = 0;
				}
				candidateBoard[i][j][baseBoard[i][j]-'1'] = 1;
			}
		}
	}

	return 0;
}

int DrawBoard(char inBoard[][BOARD_SIZE])
{
	int i,j;

	for(i=0;i<9;i++)
	{
		printf("\n");
		for(j=0;j<9;j++)
		{
			printf("%c",inBoard[i][j]);
		}
	}
	return 0;
}

int ReadBoard(char *fname)
{
	FILE *fp;
	int cnt = 0;

	fp = fopen(fname,"rb");
	if(fp == NULL)
	{
		return -1;
	}
	while(1)
	{
		int inChar;
		inChar = fgetc(fp);
		if(inChar == EOF)
		{
			break;
		}

		if(inChar == ' ' || (inChar >= '0' && inChar <= '9'))
		{
			int x,y;

			y = cnt % BOARD_SIZE;
			x = cnt / BOARD_SIZE;
			baseBoard[x][y] = (char)inChar;
			cnt++;
		}
	}
	printf(" cnt = %d\n",cnt);
	fclose(fp);

	DrawBoard(baseBoard);

	return 0;
}

int CheckCell(int y,int x,int candiBoard[][BOARD_SIZE][BOARD_SIZE])
{
	int i;
	int cnt;
	int target;

	cnt = 0;
	for(i=0;i<9;i++)
	{
		if(candiBoard[y][x][i] == 1)
		{
			cnt++;
			target = i;
		}
	}

	if(cnt == 1)
	{
		return target;
	}
	else
	{
		return (-1);
	}

}

int RemoveTarget(int y, int x, int target, int candiBoard[][BOARD_SIZE][BOARD_SIZE])
{
	//printf("%s %d",__func__,__LINE__);
	candiBoard[y][x][target] = 0;
	return 0;
}

int CalcLoop(char inBoard[][BOARD_SIZE], int candiBoard[][BOARD_SIZE][BOARD_SIZE])
{
	int i,j;
	int flag = 0;
	//check line horizontal
	for(i=0;i<9;i++)
	{
		for(j=0;j<9;j++)
		{
			if(inBoard[i][j] != ' ' && inBoard[i][j] != 0)
			{
				int target = inBoard[i][j] - '1';
				int ii;

				for(ii=0;ii<9;ii++)
				{
					if(inBoard[i][ii] == ' ' || inBoard[i][ii] == 0)
					{
						RemoveTarget(i,ii,target,candiBoard);
					}
				}
			}
		}
	}

	//check line vertical
	for(j=0;j<9;j++)
	{
		for(i=0;i<9;i++)
		{
			if(inBoard[i][j] != ' ' && inBoard[i][j] != 0)
			{
				int target = inBoard[i][j] - '1';
				int ii;

				for(ii=0;ii<9;ii++)
				{
					if(inBoard[ii][j] == ' ' || inBoard[ii][j] == 0)
					{
						RemoveTarget(ii,j,target,candiBoard);
					}
				}
			}
		}
	}

	//check sub block
	for(i=0;i<9;i++)
	{
		int x,y;

		y = (i / 3) * 3;
		x = (i % 3) * 3;
		for(j=0;j<9;j++)
		{
			int wx,wy;
			int py,px;

			wy = j / 3;
			wx = j % 3;

			py = y+wy;
			px = x+wx;

			if(inBoard[py][px] != ' ' && inBoard[py][px] != 0)
			{
				int target = inBoard[py][px] - '1';
				int ii;

				for(ii=0;ii<9;ii++)
				{
					int twx,twy;
					int tpy,tpx;

					twy = ii / 3;
					twx = ii % 3;

					tpy = y+twy;
					tpx = x+twx;
					if(inBoard[tpy][tpx] == ' ' || inBoard[tpy][tpx] == 0)
					{
						int ret;
						RemoveTarget(tpy,tpx,target,candiBoard);
					}
				}
			}
		}
	}

	for(i=0;i<9;i++)
	{
		for(j=0;j<9;j++)
		{
			if(inBoard[i][j] == ' ' || inBoard[i][j] == 0)
			{
				int ret;
				ret = CheckCell(i,j,candiBoard);
				if(ret != -1)
				{
					inBoard[i][j] = ret + '1';
					flag = 1;
				}
			}
		}
	}

#if 0
	DrawBoard();
	CalcGraphBoard();
	DrawGraphBoard();
#endif
	return flag;
}

int CalcRecursive(char inBoard[][BOARD_SIZE], int candiBoard[][BOARD_SIZE][BOARD_SIZE])
{
// Input, => Success
//       => Conflict   first conflict point using first number, until no more first number (OK)
//	   => Fail eleminate first conflict point numer.

	int flag;
	int conflictFlag;
	int i,j,k;

	char workBoard[BOARD_SIZE][BOARD_SIZE];
	int  workCandiBoard[BOARD_SIZE][BOARD_SIZE][BOARD_SIZE];
	int checkArray[BOARD_SIZE];

	int cnt;

	memcpy(workBoard,inBoard,sizeof(workBoard));
	memcpy(workCandiBoard,candiBoard,sizeof(workCandiBoard));

	conflictFlag = 0;
	flag = CalcLoop(workBoard,workCandiBoard);

	if(flag == 0)
	{
		printf("calc first conflict cell\n");

		for(i=0; i<BOARD_SIZE; i++)
		{
			for(j=0; j<BOARD_SIZE; j++)
			{
				cnt = 0;
				memset(checkArray,0,sizeof(checkArray));
				for(k=0; k<BOARD_SIZE; k++)
				{
					if(workCandiBoard[i][j][k] != 0)
					{
						checkArray[k] = 1;
						cnt ++;
					}
				}

				if(cnt >= 2)
				{
					int l,ll;
					int candiIndex;
					int indexCnt;
					int retVal;

					printf("%d,%d cell conflict(%d)\n\r",i,j,cnt);
					conflictFlag = 1;
					indexCnt = 0;
					for(l=0; l<BOARD_SIZE; l++)
					{
						printf("%d  indexCnt %d checkArrary(%d) workCandiBoard(%d)\n",l, indexCnt, checkArray[l], workCandiBoard[i][j][l]);
						if(checkArray[l] != 0)
						{
							indexCnt++;
							for(ll=0; ll<BOARD_SIZE; ll++)
							{
								workCandiBoard[i][j][ll] = 0;
							}
							workCandiBoard[i][j][l] = 1;
							workBoard[i][j] = '1' + l;
							retVal = CalcRecursive(workBoard,workCandiBoard);
							DrawBoard(workBoard);
							printf("\nretVal = %d\n",retVal);
							if(retVal == 0)
							{
								conflictFlag = 0;
								goto end_recursive;
							}
						}
					}
					break;
				}
			}
			if(conflictFlag)
			{
				break;
			}
		}
		conflictFlag = 2;
		return conflictFlag;
	}
	else
	{
		printf("OK\n");
	}

end_recursive:
	memcpy(inBoard,workBoard,sizeof(workBoard));
	memcpy(candiBoard,workCandiBoard,sizeof(workCandiBoard));
	return conflictFlag;
}


// 1: success
// 2: unknown
// 3: fail
int CalcRecursiveMain(char inBoard[][BOARD_SIZE], int candiBoard[][BOARD_SIZE][BOARD_SIZE])
{
	int retval = 0;
	int i,j;
	printf("enter\n");
	DrawBoard(baseBoard);

	for(i=0;i<9;i++)
	{
		printf("\n");
		for(j=0;j<9;j++)
		{
			if(inBoard[i][j]==' ')
			{
				printf("%d %d\n",i,j);
			}
		}
	}

	printf("exit value = %d\n",retval);
	return retval;
}


int main(int argc,char * argv[] )
{
	printf("Hello Sudoku %s %s\n",__DATE__,__TIME__);
	int flag;

	if(argc == 2)
	{
		ReadBoard(argv[1]);
	}
	else
	{
		printf("sudoku.exe input.sud\n");
	}

	InitCandidateBoard();
	InitCalcCandidateBoard();
	CalcGraphBoard(candidateBoard);
	DrawGraphBoard();
	while(1)
	{
		flag = CalcRecursive(baseBoard,candidateBoard);
		if(flag != 0)
		{
			printf("success\n");
			DrawBoard(baseBoard);
			CalcGraphBoard(candidateBoard);
			DrawGraphBoard();
			printf("success\n");
			break;
		}
		else
		{
			//find first candidata
			CalcRecursiveMain(baseBoard,candidateBoard);
			break;
		}
	}
}
