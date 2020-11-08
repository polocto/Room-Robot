///Source TP

#include "couleur.h"
#include <iostream>
#include "../svg/svgfile.h"

Couleur::Couleur(uint8_t rouge, uint8_t vert, uint8_t bleu,std::string couleur)
    : m_rouge{rouge}, m_vert{vert}, m_bleu{bleu}, m_couleur(couleur)
{ }

Couleur::Couleur(const Couleur&couleur)
    :Couleur(couleur.m_rouge,couleur.m_vert,couleur.m_bleu,couleur.m_couleur)
{

}

Couleur::Couleur()
    : Couleur(0, 0, 0,"") // Délégation de constructeur
{ }


void Couleur::afficher() const
{
    std::cout << "(" << (int)m_rouge << ", " << (int)m_vert << ", " << (int)m_bleu << ")" << std::endl;
}

void Couleur::sauvegarde(std::ofstream&ifs)const
{
    if(!m_couleur.size())
        ifs<<"("<<(int)m_rouge<<","<<(int)m_vert<<","<<(int)m_bleu<<")";
    else
        ifs<<m_couleur;
}

uint8_t saisirCanal()
{
    int val;
    std::cin >> val;
    while (val<0 || val>255)
    {
        std::cout << "Mauvaise valeur couleur, recommencer : ";
        std::cin.clear();
        std::cin >> val;
    }

    return (uint8_t)val;
}

void Couleur::saisir()
{
    std::cout << "Veuillez saisir rouge vert et bleu SVP : ";
    m_rouge = saisirCanal();
    m_vert = saisirCanal();
    m_bleu = saisirCanal();
}

Couleur::operator std::string() const
{
    if(!m_couleur.size())
        return Svgfile::makeRGB(m_rouge, m_vert, m_bleu);
    else
        return m_couleur;
}
