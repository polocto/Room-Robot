#ifndef COULEUR_H_INCLUDED
#define COULEUR_H_INCLUDED

///Source TP
#include <string>
#include <cstdint>

class Couleur
{
public :
    Couleur(uint8_t rouge, uint8_t vert, uint8_t bleu, std::string couleur);
    Couleur(const Couleur&couleur);
    Couleur();
    void afficher() const;
    void saisir();
    void sauvegarde(std::ofstream&ifs)const;

    operator std::string() const;

private :
    // uint8_t �quivalent � unsigned char :
    // unsigned 8 bits (8 bits non sign�)
    uint8_t m_rouge, m_vert, m_bleu;
    std::string m_couleur;
};

#endif // COULEUR_H_INCLUDED
