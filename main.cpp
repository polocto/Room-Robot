#include "biblio.h"

int main()
{
    Modele* a=new ModeleRoom("sauvegarde.txt");
    while(!a->get_fin())
        a->action();
    delete a;
    return 0;
}
