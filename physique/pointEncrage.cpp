#include "../biblio.h"
/**Copie point encrage*/
PointEncrage::PointEncrage(PointEncrage&point,PointEncrage*refe)
    :m_ecartPointdEncrage(point.m_ecartPointdEncrage),m_point(refe)
{
}

///Création point encrage base
PointEncrage::PointEncrage(const double&x,const double&y)
    :m_ecartPointdEncrage(x,y),m_point(nullptr)
{
}
///Ne pas envoyer un point d'encrage nullptr Attache à point encrage et devient encre
PointEncrage::PointEncrage(PointEncrage*point)
    :m_ecartPointdEncrage(0,0),m_point(point)
{
}


PointEncrage::PointEncrage(PointEncrage*point,const double&x,const double&y)
    :m_ecartPointdEncrage(x,y),m_point(point)
{
}

PointEncrage::~PointEncrage()
{}


PointEncrage* PointEncrage::get_point_encrage()const
{
    return m_point;
}

/**retourne la position selon l'axe x du buffer*/
double PointEncrage::getX()const
{
    if(!m_point)
        return m_ecartPointdEncrage.getX();
    else
        return m_point->getX()+m_ecartPointdEncrage.getX();
}
/**retourne la position selon l'axe y du buffer*/
double PointEncrage::getY()const
{
    if(!m_point)
        return m_ecartPointdEncrage.getY();
    else
        return m_point->getY()+m_ecartPointdEncrage.getY();
}

Coords PointEncrage::get_ecart_point_encrage()const
{
    return m_ecartPointdEncrage;
}


/**affiche les parametre du point (debug)*/
void PointEncrage::affichage_parametre()const
{
    Coords r;
    std::cout<<"Position par rapport au centre de son repere : ";
    if(m_point)
        m_ecartPointdEncrage.afficher();
    else
        r.afficher();
    std::cout<<"Position : ("<<getX()<<","<<getY()<<")\n";

}


void PointEncrage::sauvegarde(std::ofstream&ifs)const
{
    ifs<<"("<<m_ecartPointdEncrage.getX()<<","<<m_ecartPointdEncrage.getY()<<")";
}

void PointEncrage::translation(const Coords&decalage)
{
    m_ecartPointdEncrage=m_ecartPointdEncrage+decalage;
}

void PointEncrage::translation(const double&x,const double&y)
{
    translation({x,y});
}

void PointEncrage::nouveau_relatif(PointEncrage*nouveau)
{
    m_point=nouveau;
}

PointEncrage::operator Coords()const
{
    Coords point;
    if(m_point==nullptr)
        return m_ecartPointdEncrage;

    point=*m_point;
    return point+m_ecartPointdEncrage;
}
