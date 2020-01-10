using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace GeneticsLab
{
    class Algo
    {
        int MATCH = -3;
        int SUBS = 1;
        int INSDEL = 5;
        GeneSequence seqA;
        GeneSequence seqB;
        bool banded;
        char[,] prev;
        int[,] dis;
        int sizeRow;
        int sizeCol;
        string sRow;
        string sCol;
        public Algo(GeneSequence sequenceA, GeneSequence sequenceB, bool banded ,int size)
        {
            seqA = sequenceA;
            seqB = sequenceB;
            this.banded = banded;
            this.sizeRow = size + 1;
            this.sizeCol = size + 1;
            if(this.sizeRow > sequenceA.Sequence.Length)
            {
                this.sizeRow = sequenceA.Sequence.Length;
                this.sizeRow++;
            }
            if(this.sizeCol > sequenceB.Sequence.Length)
            {
                this.sizeCol = sequenceB.Sequence.Length;
                this.sizeCol++;
            }
            prev = new char[this.sizeRow, this.sizeCol];
            dis = new int[this.sizeRow, this.sizeCol];
        }
        private int getDisAtPoint(int row ,int col)
        {
            return dis[row, col];
        }
        private int Dif(int row , int col)
        {
            char a = seqA.Sequence[row - 1];
            char b = seqB.Sequence[col - 1];
            if(a == b)
                return MATCH;
            return SUBS;
        }
        public void RunAlgo()
        {
            int rowCount = seqA.Sequence.Length;
            int colCount = seqB.Sequence.Length;
            rowCount++;
            colCount++;
            if(sizeRow < rowCount)
            {
                rowCount = sizeRow;
                //rowCount++;
            }
            if(sizeCol < colCount)
            {
                colCount = sizeCol;
                //colCount++;
            }
            for (int row = 0; row < rowCount ; row++)
            {
                dis[row, 0] = row * INSDEL;
                prev[row, 0] = 'u';
            }
            for (int col = 0; col < colCount; col++)
            {
                dis[0, col] = col * INSDEL;
                prev[0, col] = 'l';
            }
            for (int row = 1; row < rowCount; row++) // should start at 1 i think
            {
                //this is where the code should go to determine if banded or not if banded col starts at row - d and ends at row + d;
                for(int col = 1; col < colCount; col ++)// also should start at 1 i think
                {
                    int d = getDisAtPoint(row - 1, col - 1) + Dif(row,col);
                    int u = getDisAtPoint(row -1, col) + INSDEL;
                    int l = getDisAtPoint(row, col -1) + INSDEL;
                    if(d < u && d < l)
                    {
                        dis[row, col] = d;
                        prev[row, col] = 'd';
                    }
                    else if( l < u)
                    {
                        dis[row, col] = l;
                        prev[row, col] = 'l';
                    }
                    else
                    {
                        dis[row, col] = u;
                        prev[row, col] = 'u';
                    }
                }
            }
        }
        public void CalcStrings()
        {
            StringBuilder sbr = new StringBuilder();
            StringBuilder sbc = new StringBuilder();
            int row = sizeRow  - 1;
            int col = sizeCol - 1;
            while(row != 0 || col != 0)
            {
                if (prev[row, col] == 'u')
                {
                    sbr.Insert(0, seqA.Sequence[row - 1]);
                    sbc.Insert(0, '-');
                    row--;
                }
                else if (prev[row, col] == 'l')
                {
                    sbr.Insert(0, '-');
                    sbc.Insert(0, seqB.Sequence[col - 1]);
                    col--;
                }
                else if (prev[row, col] == 'd')
                {
                    sbr.Insert(0, seqA.Sequence[row - 1]);
                    sbc.Insert(0, seqB.Sequence[col - 1]);
                    row--;
                    col--;
                }
                else
                    System.Diagnostics.Debug.Write("this failed");
            }
            sRow = sbr.ToString();
            sCol = sbc.ToString();
        }
        public string GetColString()
        {
            return sCol;
        }
        public string GetRowString()
        {
            return sRow;
        }
        public int GetScore()
        {
            return dis[sizeRow - 1, sizeCol - 1];
        }
        
    }
}
