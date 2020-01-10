#include <vector>
using namespace std;

class Ray
{
    private:
        vector<double> direction;
        double medium;//the medium of the material
        bool isShadow;
    public:
        Ray();
        Ray(vector<double> dir,double medium,bool isshad);
        vector<double> getDirection();
        double getMedium();
        bool getShadow();

};