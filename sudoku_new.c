//sudoku
#include <stdio.h>
#include <string.h>

#define	BOARD_SIZE		(9)
#define	GRAPH_UNIT_SIZE	(3)
#define GRAPH_SIZE		(BOARD_SIZE * GRAPH_UNIT_SIZE)

char	baseBoard[BOARD_SIZE][BOARD_SIZE];


int InitBoard(void)
{
    int i,j;

    for(i=0;i<BOARD_SIZE;i++)
    {
        for(j=0;j<BOARD_SIZE;j++)
        {
            baseBoard[i][j] = '0';
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
            if(inChar >= '0' && inChar <= '9') 	baseBoard[x][y] = (char)inChar;
            else if(inChar == ' ') 	baseBoard[x][y] = '0';
			cnt++;
		}
	}
	printf(" cnt = %d\n",cnt);
	fclose(fp);

	return 0;
}

int DrawBoard(char inBoard[][BOARD_SIZE])
{
	int i,j;

	for(i=0;i<BOARD_SIZE;i++)
	{
		printf("\n");
		for(j=0;j<BOARD_SIZE;j++)
		{
            if(inBoard[i][j]=='0') printf(" ");
            else printf("%c",inBoard[i][j]);
		}
	}
	return 0;
}

int CheckRow(int y, int x, char inBoard[][BOARD_SIZE], char *candidate)
{
    int i;

    for(i=0;i<BOARD_SIZE;i++)
    {
        candidate[i] = 1;
    }

    for(i=0;i<BOARD_SIZE;i++)
    {
        if(inBoard[y][i] != '0')
        {
            candidate[inBoard[y][i]-'1'] = 0;
        }
    }
#if 0
    for(i=0;i<BOARD_SIZE;i++)
    {
        if(candidate[i] == 1)
        {
            printf(" %d",i+1);
        }
    }
    printf(" row\n");
#endif
    return 0;
}

int CheckCol(int y, int x, char inBoard[][BOARD_SIZE], char *candidate)
{
    int i;

    for(i=0;i<BOARD_SIZE;i++)
    {
        candidate[i] = 1;
    }

    for(i=0;i<BOARD_SIZE;i++)
    {
        if(inBoard[i][x] != '0')
        {
            candidate[inBoard[y][i]-'1'] = 0;
        }
    }
#if 0
    for(i=0;i<BOARD_SIZE;i++)
    {
        if(candidate[i] == 1)
        {
            printf(" %d",i+1);
        }
    }
    printf(" col\n");
#endif
    return 0;
}


int CheckBlock(int y, int x, char inBoard[][BOARD_SIZE], char *candidate)
{
    int i,j;

    for(i=0;i<BOARD_SIZE;i++)
    {
        candidate[i] = 1;
    }

    y = y - y%3;
    x = x - x%3;
    for(i=0;i<BOARD_SIZE/3;i++)
    {
        for(j=0;j<BOARD_SIZE/3;j++)
        {
            if(inBoard[y+i][x+j] != '0')
            {
                candidate[inBoard[y+i][x+j]-'1'] = 0;
            }
        }
    }
#if 0
    for(i=0;i<BOARD_SIZE;i++)
    {
        if(candidate[i] == 1)
        {
            printf(" %d",i+1);
        }
    }
    printf(" block\n");
#endif
    return 0;
}


int CheckDot(int y, int x, char inBoard[][BOARD_SIZE],char *candidate)
{
    char candidataRow[BOARD_SIZE];
    char candidataCol[BOARD_SIZE];
    char candidataBlock[BOARD_SIZE];
    int i,j;
    int candidateCount = 0;
    char lastCandidate;

    int l;
    if(inBoard[y][x]=='0')
    {
        CheckRow(y,x,inBoard,candidataRow);
        CheckCol(y,x,inBoard,candidataCol);
        CheckBlock(y,x,inBoard,candidataBlock);

        for(l=0;l<BOARD_SIZE;l++)
        {
            if(candidataRow[l] == 1 && candidataCol[l] == 1 && candidataBlock[l] == 1)
            {
                candidate[l] = 1;
                candidateCount++;
                lastCandidate =  l + '1';
            }
            else
            {
                candidate[l] = 0;
            }
        }
        //printf("candidateCount=%d lastCandidate=%c can\n",candidateCount,lastCandidate);
    }
	return candidateCount;
}


char GetFirstCharactor(char *candidate)
{
    char firstCandidate;
    int l;

    for(l=0;l<BOARD_SIZE;l++)
    {
        if(candidate[l] == 1)
        {
            firstCandidate =  l + '1';
            return firstCandidate;
        }
    }
    return '0';
}

int globalCnt = 0;
int CheckBoard(char inBoard[][BOARD_SIZE])
{
    char candidata[BOARD_SIZE];
    int i,j;
    char firstCandidate = 0;
    int candidateCount = 0;

    globalCnt++;

	for(i=0;i<BOARD_SIZE;i++)
	{
		printf("\n");
		for(j=0;j<BOARD_SIZE;j++)
		{
            if(inBoard[i][j]=='0')
            {
                candidateCount = CheckDot(i,j,inBoard,candidata);
                firstCandidate = GetFirstCharactor(candidata);
                printf("i=%d, j=%d, candidateCount=%d\n",i,j,candidateCount);
                if(candidateCount == 1)
                {
                    inBoard[i][j] = firstCandidate;
                    DrawBoard(inBoard);
                    if(globalCnt >=2 )
                    {
                        globalCnt--;
                        printf("globalCnt = %d return\n",globalCnt);
                        return 0;
                    }
                    printf("\n gogo(%d,%d) %c globalCnt  %d\n",i,j,firstCandidate,globalCnt);
                }
                else if(candidateCount == 0)
                {
                    if(globalCnt >=2 )
                    {

                    }
                    else
                    {
                        DrawBoard(inBoard);
                    }
                    globalCnt--;
                    printf("globalCnt = %d return\n",globalCnt);
                    return -1;
                }
                else
                {
                    int retVal;
                    char inBoardCp[BOARD_SIZE][BOARD_SIZE];
                    //printf("i=%d, j=%d, candidateCount=%d\n",i,j,candidateCount);
                    for(int l=0;l<BOARD_SIZE;l++)
                    {
                        if(candidata[l] == 1)
                        {
                            memcpy(inBoardCp,inBoard,sizeof(inBoardCp));
                            inBoardCp[i][j] = l + '1';
                            //DrawBoard(inBoardCp);
                            retVal = CheckBoard(inBoardCp);
                            if(retVal == -1)
                            {
//                                printf("%d %d %d next\n",i,j,l);
                            }
                            else if(retVal == 0)
                            {
                                printf("%d %d %d ok\n",i,j,l);
                                inBoard[i][j] = l + '1';
                                //DrawBoard(inBoard);
                                if(globalCnt >=2 )
                                {
                                    printf("globalCnt = %d return\n",globalCnt);
                                    globalCnt--;
                                    return 0;
                                }
                                break;
                            }
                        }
                    }
                }
            }
		}
	}
    globalCnt--;
    DrawBoard(inBoard);
	return 0;
}


int main(int argc, char *argv[])
{
    InitBoard();

	if(argc == 2)
	{
		ReadBoard(argv[1]);
	}
	else
	{
		printf("sudoku.exe input.sud\n");
        return 0;
	}

	DrawBoard(baseBoard);
    CheckBoard(baseBoard);

    return 0;
}
