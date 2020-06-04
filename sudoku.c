#include <stdio.h>
#include <mem.h>


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



void CalcGraphBoard(void)
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
				if( candidateBoard[i][j][k] == 1)
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

int DrawBoard()
{
	int i,j;

	for(i=0;i<9;i++)
	{
		printf("\n");
		for(j=0;j<9;j++)
		{
			printf("%c",baseBoard[i][j]);
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

	DrawBoard();
	
	return 0;
}

int RemoveTarget(int y, int x, int target)
{
//	printf("L(%d) %d %d %d  old=%d\n",__LINE__,y,x,target,candidateBoard[y][x][target]);
	candidateBoard[y][x][target] = 0;
	return 0;
}

int CheckCell(int y,int x)
{
	int i;
	int cnt;
	int target;

	cnt = 0;
	for(i=0;i<9;i++)
	{
		if(candidateBoard[y][x][i] == 1)
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

int CalcLoop(void)
{
	int i,j;
	int flag = 0;
	//check line horizontal
	for(i=0;i<9;i++)
	{
		for(j=0;j<9;j++)
		{
			if(baseBoard[i][j] != ' ' && baseBoard[i][j] != 0)
			{
				int target = baseBoard[i][j] - '1';
				int ii;

				for(ii=0;ii<9;ii++)
				{
					if(baseBoard[i][ii] == ' ' || baseBoard[i][ii] == 0)
					{
						int ret;
						RemoveTarget(i,ii,target);
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
			if(baseBoard[i][j] != ' ' && baseBoard[i][j] != 0)
			{
				int target = baseBoard[i][j] - '1';
				int ii;

				for(ii=0;ii<9;ii++)
				{
					if(baseBoard[ii][j] == ' ' || baseBoard[ii][j] == 0)
					{
						int ret;
						RemoveTarget(ii,j,target);
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

			if(baseBoard[py][px] != ' ' && baseBoard[py][px] != 0)
			{
				int target = baseBoard[py][px] - '1';
				int ii;

				for(ii=0;ii<9;ii++)
				{
					int twx,twy;
					int tpy,tpx;

					twy = ii / 3;
					twx = ii % 3;

					tpy = y+twy;
					tpx = x+twx;
					if(baseBoard[tpy][tpx] == ' ' || baseBoard[tpy][tpx] == 0)
					{
						int ret;
						RemoveTarget(tpy,tpx,target);
					}
				}
			}
		}
	}

	for(i=0;i<9;i++)
	{
		for(j=0;j<9;j++)
		{
			if(baseBoard[i][j] == ' ' || baseBoard[i][j] == 0)
			{
				int ret;
				ret = CheckCell(i,j);
				if(ret != -1)
				{
					baseBoard[i][j] = ret + '1';
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
	CalcGraphBoard();
	DrawGraphBoard();
	while(1)
	{
		flag = CalcLoop();
		if(flag == 0)
		{
			break;
		}
	}
	DrawBoard();
	CalcGraphBoard();
	DrawGraphBoard();
}