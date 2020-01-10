using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;

namespace NetworkRouting
{
    class DykstrasAlgo
    {
        private IQueue queue;
        private List<PointF> listOfPoints;
        private List<HashSet<int>> edges;
        private int startIndex;
        private int[] prevList;
        private float[] length;
        public DykstrasAlgo(List<PointF> list, int start, List<HashSet<int>> adjacencyList, IQueue insertQueue)
        {
            listOfPoints = list;
            startIndex = start;
            edges = adjacencyList;
            queue = insertQueue;
        }
        public void SolveDykstras()
        {
            prevList = new int[listOfPoints.Count];
            length = new float[listOfPoints.Count];
            for(int i = 0; i < listOfPoints.Count; i++)
            {
                prevList[i] = -1;
                length[i] = -1;
            }
            length[startIndex] = 0;
            int index;
            queue.MakeQueue(length);
            while ((index = queue.TakeMin()) != -1)// should be while queue != empty or queueTakeMin()
            {
               // HashSet<int> edgesTemp = edges[index];
                foreach (int otherNode in edges[index])
                {
                    float dis = getDistance(listOfPoints[index], listOfPoints[otherNode]);
                    if (length[otherNode] > (dis + length[index]) || length[otherNode] == -1)
                    {
                        length[otherNode] = (dis + length[index]);
                        prevList[otherNode] = index;
                        queue.DecPriority(otherNode , length[otherNode]);
                    }
                }
            }
            int bob = 0;
        }
        float getDistance(PointF p1 , PointF p2)
        {
            float changeX = (p1.X - p2.X);
            float changeY = (p1.Y - p2.Y);
            return (float)Math.Sqrt((changeX * changeX) + (changeY * changeY));
        }
        public int[] GetPrevList()
        {
            return prevList;
        }
    }
}
