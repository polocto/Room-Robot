#include "../../biblio.h"


/**Initialise des différents point du rectangle*/
void Rectangle::init_encre_pos(PointEncrage*ref_pos,const std::string&base_pos)
{
    int x=0,y=0;

    x=(int)m_carte[base_pos].getX();
    y=(int)m_carte[base_pos].getY();
    m_encre_pos[x][y]=ref_pos;
    for(int x1=-x ; x1 < 3-x ; x1++ )
        for(int y1=-y ; y1 < 3-y ; y1++ )
            if(y1!=0||x1!=0)
            {
                double L=(m_longueur*x1)/2.0,l=(m_largeur*y1)/2.0;
                m_encre_pos[x1+x][y1+y]=new PointEncrage(m_encre_pos[x][y],L,l);//defini la distance des autres points par rapport au point de reference
            }
}

/**Initialisation des attributs d'un rectangle*/
Rectangle::Rectangle(const double&longueur,const double&largeur,const double&angle,const Couleur&couleur,const std::string&base_pos,PointEncrage*ref_point)
    :m_longueur(longueur),m_largeur(largeur),m_angle(angle),m_couleur(couleur)
{
    m_carte["tl"]= {0,0};
    m_carte["tc"]= {1,0};
    m_carte["tr"]= {2,0};
    m_carte["ml"]= {0,1};
    m_carte["mc"]= {1,1};
    m_carte["mr"]= {2,1};
    m_carte["bl"]= {0,2};
    m_carte["bc"]= {1,2};
    m_carte["br"]= {2,2};
    init_encre_pos(ref_point,base_pos);
}

/**Copie d'un rectangle*/
Rectangle::Rectangle(Rectangle&rect,PointEncrage*ref_point,const std::string&base_pos)
    :Rectangle(rect.m_longueur,rect.m_largeur,rect.m_angle,rect.m_couleur,base_pos,ref_point)
{
    for(int x=0 ; x < 3 ; x++ )
        for(int y=0 ; y < 3 ; y++ )
        {
            Coords tampon=*rect.m_encre_pos[x][y];
            Coords tampon1=*m_encre_pos[x][y];
            m_encre_pos[x][y]->translation(tampon-tampon1);
        }
}

/**Destructeur avec libération de tout les points d'encrages du rectangles*/
Rectangle::~Rectangle()
{
    for(int x=0 ; x < 3 ; x++ )
        for(int y=0 ; y < 3 ; y++ )
            delete m_encre_pos[x][y];//libere l'espace reservé pour chacun des points encrage
    m_carte.clear();//libere la map
}

/**dessines un rectangle et ses axes si bool rulers=true*/
void Rectangle::dessiner(Svgfile&svgout)const
{
    svgout.addPallelepipede(*m_encre_pos[0][0],*m_encre_pos[0][2],*m_encre_pos[2][2],*m_encre_pos[2][0],m_couleur);
}

/**dessines les axes du rectangle avec début et fin*/
void Rectangle::dessiner_axe(Svgfile&svgout,const std::string&ref_pos,const std::string&end_pos)
{
    int xref=(int)m_carte[ref_pos].getX(),yref=(int)m_carte[ref_pos].getY(),xend=(int)m_carte[end_pos].getX(),yend=(int)m_carte[end_pos].getY();
    Coords fin=*m_encre_pos[xend][yend],debut=*m_encre_pos[xref][yref],ortho= {debut.getY()-fin.getY(),fin.getX()-debut.getX()};
    double coeff=3.5,module=fin*debut;

    ortho=ortho*(1.0/module);
    svgout.addLine(m_encre_pos[xref][yref]->getX(),m_encre_pos[xref][yref]->getY(),m_encre_pos[xend][yend]->getX(),m_encre_pos[xend][yend]->getY(),"red");
    svgout.addLine(debut.getX()-ortho.getX()*coeff,debut.getY()-ortho.getY()*coeff,debut.getX()+ortho.getX()*coeff,debut.getY()+ortho.getY()*coeff,"black");
    svgout.addTriangle( fin.getX()-ortho.getX()*coeff, fin.getY()-ortho.getY()*coeff, fin.getX()+ortho.getX()*coeff, fin.getY()+ortho.getY()*coeff, fin.getX() + (fin.getX()-debut.getX()) * (1.0/module) *coeff, fin.getY() + (fin.getY()-debut.getY()) * (1.0/module) *coeff, "black");
}

/**sauvegarde dans le fichier*/
void Rectangle::sauvegarde(std::ofstream&ifs)
{
    ifs<<"      forme=rectangle      longueur="<<m_longueur<<"       largeur="<<m_largeur<<"    angle="<<m_angle<<"     couleur=";
    m_couleur.sauvegarde(ifs);
}


/**rotation du rectangle*/
void Rectangle::rotation(double angle,const std::string&base_pos,bool refe, const bool&somme, bool init)
{
    const long double pi=3.14159265358979323846264;
    int x1=m_carte[base_pos].getX(),y1=m_carte[base_pos].getY();
    Coords tampon,tampon1=*m_encre_pos[x1][y1]->get_point_encrage(),tampon2=*m_encre_pos[x1][y1];
    if(somme)
    {
        m_angle+=angle;
        if(init)
            angle=m_angle*pi/180.0;
        else
            angle=angle*pi/180.00;
    }
    else
    {
        double tamp=angle;
        m_angle=angle-m_angle;
        angle=m_angle*pi/180.00;
        m_angle=tamp;
    }
    if(refe)
    {
        tampon= {cos(angle)*(tampon2.getX()-tampon1.getX())-sin(angle)*(tampon2.getY()-tampon1.getY())+tampon1.getX(),cos(angle)*(tampon2.getY()-tampon1.getY())+sin(angle)*(tampon2.getX()-tampon1.getX())+tampon1.getY()};
        tampon=tampon-tampon2;
        m_encre_pos[x1][y1]->translation(tampon);
    }

    tampon1=*m_encre_pos[x1][y1];
    for(int x=0; x<3; x++)
        for(int y=0; y<3; y++)
        {
            tampon2=*m_encre_pos[x][y];
            tampon= {cos(angle)*(tampon2.getX()-tampon1.getX())-sin(angle)*(tampon2.getY()-tampon1.getY())+tampon1.getX(),cos(angle)*(tampon2.getY()-tampon1.getY())+sin(angle)*(tampon2.getX()-tampon1.getX())+tampon1.getY()};
            tampon=tampon-tampon2;
            m_encre_pos[x][y]->translation(tampon);
        }
}

/**recupère la position d'un point d'encrage*/
Coords Rectangle::get_pos(const std::string&pos)
{
    return (*m_encre_pos[(int)m_carte[pos].getX()][(int)m_carte[pos].getY()]);
}

/**recupère la position centrales du rectangle - la "longueur" du text*/
Coords Rectangle::get_centre(double taille)const
{
    return{m_encre_pos[1][1]->getX()-(taille*2),m_encre_pos[1][1]->getY()};
}

/**initialise les points d'encrage pour le bloc suivant*/
void Rectangle::init_map_chargement(std::map<std::string,PointEncrage*>&carte)const
{
    carte["tl"]=m_encre_pos[0][0];
    carte["tc"]=m_encre_pos[1][0];
    carte["tr"]=m_encre_pos[2][0];
    carte["ml"]=m_encre_pos[0][1];
    carte["mc"]=m_encre_pos[1][1];
    carte["mr"]=m_encre_pos[2][1];
    carte["bl"]=m_encre_pos[0][2];
    carte["bc"]=m_encre_pos[1][2];
    carte["br"]=m_encre_pos[2][2];
}
/**Retourne le pointeur de ref pour pos ref de nouvelle form*/
PointEncrage* Rectangle::encre(const std::string&pos)
{
    return m_encre_pos[(int)m_carte[pos].getX()][(int)m_carte[pos].getY()];
}
/**verifie si un point est "au-dessus" d'un segement*/
bool Rectangle::au_dessus(const Coords c,const Coords c1,const Coords c2)const
{
    double y=0;
    if(c2.getX()!=c1.getX())
    {
        y=(c.getX()-c1.getX())*(c2.getY()-c1.getY())/(c2.getX()-c1.getX())+c1.getY();//point sur la droit c1 c2 de position x c
        if(c.getY()<=y)//si point au dessus du segement
            return true;
        else
            return false;
    }
    else if(c.getX()<=c1.getX())//si point a gauche du segement
        return true;
    else
        return false;
}
/**verifie s'il y a intersection entre deux segements pour chaque côté du rectangle*/
bool Rectangle::collision(const Rectangle&bloc,const Coords&deplacement)const
{
    Coords a=*m_encre_pos[0][0]+deplacement,b=*m_encre_pos[2][0]+deplacement,c=*m_encre_pos[2][2]+deplacement,d=*m_encre_pos[0][2]+deplacement;
    Coords e=*bloc.m_encre_pos[0][0],f=*bloc.m_encre_pos[2][0],g=*bloc.m_encre_pos[2][2],h=*bloc.m_encre_pos[0][2];
    Coords ab= (b-a)/((a*b)*2), bc = (c-b)/((b*c)*2), ef = (f-e)/((e*f)*2), fg = (g-f)/((f*g)*2) ;

    a=a+ab+bc;
    b=b-ab+bc;
    c=c-ab-bc;
    d=d+ab-bc;

    e=e+ef+fg;
    f=f-ef+fg;
    g=g-ef-fg;
    h=h+ef-fg;

    //Début côté ef
    if( au_dessus(a,e,f) != au_dessus(b,e,f) &&  au_dessus(e,a,b) != au_dessus(f,a,b) )
        return true;//ab
    if( au_dessus(b,e,f) != au_dessus(c,e,f) &&  au_dessus(e,b,c) != au_dessus(f,b,c) )
        return true;//bc
    if( au_dessus(c,e,f) != au_dessus(d,e,f) &&  au_dessus(e,c,d) != au_dessus(f,c,d) )
        return true;//cd
    if( au_dessus(d,e,f) != au_dessus(a,e,f) &&  au_dessus(e,d,a) != au_dessus(f,d,a) )
        return true;//da
    //fin ef debut fg
    if( au_dessus(a,f,g) != au_dessus(b,f,g) &&  au_dessus(f,a,b) != au_dessus(g,a,b) )
        return true;//ab
    if( au_dessus(b,f,g) != au_dessus(c,f,g) &&  au_dessus(f,b,c) != au_dessus(g,b,c) )
        return true;//bc
    if( au_dessus(c,f,g) != au_dessus(d,f,g) &&  au_dessus(f,c,d) != au_dessus(g,c,d) )
        return true;//cd
    if( au_dessus(d,f,g) != au_dessus(a,f,g) &&  au_dessus(f,d,a) != au_dessus(g,d,a) )
        return true;//da
    //fin fg debut gh
    if( au_dessus(a,g,h) != au_dessus(b,g,h) &&  au_dessus(g,a,b) != au_dessus(h,a,b) )
        return true;//ab
    if( au_dessus(b,g,h) != au_dessus(c,g,h) &&  au_dessus(g,b,c) != au_dessus(h,b,c) )
        return true;//bc
    if( au_dessus(c,g,h) != au_dessus(d,g,h) &&  au_dessus(g,c,d) != au_dessus(h,c,d) )
        return true;//cd
    if( au_dessus(d,g,h) != au_dessus(a,g,h) &&  au_dessus(g,d,a) != au_dessus(h,d,a) )
        return true;//da
    //fin gh debut he
    if( au_dessus(a,h,e) != au_dessus(b,h,e) &&  au_dessus(h,a,b) != au_dessus(e,a,b) )
        return true;//ab
    if( au_dessus(b,h,e) != au_dessus(c,h,e) &&  au_dessus(h,b,c) != au_dessus(e,b,c) )
        return true;//bc
    if( au_dessus(c,h,e) != au_dessus(d,h,e) &&  au_dessus(h,c,d) != au_dessus(e,c,d) )
        return true;//cd
    if( au_dessus(d,h,e) != au_dessus(a,h,e) &&  au_dessus(h,d,a) != au_dessus(e,d,a) )
        return true;//da
    //fin côté he
    return false;
}
/**Verifie s'il y a collision avec un cercle si oui retourne vrai sinon faux*/
bool Rectangle::collision_cercle(const double&rayon,Coords c1,const Coords&deplacement,const bool rectangle)const
{
    Coords depr;
    if(rectangle)
        depr=deplacement;
    else
        c1=c1+deplacement;
    Coords a=*m_encre_pos[0][0]+depr,b=*m_encre_pos[2][0]+depr,c=*m_encre_pos[2][2]+depr,d=*m_encre_pos[0][2]+depr;
    Coords ab= {(a.getY()-b.getY())/(a*b),(b.getX()-a.getX())/(a*b)},bc= {(b.getY()-c.getY())/(b*c),(c.getX()-b.getX())/(b*c)},cd= {(c.getY()-d.getY())/(c*d),(d.getX()-c.getX())/(c*d)},da= {(d.getY()-a.getY())/(d*a),(a.getX()-d.getX())/(d*a)};

    //Collision d'un cercle avec les côté d'un rectangle à l'aide du vecteur orthogonal au segment du rect
    if( au_dessus(a,c1,c1+ab*rayon) != au_dessus(b,c1,c1+ab*rayon) &&  au_dessus(c1,a,b) != au_dessus(c1+ab*rayon,a,b) )
        return true;//ab
    if( au_dessus(b,c1,c1+bc*rayon) != au_dessus(c,c1,c1+bc*rayon) &&  au_dessus(c1,b,c) != au_dessus(c1+bc*rayon,b,c) )
        return true;//bc
    if( au_dessus(c,c1,c1+cd*rayon) != au_dessus(d,c1,c1+cd*rayon) &&  au_dessus(c1,c,d) != au_dessus(c1+cd*rayon,c,d) )
        return true;//cd
    if( au_dessus(d,c1,c1+da*rayon) != au_dessus(a,c1,c1+da*rayon) &&  au_dessus(c1,d,a) != au_dessus(c1+da*rayon,d,a) )
        return true;//da

    ab=ab*(-1);
    bc=bc*(-1);
    cd=cd*(-1);
    da=da*(-1);

    //Collision d'un cercle avec les côté d'un rectangle à l'aide du vecteur orthogonal au segment du rect
    if( au_dessus(a,c1,c1+ab*rayon) != au_dessus(b,c1,c1+ab*rayon) &&  au_dessus(c1,a,b) != au_dessus(c1+ab*rayon,a,b) )
        return true;//ab
    if( au_dessus(b,c1,c1+bc*rayon) != au_dessus(c,c1,c1+bc*rayon) &&  au_dessus(c1,b,c) != au_dessus(c1+bc*rayon,b,c) )
        return true;//bc
    if( au_dessus(c,c1,c1+cd*rayon) != au_dessus(d,c1,c1+cd*rayon) &&  au_dessus(c1,c,d) != au_dessus(c1+cd*rayon,c,d) )
        return true;//cd
    if( au_dessus(d,c1,c1+da*rayon) != au_dessus(a,c1,c1+da*rayon) &&  au_dessus(c1,d,a) != au_dessus(c1+da*rayon,d,a) )
        return true;//da

    //verifie que le module du vecteur de chacun des quatres sommets du rectangle soit inferieur au rayon
    if( a*c1 < rayon )
        return true;//a->centre du cerlce
    if( b*c1 < rayon )
        return true;//b->centre du cerlce
    if( c*c1 < rayon )
        return true;//c->centre du cerlce
    if( d*c1 < rayon )
        return true;//d->centre du cerlce

    //si aucne condition retourne faux
    return false;
}

/**collision avec autre forme*/
bool Rectangle::collision(Forme*bloc,const Coords&deplacement)const
{
    Rectangle*pr=dynamic_cast<Rectangle*>(bloc);
    if(pr)
        return collision(*pr,deplacement);
    else
    {
        Cercle*pc=dynamic_cast<Cercle*>(bloc);
        if(pc)
        {
            return collision_cercle(pc->get_rayon(),pc->get_centre(),deplacement,true);
        }
    }
    return false;
}


Forme* Rectangle::rattachement(Forme*forme,std::string&base_pos,std::string&ref_pos)const
{
    int a=0,b=0,c=0,d=0;
    double distance=-1;
    PointEncrage* point=nullptr;
    Forme* nouveau=nullptr;
    Coords tampon1,tampon2;
    Rectangle*p=dynamic_cast<Rectangle*>(forme);
    if(p)
    {
        for(int x1=0; x1<3; x1++)
            for(int y1=0; y1<3; y1++)
            {
                tampon1=*m_encre_pos[x1][y1];
                for(int x2=0; x2<3; x2++)
                    for(int y2=0; y2<3; y2++)
                    {
                        tampon2=*p->m_encre_pos[x2][y2];
                        if(tampon1*tampon2<distance||distance<0)
                        {
                            a=x1;
                            b=y1;
                            c=x2;
                            d=y2;
                            distance=tampon1*tampon2;
                        }
                    }
            }
        tampon1=*m_encre_pos[a][b];
        tampon2=*p->m_encre_pos[c][d];
        tampon2=tampon2-tampon1;
        point=new PointEncrage(m_encre_pos[a][b],tampon2.getX(),tampon2.getY());
        if(point)
        {
            std::string tampon=base_pos;
            base_pos=pos_en_string(c,d);
            nouveau=new Rectangle(*p,point,base_pos);
            if(nouveau)
            {
                ref_pos=pos_en_string(a,b);
                delete forme;
                return nouveau;
            }
            else
            {
                base_pos=tampon;
                delete point;
            }
        }
    }
    return forme;
}
