#ifndef RECTANGLE_H_INCLUDED
#define RECTANGLE_H_INCLUDED

#include "forme.h"

class Rectangle : public Forme
{
public:
    Rectangle(Rectangle&rect,PointEncrage*ref_point,const std::string&base_pos);
    Rectangle(const double&longueur,const double&largeur,const double&angle,const Couleur&couleur,const std::string&base_pos,PointEncrage*ref_point);
    ~Rectangle();

    bool collision(Forme*bloc,const Coords&deplacement)const;
    bool collision(const Rectangle&bloc,const Coords&deplacement)const;
    bool collision_cercle(const double&rayon,Coords c1,const Coords&deplacement,const bool rectangle)const;
    bool au_dessus(const Coords c,const Coords c1,const Coords c2)const;

    void dessiner(Svgfile&svgout)const;
    void dessiner_axe(Svgfile&svgout,const std::string&ref_pos,const std::string&end_pos);
    void sauvegarde(std::ofstream&ifs);
    void rotation(double angle,const std::string&base_pos,bool refe,const bool&somme, bool init);
    Coords get_centre(double taille)const;
    Coords get_pos(const std::string&pos);
    PointEncrage* encre(const std::string&pos);

    void init_map_chargement(std::map<std::string,PointEncrage*>&carte)const;

    Forme* rattachement(Forme*forme,std::string&base_pos,std::string&ref_pos)const;

private:
    double m_longueur,m_largeur,m_angle;
    Couleur m_couleur;
    PointEncrage* m_encre_pos[3][3];
    std::map<std::string,Coords> m_carte;


    void init_encre_pos(PointEncrage*ref_pos,const std::string&base_pos);
};

#endif // RECTANGLE_H_INCLUDED
