#include "../biblio.h"

ModeleRoom::ModeleRoom(const int&piece,const bool&fin)
    :m_piece(piece),m_fin(fin),m_store(nullptr)
{
    m_menu["load"]=1;
    m_menu["reload"]=2;
    m_menu["ids"]=3;
    m_menu["noids"]=3;
    m_menu["rulers"]=3;
    m_menu["norulers"]=3;
    m_menu["@"]=3;
    m_menu["store"]=4;
    m_menu["restore"]=5;
    m_menu["exit"]=6;
    m_menu["undo"]=7;
    m_menu["redo"]=8;
    m_menu["fin"]=9;
    m_menu["sauvegarder"]=10;
    m_menu["script"]=11;
    m_menu["help"]=12;
}

ModeleRoom::ModeleRoom()
    :ModeleRoom(-1,false)
{
    Svgfile svgout;
    m_room.push_back(new RoomRobot());
    m_piece=0;
    m_room[m_piece]->dessiner(svgout);
}

ModeleRoom::ModeleRoom(const std::string&nom_fichier)
    :ModeleRoom(0,false)
{
    chargement(nom_fichier);
}

ModeleRoom::~ModeleRoom()
{
    for(size_t i=0; i<m_room.size(); i++)
        delete m_room[i];
    m_room.clear();
}
/**Charge dans fichier de nom reçus en paramètre*/
void ModeleRoom::chargement(const std::string&nom_fichier)
{
    try
    {
        std::ifstream ifs(nom_fichier);
        if(!ifs)
            throw ("Impossible d'ouvrir le fichier "+nom_fichier);

        for(size_t i=0; i<m_room.size(); i++)
            delete m_room[i];
        m_room.clear();
        m_room.push_back(new RoomRobot(ifs));
        ifs.close();
        m_piece=m_room.size()-1;
    }
    catch(std::string erreur)
    {
        std::cerr<<erreur<<std::endl;
    }
}
/**Sauvegarde dans fichier de nom reçus en paramètre*/
void ModeleRoom::sauvegarde(const std::string&nom_fichier)const
{
    try
    {
        std::ofstream ifs(nom_fichier);
        if(!ifs)
            throw(nom_fichier);
        m_room[m_piece]->sauvegarde(ifs);
        ifs.close();
        std::cout<<"Sauvegarde du fichier "<<nom_fichier<<" effectuee avec succes"<<std::endl;
    }
    catch(std::string erreur)
    {
        std::cerr<<"Sauvegarde Impossible verifier emplacement "<<erreur<<std::endl;
    }
}
/**retourne la valeur de fin pour savoir si on quitte ou on continue */
bool ModeleRoom::get_fin()const
{
    return m_fin;
}

/**menu action a réaliser*/
void ModeleRoom::ordre(std::string&tampon,std::istringstream&iss,std::string&commande)
{
    RoomRobot*p=nullptr;
    std::string fichier;
    switch(m_menu[tampon])
    {
    case 0://commande avec ancienne selection
        if( m_room.size()>0 && m_piece>=0 && m_piece<(int)m_room.size() )
        {
            commande=m_derniere_selection+" "+commande;
            if(!m_room[m_piece]->action(commande,iss)&&m_piece>0)
                m_piece--;
        }
        m_menu.erase(tampon);
        break;
    case 1://load
        iss>>m_fichier;
        if(iss.fail())
            return;
    case 2://reload
        if(!m_fichier.size())
            return;
        chargement(m_fichier);
        break;
    case 3://ids true//ids false//rulers true//rulers false//@
        if( m_room.size()>0 && m_piece>=0 && m_piece<(int)m_room.size() )
            if(!m_room[m_piece]->action(commande,iss)&&m_piece>0)
                m_piece--;
        break;
    case 4://store
        p=dynamic_cast<RoomRobot*>(m_room[m_piece]);
        if(p)
        {
            if(m_store)
                delete m_store;
            m_store=new RoomRobot(*p);
        }
        break;
    case 5://restore
        if(!m_store)
            return;
        p=dynamic_cast<RoomRobot*>(m_store);
        if(p)
        {
            m_room.push_back(new RoomRobot(*p));
            m_piece=m_room.size()-1;
        }
        break;
    case 6://quitter
        m_fin=true;
        break;
    case 7://undo
        if(m_piece>0)
            m_piece--;
        break;
    case 8://redo
        if(m_piece<(int)m_room.size()-1)
            m_piece++;
        break;
    case 9://arrête l'auto cin
        util::stopAutoCin();
        break;
    case 10://sauvegarde la room dans unfichier .txt
        iss>>fichier;
        if(!iss.fail())
            m_fichier=fichier;
        sauvegarde(m_fichier);
        break;
    case 11://débute l'auto cin
        util::startAutoCin("test.txt",90);
        break;
    case 12:
        help();
        break;
    }
}

/**saisie de la commande*/
void ModeleRoom::action()
{
    std::string line,commande,tampon;

    if(m_derniere_selection.size()>0)
        std::cout<<m_derniere_selection<<">";
    else
        std::cout<<">";
    std::getline(std::cin,line);
    std::istringstream iss(line);

    iss>>commande;
    if(commande[0]=='@')
    {
        tampon="@";
        m_derniere_selection=commande;
    }
    else
        tampon=commande;

    if(tampon=="@" || tampon=="move" || tampon=="rotate")
    {
        for(size_t i=m_piece+1; i<m_room.size(); i++)
        {
            delete m_room[m_piece+1];
            m_room.erase(m_room.begin()+m_piece+1);
        }

        RoomRobot*p=dynamic_cast<RoomRobot*>(m_room[m_piece]);
        if(p)
            m_room.push_back(new RoomRobot(*p));

        m_piece=(int)m_room.size()-1;
    }

    ordre(tampon,iss,commande);

    if(m_piece>=0&&m_piece<(int)m_room.size())
    {
        Svgfile svgout;
        m_room[m_piece]->dessiner(svgout);
    }
}

void ModeleRoom::help()
{
    std::string mot_clef;
    std::map<std::string,int>::iterator it = m_menu.begin();
    std::cout<<"Commands :"<<std::endl;
    while(it!=m_menu.end())
    {
        if(it->second>0 && it->second<12)
        {
            mot_clef = it->first;
            std::cout<<">"<<it->first<<std::endl;
        }
        it++;
    }
}
