#include "../../biblio.h"


void Cercle::init_point(PointEncrage*refe,const std::string&base_pos)
{
    const long double pi=3.14159265358979323846264;
    const long double angle=pi/4.0;
    m_carte["tr"]=7;
    m_carte["tc"]=6;
    m_carte["tl"]=5;
    m_carte["ml"]=4;
    m_carte["bl"]=3;
    m_carte["bc"]=2;
    m_carte["br"]=1;
    m_carte["mr"]=0;
    m_carte["mc"]=-1;
    if(m_carte[base_pos]<0)
        m_centre=refe;
    else
    {
        m_point[m_carte[base_pos]]=refe;
        m_centre=new PointEncrage(m_point[m_carte[base_pos]],-cos(m_carte[base_pos]*angle)*m_rayon,-sin(m_carte[base_pos]*angle)*m_rayon);
    }
    for(int i=0; i<8; i++)
        if(i!=m_carte[base_pos])
            m_point[i]=new PointEncrage(m_centre,cos(i*angle)*m_rayon,sin(i*angle)*m_rayon);
}

Cercle::Cercle(const Cercle&cercle,PointEncrage*centre,const std::string&base_pos)
    :m_rayon(cercle.m_rayon),m_angle(cercle.m_angle),m_couleur(cercle.m_couleur)
{
    init_point(centre,base_pos);

    Coords tampon;
    Coords tampon1;
    if(m_carte[base_pos]<0)
        tampon=*cercle.m_centre;
    else
        tampon=*cercle.m_point[m_carte[base_pos]];
    tampon1=*centre;
    centre->translation(tampon-tampon1);

    for(int x=0 ; x < 8 ; x++ )
    {
        tampon=*cercle.m_point[x];
        tampon1=*m_point[x];
        m_point[x]->translation(tampon-tampon1);
    }
}

Cercle::Cercle(PointEncrage*centre,const double&rayon,const double&angle,const Couleur&couleur,const std::string&base_pos)
    :m_rayon(rayon),m_angle(angle),m_couleur(couleur)
{
    init_point(centre,base_pos);
}

Cercle::~Cercle()
{
    delete m_centre;
}
/** Collision avec autre forme si oui retourne true sinon false*/
bool Cercle::collision(Forme*bloc,const Coords&deplacement)const
{
    Cercle* pc=dynamic_cast<Cercle*>(bloc);
    if(pc)
    {
        Coords c1=*m_centre+deplacement,c2=*pc->m_centre;
        if(c1*c2<pc->m_rayon+m_rayon)
            return true;
    }
    else
    {
        Rectangle*pr=dynamic_cast<Rectangle*>(bloc);
        if(pr)
            return pr->collision_cercle(m_rayon,*m_centre,deplacement,false);

    }
    return false;
}
/**dessine le disk*/
void Cercle::dessiner(Svgfile&svgout)const
{
    svgout.addDisk(m_centre->getX(),m_centre->getY(),m_rayon,m_couleur);
}
/**dessine le cercle*/
void Cercle::dessiner_axe(Svgfile&svgout,const std::string&ref_pos,const std::string&end_pos)
{
    svgout.addCircle(m_centre->getX(),m_centre->getY(),m_rayon,2,"black");
}
/**sauvegarde des attributs*/
void Cercle::sauvegarde(std::ofstream&ifs)
{
    ifs<<"      forme=cercle      rayon="<<m_rayon<<"    angle="<<m_angle<<"     couleur=";
    m_couleur.sauvegarde(ifs);
}
/**rotation par rapport à un point de reference*/
void Cercle::rotation(double angle,const std::string&base_pos,bool refe,const bool&somme, bool init)
{
    m_angle=angle;
}
/**retourne les coords de la position centrales du cercle - 3* taille sur l'axe des abscisse*/
Coords Cercle::get_centre(double taille)const
{
    return {m_centre->getX()-3*taille,m_centre->getY()};
}
/**retourne les coords du centre du cercle*/
Coords Cercle::get_centre()const
{
    return *m_centre;
}


Coords Cercle::get_pos(const std::string&pos)
{
    return *m_centre;
}

void Cercle::init_map_chargement(std::map<std::string,PointEncrage*>&carte)const
{
    carte["mr"]=m_point[0];
    carte["br"]=m_point[1];
    carte["bc"]=m_point[2];
    carte["bl"]=m_point[3];
    carte["ml"]=m_point[4];
    carte["tl"]=m_point[5];
    carte["tc"]=m_point[6];
    carte["tr"]=m_point[7];
    carte["mc"]=m_centre;
}

PointEncrage* Cercle::encre(const std::string&pos)
{
    if(m_carte[pos]<0)
        return m_centre;
    else
        return m_point[m_carte[pos]];
}

double Cercle::get_rayon()const
{
    return m_rayon;
}

Forme* Cercle::rattachement(Forme*forme,std::string&base_pos,std::string&ref_pos)const
{
    return forme;
}
