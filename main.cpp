//
//  main.cpp
//  vektor
//
//  Created by Daniil Rabizo on 09.03.17.
//  Copyright © 2017 Daniil Rabizo. All rights reserved.
//

#include<iostream>
#include<stdexcept>

#include "vektor.h"

using namespace std;

int main ()
{
    try
    {
        Vektor vektor;
        Vektor vektor2 {256};
        Vektor vektor3 {vektor2};
        Vektor vektor4;
        
        vektor.reserve(2);
        cout << vektor2 << vektor2.max_size() << endl;
        
        for (int i {0}; i < 21; i++)
        {
            vektor.push_back(i);
            cout << vektor <<endl;
        }
        cout << "TEST";
        vektor.insert(10, 1234);
        cout << vektor << endl << vektor.size() << endl;
        
        vektor2 = vektor;
        cout << vektor << endl << vektor2 << endl;
        
        bool equal = (vektor == vektor2) ? true : false; //diese Schreibweise ist umst‰ndlich und nur ein Test des ? Operators
        if (equal) cout << "vektor == vektor2" << endl;
        
        vektor[20] = vektor2[10] - vektor[10];
        cout << vektor[20] << endl;
        
        for (int i {0}; i < 5; i++) vektor.pop_back();
        cout << vektor << endl << vektor.size() << " "<<vektor.max_size() << endl;
        vektor.shrink_to_fit();
        cout << vektor.size() << " "<<vektor.max_size() << endl;
        
        vektor.clear();
        cout << vektor << endl;
        
        bool empty = (vektor.empty()) ? true : false; //diese Schreibweise ist umst‰ndlich und nur ein Test des ? Operators
        if (empty) cout << "vektor is empty" << endl;
        
        vektor3 = vektor2;
        vektor4 = vektor3 + vektor2 + vektor3;
        cout << vektor4 << endl;
        
        bool not_equal = (vektor4 != vektor2) ? true : false; //diese Schreibweise ist umst‰ndlich und nur ein Test des ? Operators
        if (not_equal) cout << "vektor4 != vektor2" << endl;
        
        if (vektor2 < vektor4) cout << "vektor2 < vektor4" << endl;
        
        if (vektor4 < vektor2) cout << "vektor4 < vektor2" << endl;
        
        if (vektor4 < vektor4) cout << "vektor4 < vektor4" << endl;
        
        if (vektor2 <= vektor4) cout << "vektor2 <= vektor4" << endl;
        
        if (vektor4 <= vektor2) cout << "vektor4 <= vektor2" << endl;
        
        if (vektor4 <= vektor4) cout << "vektor4 <= vektor4" << endl;
        
        if (vektor2 >= vektor4) cout << "vektor2 >= vektor4" << endl;
        
        if (vektor4 >= vektor2) cout << "vektor4 >= vektor2" << endl;
        
        if (vektor4 >= vektor4) cout << "vektor4 >= vektor4" << endl;
        
        
        while(cin)
        {
            cin >> vektor;
            cout << vektor << endl;
        }
        
        //cout << vektor.size() << " " << vektor.max_size() << endl;
        //vektor.erase(4);
        
        //cout << vektor;
    }
    
    catch (size_t &e)
    {
        cout << e;
    }
    
    catch (char const* &e)
    {
        cout << e << endl;
    }
    
    return 0;
}

