#include "Atom.h"

bool Atom::InitOk = false;
std::string Atom::_TypesStr[NB_ATOM_UDOCK_ID]; //Pour faire le lien avec chaque atome et son type sybyl
double Atom::_AtomsRadius[NB_ATOM_UDOCK_ID]; //Radius de chaque atome
double Atom::_AtomsEpsilons[NB_ATOM_UDOCK_ID]; //Voir cornell 1995
double Atom::_AtomsEpsilonsSquared[NB_ATOM_UDOCK_ID][NB_ATOM_UDOCK_ID]; //Voir cornell 1995, pour le calcul de Aij Bij