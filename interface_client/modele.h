#ifndef MODELE_H_INCLUDED
#define MODELE_H_INCLUDED

class Modele
{
public:
    virtual ~Modele()=default;
    virtual bool get_fin()const=0;
    virtual void action()=0;
};

#endif // MODELE_H_INCLUDED
