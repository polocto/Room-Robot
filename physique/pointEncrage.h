#ifndef POINTENCRAGE_H_INCLUDED
#define POINTENCRAGE_H_INCLUDED

#include "coords.h"


class PointEncrage
{
protected:
    Coords m_ecartPointdEncrage;
    //bool m_encre;
    PointEncrage* m_point;

public:
    PointEncrage(PointEncrage&point,PointEncrage*refe);
    PointEncrage(const double&x,const double&y);
    PointEncrage(PointEncrage*point,const double&x,const double&y);
    PointEncrage(PointEncrage*point);
    ~PointEncrage();

    void affichage_parametre()const;

    PointEncrage* get_point_encrage()const;
    double getX()const;
    double getY()const;
    Coords get_ecart_point_encrage()const;

    void sauvegarde(std::ofstream&ifs)const;
    void translation(const Coords&decalage);
    void translation(const double&x,const double&y);
    void nouveau_relatif(PointEncrage*nouveau);
    operator Coords()const;
};

#endif // POINTENCRAGE_H_INCLUDED
