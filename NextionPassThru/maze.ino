
unsigned char Pixel[50][50];
int dirx[4] = {10, 0, 0, -10};
int diry[4] = {0, -10, 10, 0};

void startMaze()
{
  for (int i=0;i<50;i++)
    for (int j=0;j<50;j++)
      Pixel[i][j]=0;

  SendCmd("cls BLACK");
  Pixel[0][0]=1;
  doMove(0, 0, 3);
  
}

void doMove(int x, int y, int d)
{
  int i, j, x1, y1, x2, y2;
  int possible[3];
  
  int Possible = 3;  
  int dir = random(4);

  // Build list of possible direction less one
  j = 0;
  for (i=0;i<4;i++)
    if (i != d)
      possible[j++] = i;

  while (Possible > 0)
  {
    dir = random(Possible);
    d = possible[dir];
    x1 = x + dirx[d];
    x2 = x1/10;
    y1 = y + diry[d];
    y2 = y1/10;
    if ((x1 > 479) || (x1 < 0))
      d = -1;
    if ((y1 > 271) || (y1 < 0))
      d = -1;
    if (d >= 0)
      if (Pixel[x2][y2] != 0)
        d = -1;
    if (d >= 0)
    {
      Pixel[x2][y2]=1;
      doLine(x, y, x1, y1);
      doMove(x1, y1, d);
    }
    // Remove a possible direction
    Possible--;
    if (possible[dir] < Possible)
      possible[dir] = possible[Possible];
  }
  
}

void doLine(int x, int y, int x1, int y1)
{
  char Buffer[50];

  sprintf(Buffer, "line %d,%d,%d,%d,RED", x, y, x1, y1);
//  Serial.println(Buffer);
  Serial1.print(Buffer);Serial1.write(0xff);Serial1.write(0xff);Serial1.write(0xff);
}

