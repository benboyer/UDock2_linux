#ifndef ENERGY_H
#define ENERGY_H

#include <vector>
#include "Molecule.h"
#include "hasher3d.h"




#define CUTOFF_DISTANCE 16.0f //Taille des cubes du hasher 3D

class Energy
{
public:
	double energy;
	double energyContact;
	double energyCharge;
	Molecule mol1;
	Molecule mol2;// a modifier aprés
	Hasher3D h;
    Energy()
    {
    energy =0.0f;
	energyContact=0.0f;
	energyCharge=0.0f;
	h = Hasher3D();
    }


	/**
	calcMol2Pos : on se laisse la possibilite de ne pas recalculer la position de la mol2. C'est utile
	quand on fait de l'optimisation, dans ce cas on ne modifie que la position de la mol1, et on ne
	calcule qu'une seule fois la position de la mol2 et son hashage.
	*/
	//ne pas oublier de multiplier les position des molecules par la matrices produite pas bullet pour avoir leur position correcte.
    double calc(Molecule& mol1,Molecule& mol2) 
    {
    	// initialize the hasher
    	float minX,maxX,minY,maxY,minZ,maxZ;
    	mol2.getDimensions(minX,maxX,minY,maxY,minZ,maxZ);
    	float sizeMax = glm::max(glm::max((maxX-minX),(maxY-minY)),(maxZ-minZ));
    	h.createBuckets(glm::vec3(minX,minY,minZ),sizeMax,CUTOFF_DISTANCE);

		//On ajoute les atomes aux buckets
		glm::uint size = mol2.size();
		for(glm::uint i=0;i<size;i++)
		{
			h.putInBucket(mol2.atoms[i].pos,&mol2.atoms[i]);
		}

		BucketRequest request;

		float radius1 = 0;
		double charge1 = 0;
		int udockId1 = 0;
		glm::vec3 pos1,pos2;

		double r;
		double rmSurR;
		double rmSurR6;
		double rmSurR8;
		double epsilon;
		double contact;
		double charge;
		Atom * atomek;
		//pour chaque atom de mol1
		size = mol1.size();
		for(glm::uint i=0;i<size;i++)
		{
			pos1 = mol1.atoms[i].pos;
			radius1 =mol1.atoms[i].radius;
			charge1 = mol1.atoms[i].charge;
			udockId1 = mol1.atoms[i].UdockId;

			h.getBucketContentWithNeighbours(pos1,&request);

			for(int k=0;k<27;k++)
			{
				atomek = (Atom*) (request._Buckets[k]);
				while(atomek)
				{
					pos2 = atomek->pos;
					pos2 -= pos1;
					r = glm::length(pos2);

					//Lennard Jones
					rmSurR = (radius1 + atomek->radius)/r;
					rmSurR6 = rmSurR * rmSurR * rmSurR;
					rmSurR6 = rmSurR6 * rmSurR6;

					//double rmSurR12 = rmSurR6 * rmSurR6;
					rmSurR8 = rmSurR6 * rmSurR * rmSurR;

					//On prend en compte cornell 1995 pour mixer les deux parties
    				epsilon = Atom::_AtomsEpsilonsSquared[atomek->UdockId][udockId1];

					contact = epsilon * rmSurR8-(2*epsilon*rmSurR6);
					energyContact += contact;

					//Coulombic
					charge = (332.0761f/20.0f)* ((atomek->charge * charge1)/r);
					energyCharge += charge;

					//Total
					energy += contact + charge;

					//Suivant
					atomek = (Atom*) (atomek->_NextInBucket);

					// if(nbCouplesCalc)
					// 	(*nbCouplesCalc)++;
				}
			}
		}



        return energy;
    }
};

#endif




// 				_AtomsEpsilons[C_3] = 0.1094f;
// 				_AtomsEpsilons[C_2] = 0.0860f;
// 				_AtomsEpsilons[C_AR] = 0.0860f;
// 				_AtomsEpsilons[C_CAT] = 0.0860f;
// 				_AtomsEpsilons[N_3] = 0.17f;
// 				_AtomsEpsilons[N_2] = 0.17f;
// 				_AtomsEpsilons[N_4] = 0.17f;
// 				_AtomsEpsilons[N_AR] = 0.17f;
// 				_AtomsEpsilons[N_AM] = 0.17f;
// 				_AtomsEpsilons[N_PL3] = 0.17f;
// 				_AtomsEpsilons[O_3] = 0.2104f;
// 				_AtomsEpsilons[O_2] = 0.21f;
// 				_AtomsEpsilons[O_CO2] = 0.21f;
// 				_AtomsEpsilons[S_3] = 0.25f;
// 				_AtomsEpsilons[P_3] = 0.2f;
// 				_AtomsEpsilons[F] = 0.061f;
// 				_AtomsEpsilons[H] = 0.0157f;
// 				_AtomsEpsilons[LI] = 0.0183f;

// 				for(int i=0;i<NB_ATOM_UDOCK_ID;i++)
// 				{
// 					for(int j=0;j<NB_ATOM_UDOCK_ID;j++)
// 					{
// 						_AtomsEpsilonsSquared[i][j] = sqrt(_AtomsEpsilons[i]*_AtomsEpsilons[j]);
// 					}
// 				}




/**
		calcMol2Pos : on se laisse la possibilite de ne pas recalculer la position de la mol2. C'est utile
		quand on fait de l'optimisation, dans ce cas on ne modifie que la position de la mol1, et on ne
		calcule qu'une seule fois la position de la mol2 et son hashage.
		*/
// 		double calcEnergie(Molecule * mol1, Molecule * mol2, bool calcMol2Pos = true, int * nbCouplesCalc = NULL, double * pEnergyContact = NULL, double * pEnergyCharge = NULL)
// 		{
// 			//LARGE_INTEGER t1;
// 			//QueryPerformanceCounter(&t1);

// 			//On transforme les molécules (passage des atomes en repère global)
// 			NYVert3Df pos;

// 			for(int i=0;i<mol1->_NbAtomes;i++)
// 			{
// 				pos = mol1->_Atomes[i]._Pos;
// 				pos -= mol1->_Barycenter;
// 				pos = mol1->_Transform._Rot * pos;
// 				pos += mol1->_Transform._Pos;
// 				AtomesTransformesMol1[i]._Pos = pos;
// 				AtomesTransformesMol1[i]._Radius = mol1->_Atomes[i]._Radius;
// 				AtomesTransformesMol1[i]._Charge = mol1->_Atomes[i]._Charge;
// 				AtomesTransformesMol1[i]._UdockId = mol1->_Atomes[i]._UdockId;
// 			}

// 			if(calcMol2Pos)
// 			{
// 				//On va placer la seconde molecule dans le hasher pour trouver rapidement les voisins
// 				//Donc on en profite pour calculer sa taille max
// 				float minX =  mol2->_Atomes[0]._Pos.X;
// 				float minY =  mol2->_Atomes[0]._Pos.Y;
// 				float minZ =  mol2->_Atomes[0]._Pos.Z;
// 				float maxX =  mol2->_Atomes[0]._Pos.X;
// 				float maxY =  mol2->_Atomes[0]._Pos.Y;
// 				float maxZ =  mol2->_Atomes[0]._Pos.Z;

// 				for(int i=0;i<mol2->_NbAtomes;i++)
// 				{
// 					pos = mol2->_Atomes[i]._Pos;
// 					pos -= mol2->_Barycenter;
// 					pos = mol2->_Transform._Rot * pos;
// 					pos += mol2->_Transform._Pos;
// 					AtomesTransformesMol2[i]._Pos = pos;
// 					AtomesTransformesMol2[i]._Radius = mol2->_Atomes[i]._Radius;
// 					AtomesTransformesMol2[i]._Charge = mol2->_Atomes[i]._Charge;
// 					AtomesTransformesMol2[i]._UdockId = mol2->_Atomes[i]._UdockId;

// 					if(pos.X < minX)
// 						minX = pos.X;
// 					if(pos.Y < minY)
// 						minY = pos.Y;
// 					if(pos.Z < minZ)
// 						minZ = pos.Z;
// 					if(pos.X > maxX)
// 						maxX = pos.X;
// 					if(pos.Y > maxY)
// 						maxY = pos.Y;
// 					if(pos.Z > maxZ)
// 						maxZ = pos.Z;
// 				}

// 				float sizeMax = max(max((maxX-minX),(maxY-minY)),(maxZ-minZ));

// 				//On crée le hasher
// 				_Hasher->createBuckets(NYVert3Df(minX,minY,minZ),sizeMax,CUTOFF_DISTANCE);

// 				//On ajoute les atomes aux buckets
// 				for(int i=0;i<mol2->_NbAtomes;i++)
// 				{
// 					_Hasher->putInBucket(AtomesTransformesMol2[i]._Pos,AtomesTransformesMol2 + i);
// 				}
// 			}

// 			BucketRequest request;
// 			double energy = 0;
// 			double energyContact = 0;
// 			double energyCharge = 0;
// 			float radius1 = 0;
// 			double charge1 = 0;
// 			int udockId1 = 0;
// 			NYVert3Df pos1,pos2;

// 			double r;
// 			double rmSurR;
// 			double rmSurR6;
// 			double rmSurR8;
// 			double epsilon;
// 			double contact;
// 			double charge;

// 			for(int i=0;i<mol1->_NbAtomes;i++)
// 			{
// 				pos1 = AtomesTransformesMol1[i]._Pos;
// 				radius1 = AtomesTransformesMol1[i]._Radius;
// 				charge1 = AtomesTransformesMol1[i]._Charge;
// 				udockId1 = AtomesTransformesMol1[i]._UdockId;

// 				_Hasher->getBucketContentWithNeighbours(pos1,&request);

// 				for(int k=0;k<27;k++)
// 				{
// 					Atome * atomek = (Atome*) (request._Buckets[k]);
// 					while(atomek)
// 					{
// 						pos2 = atomek->_Pos;
// 						pos2 -= pos1;
// 						r = pos2.getSize();

// 						//Lennard Jones
// 						rmSurR = (radius1 + atomek->_Radius)/r;
// 						rmSurR6 = rmSurR * rmSurR * rmSurR;
// 						rmSurR6 = rmSurR6 * rmSurR6;

// 						//double rmSurR12 = rmSurR6 * rmSurR6;
// 						rmSurR8 = rmSurR6 * rmSurR * rmSurR;

// 						//On prend en compte cornell 1995 pour mixer les deux parties
//         				epsilon = Atome::_AtomsEpsilonsSquared[atomek->_UdockId][udockId1];

// 						contact = epsilon * rmSurR8-(2*epsilon*rmSurR6);
// 						energyContact += contact;

// 						//Coulombic
// 						charge = (332.0761f/20.0f)* ((atomek->_Charge * charge1)/r);
// 						energyCharge += charge;

// 						//Total
// 						energy += contact + charge;

// 						//Suivant
// 						atomek = (Atome*) (atomek->_NextInBucket);

// 						if(nbCouplesCalc)
// 							(*nbCouplesCalc)++;
// 					}
// 				}
// 			}

// 			if(pEnergyContact)
// 				*pEnergyContact = energyContact;
// 			if(pEnergyCharge)
// 				*pEnergyCharge = energyCharge;

// 			/*LARGE_INTEGER t2;
// 			QueryPerformanceCounter(&t2);
// 			LONGLONG elapsedLong = t2.QuadPart-t1.QuadPart;
// 			LARGE_INTEGER li_freq;
// 			QueryPerformanceFrequency(&li_freq);
// 			LONGLONG freq = li_freq.QuadPart;
// 			freq /= 1000;
// 			float elapsed = (float) ((float)elapsedLong/(float)freq);
// 			_cprintf("%ld ms for energy calc\n",elapsedLong);*/

// 			return energy;
// 		}