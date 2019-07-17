#ifndef ATOM_H
#define ATOM_H

#include <iostream>
#include <glm/glm.hpp>
#include "hasher3d.h"

struct Atom : public Bucketable
{
	public:
    glm::vec3 pos; ///< Atom cartesian coordinates
    std::string atomType; ///< CA, N, HN1, ...
    std::string atomElement; ///< C, N, H, O, etc.
    std::string residueType; ///< LEU, ARG, ...
    std::string chain; ///< A, B, etc.
    glm::uint residueId; ///< residue number
    glm::uint atomId; ///< atom number
    glm::uint UdockId; ////<internal ID
    double charge; ///< charge of the atom
    float radius;

    enum
	{
		C_3 = 0, //Pour chque atome, du plus court au plus long
		C_2,
		C_AR,
		C_CAT,
		N_3,
		N_2,
		N_4,
		N_AR,
		N_AM,
		N_PL3,
		O_3,
		O_2,
		O_CO2,
		S_3,
		P_3,
		F,
		H,
		LI,
		NB_ATOM_UDOCK_ID
	};


	// static atribute*
	public:
	static std::string _TypesStr[NB_ATOM_UDOCK_ID]; //Pour faire le lien avec chaque atome et son type sybyl
	static double _AtomsRadius[NB_ATOM_UDOCK_ID]; //Radius de chaque atome
	static double _AtomsEpsilons[NB_ATOM_UDOCK_ID]; //Voir cornell 1995
	static double _AtomsEpsilonsSquared[NB_ATOM_UDOCK_ID][NB_ATOM_UDOCK_ID]; //Voir cornell 1995, pour le calcul de Aij Bij
	static bool InitOk;

	Atom()
	{
		radius = 0.0f;
		charge = 0.0f;
		UdockId = -1;


		if(!InitOk)
		{
			InitOk = true;
			_TypesStr[C_3] = "C.3";
			_TypesStr[C_2] = "C.2";
			_TypesStr[C_AR] = "C.ar";
			_TypesStr[C_CAT] = "C.cat";
			_TypesStr[N_3] = "N.3";
			_TypesStr[N_2] = "N.2";
			_TypesStr[N_4] = "N.4";
			_TypesStr[N_AR] = "N.ar";
			_TypesStr[N_AM] = "N.am";
			_TypesStr[N_PL3] = "N.pl3";
			_TypesStr[O_3] = "O.3";
			_TypesStr[O_2] = "O.2";
			_TypesStr[O_CO2] = "O.co2";
			_TypesStr[S_3] = "S.3";
			_TypesStr[P_3] = "P.3";
			_TypesStr[F] = "F";
			_TypesStr[H] = "H";
			_TypesStr[LI] = "Li";

			_AtomsRadius[C_3] = 1.908f;
			_AtomsRadius[C_2] = 1.908f;
			_AtomsRadius[C_AR] = 1.908f;
			_AtomsRadius[C_CAT] = 1.908f;
			_AtomsRadius[N_3] = 1.875f;
			_AtomsRadius[N_2] = 1.824f;
			_AtomsRadius[N_4] = 1.824f;
			_AtomsRadius[N_AR] = 1.824f;
			_AtomsRadius[N_AM] = 1.824f;
			_AtomsRadius[N_PL3] = 1.824f;
			_AtomsRadius[O_3] = 1.721f;
			_AtomsRadius[O_2] = 1.6612f;
			_AtomsRadius[O_CO2] = 1.6612f;
			_AtomsRadius[S_3] = 2.0f;
			_AtomsRadius[P_3] = 2.1f;
			_AtomsRadius[F] = 1.75f;
			_AtomsRadius[H] = 1.4870f;
			_AtomsRadius[LI] = 1.137f;

			_AtomsEpsilons[C_3] = 0.1094f;
			_AtomsEpsilons[C_2] = 0.0860f;
			_AtomsEpsilons[C_AR] = 0.0860f;
			_AtomsEpsilons[C_CAT] = 0.0860f;
			_AtomsEpsilons[N_3] = 0.17f;
			_AtomsEpsilons[N_2] = 0.17f;
			_AtomsEpsilons[N_4] = 0.17f;
			_AtomsEpsilons[N_AR] = 0.17f;
			_AtomsEpsilons[N_AM] = 0.17f;
			_AtomsEpsilons[N_PL3] = 0.17f;
			_AtomsEpsilons[O_3] = 0.2104f;
			_AtomsEpsilons[O_2] = 0.21f;
			_AtomsEpsilons[O_CO2] = 0.21f;
			_AtomsEpsilons[S_3] = 0.25f;
			_AtomsEpsilons[P_3] = 0.2f;
			_AtomsEpsilons[F] = 0.061f;
			_AtomsEpsilons[H] = 0.0157f;
			_AtomsEpsilons[LI] = 0.0183f;

			for(int i=0;i<NB_ATOM_UDOCK_ID;i++)
			{
				for(int j=0;j<NB_ATOM_UDOCK_ID;j++)
				{
					_AtomsEpsilonsSquared[i][j] = sqrt(_AtomsEpsilons[i]*_AtomsEpsilons[j]);
				}
			}
		}
	}

	int findAtomUdockId(void)
	{
		for(int i=0;i<NB_ATOM_UDOCK_ID;i++)
		{
			std::string type = atomType;
			if(_TypesStr[i].substr(0,type.length()) == type)
				return i;
		}
		
		std::cout<<("Unknown atom "+atomType).c_str()<< std::endl; 
		return 0;
	}

};


#endif