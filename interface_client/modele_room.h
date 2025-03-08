#ifndef MODELE_ROOM_H_INCLUDED
#define MODELE_ROOM_H_INCLUDED

#include "modele.h"

class ModeleRoom : public Modele
{
public:
    ModeleRoom(const int&piece,const bool&fin);
    ModeleRoom(const std::string&nom_fichier);
    ModeleRoom();
    ~ModeleRoom();
    void chargement(const std::string&nom_fichier);
    void sauvegarde(const std::string&nom_fichier)const;
    bool get_fin()const;
    void action();
    void ordre(std::string&tampon,std::istringstream&iss,std::string&commande);
    void help();
private:
    std::string m_fichier;
    std::string m_derniere_selection;
    std::map<std::string,int> m_menu;
    int m_piece;
    bool m_fin;
    std::vector<Room*>m_room;
    Room* m_store;
};

#endif // MODELE_ROOM_H_INCLUDED
