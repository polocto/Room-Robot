#ifndef CERCLE_H_INCLUDED
#define CERCLE_H_INCLUDED

#include "forme.h"


class Cercle : public Forme
{
public:
    Cercle(const Cercle&cercle,PointEncrage*centre,const std::string&base_pos);
    Cercle(PointEncrage*centre,const double&rayon,const double&angle,const Couleur&couleur,const std::string&base_pos);
    ~Cercle();

    bool collision(Forme*bloc,const Coords&deplacement)const;

    void dessiner(Svgfile&svgout)const;
    void dessiner_axe(Svgfile&svgout,const std::string&ref_pos,const std::string&end_pos);

    void sauvegarde(std::ofstream&ifs);
    void rotation(double angle,const std::string&base_pos,bool refe,const bool&somme, bool init);

    Coords get_centre(double taille)const;
    Coords get_pos(const std::string&pos);

    void init_map_chargement(std::map<std::string,PointEncrage*>&carte)const;


    PointEncrage* encre(const std::string&pos);
    double get_rayon()const;
    Coords get_centre()const;


    Forme* rattachement(Forme*forme,std::string&base_pos,std::string&ref_pos)const;

protected:
    PointEncrage* m_centre;
    PointEncrage* m_point[8];
    std::map<std::string,int>m_carte;
    double m_rayon,m_angle;
    Couleur m_couleur;

    void init_point(PointEncrage*refe,const std::string&base_pos);
};


#endif // CERCLE_H_INCLUDED
