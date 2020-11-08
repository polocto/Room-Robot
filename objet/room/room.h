#ifndef ROOM_H_INCLUDED
#define ROOM_H_INCLUDED

class Room
{
public:
    virtual ~Room()=default;
    virtual void sauvegarde(std::ofstream&ifs)=0;
    virtual void chargement(std::ifstream&ifs)=0;
    virtual bool action(std::string&commande,std::istringstream&iss)=0;
    virtual void dessiner(Svgfile&svgout)const=0;
};

#endif // ROOM_H_INCLUDED
