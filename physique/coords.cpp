#include "../biblio.h"
///Source TP

Coords::Coords(const Coords&position)
    :m_x(position.m_x),m_y(position.m_y)
{

}

Coords::Coords(const double x,const double y)
    :m_x(x),m_y(y)
{

}

Coords::Coords()
    :m_x(0),m_y(0)
{

}

Coords::~Coords()
{

}

///afficher les coordonnées "(x,y)"
void Coords::afficher()const
{
    std::cout<<"("<<m_x<<","<<m_y<<")"<<std::endl;
}

///Getter de la position sur l'axe x
double Coords::getX()const
{
    return m_x;
}

///Getter de la position sur l'axe y
double Coords::getY()const
{
    return m_y;
}

/// Somme vectorielle
Coords operator+(const Coords c1, const Coords c2)
{
    return { c1.m_x + c2.m_x, c1.m_y + c2.m_y };
}

/// Soustraction vectorielle
Coords operator-(const Coords c1, const Coords c2)
{
    return { c1.m_x - c2.m_x, c1.m_y - c2.m_y };
}

double operator*(const Coords c1, const Coords c2)
{
    return sqrt((c2.getX()-c1.getX())*(c2.getX()-c1.getX())+(c2.getY()-c1.getY())*(c2.getY()-c1.getY()));
}

Coords operator*(const Coords c1, const double a)
{
    return { c1.m_x * a, c1.m_y * a };
}

Coords operator/(const Coords c1, const double coeff)
{
    return { c1.m_x / coeff, c1.m_y / coeff };
}

bool operator!=(const Coords c1, const Coords c2)
{
    if(c1.m_x!=c2.m_x||c1.m_y!=c2.m_y)
        return true;
    return false;
}

bool operator==(const Coords c1, const Coords c2)
{
    if(c1.m_x!=c2.m_x||c1.m_y!=c2.m_y)
        return false;
    return true;
}

