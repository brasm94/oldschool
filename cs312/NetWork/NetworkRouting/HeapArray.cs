using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NetworkRouting
{
    class HeapArray : IQueue
    {
        private float[] length;
        private int[] places; // holds the index of their position in the heap
        private bool[] used;
        private int[] heap;
        private int minCount = 1;
        private int size = 1;
        public void DecPriority(int i, float dis)
        {
            if (i == 6)
            {
                int pizza = 0;
            }
            length[i] = dis;
            if (!used[i])
            {
                used[i] = true;
                Swap(size, places[i]);
                BubbleUp(size);
                size++;
            }
            else
            {
                BubbleUp(places[i]);
            }
        }

        public void MakeQueue(float[] lengths)
        {
            length = lengths;
            heap = new int[length.Length];
            places = new int[length.Length];
            used = new bool[length.Length];
            int index = -1;
            for (int i = 0; i < length.Length; ++i)
            {
                used[i] = false;
                if (length[i] == 0)
                {
                    index = i;
                }
            }
            heap[0] = index;
            places[index] = 0;
            int count = 1;
            for (int i = 0; i < index; i++)
            {
                heap[count] = i;
                places[i] = count;
                count++;
            }
            for (int j = index + 1; j < length.Length; j++)
            {
                heap[count] = j;
                places[j] = count;
                count++;
            }
        }

        public int TakeMin()
        {
            if (minCount == heap.Length)
                return -1;
            int retInt = heap[0];
            if (length[retInt] == -1)
                return -1;
            //used[heap[0]] = true; ;
            if (size == -1)
                return -1;
            if (size != 0)
            {
                Swap(0, size - 1);
                //minCount++;
                size--;
                BubbleDown(0);
            }
            else
                return -1;
            return retInt;
        }
        private void BubbleUp(int index)
        {
            if (index == 6)
            {
                int schse = 19;
            }
            if (index == 0)
                return;
            int indexToCheck = (index - 1) / 2;
            if (length[heap[indexToCheck]] > length[heap[index]] || length[heap[indexToCheck]] == -1)
            {
                Swap(indexToCheck, index);
                BubbleUp(indexToCheck);
            }
        }
        private void BubbleDown(int index)
        {
            int c = MinChild(index);
            if (c == -1)
                return;
            if (length[heap[index]] > length[heap[c]] || length[heap[index]] == -1)
            {
                Swap(index, c);
                BubbleDown(c);
            }
            else
                return;
        }
        private void Swap(int indexA, int indexB)
        {
            if(size == -1)
            {
                int hotdog = 10;
                return;
            }
            places[heap[indexA]] = indexB;
            places[heap[indexB]] = indexA;
            int temp1 = heap[indexB];
            heap[indexB] = heap[indexA];
            heap[indexA] = temp1;
        }
        private int MinChild(int index)
        {
            if ((index * 2) + 1 < size)
            {
                if ((index * 2 + 2) < size)
                {
                    float childALength = length[heap[(index * 2) + 1]];
                    float childBLength = length[heap[(index * 2) + 2]];
                    // if (used[heap[(index * 2) + 1]])
                    //     return -1;
                    // if (used[heap[(index * 2) + 2]])
                    //     return -1;
                    if (childALength < childBLength && childALength != -1)
                        return index * 2 + 1;
                    else if(childBLength == -1)
                    {
                        return index * 2 + 1;
                    }
                    else
                        return (index * 2) + 2;
                }
                else
                    return index * 2 + 1;

            }
            return -1;
        }
    }
}
