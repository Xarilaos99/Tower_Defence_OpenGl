

#include "../common/renderer.h"
#include <iostream>
using namespace std;




Renderer towerDefence;






int main()
{
    
    
    try
    {
       
        towerDefence.createContext();
        towerDefence.scene();


        //cout << "Ola kala" << endl;
    }
    catch (exception& ex)
    {
        cout << ex.what() << endl;
        getchar();
        //free();
        return -1;
    }

    return 0;
}