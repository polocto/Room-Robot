#include "../../biblio.h"

/**Copie entière d'un RoomRobot
nouvelle réallocation de bloc*/
RoomRobot::RoomRobot(RoomRobot&robot)
    :m_afficher_id(robot.m_afficher_id),m_afficher_rulers(robot.m_afficher_rulers),m_menu(robot.m_menu)
{
    for(size_t i=0; i<robot.m_ensemble_blocs.size(); i++)
        if(!robot.m_ensemble_blocs[i]->precedent())
            robot.m_blocs_selectionnes.push_back(robot.m_ensemble_blocs[i]);
    for(size_t i=0; i<robot.m_blocs_selectionnes.size(); i++)
        m_blocs_selectionnes.push_back(new Bloc(*robot.m_blocs_selectionnes[i],nullptr));
    robot.m_blocs_selectionnes.clear();
    for(size_t i=0; i<m_blocs_selectionnes.size(); i++)
        m_blocs_selectionnes[i]->reinsserer_dans_vecteur_ensemble(m_ensemble_blocs);

    m_blocs_selectionnes.clear();
}

/**Nouveau RoomRobot avec aucun bloc*/
RoomRobot::RoomRobot(const bool&id,const bool&rulers)
    :m_afficher_id(id),m_afficher_rulers(rulers)
{
    ///Initialisation de la map pour le menu
    m_menu["@"]=1;
    m_menu["ids"]=2;
    m_menu["noids"]=3;
    m_menu["rulers"]=4;
    m_menu["norulers"]=5;
}

/**Nouveau RoomRobot avec chargement
dans un fichier text pour réccupérer des bloc
avec ids et rulers a initialiser*/
RoomRobot::RoomRobot(std::ifstream&ifs,const bool&id,const bool&rulers)
    :RoomRobot(id,rulers)
{
    chargement(ifs);
}
/**Nouveau RoomRobot avec chargement
dans un fichier text pour réccupérer des bloc
ids et rulers initialiser à false*/
RoomRobot::RoomRobot(std::ifstream&ifs)
    :RoomRobot(ifs,false,false)
{

}
/**Nouveau RoomRobot avec aucun bloc
ids et rulers initialiser à false*/
RoomRobot::RoomRobot()
    :RoomRobot(false,false)
{
}
/**Destructeur
ensemble de espace réservé pour les blocs sont supprimer*/
RoomRobot::~RoomRobot()
{
    for(size_t i=0; i<m_ensemble_blocs.size(); i++)
        delete m_ensemble_blocs[i];
}


/**Selection de bloc selon l'entrée de commande
Soit @bot.bla...blabla alors blabla ayant pour ancêtre @bot bla et ... sera selectionné
Soit @bot alors bot sera selectionné*/
void RoomRobot::selection_bloc(std::string selection,Bloc*bloc)
{
    std::string tampon=separer_commande_de_selection(selection,'.','@');
    if(!comparaison_string(tampon,bloc->id()) || !tampon.size())
        return;
    if(selection.size()==0&&bloc!=nullptr)
        m_blocs_selectionnes.push_back(bloc);
    else if(bloc!=nullptr&&bloc->taille_suivants()>0)
    {
        for(size_t i=0; i<bloc->taille_suivants(); i++)
            selection_bloc(selection,bloc->get_suivant(i));
    }
}

/**Selection des blocs en parcourant l'ensemble des blocs*/
void RoomRobot::selection_blocs(std::string&selection)
{
    for(size_t i=0; i<m_ensemble_blocs.size(); i++)
        selection_bloc(selection,m_ensemble_blocs[i]);
}

/**Action réaliser par la room tel que :
appeler action de bot
afficher ou masque:
-ids
-rulers
*/
bool RoomRobot::action(std::string&commande,std::istringstream&iss)
{
    std::string tampon, selection, info,supplement;

    if(commande[0]=='@')
    {
        tampon="@";
        ///separe commande et bloc a selectionner
        selection=separer_commande_de_selection(commande,' ','@');
        if(commande.size()==0)
        {
            iss>>commande;
            if(iss.fail())
                return true;
        }
    }
    else
        tampon=commande;

    switch(m_menu[tampon])
    {
    case 0:///menu par default si tampon n'existe pas
        m_menu.erase(tampon);
        break;
    case 1:
        iss>>info;
        if(iss.fail())
            return true;
        iss>>supplement;
        //sous-programme selection bloc;
        selection_blocs(selection);
        for(size_t i=0; i<m_blocs_selectionnes.size(); i++)
            if(!m_blocs_selectionnes[i]->action(commande,info,m_ensemble_blocs,supplement))
            {
                std::cout<<"Collision"<<std::endl;
                return false;
            }

        m_blocs_selectionnes.clear();
        break;
    case 2:
        m_afficher_id=true;
        break;
    case 3:
        m_afficher_id=false;
        break;
    case 4:
        m_afficher_rulers=true;
        break;
    case 5:
        m_afficher_rulers=false;
        break;
    }
    return true;
}

/**Sauvegarde les attributs valeur
appel les sous-programmes sauvagardes
des attributs entitées si existant*/
void RoomRobot::sauvegarde(std::ofstream&ifs)
{
    if(!ifs)
        throw std::runtime_error("Can't read/open ");
    for(size_t i=0; i<m_ensemble_blocs.size(); i++)
        if(!m_ensemble_blocs[i]->precedent())
            m_blocs_selectionnes.push_back(m_ensemble_blocs[i]);
    for(size_t i=0; i<m_blocs_selectionnes.size(); i++)
        m_blocs_selectionnes[i]->sauvegarde(ifs,"");
    m_blocs_selectionnes.clear();
}

/**Initialisation d'une map de traduction
pour point encreage*/
void RoomRobot::init_map_longueur_largeur(std::map<std::string,Coords>&carte)
{
    carte["tl"]= {0,0};
    carte["tc"]= {1,0};
    carte["tr"]= {2,0};
    carte["ml"]= {0,1};
    carte["mc"]= {1,1};
    carte["mr"]= {2,1};
    carte["bl"]= {0,2};
    carte["bc"]= {1,2};
    carte["br"]= {2,2};
}

Forme* RoomRobot::reccup_cercle(std::istringstream&iss,PointEncrage*point_encrage,const std::string&base_pos)
{
    std::string line,tampon,couleur;
    double rayon=-1,angle=0;
    int r=0,v=0,b=0;

    do
    {
        iss>>line;
        tampon=separer_commande_de_selection(line,'=',' ');

        if(tampon=="rayon")
            rayon=transfo_en_nombre(line);
        else if(tampon=="couleur")
        {
            tampon=separer_commande_de_selection(line,'(',' ');
            if(!line.size())
                couleur=tampon;
            else
            {
                tampon=separer_commande_de_selection(line,',',' ');

                r=(int)transfo_en_nombre(tampon);
                tampon=separer_commande_de_selection(line,',',' ');
                v=(int)transfo_en_nombre(tampon);
                tampon=separer_commande_de_selection(line,')',' ');
                b=(int)transfo_en_nombre(tampon);
            }
        }
        else if(tampon=="angle")
            angle=transfo_en_nombre(line);

    }
    while(!iss.fail());

    if(!base_pos.size()|| !point_encrage)
    {
        delete point_encrage;
        throw(tampon+"pas de base_pos impossible de continuer la lecture");
    }
    if(0>rayon)
    {
        delete point_encrage;
        throw(tampon+"longueur ou largeur egale a 0");
    }

    return new Cercle(point_encrage,rayon, angle, {(uint8_t)r,(uint8_t)v,(uint8_t)b, couleur},base_pos);
}

/**Chargement si forme rectangle
lit le point de ref de fin et de base
lit la longueur la largeur, la couleur
retourne le rectangle en type forme*/
Forme* RoomRobot::reccup_rectangle(std::istringstream&iss,PointEncrage*point_encrage,const std::string&base_pos)
{
    std::string line,tampon,couleur;
    double longueur=-1,largeur=-1,angle=0;
    int r=0,v=0,b=0;

    do
    {
        iss>>line;
        tampon=separer_commande_de_selection(line,'=',' ');

        if(tampon=="longueur")
            longueur=transfo_en_nombre(line);
        else if(tampon=="largeur")
            largeur=transfo_en_nombre(line);
        else if(tampon=="couleur")
        {
            tampon=separer_commande_de_selection(line,'(',' ');
            if(!line.size())
                couleur=tampon;
            else
            {
                tampon=separer_commande_de_selection(line,',',' ');

                r=(int)transfo_en_nombre(tampon);
                tampon=separer_commande_de_selection(line,',',' ');
                v=(int)transfo_en_nombre(tampon);
                tampon=separer_commande_de_selection(line,')',' ');
                b=(int)transfo_en_nombre(tampon);
            }
        }
        else if(tampon=="angle")
            angle=transfo_en_nombre(line);

    }
    while(!iss.fail());
    tampon.clear();
    if(!base_pos.size() || !point_encrage)
    {
        delete point_encrage;
        throw(tampon+"pas de base_pos impossible de continuer la lecture/erreur new PointEncrage*");
    }
    if(0>longueur || 0>largeur )
    {
        delete point_encrage;
        throw(tampon+"longueur ou largeur inferrieur a 0 ou imprecisee");
    }

    return new Rectangle(longueur,largeur, angle, {(uint8_t)r,(uint8_t)v,(uint8_t)b, couleur},base_pos,point_encrage);
}
/**reccup les valeurs de bloc*/
void RoomRobot::reccup_valeur(std::istringstream&iss,const size_t&box)
{
    std::string line,tampon,ref_pos="null",base_pos="tl",end_pos="null",id;
    int entitee=1;
    double x=0,y=0,translation=50;
    Forme* forme=nullptr;
    PointEncrage* point_ref=nullptr;
    Bloc* precedent=nullptr;
    std::map<std::string,PointEncrage*> carte;

    if(box>=0 && m_ensemble_blocs.size()>box )
    {
        precedent=m_ensemble_blocs[box];
        carte.clear();
        precedent->init_map_chargement(carte);
    }

    do
    {
        size_t i=0;
        iss>>line;
        while(i<line.size() && line[i]!='=')
            i++;
        if(line.size()>i && line[i]=='=')
            tampon=separer_commande_de_selection(line,'=',' ');
        else if(line.size()>0 && line[0]=='(')
        {
            separer_commande_de_selection(line,'(',' ');
            tampon=separer_commande_de_selection(line,',',' ');
            x=transfo_en_nombre(tampon);
            tampon=separer_commande_de_selection(line,')',' ');
            y=transfo_en_nombre(tampon);
        }

        if(tampon=="id")
            id=line;
        else if(tampon=="entitee")
            entitee=(int)transfo_en_nombre(line);
        else if(tampon=="ref_pos")
            ref_pos=line;
        else if(tampon=="base_pos")
            base_pos=line;
        else if(tampon=="end_pos")
            end_pos=line;
        else if(tampon=="translation")
            translation=transfo_en_nombre(line);
        else if(tampon=="forme")
        {
            if(((ref_pos=="null" || !ref_pos.size()) && precedent)||(ref_pos!="null" && ref_pos.size() && !precedent))
                throw(ref_pos+" et precedent pas coordonnees");
            if(ref_pos=="null")
                point_ref=new PointEncrage(x,y);
            else
                point_ref=new PointEncrage(carte[ref_pos],x,y);
            if(line=="rectangle")
                forme=reccup_rectangle(iss,point_ref,base_pos);
            else if(line=="cercle")
                forme=reccup_cercle(iss,point_ref,base_pos);
        }

    }
    while(!iss.fail());
    tampon.clear();
    if(!forme )
    {
        delete point_ref;
        throw(tampon+"forme=null impossible de continuer la lecture");
    }

    m_ensemble_blocs.push_back(new Bloc(id,ref_pos,base_pos,end_pos,entitee,point_ref,forme,precedent));
    m_ensemble_blocs[m_ensemble_blocs.size()-1]->deplacement(translation,m_blocs_selectionnes,false,tampon);
    if(precedent)
        forme->rotation(0,base_pos,false,true,true);
}


/**Chargement de l'ensemble des blocs pour une pièce
par lecture de fichier*/
void RoomRobot::reccup_sauvegarde(std::ifstream&ifs)
{
    size_t box=-1,niveau=-1;
    std::vector<size_t> ecart;
    std::string line,tampon;
    while(std::getline(ifs,line))
    {
        try
        {
            std::istringstream iss(line);
            tampon.clear();
            iss>>tampon;
            if(tampon=="{")
            {
                box=m_ensemble_blocs.size()-1;
                ecart.push_back(box);
                niveau++;
            }
            else if(tampon=="}")
            {
                niveau--;
                box=ecart[niveau];
                for(size_t i=ecart.size()-1; i>niveau; i--)
                    ecart.erase(ecart.begin()+i);
            }
            else if(!iss.fail())
                throw (1);
        }
        catch(int a)
        {
            try
            {
                std::istringstream iss(line);
                reccup_valeur(iss,box);
            }
            catch(std::string erreur)
            {
                size_t mem=niveau;
                std::cerr<<erreur<<std::endl;
                do
                {
                    ifs>>tampon;
                    if(tampon=="{")
                        niveau++;
                    else if(tampon=="}")
                        niveau--;
                }
                while(!ifs.fail() && niveau!=mem);
            }
        }
    }
}



/**charge à partir d'une sauvegardela piece
appel dessiner dans un fichier .svg*/
void RoomRobot::chargement(std::ifstream&ifs)
{
    reccup_sauvegarde(ifs);
    Svgfile svgout;
    dessiner(svgout);
}

/**dessine l'ensemble des bloc de la piece*/
void RoomRobot::dessiner(Svgfile&svgout)const
{
    for(size_t i=0; i<m_ensemble_blocs.size(); i++)
        m_ensemble_blocs[i]->dessiner(svgout,m_afficher_id,m_afficher_rulers);
    if(m_afficher_id)
        for(size_t i=0; i<m_ensemble_blocs.size(); i++)
            m_ensemble_blocs[i]->dessiner_id(svgout);
    if(m_afficher_rulers)
        for(size_t i=0; i<m_ensemble_blocs.size(); i++)
            m_ensemble_blocs[i]->dessiner_rulers(svgout);
}
