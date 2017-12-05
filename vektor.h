//
//  vektor.hpp
//  vektor
//
//  Created by Daniil Rabizo on 09.03.17.
//  Copyright © 2017 Daniil Rabizo. All rights reserved.
//

#pragma once
#include<iostream>


using namespace std;

enum class State {begin, open_parentheses, close_parentheses, number, comma};
enum class Position {predot, postdot};

class Vektor
{
private:
    // Variablen
    size_t max_sz;
    size_t sz; // aktuelle size
    double* values {nullptr};
    
public:
    //Konstanten
    static const size_t vektor_min_size {5}; //static damit f¸r alle
    //klassenelemente nur eine konstante
    // dieses typs da ist
    
    // Methoden
    inline size_t size() const; // inline, wenn explizit geschrieben, braucht
    // definition im header file
    // wenn ich inline impliz verwenden will,
    // schreibe ich einfach direkt die funktion mit
    //den geschwungenen klammern in das head file,
    //ohne trennung von deklaration und definiton
    inline size_t max_size() const;
    inline bool empty() const;
    inline double get_elem(size_t) const; // brauch ich, da der [] operator kein const ist ist und werte ‰ndern darf
    
    void push_back(double);
    void reserve (size_t);
    ostream& print(ostream&) const;
    istream& getline(istream&);
    void insert (size_t, double);
    void erase (size_t);
    void shrink_to_fit();
    void pop_back();
    void clear();
    
    //Ueberadungen - Methoden:
    double& operator[](size_t);
    Vektor& operator=(const Vektor&);
    
    //Ueberladungen -Friends:
    friend bool operator== (const Vektor&, const Vektor&);
    friend bool operator!= (const Vektor&, const Vektor&);
    friend Vektor operator+ (const Vektor&, const Vektor&);
    friend bool operator> (const Vektor&, const Vektor&);
    friend bool operator< (const Vektor&, const Vektor&);
    friend bool operator<= (const Vektor&, const Vektor&);
    friend bool operator>= (const Vektor&, const Vektor&);
    
    //Konstruktoren - initialisierungsliste?
    Vektor();
    Vektor(size_t);
    Vektor(const Vektor&); //Kopierkonstruktor, "Alternative" (?) zum Ueberladen des Zuweisungsoperators.
    
    //Dekstruktoren
    ~Vektor();
    
};


//inline Funktionen (werden direkt im Header File definiert):
inline size_t Vektor::size() const
{
    return (sz);
}

inline size_t Vektor::max_size() const
{
    return (max_sz);
}

inline double Vektor::get_elem(size_t pos) const
{
    return values[pos];
}

inline bool Vektor::empty () const
{
    if (sz < 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}


//Globale Funktionen - Ueberladungen:
ostream& operator<< (ostream&, Vektor&); //Ueberladungen entwerder als friend oder ausserhalb d klasse
// friend macht die funktion quasi zur globalen funktion

istream& operator>> (istream&, Vektor&);
