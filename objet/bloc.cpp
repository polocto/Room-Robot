#include "../biblio.h"

/**Copie entière
des attributs valeurs d'un Bloc
nouvelle réallocation de Point encrage et de forme (entitées)*/
Bloc::Bloc(const Bloc&bloc,Bloc*precedent)
    :m_id(bloc.m_id),m_ref_pos(bloc.m_ref_pos),m_base_pos(bloc.m_base_pos),m_end_pos(bloc.m_end_pos),m_couleur_text(bloc.m_couleur_text),m_entitee(bloc.m_entitee)/*,m_reverse(bloc.m_reverse)*/,m_precedent(precedent),m_menu(bloc.m_menu)
{
    if(!m_precedent)
        m_ref_point=new PointEncrage(*bloc.m_ref_point,nullptr);
    else
        m_ref_point=new PointEncrage(*bloc.m_ref_point,m_precedent->m_forme->encre(bloc.m_ref_pos));

    Rectangle* pr=dynamic_cast<Rectangle*>(bloc.m_forme);
    if(pr)
    {
        m_forme=new Rectangle(*pr,m_ref_point,m_base_pos);
    }
    else
    {
        Cercle*pc=dynamic_cast<Cercle*>(bloc.m_forme);
        if(pc)
            m_forme=new Cercle(*pc,m_ref_point,m_base_pos);
    }
    for(size_t i=0; i<bloc.m_suivants.size(); i++)
        m_suivants.push_back(new Bloc(*bloc.m_suivants[i],this));
}

/**Initialisation d'un bloc*/
Bloc::Bloc(const std::string&id,const std::string&ref_pos,const std::string&base_pos,const std::string&end_pos,const int&entitee,PointEncrage*ref_point,Forme* forme,Bloc*precedent)
    :m_id(id),m_ref_pos(ref_pos),m_base_pos(base_pos),m_end_pos(end_pos),m_couleur_text(0,0,0,""),m_entitee(entitee),m_ref_point(ref_point),m_forme(forme),m_precedent(precedent)
{
    if(precedent)
        precedent->m_suivants.push_back(this);
    m_menu["move"]=1;
    m_menu["rotate"]=2;
}

/**destructeur Bloc
destruction type entitée forme
et m_pos par le biais de forme*/
Bloc::~Bloc()
{
    delete m_forme;
}

/**lorsque copie de roombloc*/
void Bloc::reinsserer_dans_vecteur_ensemble(std::vector<Bloc*>&ensemble_blocs)
{
    ensemble_blocs.push_back(this);
    for(size_t i=0; i<m_suivants.size(); i++)
        m_suivants[i]->reinsserer_dans_vecteur_ensemble(ensemble_blocs);
}

void Bloc::init_map_chargement(std::map<std::string,PointEncrage*>&carte)const
{
    m_forme->init_map_chargement(carte);
}

/**verifie les collision pour lui et tout ses enfants*/
bool Bloc::collision(const Bloc&bloc,const Coords&deplacement)const
{
    if(m_forme->collision(bloc.m_forme,deplacement))
        return true;
    for(size_t i=0; i<m_suivants.size(); i++)
        if(m_suivants[i]->collision(bloc,deplacement))//si collision return true sinon verifie les autres
            return true;
    return false;
}

/**retourne le vecteurs de deplacement final vis a vis des collisions*/
Coords Bloc::collision(Bloc*bloc,double&deplacement,Bloc**mem)const
{
    Coords dep;

    if(deplacement>0)
    {
        for(double axe=0; axe<=deplacement; axe+=0.1)
        {
            dep=(m_precedent->m_forme->get_pos(m_end_pos)-m_precedent->m_forme->get_pos(m_ref_pos))*axe/100.0;
            if(collision(*bloc,dep))
            {
                deplacement=axe-0.1;
                *mem=bloc;
                return (m_precedent->m_forme->get_pos(m_end_pos)-m_precedent->m_forme->get_pos(m_ref_pos))*deplacement/100.0;
            }
        }
    }
    else
    {
        for(double axe=0; axe>=deplacement; axe-=0.1)
        {
            dep=(m_precedent->m_forme->get_pos(m_end_pos)-m_precedent->m_forme->get_pos(m_ref_pos))*axe/100.0;
            if(collision(*bloc,dep))
            {
                deplacement=axe+0.1;
                *mem=bloc;
                return (m_precedent->m_forme->get_pos(m_end_pos)-m_precedent->m_forme->get_pos(m_ref_pos))*deplacement/100.0;
            }
        }
    }
    return (m_precedent->m_forme->get_pos(m_end_pos)-m_precedent->m_forme->get_pos(m_ref_pos))*deplacement/100.0;
}

/**verifie si lien avec ancêtre (blocs précédent)*/
bool Bloc::ancetre(const Bloc*bloc)const
{
    if(m_precedent!=nullptr && (m_precedent->m_entitee==bloc->m_entitee||bloc->m_entitee==0) && bloc->m_entitee!=-1 )
    {
        if(m_precedent!=bloc&&bloc->m_entitee!=0)
            return m_precedent->ancetre(bloc);
        else
            return true;
    }
    return false;
}

/**verifie si lien avec descendants (blocs suivants)*/
bool Bloc::descendant(const Bloc*bloc)const
{
    for(size_t i=0; i<m_suivants.size(); i++)
    {
        if(bloc!=m_suivants[i])
        {
            if(m_suivants[i]->descendant(bloc))
                return true;
        }
        else
            return true;
    }
    return false;
}

/**verifie si de la même famille*/
bool Bloc::parentee(const Bloc*bloc)const
{
    if((bloc==m_precedent && (!m_precedent || m_precedent->m_entitee!=-1))||ancetre(bloc))
        return true;
    return descendant(bloc);
}

/**sauvegarde des types valeur
appel sous-programe sauvegarde de type entitée*/
void Bloc::sauvegarde_valeur(std::ofstream&ifs,const std::string indent)const
{
    Coords a,b,c;
    if(m_id.size())
        ifs<<indent<<"id="<<m_id;
    ifs<<"     entitee="<<m_entitee<<"     ref_pos="<<m_ref_pos;
    ifs<<" ";
    if(!m_precedent)
        m_ref_point->sauvegarde(ifs);
    else
    {
        a=*m_precedent->m_forme->encre(m_ref_pos);
        b=*m_precedent->m_forme->encre(m_end_pos);
        c=*m_ref_point;
        ifs<<"  translation="<<(a*c)*100/(a*b);
    }
    ifs<<"     base_pos="<<m_base_pos<<"     end_pos="<<m_end_pos;
    m_forme->sauvegarde(ifs);
}

/**sauvegarde arborescente*/
void Bloc::sauvegarde(std::ofstream&ifs,const std::string indent)const
{
    sauvegarde_valeur(ifs,indent);
    ifs<<"\n";
    if(m_suivants.size()>0)
    {
        ifs<<indent<<"{\n";
        for(size_t i=0; i<m_suivants.size(); i++)
            m_suivants[i]->sauvegarde(ifs,indent+"    ");
        ifs<<indent<<"}\n";
    }
    else
        ifs<<indent<<"\n";

}

/**dessine bloc*/
void Bloc::dessiner(Svgfile&svgout,const bool&id,const bool&rulers)const
{
    m_forme->dessiner(svgout);
}

void Bloc::dessiner_id(Svgfile&svgout)const
{
    Coords centre=m_forme->get_centre((double)m_id.size());
    svgout.addText(centre.getX(),centre.getY(),m_id,m_couleur_text);
}

void Bloc::dessiner_rulers(Svgfile&svgout)const
{
    if(m_precedent&&m_end_pos!="null")
    {
        m_precedent->m_forme->dessiner_axe(svgout,m_ref_pos,m_end_pos);
        svgout.addDisk(m_ref_point->getX(),m_ref_point->getY(),1.5,"black");
    }
}


void Bloc::changer_de_parent(Bloc*nouveau_pere)
{
    std::map<std::string,PointEncrage*>carte;
    Forme*mem=m_forme;
    if(m_precedent)
    {
        for(size_t i=0; i<m_precedent->m_suivants.size(); i++)
            if(m_precedent->m_suivants[i]==this)
                m_precedent->m_suivants.erase(m_precedent->m_suivants.begin()+i);
    }
    m_precedent=nouveau_pere;
    m_precedent->m_suivants.push_back(this);

    m_forme=m_precedent->m_forme->rattachement(m_forme,m_base_pos,m_ref_pos);
    if(mem==m_forme)
        return;
    m_forme->init_map_chargement(carte);
    m_ref_point=carte[m_base_pos];
    for(size_t i=0; i<m_suivants.size(); i++)
    {
        m_suivants[i]->m_ref_point->nouveau_relatif(carte[m_suivants[i]->m_ref_pos]);
        if(m_suivants[i]->m_entitee==m_entitee)
            m_suivants[i]->m_entitee=m_precedent->m_entitee;
    }
    m_entitee=m_precedent->m_entitee;
    m_end_pos="null";
}


/**deplace un bloc de façon proportionnelle et s'arrête si collision*/
void Bloc::deplacement(double deplacement,std::vector <Bloc*>&blocs,const bool&somme,std::string&supplement)
{
    Coords dep;
    Bloc*mem=nullptr;

    if(!m_precedent || deplacement<-100 || deplacement>100 || m_entitee==-1 || !m_entitee || m_ref_pos=="null" || m_end_pos=="null" )
        return;

    if(!somme)
        deplacement-=100*((*m_ref_point)*m_precedent->m_forme->get_pos(m_ref_pos))/(m_precedent->m_forme->get_pos(m_end_pos)*m_precedent->m_forme->get_pos(m_ref_pos));
    else
    {
        if( deplacement < 0 && deplacement < -100 * ( (*m_ref_point) * m_precedent->m_forme->get_pos(m_ref_pos) ) / ( m_precedent->m_forme->get_pos(m_end_pos) * m_precedent->m_forme->get_pos(m_ref_pos) ))
            deplacement= -100 * ( (*m_ref_point) * m_precedent->m_forme->get_pos(m_ref_pos) ) / ( m_precedent->m_forme->get_pos(m_end_pos) * m_precedent->m_forme->get_pos(m_ref_pos) );
        else if(deplacement>100*((*m_ref_point)*m_precedent->m_forme->get_pos(m_end_pos))/(m_precedent->m_forme->get_pos(m_end_pos)*m_precedent->m_forme->get_pos(m_ref_pos)))
            deplacement=100*((*m_ref_point)*m_precedent->m_forme->get_pos(m_end_pos))/(m_precedent->m_forme->get_pos(m_end_pos)*m_precedent->m_forme->get_pos(m_ref_pos));
    }
    dep=(m_precedent->m_forme->get_pos(m_end_pos)-m_precedent->m_forme->get_pos(m_ref_pos))*deplacement/100.0;
    for(size_t i=0; i<blocs.size(); i++)
        if(!parentee(blocs[i]) && blocs[i]!=this )
            dep=collision(blocs[i],deplacement,&mem);
    m_ref_point->translation(dep);

    if(mem)
    {
        std::cout<<"Collision"<<std::endl;
        if(supplement=="catch")
            mem->changer_de_parent(this);
    }
    supplement.clear();
}

void Bloc::rotation(double angle,std::vector<Bloc*>&blocs,bool refe,bool somme)
{
    for(size_t i=0; i<m_suivants.size(); i++)
        m_suivants[i]->rotation(angle,blocs,true,somme);
    m_forme->rotation(angle,m_base_pos,refe,somme,false);
}


/**menu reçoit commande et la decortique, puis appel*/
bool Bloc::action(std::string&commande,std::string info,std::vector <Bloc*>&blocs,std::string&supplement)
{
    double dep=0;
    bool somme=false;
    dep=transfo_en_nombre(info);
    if(info.size()>0 && (info[0]=='+' || info[0]=='-'))
        somme=true;
    switch(m_menu[commande])
    {
    case 0:
        m_menu.erase(commande);
        break;
    case 1:
        deplacement(dep,blocs,somme,supplement);
        break;
    case 2:
        rotation(dep,blocs,false,somme);
        for(size_t i=0; i<blocs.size(); i++)
            if(!parentee(blocs[i]) && blocs[i]!=this && collision(*blocs[i], {0,0}))
                return false;
        break;
    }
    return true;
}

/**savoir le ième bloc suivant si existant sinon retourne nullptr*/
Bloc* Bloc::get_suivant(const size_t&i)const
{
    if(i<m_suivants.size())
        return m_suivants[i];
    else
        return nullptr;
}
/**rtourne le nombre de blocs suivants*/
size_t Bloc::taille_suivants()const
{
    return m_suivants.size();
}
/**Retourne la position de réference du bloc*/
PointEncrage* Bloc::get_ref_pos()const
{
    return m_ref_point;
}
/**Retourne l'id du bloc*/
std::string Bloc::id()const
{
    return m_id;
}
/**true si precedent différent de nullptr sinon false*/
bool Bloc::precedent()const
{
    if(m_precedent==nullptr)
        return false;
    else
        return true;
}
