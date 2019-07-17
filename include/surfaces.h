#ifndef SURFACES_H
#define SURFACES_H


#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

#include "Molecule.h"
#include "marching_cubes.h"

#define PROBE_SIZE 1.4f

#define BUFFER_OFFSET(i) (reinterpret_cast<void*>(i))

/// Nettoyage d'un pointeur (cree avec new)
#define SAFEDELETE(a) { if (a) {delete a; a = NULL; }}

/// Nettoyage d'un tableau (cree par new[])
#define SAFEDELETE_TAB(a) { if (a) {delete [] a; a = NULL; }}

class Surface
{
public:
	MarchingCubes mc;
	int _NbThreads;

	typedef struct
	{
		MarchingCubes * _MCubes;
		Molecule * _Molecule;
		int _XStart;
		int _YStart;
		int _ZStart;
		int _NbX;
		int _NbY;
		int _NbZ;
	}PARAMS_THREAD_ELECTROSTATICS;

    Surface()
    {
        mc =  MarchingCubes();
        _NbThreads =  boost::thread::hardware_concurrency();
    }

    void getLowPolySurface(Molecule m){
		float minX,maxX,minY,maxY,minZ,maxZ;
		m.getDimensions(minX,maxX,minY,maxY,minZ,maxZ);
		float probe_boost = 0.0f; //a modifier ulterieurement 

		//pour l'instant on fait uniquement du low poly
		float taille_cube = 0.6f;
		// float taille_cube = 2.8f;

		mc.setOrigin(glm::vec3(minX-5.0f - (probe_boost/2.0f),minY-5.0f- (probe_boost/2.0f),minZ-5.0f- (probe_boost/2.0f)));
	    std::cout <<"origin "<< minX-5.0f - (probe_boost/2.0f)<<" "<<minY-5.0f- (probe_boost/2.0f)<<" "<<minZ-5.0f- (probe_boost/2.0f)<< std::endl;
		mc.create(maxX-minX+10.0f + (probe_boost),maxY-minY + 10.0f + (probe_boost),maxZ-minZ + 10.0f + (probe_boost),taille_cube,true);
		std::cout <<"create "<<maxX-minX+10.0f + (probe_boost)<<" "<<maxY-minY + 10.0f + (probe_boost)<<" "<<maxZ-minZ + 10.0f + (probe_boost)<< std::endl;

		//On crée la van der valls sans interpolation
		mc.setInterpolation(false);

		float pcent = 0;
		glm::uint NbAtom = m.size();

		for(glm::uint i=0;i<NbAtom;i++)
		{
			if(pcent)
				pcent += 1.0f/(float)NbAtom;
			mc.valideSphere(m.atoms[i].pos,m.atoms[i].radius + (PROBE_SIZE + probe_boost),m.atoms[i].charge);
		}

		mc.setLissageNormales(false);
		mc.destroyTempGeometry();
		// mc.makeVerticesOnlyBuffer(glm::vec3(0.0f,0.0f,0.0f));
		
		// mc.makeGeometryFaces(glm::vec3(0.0f,0.0f,0.0f));

	}


	void getSurface(Molecule m,float cube_size){
		float minX,maxX,minY,maxY,minZ,maxZ;
		m.getDimensions(minX,maxX,minY,maxY,minZ,maxZ);

		float probe_boost = 0.0f; //a modifier ulterieurement 

		//pour l'instant on fait uniquement du low poly
		// float cube_size = 0.5f;

		mc.setOrigin(glm::vec3(minX-5.0f - (probe_boost/2.0f),minY-5.0f- (probe_boost/2.0f),minZ-5.0f- (probe_boost/2.0f)));
		std::cout <<"origin "<< minX-5.0f - (probe_boost/2.0f)<<" "<<minY-5.0f- (probe_boost/2.0f)<<" "<<minZ-5.0f- (probe_boost/2.0f)<< std::endl;
		mc.create(maxX-minX+10.0f + (probe_boost),maxY-minY + 10.0f + (probe_boost),maxZ-minZ + 10.0f + (probe_boost),cube_size,true);
		std::cout <<"create "<<maxX-minX+10.0f + (probe_boost)<<" "<<maxY-minY + 10.0f + (probe_boost)<<" "<<maxZ-minZ + 10.0f + (probe_boost)<< std::endl;


		// On crée la SAS sans interpolation, on marque juste les cubes
		#ifdef MSVC
		if(_CrtCheckMemory() == false)
		#endif
		// On crée la van der valls sans interpolation
		mc.setInterpolation(false);

		float pcent = 0;
		glm::uint NbAtom = m.size();

		for(glm::uint i=0;i<NbAtom;i++)
		{
			if(pcent)
				pcent += 1.0f/(float)NbAtom;
			mc.valideSphere(m.atoms[i].pos,m.atoms[i].radius *0.95f+ (PROBE_SIZE + probe_boost),0);
		}

		#ifdef MSVC
		if(_CrtCheckMemory() == false)
		#endif
		
		// STEP 2
		// On reduit la SAS sur un diamètre PROBE_SIZE
		mc.contractSurface(PROBE_SIZE + probe_boost);
		// mc.contractSurface(probe_boost);
		
		
		//STEP 3
		//On calcule tout bien la partie van der walls de la SES
		mc.setInterpolation(true);

		for(glm::uint i=0;i<NbAtom;i++)
		{
			if(pcent)
				pcent += 0.33f/(float)NbAtom;
			mc.valideSphere(m.atoms[i].pos,m.atoms[i].radius * 0.95,0);
			// mc.valideSphere(m.atoms[i].pos,m.atoms[i].radius * 0.95,m.atoms[i].charge);

		}

		//On précalcule nous meme les normales
		// mc.setLissageNormales(true);
		mc.lissageNormales();
		// mc.destroyTempGeometry();

		// STEP 4 
		// On calcule la charge des cubes
		calculateElectrostatics(m);



		// mc.makeVerticesOnlyBuffer(glm::vec3(0.0f,0.0f,0.0f));
		

		// std::vector<float> vertices;
		// glm::uint NbVertices = mc._NbVertices;
		// for (glm::uint i =0 ;i<NbVertices;i++)
		// {
		// 	vertices.push_back(mc._Vertices[i]);
		// }

		// return vertices;
	}

	//the surface used for the physic engine, with coarser cube and sligthy more contracted surface
	void getPhysicSurface(Molecule m,float cube_size = 1.2f){
		float minX,maxX,minY,maxY,minZ,maxZ;
		m.getDimensions(minX,maxX,minY,maxY,minZ,maxZ);

		float probe_boost = 0.0f; //a modifier ulterieurement 

		//pour l'instant on fait uniquement du low poly
		// float cube_size = 0.5f;

		mc.setOrigin(glm::vec3(minX-5.0f - (probe_boost/2.0f),minY-5.0f- (probe_boost/2.0f),minZ-5.0f- (probe_boost/2.0f)));
		std::cout <<"origin "<< minX-5.0f - (probe_boost/2.0f)<<" "<<minY-5.0f- (probe_boost/2.0f)<<" "<<minZ-5.0f- (probe_boost/2.0f)<< std::endl;
		mc.create(maxX-minX+10.0f + (probe_boost),maxY-minY + 10.0f + (probe_boost),maxZ-minZ + 10.0f + (probe_boost),cube_size,true);
		std::cout <<"create "<<maxX-minX+10.0f + (probe_boost)<<" "<<maxY-minY + 10.0f + (probe_boost)<<" "<<maxZ-minZ + 10.0f + (probe_boost)<< std::endl;


		// On crée la SAS sans interpolation, on marque juste les cubes
		#ifdef MSVC
		if(_CrtCheckMemory() == false)
		#endif
		// On crée la van der valls sans interpolation
		mc.setInterpolation(false);

		float pcent = 0;
		glm::uint NbAtom = m.size();

		for(glm::uint i=0;i<NbAtom;i++)
		{
			if(pcent)
				pcent += 1.0f/(float)NbAtom;
			mc.valideSphere(m.atoms[i].pos,m.atoms[i].radius *0.95f+ (PROBE_SIZE + probe_boost),0);
		}

		#ifdef MSVC
		if(_CrtCheckMemory() == false)
		#endif
		
		//STEP 2
		//On reduit la SAS sur un diamètre PROBE_SIZE
		mc.contractSurface(PROBE_SIZE + probe_boost);
		
		//STEP 3
		//On calcule tout bien la partie van der walls de la SES
		mc.setInterpolation(true);

		for(glm::uint i=0;i<NbAtom;i++)
		{
			if(pcent)
				pcent += 0.33f/(float)NbAtom;
			mc.valideSphere(m.atoms[i].pos,m.atoms[i].radius * 0.95,0);
		}

		//On précalcule nous meme les normales
		// mc.setLissageNormales(true);
		mc.lissageNormales();
		// mc.destroyTempGeometry();

		// STEP 4 // a integrer plus tard
		// On calcule la charge des cubes
		// this->calculateElectrostatics();



		// mc.makeVerticesOnlyBuffer(glm::vec3(0.0f,0.0f,0.0f));
		

		// std::vector<float> vertices;
		// glm::uint NbVertices = mc._NbVertices;
		// for (glm::uint i =0 ;i<NbVertices;i++)
		// {
		// 	vertices.push_back(mc._Vertices[i]);
		// }

		// return vertices;
	}



	void render(){
		// mc.renderFaces();
		mc.renderFacesNormal();
	}

	private:



	void calculateElectrostatics(Molecule m)
	{
	    int stepZ = (mc.NbZ / _NbThreads);

		boost::thread **  threads = new boost::thread *[_NbThreads];
		PARAMS_THREAD_ELECTROSTATICS * params = new PARAMS_THREAD_ELECTROSTATICS[_NbThreads];

	    for(int i=0;i<_NbThreads;i++)
	    {
	        params[i]._MCubes = &mc;
	        params[i]._Molecule = &m;
	        params[i]._XStart = 0;// + (radius/(2*TailleCube)) + 1;
	        params[i]._YStart = 0;
	        params[i]._ZStart = i*(mc.NbZ / _NbThreads);
	        params[i]._NbX = mc.NbX;
	        params[i]._NbY = mc.NbY;
	        params[i]._NbZ = stepZ;
	        threads[i] = new boost::thread(threadElectrostatics,params + i);
	    }

	    //On attend la fin des threads
	    for(int i=0;i<_NbThreads;i++)
	       threads[i]->join();

	    for(int i=0;i<_NbThreads;i++)
	        SAFEDELETE(threads[i]);

		SAFEDELETE_TAB(threads);
		SAFEDELETE_TAB(params);
	}

	static void threadElectrostatics(PARAMS_THREAD_ELECTROSTATICS * params)
	{
	    glm::vec3 pos;
	    glm::vec3 normal;
	    float charge;
	    uint code;

	    //Log::log(Log::USER_INFO,("Thread electrostatics from " + toString(params->ZStart) + " to " + toString(params->ZStart+params->NbZ)).c_str());

	    MarchingCubes * mcubes = params->_MCubes;
	    Molecule * mol = params->_Molecule;
	    for (int x=params->_XStart;x<params->_NbX;x++)
	    {
	        for (int y=params->_YStart;y<params->_NbY;y++)
	        {
	            for (int z=params->_ZStart;z<params->_ZStart+params->_NbZ;z++)
	            {
	                code = mcubes->getCubeCode(x,y,z);
	                if(code !=0 && code != 255)
	                {
	                    mcubes->getCubeBarycentreCoords(pos,x,y,z);
	                    mcubes->getCubeFaceNormal(normal,x,y,z);
	                    charge = mol->getCharge(pos + (normal * PROBE_SIZE));
	                    mcubes->setCubeColorShift(charge,x,y,z);
	                }
	            }
	        }
	    }

	}

};





#endif