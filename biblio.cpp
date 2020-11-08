#include "biblio.h"

std::string separer_commande_de_selection(std::string&commande,const char&separation1,const char&separation2)
{
    std::string selection;
    if(commande.size()>0 && commande[0]==separation2)
        commande.erase(0,1);
    while(commande.size()>0 && commande[0]!=separation1)
    {
        selection.push_back(commande[0]);
        commande.erase(0,1);
    }
    if(commande.size()>0 && commande[0]==separation1)
        commande.erase(0,1);

    return selection;
}

bool comparaison_string(const std::string&refe,const std::string&compar)
{
    std::string nombre;
    int n=0,m=0,num;
    bool modulo=false;

    if(!refe.size())
        return false;

    if(refe==compar)
        return true;

    for(size_t i=0; i<refe.size(); i++)
    {
        if(refe[i]=='%')
        {
            modulo=true;
            n=(int)transfo_en_nombre(nombre);
            nombre.clear();
        }
        else if(refe[i]>='0'&&refe[i]<='9')
            nombre+=refe[i];
    }
    m=(int)transfo_en_nombre(nombre);
    nombre.clear();
    if(modulo)
    {
        size_t i=0;
        while(i<refe.size()&&(refe[i]<'0' || refe[i]>'9'))
        {
            if(i>=compar.size() || compar[i]!=refe[i])
                return false;
            i++;
        }
        while(i<compar.size() && compar[i]>='0' && compar[i]<='9')
        {
            nombre+=compar[i];
            i++;
        }
        if( i > compar.size() || ( i < compar.size() && ( compar[i] < '0' || compar[i] > '9' ) ) )
            return false;
        if(i==compar.size() && i>0 && ( compar[i-1] < '0' || compar[i-1] > '9' ))
            return false;

        num=(int)transfo_en_nombre(nombre);
        nombre.clear();
        if(num%m==n%m)
            return true;
        else
            return false;
    }
    else if(refe.size()>compar.size())
        return false;
    else
    {
        for(size_t i=0; i<refe.size(); i++)
            if(refe[i]!=compar[i])
                return false;
        return true;
    }
}

double transfo_en_nombre(const std::string&lettres)
{
    double nombre=0,apres_virgule=1;
    bool positif=true,avant_virgule=true;
    size_t i=0;

    while(i<lettres.size())
    {
        if(lettres[i]>='0'&&lettres[i]<='9')
        {
            if(avant_virgule)
            {
                nombre*=10;
                nombre+=(double)(lettres[i]-'0');
            }
            else
            {
                nombre+=((double)(lettres[i]-'0'))/(10.0*apres_virgule);
                apres_virgule++;
            }
        }
        else if(lettres[i]=='.')
            avant_virgule=false;
        else if(lettres[i]=='-')
            positif=false;
        else if(lettres[i]=='%')
            i=lettres.size();
        else if(lettres[i]!='+')
            return 0;
        i++;
    }
    if(positif)
        return nombre;
    else
        return -nombre;
}

std::string pos_en_string(int x,int y)
{
    std::string retour;
    if(!y)
        retour+="t";
    else if(y==1)
        retour+="m";
    else
        retour+="b";
    if(!x)
        retour+="l";
    else if(x==1)
        retour+="c";
    else
        retour+="r";
    return retour;
}
