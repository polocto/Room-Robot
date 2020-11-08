#ifndef BLOC_H_INCLUDED
#define BLOC_H_INCLUDED

#include <iostream>
#include <string>
#include "../physique/pointEncrage.h"
#include "forme/forme.h"

class RoomRobot;

class Bloc
{
public:
    Bloc(const Bloc&bloc,Bloc*precedent);
    Bloc(const std::string&id,const std::string&ref_pos,const std::string&base_pos,const std::string&end_pos,const int&entitee,PointEncrage*ref_point,Forme* forme,Bloc*precedent);
    ~Bloc();

    void reinsserer_dans_vecteur_ensemble(std::vector<Bloc*>&m_ensemble_blocs);

    ///Commande utilisateur saisie en ligne
    bool action(std::string&commande,std::string info,std::vector <Bloc*>&blocs,std::string&supplement);
    ///collision entre bloc
    bool collision(const Bloc&bloc,const Coords&deplacement)const;
    Coords collision(Bloc*bloc,double&deplacement,Bloc**mem)const;


    void changer_de_parent(Bloc*nouveau_pere);
    ///deplacement bloc selon son axe de translation
    void translater(const double&deplacement);
    void deplacement(double deplacement,std::vector <Bloc*>&blocs,const bool&somme,std::string&supplement);
    void rotation(double dep,std::vector<Bloc*>&blocs,bool refe,bool somme);
    ///Sauvegarde dans un fichier text les donnees d'un bloc
    void sauvegarde(std::ofstream&ifs,const std::string indent)const;
    void sauvegarde_valeur(std::ofstream&ifs,const std::string indent)const;

    void init_map_chargement(std::map<std::string,PointEncrage*>&carte)const;

    /**dessine le bloc dans un fichier Svg*/
    ///dessine les parametre du bloc : forme id et vecteurs
    void dessiner(Svgfile&svgout,const bool&id,const bool&rulers)const;
    void dessiner_rulers(Svgfile&svgout)const;
    void dessiner_id(Svgfile&svgout)const;

    /**lien de parent mais pas pour les cousins*/
    //descendant
    bool descendant(const Bloc*bloc)const;
    //ancetre
    bool ancetre(const Bloc*bloc)const;
    //verifie si lien de parentee
    bool parentee(const Bloc*bloc)const;

    /**Getter*/
    PointEncrage* get_ref_pos()const;
    size_t taille_suivants()const;
    std::string id()const;
    Bloc* get_suivant(const size_t&i)const;
    bool precedent()const;

private:
    std::string m_id;//nom du bloc
    std::string m_ref_pos,m_base_pos,m_end_pos;//position de ref et end par rapport au bloc precedent et base par rapport au this
    Couleur m_couleur_text;//couleur du texte(id)
    int m_entitee;//permet de determiner si de la même famille
    PointEncrage* m_ref_point;//point de ref permet de faire un translation sans avoir à aller dans forme
    Forme* m_forme;//forme du bloc : rectange cercle...
    Bloc*m_precedent;//Bloc sur lequel "je" suis rattaché
    std::vector<Bloc*>m_suivants;//Bloc qui sont rattaché à "moi"
    std::map<std::string,int> m_menu;
};



#endif // BLOC_H_INCLUDED
