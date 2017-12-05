//
//  vektor.cpp
//  vektor
//
//  Created by Daniil Rabizo on 09.03.17.
//  Copyright © 2017 Daniil Rabizo. All rights reserved.
//

#include "vektor.h"
#include<stdexcept>
#include<iostream>
#include<iterator>

using namespace std;


class Iterator {
    double* ptr;
public:
    Iterator(double*);
    const Iterator& operator++();
    bool operator!=(const Iterator&);
    double& operator*();
    const double& operator*() const;
};






//Konstruktoren:
Vektor::Vektor() : max_sz {vektor_min_size} , sz {0} , values {new double [max_sz]}
{
    /*
     max_sz = vektor_min_size;
     sz = 0;
     values = new double [max_sz]; // rechts kommt ein pointer zur¸ck, deswegen nur values und nciht *values
     */
}

Vektor::Vektor(size_t max_sz) : max_sz {max_sz} , sz {0} , values {new double [max_sz]}
{
    /*
     this->max_sz = max_sz;
     sz = 0;
     values = new double [max_sz];
     */
    if (max_sz == 0)
        values = nullptr;
    else values = new double [max_sz];
}

//Kopierkonstruktor:
Vektor::Vektor(const Vektor& orig) // bekommt Quellobjekt als const Referenz, legt einen zweiten buffer f¸r das neue Objekt an
{
    max_sz = orig.max_sz; sz = orig.sz;
    values = max_sz ? new double [max_sz] : nullptr; // ist max_sz true dh > 0, setze den this->values pointer auf die adresse
    // des neu angelegten Speicherplatzes, ansonsten auf nullptr
    for(size_t i {0}; i < sz; ++i)
    {
        values[i] = orig.values[i];
    }
}

//Destruktoren:
Vektor::~Vektor()
{
    delete [] values;
}

//Methoden:
void Vektor::reserve(size_t factor)
{
    double* transfer {values};
    
    values = new double [max_sz*factor]; // gef‰hrliche dinge erst ausprobieren, dann das objekt umbauen
    max_sz *= factor;
    
    size_t i {0};
    while (i <= (max_sz/factor))
    {
        *(values + i) = *(transfer + i);
        ++i;
    }
    delete [] transfer; // eckige klammern, weil transer ja auf ein array zeigt, das mit new gea,cht wurde
}

void Vektor::insert(size_t pos, double val) // mit dem [] operator gelˆst?
{
    if (sz+1 > max_sz)
    {
        reserve(2);
    }
    
    size_t difference {0};
    if (pos < sz)
    {
        difference = sz-pos;
    }
    else
    {
        throw ("ERROR - pos out of range, cant insert");
    }
    
    size_t i {0};
    i = sz;
    while (difference+1 > 0)
    {
        values[i+1] = values [i];
        if (i == pos)
        {
            values[pos] = val;
        }
        
        --i;
        --difference;
    }
    sz++;
    shrink_to_fit();
}

void Vektor::erase(size_t pos) // mit dem [] operator gelˆst?
{
    size_t difference {0};
    if (pos < sz)
    {
        difference = sz-pos;
    }
    else
    {
        throw ("ERROR - pos out of range, element to erase doesnt exist");
    }
    
    size_t i {0};
    i = pos;
    
    while (difference > 0)
    {
        values[i] = values [i+1];
        ++i;
        --difference;
    }
    --sz;
    shrink_to_fit();
}

void Vektor::push_back(double number)
{
    if (sz + 1 > max_sz)
    {
        reserve(2);
        values[sz] = number;
        sz += 1;
    }
    else
    {
        values[sz] = number;
        sz += 1;
    }
    shrink_to_fit();
}

void Vektor::shrink_to_fit()
{
    double* transfer {values};
    
    (sz > 4) ? max_sz = sz : max_sz = vektor_min_size;
    values = new double [max_sz]; // gef‰hrliche dinge erst ausprobieren, dann das objekt 						//umbauen
    size_t i {0};
    
    while (i < sz)
    {
        *(values + i) = *(transfer + i);
        ++i;
    }
    delete [] transfer; // eckige klammern, weil transer ja auf ein array zeigt, das mit new
    //geamcht wurd
}

void Vektor::pop_back ()
{
    if (sz <= 0)
    {
        throw runtime_error ("ERROR - pop_back not possible, vektor has already size 0");
    }
    else
    {
        sz -= 1;
    }
}


double& Vektor::operator[](size_t sz)
{
    if (sz < max_sz)
    {
        return values[sz];
    }
    else
    {
        throw runtime_error ("ERROR - Index called by [] operator was larger than max_sz");
    }
    
}


Vektor& Vektor::operator=(const Vektor& rop)
{
    if (&rop == this)
    {
        return *this;
    }
    
    if (values)
    {
        delete[] values;
    }
    
    max_sz = rop.max_sz; sz = rop.sz;
    values = max_sz ? new double [max_sz] : nullptr; // ist max_sz true dh > 0, setze den this->values pointer auf die adresse
    // des neu angelegten Speicherplatzes, ansonsten auf nullptr
    for(size_t i {0}; i < sz; ++i)
    {
        values[i] = rop.values[i];
    }
    return *this;
}

void Vektor::clear ()
{
    sz = 0;
    shrink_to_fit();
}
/*
ostream& Vektor::print (ostream& os) const
{
    os << "[";
    for (size_t i {0}; i < sz; i++) // leichter lesbar: i < vektor_max_size
    {
        if(i==0) os << values[i];
        else  os << "," << values[i];
    }
    os << "]";
    return os;
}

istream& Vektor::getline(istream &is)
{
    clear();
    State state {State::open_parentheses};
    Position pos {Position::predot};
    is.unsetf(ios_base::skipws); //no skip white spaces
    size_t i {0};
    size_t y {0};
    char input;
    
    int prebuffer [256];
    int postbuffer [256];
    
    bool inloop {true};
    while (inloop)
    {
        is >> input;
        switch (state)
        {
            case State::open_parentheses:
            {
                switch (input)
                {
                    case'[': state = State::number; break;
                    default: throw ("ERROR - incorrect input format - op"); break;
                }
            }
                break;
            case State::number:
            {
                switch(input)
                {
                    case']': state = State::close_parentheses; break;
                    case',': state = State::number; break;
                    case'.':
                    {
                        if (pos == Position::postdot) throw ("ERROR - incorrect input format - op");
                        else state = State::number; pos = Position::postdot; break;
                    }
                        
                    case'0':
                    {
                        state = State::comma;
                        switch(pos)
                        {
                            case Position::predot : prebuffer[i] = input-48; i++; break;
                            case Position::postdot: postbuffer[y] = input-48; y++; break;
                            default: throw ("ERROR - buffer switch undefined state"); break;
                        }
                    }
                        break;
                        
                    case'1': state = State::comma;
                    {
                        state = State::comma;
                        switch(pos)
                        {
                            case Position::predot : prebuffer[i] = input-48; i++; break;
                            case Position::postdot: postbuffer[y] = input-48; y++; break;
                            default: throw ("ERROR - buffer switch undefined state"); break;
                        }
                    }
                        break;
                        
                    case'2': state = State::comma;
                    {
                        state = State::comma;
                        switch(pos)
                        {
                            case Position::predot : prebuffer[i] = input-48; i++; break;
                            case Position::postdot: postbuffer[y] = input-48; y++; break;
                            default: throw ("ERROR - buffer switch undefined state"); break;
                        }
                    }
                        break;
                        
                    case'3': state = State::comma;
                    {
                        state = State::comma;
                        switch(pos)
                        {
                            case Position::predot : prebuffer[i] = input-48; i++; break;
                            case Position::postdot: postbuffer[y] = input-48; y++; break;
                            default: throw ("ERROR - buffer switch undefined state"); break;
                        }
                    }
                        break;
                        
                    case'4': state = State::comma;
                    {
                        state = State::comma;
                        switch(pos)
                        {
                            case Position::predot : prebuffer[i] = input-48; i++; break;
                            case Position::postdot: postbuffer[y] = input-48; y++; break;
                            default: throw ("ERROR - buffer switch undefined state"); break;
                        }
                    }
                        break;
                        
                    case'5': state = State::comma;
                    {
                        state = State::comma;
                        switch(pos)
                        {
                            case Position::predot : prebuffer[i] = input-48; i++; break;
                            case Position::postdot: postbuffer[y] = input-48; y++; break;
                            default: throw ("ERROR - buffer switch undefined state"); break;
                        }
                    }
                        break;
                        
                    case'6': state = State::comma;
                    {
                        state = State::comma;
                        switch(pos)
                        {
                            case Position::predot : prebuffer[i] = input-48; i++; break;
                            case Position::postdot: postbuffer[y] = input-48; y++; break;
                            default: throw ("ERROR - buffer switch undefined state"); break;
                        }
                    }
                        break;
                        
                    case'7': state = State::comma;
                    {
                        state = State::comma;
                        switch(pos)
                        {
                            case Position::predot : prebuffer[i] = input-48; i++; break;
                            case Position::postdot: postbuffer[y] = input-48; y++; break;
                            default: throw ("ERROR - buffer switch undefined state"); break;
                        }
                    }
                        break;
                        
                    case'8': state = State::comma;
                    {
                        state = State::comma;
                        switch(pos)
                        {
                            case Position::predot : prebuffer[i] = input-48; i++; break;
                            case Position::postdot: postbuffer[y] = input-48; y++; break;
                            default: throw ("ERROR - buffer switch undefined state"); break;
                        }
                    }
                        break;
                        
                    case'9': state = State::comma;
                    {
                        state = State::comma;
                        switch(pos)
                        {
                            case Position::predot : prebuffer[i] = input-48; i++; break;
                            case Position::postdot: postbuffer[y] = input-48; y++; break;
                            default: throw ("ERROR - buffer switch undefined state"); break;
                        }
                    }
                        break;
                        
                    default: throw ("ERROR - incorrect input format - num"); break;
                }
            }
                break;
            case State::comma:
            {
                switch(input)
                {
                    case'.':
                    {
                        if (pos == Position::postdot) throw ("ERROR - incorrect input format - op");
                        else state = State::number; pos = Position::postdot; break;
                    }
                    case ',':
                    {
                        double result {0};
                        double presult {0};
                        double postsult {0};
                        if ((state == State::comma) &&  pos == Position::predot)
                        {
                            for (int j {0}; j < i; j++)
                            {
                                presult += prebuffer[j];
                                presult *=10;
                            }
                        }
                        
                        if ((state == State::comma) &&  pos == Position::postdot)
                        {
                            for (int j {0}; j < i; j++)
                            {
                                presult += prebuffer[j];
                                presult *=10;
                            }
                            
                            double exp_shift {1};
                            for (int j {0}; j < y; j++)
                            {
                                exp_shift *=10;
                                postsult += (postbuffer[j]/exp_shift);
                            }
                        }
                        result = presult/10 + postsult;
                        if (sz+1 > max_sz) reserve(2);
                        values[sz] = result;
                        ++sz;
                        
                        state = State::number; pos = Position::predot;
                        i = 0;
                        y = 0;
                    }
                        break;
                    case ']':
                    {
                        double result {0};
                        double presult {0};
                        double postsult {0};
                        
                        if ((state == State::comma) &&  pos == Position::predot)
                        {
                            for (int j {0}; j < i; j++)
                            {
                                presult += prebuffer[j];
                                presult *=10;
                            }
                        }
                        
                        if ((state == State::comma) &&  pos == Position::postdot)
                        {
                            for (int j {0}; j < i; j++)
                            {
                                presult += prebuffer[j];
                                presult *=10;
                            }
                            
                            double exp_shift {1};
                            for (int j {0}; j < y; j++)
                            {
                                exp_shift *=10;
                                postsult += (postbuffer[j]/exp_shift);
                            }
                        }
                        
                        result = presult/10 + postsult;
                        if (sz+1 > max_sz) reserve(2);
                        values[sz] = result;
                        ++sz;
                        
                        state = State::close_parentheses; is.unsetf(ios_base::skipws); break;
                        
                        i = 0;
                        y = 0;
                        
                    }
                        
                    case'0':
                    {
                        state = State::comma;
                        switch(pos)
                        {
                            case Position::predot : prebuffer[i] = input-48; i++; break;
                            case Position::postdot: postbuffer[y] = input-48; y++; break;
                            default: throw ("ERROR - buffer switch undefined state"); break;
                        }
                    }
                        break;
                        
                    case'1':
                    {
                        state = State::comma;
                        switch(pos)
                        {
                            case Position::predot : prebuffer[i] = input-48; i++; break;
                            case Position::postdot: postbuffer[y] = input-48; y++; break;
                            default: throw ("ERROR - buffer switch undefined state"); break;
                        }
                    }
                        break;
                        
                    case'2':
                    {
                        state = State::comma;
                        switch(pos)
                        {
                            case Position::predot : prebuffer[i] = input-48; i++; break;
                            case Position::postdot: postbuffer[y] = input-48; y++; break;
                            default: throw ("ERROR - buffer switch undefined state"); break;
                        }
                    }
                        break;
                        
                    case'3':
                    {
                        state = State::comma; 
                        switch(pos)
                        {
                            case Position::predot : prebuffer[i] = input-48; i++; break;
                            case Position::postdot: postbuffer[y] = input-48; y++; break;
                            default: throw ("ERROR - buffer switch undefined state"); break;
                        }
                    }
                        break;
                        
                    case'4':
                    {
                        state = State::comma; 
                        switch(pos)
                        {
                            case Position::predot : prebuffer[i] = input-48; i++; break;
                            case Position::postdot: postbuffer[y] = input-48; y++; break;
                            default: throw ("ERROR - buffer switch undefined state"); break;
                        }
                    }
                        break;
                        
                    case'5':
                    {
                        state = State::comma; 
                        switch(pos)
                        {
                            case Position::predot : prebuffer[i] = input-48; i++; break;
                            case Position::postdot: postbuffer[y] = input-48; y++; break;
                            default: throw ("ERROR - buffer switch undefined state"); break;
                        }
                    }
                        break;
                        
                    case'6':
                    {
                        state = State::comma; 
                        switch(pos)
                        {
                            case Position::predot : prebuffer[i] = input-48; i++; break;
                            case Position::postdot: postbuffer[y] = input-48; y++; break;
                            default: throw ("ERROR - buffer switch undefined state"); break;
                        }
                    }
                        break;
                        
                    case'7':
                    {
                        state = State::comma; 
                        switch(pos)
                        {
                            case Position::predot : prebuffer[i] = input-48; i++; break;
                            case Position::postdot: postbuffer[y] = input-48; y++; break;
                            default: throw ("ERROR - buffer switch undefined state"); break;
                        }
                    }
                        break;
                        
                    case'8':
                    {
                        state = State::comma; 
                        switch(pos)
                        {
                            case Position::predot : prebuffer[i] = input-48; i++; break;
                            case Position::postdot: postbuffer[y] = input-48; y++; break;
                            default: throw ("ERROR - buffer switch undefined state"); break;
                        }
                    }
                        break;
                        
                    case'9':
                    {
                        state = State::comma; 
                        switch(pos)
                        {
                            case Position::predot : prebuffer[i] = input-48; i++; break;
                            case Position::postdot: postbuffer[y] = input-48; y++; break;
                            default: throw ("ERROR - buffer switch undefined state"); break;
                        }
                    }
                        break;
                        
                    default: throw ("ERROR - incorrect input format - com"); break;
                }
            }
                break;
            case State::close_parentheses:
            {
                inloop = false;
                cout << "success " << endl;
                shrink_to_fit();
                state = State::open_parentheses;
            }
                break;	
            default: throw ("ERROR - state error undefined input state"); break;
        }
    }
    return is;
}
*/


//Globale FUnktionen
bool operator==(const Vektor& lop, const Vektor& rop) // ¸bergabe als &, damit keine "klone" der objekte erstellt werden.
{	
    if(lop.size() == rop.size())
    {
        for(size_t i {0}; i < lop.sz; i++)
        {
            if(rop.values[i] != *(lop.values+i)) // direkter Zugriff aus Instanzvariablen erlaubt, da friend Funtkion.
            {
                return false;
            }
        }
    }
    else
    {
        return false;
    }
    return true;
}

bool operator!=(const Vektor& lop, const Vektor& rop) // ¸bergabe als &, damit keine "klone" der objekte erstellt werden.
{	
    if (lop == rop) return false;
    else return true;
}

bool operator> (const Vektor& lop, const Vektor& rop)
{
    if(lop == rop) return false;
    if(lop.size() < rop.size()) return false;
    if(lop.size() > rop.size()) return true;
    if (lop.size() == rop.size())
    {
        for(size_t i {0}; i < lop.size(); i++)
        {
            if(lop.values[i] < lop.values[i])
            {
                return false;
            }
        }
    }
    return true;
}

bool operator< (const Vektor& lop, const Vektor& rop)
{
    if (lop == rop) return false;
    return lop>rop ? false : true; //kurzschreibweise f¸r if lop>rop return false; else return true;
}

bool operator<= (const Vektor& lop, const Vektor& rop)
{
    return lop>rop ? false : true;
}

bool operator>= (const Vektor& lop, const Vektor& rop)
{
    return lop<rop ? false : true; 
}


Vektor operator+(const Vektor& lop, const Vektor& rop) // linke seite variable, recht seite const, da zB lop = lop + rop mgl ist.
{
    Vektor sum;
    if (lop.empty() || rop.empty())
    {
        if (lop.empty() && rop.empty()) throw runtime_error ("ERROR - cant add two empty vektors");
        if (lop.empty()) 
        {
            sum = rop;
            return sum;
        }
        if (rop.empty())
        {
            sum = lop;
            return sum;
        }
    }
    else
    {
        sum.values = new double [lop.sz + rop.sz]; // gef‰hrliche dinge erst ausprobieren, dann das objekt umbauen
        sum.max_sz = lop.sz + rop.sz;
        
        size_t i {0};
        double* transfer {lop.values};
        while (i < (lop.sz))
        {
            *(sum.values + i) = *(transfer + i);
            ++i;
            ++sum.sz;
        }
        
        size_t j {0};
        transfer = rop.values;
        while (j < (rop.sz))
        {	
            *(sum.values + i + j) = *(transfer + j);
            ++j;
            ++sum.sz;
        }
        //delete [] transfer; crash wenn transfer deleted wird. warum? der grund: transfer zeigt auf const objekte!
    }
    return sum;
}

ostream& operator<< (ostream& os, Vektor& vek)
{	
    return vek.print(os);
}

istream& operator>> (istream& is, Vektor& vek) // liets den stream ein, checkt ob grammatik ok, wenn ja gibt zur¸ck wenn nein error
{	
    return vek.getline(is);
}

