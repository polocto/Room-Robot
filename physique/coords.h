#ifndef COORDS_H_INCLUDED
#define COORDS_H_INCLUDED
///Source TP
#include <iostream>

class Coords
{
protected:
    double m_x,m_y;
public:
    Coords(const Coords&position);
    Coords(const double x,const double y);
    Coords();
    ~Coords();

    ///afficher les coordonnées "(x,y)"
    void afficher()const;
    ///Getter de la position sur l'axe x
    double getX()const;
    ///Getter de la position sur l'axe y
    double getY()const;
    /// Somme vectorielle
    friend Coords operator+(const Coords c1, const Coords c2);

    /// Soustraction vectorielle
    friend Coords operator-(const Coords c1, const Coords c2);

    /// Calcul la distance de c1 à c2 (module du vecteur c1c2)
    friend double operator*(const Coords c1, const Coords c2);

    /// multiplie les coordonnée x et y par le coeff a
    friend Coords operator*(const Coords c1, const double a);

    /// divise les coordonnée x et y par le coeff
    friend Coords operator/(const Coords c1, const double coeff);

    ///retourne vrai si x et y de c1 et c2 sont différent, faux sinon
    friend bool operator!=(const Coords c1, const Coords c2);

    ///retourne vrai si x et y de c1 et c2 sont identiques, faux sinon
    friend bool operator==(const Coords c1, const Coords c2);
    //friend Coords operator+=(Coords& c1, const Coords& c2);
    /// Egalite vectorielle
    // friend Coords operator=(const Coords& c);
};


#endif // COORDS_H_INCLUDED
