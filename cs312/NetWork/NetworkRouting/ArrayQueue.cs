using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NetworkRouting
{

    class ArrayQueue : IQueue
    {
        private bool[] used;
        private float[] length;
        
        int IQueue.TakeMin()
        {
            int index = -1;
            float dis = -1;
            for (int i = 0; i < length.Length; ++i)
            {
                if (used[i] == false)
                {
                    if(length[i] == 0)
                    {
                        used[i] = true;
                        return i;
                    }
                    else if (dis == -1 || (length[i] < dis && length[i] != -1))
                    {
                        dis = length[i];
                        index = i;
                    }
                }
            }
            if(index != -1)
                used[index] = true;
            if (dis == -1)
                return -1;
            return index;
        }

        void IQueue.DecPriority(int index, float dis)
        {
            length[index] = dis;
        }

        void IQueue.MakeQueue(float[] lengths)
        {
            length = lengths;
            used = new bool[lengths.Length];
            for(int i = 0; i < used.Length;i++)
            {
                used[i] = false;
            }
        }
        
    }
}
