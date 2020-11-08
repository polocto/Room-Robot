#ifndef ROOM_ROBOT_H_INCLUDED
#define ROOM_ROBOT_H_INCLUDED

#include "room.h"

class RoomRobot : public Room
{
public:
    RoomRobot(RoomRobot& robot);
    RoomRobot(std::ifstream&ifs,const bool&id,const bool&rulers);
    RoomRobot(const bool&id,const bool&rulers);
    RoomRobot(std::ifstream&ifs);
    RoomRobot();
    ~RoomRobot();

    bool action(std::string&commande,std::istringstream&iss);
    void sauvegarde(std::ofstream&ifs);
    void chargement(std::ifstream&ifs);
    void dessiner(Svgfile&svgout)const;


    void selection_blocs(std::string&selection);
    void selection_bloc(std::string selection,Bloc*bloc);

    void reccup_sauvegarde(std::ifstream&ifs);
    void reccup_valeur(std::ifstream&ifs,const std::string&id,const size_t&box);
    void reccup_valeur(std::istringstream&iss,const size_t&box);

    Forme* reccup_cercle(std::istringstream&iss,PointEncrage*point_encrage,const std::string&base_pos);
    Forme* reccup_rectangle(std::istringstream&ifs,PointEncrage*point_encrage,const std::string&base_pos);

    void init_map_longueur_largeur(std::map<std::string,Coords>&carte2);
private:
    std::vector<Bloc*>m_ensemble_blocs;
    std::vector<Bloc*>m_blocs_selectionnes;
    bool m_afficher_id,m_afficher_rulers;
    std::map<std::string,int> m_menu;
};

#endif // ROOM_ROBOT_H_INCLUDED
