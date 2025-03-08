#ifndef BIBLIO_H_INCLUDED
#define BIBLIO_H_INCLUDED

#include <iostream>
#include <fstream>


#include <string>
#include <vector>
#include <random>
#include <ctime>
#include <sstream>
#include <cstdio>
#include <map>
#include <cmath>
#include <set>
#include <sstream>

#include "util/util.h"


#include "graphisme/couleur.h"


#include "physique/coords.h"
#include "physique/pointEncrage.h"


#include "objet/bloc.h"

#include "objet/forme/forme.h"
#include "objet/forme/rectangle.h"
#include "objet/forme/cercle.h"

#include "objet/room/room.h"
#include "objet/room/room_robot.h"


#include "interface_client/modele.h"
#include "interface_client/modele_room.h"


#include "svg/svgfile.h"


std::string separer_commande_de_selection(std::string&commande,const char&separation1,const char&separation2);
bool comparaison_string(const std::string&refe,const std::string&compar);
double transfo_en_nombre(const std::string&lettres);
std::string pos_en_string(int x,int y);

#endif // BIBLIO_H_INCLUDED
