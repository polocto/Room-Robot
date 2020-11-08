#ifndef FORME_H_INCLUDED
#define FORME_H_INCLUDED

#include "../../physique/coords.h"
#include "../../svg/svgfile.h"
#include "../../physique/pointEncrage.h"
#include "../../graphisme/couleur.h"
#include <map>



class Forme
{
public :
    virtual ~Forme()=default;

    virtual bool collision(Forme*bloc,const Coords&deplacement)const=0;

    virtual void dessiner(Svgfile&svgout)const=0;
    virtual void dessiner_axe(Svgfile&svgout,const std::string&ref_pos,const std::string&end_pos)=0;

    virtual void sauvegarde(std::ofstream&ifs)=0;
    virtual void rotation(double angle,const std::string&base_pos,bool refe,const bool&somme, bool init)=0;

    virtual Coords get_centre(double taille)const=0;
    virtual Coords get_pos(const std::string&pos)=0;

    virtual void init_map_chargement(std::map<std::string,PointEncrage*>&carte)const=0;
    virtual PointEncrage* encre(const std::string&pos)=0;


    virtual Forme* rattachement(Forme*forme,std::string&base_pos,std::string&ref_pos)const=0;
};

#endif // FORME_H_INCLUDED
