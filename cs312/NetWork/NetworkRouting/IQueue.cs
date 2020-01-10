using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NetworkRouting
{
    public interface IQueue
    {
        void MakeQueue(float[] lengths);
        int TakeMin();
        void DecPriority(int i, float dis);
    }
}
