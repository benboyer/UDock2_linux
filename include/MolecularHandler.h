#ifndef MOLECULE_HANDLER_H
#define MOLECULE_HANDLER_H

#include <vector>
#include "Molecule.h"
#include "surfaces.h"
//bullet header
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/Gimpact/btGImpactShape.h"
#include "BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h"

// GLM headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/string_cast.hpp>

#include <boost/thread.hpp>


#include "Energy.h"
#include "motion.h"

//manage the different molecules present in the scene and generate the coresponding vertices.












btDiscreteDynamicsWorld * initBullet(){
        ///-----initialization_start-----

    ///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

    ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

    ///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
    btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();

    ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

    btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,overlappingPairCache,solver,collisionConfiguration);

    btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher);//
    // dynamicsWorld->setGravity(btVector3(0,-10,0));
    dynamicsWorld->setGravity(btVector3(0,0.0,0.0));
    return dynamicsWorld;
}

btGImpactMeshShape * calcTriMeshShape(float * vertices, int nbVertices)
{
    btTriangleMesh * trimesh = new btTriangleMesh();

    for (int i = 0; i < nbVertices; i+=3)
    {
        btVector3 vertex0(vertices[3*i], vertices[(3*i)+1],  vertices[(3*i)+2]);
        btVector3 vertex1(vertices[3*(i+1)], vertices[(3*(i+1))+1],  vertices[(3*(i+1))+2]);
        btVector3 vertex2(vertices[3*(i+2)], vertices[(3*(i+2))+1],  vertices[(3*(i+2))+2]);

        //Attention au sens de la normale !
        trimesh->addTriangle(vertex2,vertex1,vertex0);
    }

    btGImpactMeshShape  * shape  = new btGImpactMeshShape(trimesh);
    // shape->setMargin(0.05f);
    shape->setMargin(0.20f);
    shape->updateBound();

    //si modif call btGImpactMeshShape.postUpdate()

    return shape;

}



class MolecularHandler
{
// private:
public:
    std::vector<Molecule> molecules; ///liste des differentes molecules
    // std::vector<Surface> surfaces;
    std::vector<std::vector<float>> vertbb;//vertices for the bilboard shader 
    std::vector<std::vector<float>> vertsurf;//vertices for the bilboard shader
    // std::vector<btGImpactMeshShape *> trimeshShape;
    // std::vector<glm::mat4> initialPosition;
    std::vector<glm::mat4> matrixToCenter;
    std::vector<btRigidBody*> rigidBody;
    std::vector<bool> active;
    bool doSurface;
    bool doPhysic;
    btDiscreteDynamicsWorld * dynamicsWorld;

    float energy=0.0f;

public:
    int selectedMol;

    MolecularHandler(btDiscreteDynamicsWorld * dynWorld)
    {
        molecules = std::vector<Molecule>();
        // surfaces  = std::vector<Surface>();
        vertbb  = std::vector<std::vector<float>>();
        vertsurf  = std::vector<std::vector<float>>();
        selectedMol = -1;
        dynamicsWorld = dynWorld;
    }

    void clear(){
        //delete mol from physic simulation
        std::cout << rigidBody.size()<< std::endl;

        for (unsigned int i = 0; i<rigidBody.size(); i ++){
            dynamicsWorld->removeRigidBody(rigidBody[i]);
            // delete rigidBody[i]->getMotionState();
            // delete rigidBody[i];
        }
        // erase all of molecule data
        molecules.clear();
        vertsurf.clear();
        vertbb.clear();
        matrixToCenter.clear();
        rigidBody.clear();
        active.clear();

        // dynamicsWorld->stepSimulation(0.001, 7);
    }
    std::vector<float> getVertices()
    {
        std::vector<float> vertices;

        glm::vec3 temp;
        glm::uint molSize = 0;
        Molecule mol;
        for (glm::uint i = 0; i < molecules.size(); i++)
        {
            mol = molecules[i];
            molSize = mol.size();

            for (glm::uint j = 0; j < molSize * 3; j += 3)
            {
                temp = mol.atoms[j / 3].pos;
                vertices.push_back(temp.x);
                vertices.push_back(temp.y);
                vertices.push_back(temp.z);
            }
        }
        return vertices;
    }

    glm::vec3 getCenterofScene()
    {
        glm::vec3 center =glm::vec3(0.0);
        glm::uint molNB=molecules.size();
        if (molNB > 0 ){
            for (glm::uint i = 0; i < molNB; i++)
            {
                // center += glm::vec3((getModelMatrix(i) *matrixToCenter[i]*initialPosition[i])*glm::vec4(molecules[i].barycenter,1.0f));
                center +=getInitialPos(i);//to change for moving protein ?
            }

            return center / (float)molNB; 
        }
        return center;
    }

    glm::mat4 getModelMatrix(int i)
    {
        btScalar m[16];
        // glm::mat4 phymodel =  glm::mat4(1);
        // rigidBody[i]->getWorldTransform().getOpenGLMatrix(glm::value_ptr(phymodel ));
        rigidBody[i]->getWorldTransform().getOpenGLMatrix(m );
        glm::mat4 phymodel = glm::make_mat4(m);
        // return   initialPosition[i]* phymodel  *matrixToCenter[i];


        // return   phymodel  *matrixToCenter[i];
        return phymodel;
    }

    glm::vec3 getInitialPos( int i)//!\ solution non perenne, a ameliorer! 
    {
        float step = 65.0f;
        if (i < 8){
            std::vector<glm::vec3> v;
            v.push_back(glm::vec3(-step,0.0f,0.0f));
            v.push_back(glm::vec3(step,0.0f,0.0f));
            v.push_back(glm::vec3(0.0f,step,0.0f));
            v.push_back(glm::vec3(0.0f,-step,0.0f));
            v.push_back(glm::vec3(step,step,0.0f));
            v.push_back(glm::vec3(-step,-step,0.0f));
            v.push_back(glm::vec3(step,-step,0.0f));
            v.push_back(glm::vec3(-step,step,0.0f));
            return v[i];
        }
        else{
            // std::cout <<(360.0f/16.0f)*((i-8)%16)<<" " <<-step *(((i-8)/16)+2) << std::endl;
            // std::cout << glm::to_string(glm::vec3(glm::rotate(glm::radians((360.0f/16.0f)*((i-8)%16)), glm::vec3(0.0f,0.0f,1.0f) ) * glm::vec4(-step *(((i-8)/16)+2),0.0f,0.0f,1.0f)))<< std::endl;
            return glm::vec3(glm::rotate(glm::radians((360.0f/16.0f)*((i-8)%16)), glm::vec3(0.0f,0.0f,1.0f) ) * glm::vec4(-step *(((i-8)/16)+3),0.0f,0.0f,1.0f));
        }
    }

    glm::mat4 getMatricToCenter(Molecule m, int i)//!\ solution non perenne, a ameliorer! 
    {
        m.calcBarycenter();
        glm::mat4 model = glm::translate(glm::mat4(1.0f),glm::vec3()-m.barycenter);;
        return model;
    }

    void addMol(Molecule m)
    {
        molecules.push_back(m);
        selectedMol = molecules.size()-1;
        //configuration for optimisation, by default the first mol is active and the others are not
        if (selectedMol == 0)
        {
            active.push_back(true);
        }
        else
        {
            active.push_back(false);
        }
        //data for bb shader
        std::vector<float> vertices;
        glm::uint molSize = m.size();
        glm::vec3 temp;
        for (glm::uint j = 0; j < molSize * 3; j += 3)
        {
            temp = m.atoms[j / 3].pos;
            vertices.push_back(temp.x);
            vertices.push_back(temp.y);
            vertices.push_back(temp.z);
        }
        vertbb.push_back(vertices);

        matrixToCenter.push_back(getMatricToCenter(m, selectedMol));//to replace with a proper function
        // matrixToCenter.push_back(glm::mat4(1));//to replace with a proper function


        // initialPosition.push_back(getInitialPos(m, selectedMol));
        // data for surf shader
        //data for surface shader
        Surface renderSurf = Surface();
        renderSurf.getSurface(m,0.8);
        vertsurf.push_back(renderSurf.mc.getSurfVerticesColorsNormal());


        Surface s = Surface();
        s.getPhysicSurface(m,1.1);

        //trimesh computation
        s.mc.makeVerticesOnlyBuffer(glm::vec3()-m.barycenter);
        float * surfvertices = s.mc._Vertices;
        int nbPoints = s.mc._NbVertices;
        btGImpactMeshShape * shape=calcTriMeshShape(surfvertices,nbPoints);
        
        /// Create Dynamic Objects
        btTransform startTransform;
        startTransform.setIdentity();

        btScalar mass(1.f);

        //rigidbody is dynamic if and only if mass is non zero, otherwise static
        bool isDynamic = (mass != 0.f);

        btVector3 localInertia(0,0,0);
        if (isDynamic)
            shape->calculateLocalInertia(mass,localInertia);


        glm::vec3 ip =  getInitialPos(selectedMol);
        startTransform.setOrigin(btVector3(ip.x,ip.y,ip.z));
        //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
        btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,shape,localInertia);
        
        btRigidBody* molrigid = new btRigidBody(rbInfo);
        // setBodyPosition(molrigid,glm::vec3(mh.molecules[0].barycenter.x,mh.molecules[0].barycenter.y,mh.molecules[0].barycenter.z));
        dynamicsWorld->addRigidBody(molrigid);
        rigidBody.push_back(molrigid);
        // btVector3 molrigidpos = molrigid->getWorldTransform().getOrigin();
        // std::cout <<"pos molrigid "<<molrigidpos.getX()<<" "<<molrigidpos.getY()<<" "<<molrigidpos.getZ() <<" "<< std::endl;


    }

    Molecule getMol (btRigidBody* r){
        int nbmol = molecules.size();
        for (int i = 0; i < nbmol; i++)
        {
            if (r == rigidBody[i]){
                return molecules[i];
            }
        } 
        return Molecule();
    }

    Molecule getNextMol()
    {
        selectedMol = (selectedMol + 1) % molecules.size();
        return molecules[selectedMol];
    }

    void threadedEnergyCalc ()
    { 
        new boost::thread ( &MolecularHandler::energyCalc, this);
    }



    void energyCalc ()
    {
        int nbmol = molecules.size();
        if (molecules.size()>=2)
        {
            double totalEner =0;

            std::vector<Molecule> phymol;

            for (int i = 0; i < nbmol; i++)
            {  
                glm::mat4 mat1 = getModelMatrix(i);
                Atom at;
                Molecule mNew;
                Molecule m = molecules[i];
                int msize = m.size();
                for (int k = 0; k < msize; k++)
                {   
                    at = m.atoms[k];
                    at.pos = glm::vec3(mat1*glm::vec4(at.pos,1.0f));
                    mNew.atoms.push_back(at);
                }
                phymol.push_back(mNew);
                // //test
                // mNew.calcBarycenter();
                // std::cout << glm::to_string(mNew.barycenter)<<" "<<std::endl;

            }

            Energy ener = Energy();
            for (int i = 0; i < nbmol; i++)
            {             
                for (int j = i+1; j < nbmol; j++)
                {
                    
                    totalEner += ener.calc(phymol[i], phymol[j]);
                }
            }

            
            energy = totalEner;
            // return totalEner;
        }
        else
        {
            energy = 0.0f;
            // return 0;
        }
    }

void newOptimMC(float elapsed,bool& optim,  float& prog){
    static float startTime = 0.0f;
    static float endTime = 3.5f;// length of the simulation, in second
    static std::vector<Molecule> phymol;
    static std::vector<glm::mat4>  physrotmat;
    std::vector<Molecule> randmol;
    glm::mat4 randmat;

    double epsilon = 0.0001; // for energie comparaison to avoid precision error

    static glm::mat4 bestMat = glm::mat4(1.0f); 
    static double bestEner=0.0;
    double newEner =0;

    // if there is at least a pair of molecule
    int nbmol = molecules.size();
    if (optim)
    {    
        //init---------------------------------------------------------------------
        if (startTime == 0.0f)
        {       
            
            for (int i = 0; i < nbmol; i++)
            {  
                //we freeze the mol so they can't move
                setFreeze(rigidBody[i],true);

                //we calculate the proper position of the molecules
                glm::mat4 mat1 = getModelMatrix(i);
                Atom at;
                Molecule mNew;
                Molecule m = molecules[i];

                int msize = m.size();

                for (int k = 0; k < msize; k++)
                {   
                    at = m.atoms[k];
                    at.pos = glm::vec3(mat1*glm::vec4(at.pos,1.0f));
                    mNew.atoms.push_back(at);
                }
                mNew.calcBarycenter();

                phymol.push_back(mNew);

                randmol.push_back(mNew); //we enter the mol so the vector isn't empty

                //we keep track of the matrix
                physrotmat.push_back(mat1);

            }

            energyCalc ();
            bestEner = energy;
            // std::cout <<""<<std::endl;
            // std::cout <<"------------------------ "<<std::endl;
            // std::cout <<"starting energy "<<bestEner <<std::endl;
        }


        // #main-----------------------------------------------------------------------------------

        Atom at;
        Molecule m;
        //we modify the position of active molecule at random
        for (int i = 0; i < nbmol; i++)
        {  
            if (active[i])
            {

                //random rotation
                float rotx,roty,rotz;
                int angle = 15*100;
                rotx = glm::radians((float)(std::rand()%angle - (angle/2))/100.0f);
                roty = glm::radians((float)(std::rand()%angle - (angle/2))/100.0f);
                rotz = glm::radians((float)(std::rand()%angle - (angle/2))/100.0f);

                randmat = glm::eulerAngleYXZ(roty,rotx,rotz);//everything must be in radian AND BEWARE the order is |Y|xz
                // glm::mat4 randmat = glm::eulerAngleYXZ(0.1f,0.0f,0.0f);

                // // glm::mat4 randmat = glm::mat4(1); 

                //random translation
                float transx,transy,transz;
                int limit = 3*100;
                transx = (float)(std::rand()%limit - (float)(limit/2.0f))/100.0f;
                transy = (float)(std::rand()%limit - (float)(limit/2.0f))/100.0f;
                transz = (float)(std::rand()%limit - (float)(limit/2.0f))/100.0f;
                // glm::vec3 randtrans = glm::vec3(transx,transy,transz);
                randmat = glm::translate(randmat, glm::vec3(transx,transy,transz));
                // // glm::vec3 randtrans = glm::vec3(0.0,0.0,0.0);




                // we calculate the proper position of the molecules
                glm::mat4 mat1 = physrotmat[i];
                Atom at;
                Molecule mNew;
                Molecule m = molecules[i];

                int msize = m.size();

                // glm::mat4 randmat = glm::eulerAngleYXZ(glm::radians(360.0),0.0,0.0)
                for (int k = 0; k < msize; k++)
                {   
                    at = m.atoms[k];
                    at.pos = glm::vec3(mat1 * randmat *  bestMat * glm::vec4(at.pos,1.0f));//multiplication order is important
                    mNew.atoms.push_back(at);
                }
                mNew.calcBarycenter();

                randmol.push_back(mNew);

               


            } 
            else
            {
                randmol.push_back(phymol[i]);
            }


        }

        Energy ener = Energy();
        for (int i = 0; i < nbmol; i++)
        {             
            for (int j = i+1; j < nbmol; j++)
            {
                
                newEner += ener.calc(randmol[i], randmol[j]);
            }
        }

        std::cout << "newEner "<<newEner<<" \tcontact "<<ener.energyContact<< " \tcharge " << ener.energyCharge << std::endl;

    }


    if (newEner < (bestEner - epsilon))
    {
        // std::cout << "hit! " << std::endl;
        bestEner = newEner;
        bestMat = randmat* bestMat;


    // for (int i = 0; i < nbmol; i++)
    //     {
    //         if (active[i])
    //         {
    // std::cout << "bestMat "<< glm::to_string(physrotmat[i] *bestMat) <<std::endl;
    // }}


    }
    // std::cout << "best energy "<< bestEner <<" new energy "<< newEner <<std::endl;
    

    // std::cout << "best mat: "<< glm::to_string(bestMat) <<std::endl;

    
    startTime += elapsed;
    
    //progress
    prog = (startTime/endTime) *100;


    //end-----------------------------------------------------------------------------------
    if (startTime> endTime)
    {

        //we move the active mol according to the best solution
        for (int i = 0; i < nbmol; i++)
        {
            if (active[i])
            {
                                glm::mat4 currentphysmat = getModelMatrix(i);

                // std::cout << "currentphysmat: "<< glm::to_string(currentphysmat) <<std::endl;

                rigidBody[i]->setActivationState(ACTIVE_TAG);
                btDefaultMotionState* myMotionState = (btDefaultMotionState*)rigidBody[i]->getMotionState();

                // btTransform oldTrans,newTrans;



                btTransform trans;
                myMotionState->getWorldTransform(trans);
                float matrix[16] ;
                trans.getOpenGLMatrix(matrix);
                glm::mat4 bulletMat = glm::make_mat4(matrix);


                // glm::mat4 m1 = glm::eulerAngleYXZ(0.0f,0.0f,glm::radians(90.0f));
                // glm::mat4 m2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0,10.0,0.0));


                //operation
                // std::cout << "bullet Mat: "<< glm::to_string(bulletMat) <<std::endl;

                // std::cout << "best mat: "<< glm::to_string(bestMat) <<std::endl;
                // std::cout << "matrixToCenter: "<< glm::to_string(matrixToCenter[i]) <<std::endl;

                // glm::mat4 newMat =  bulletMat * m2* m1 ;
                glm::mat4 newMat =  bulletMat * matrixToCenter[i] * bestMat ;
                // glm::mat4 newMat = physrotmat[i]* bestMat;
                // glm::mat4 newMat =  bulletMat;


                // std::cout << "physrotmat[i]: "<< glm::to_string(physrotmat[i]) <<std::endl;
                // std::cout << "physrotmat[i] * inv: "<< glm::to_string(physrotmat[i]* inverse (matrixToCenter[i])) <<std::endl;
                // std::cout << "newMat mat: "<< glm::to_string(newMat) <<std::endl;

                newMat = newMat * inverse (matrixToCenter[i]);

                // std::cout << "newMat mat: "<< glm::to_string(newMat) <<std::endl;




                trans.setFromOpenGLMatrix (glm::value_ptr(newMat));
                myMotionState->setWorldTransform(trans);
                rigidBody[i]->setWorldTransform(trans);
                rigidBody[i]->setMotionState(myMotionState);


                currentphysmat = getModelMatrix(i);
                                // std::cout << "physrotmat[i] * inv: "<< glm::to_string(physrotmat[i]) <<std::endl;

                // std::cout << "currentphysmat : "<< glm::to_string(currentphysmat)<<std::endl;

            }
        }

        //clean up
        optim =false;
        startTime = 0.0f;

        bestEner = 0.0f;
        bestMat = glm::mat4(1.0);

        //we clear the molecules:
        phymol.clear();
        physrotmat.clear();


        //we free the mol to move again
        for (int i = 0; i < nbmol; i++)
        {  
            // setFreeze(rigidBody[i],false);
            stopMol(rigidBody[i]);
        }

            // energyCalc ();
            // bestEner = energy;
            // std::cout <<""<<std::endl;
            // std::cout <<"End energy "<<bestEner <<std::endl;
            // bestEner = 0.0f;
    }



        
}

// void optimMC (float elapsed,bool& optim, double& ener, float& prog ){
//         static float startTime = 0.0f;
//         static float endTime = 2.5f;// length of the simulation, in second
//         static std::vector<Molecule> phymol;
//         std::vector<Molecule> randmol;
//         static std::vector<glm::mat4>  physrotmat;
//         std::vector<glm::mat4>  randrotmat;
//         static std::vector<glm::vec3> physpos;
//         std::vector<glm::vec3> randtransvec;
//         std::vector<btTransform> oldTransform;
//         double newEner =0.0f;

//         if (optim)
//         {
//             // if there is at least a pair of molecule
//             int nbmol = molecules.size();
//             if (molecules.size()>=2)
//             {
//                 //init---------------------------------------------------------------------
//                 if (startTime == 0.0f)
//                 {       
//                     // std::cout << "yo "<<std::endl;

                    
//                     for (int i = 0; i < nbmol; i++)
//                     {  
//                         //we freeze the mol so they can't move
//                         setFreeze(rigidBody[i],true);

//                         //we calculate the proper position of the molecules
//                         glm::mat4 mat1 = getModelMatrix(i);
//                         Atom at;
//                         Molecule mNew;
//                         Molecule m = molecules[i];
//                         int msize = m.size();
//                         for (int k = 0; k < msize; k++)
//                         {   
//                             at = m.atoms[k];
//                             at.pos = glm::vec3(mat1*glm::vec4(at.pos,1.0f));
//                             mNew.atoms.push_back(at);
//                         }
//                         mNew.calcBarycenter();

//                         phymol.push_back(mNew);
//                         //we keep track of the matrix
//                         physrotmat.push_back(mat1);
//                         //and of the position
//                         btVector3 mpos=rigidBody[i]->getWorldTransform().getOrigin();
//                         physpos.push_back(glm::vec3(mpos.getX(),mpos.getY(),mpos.getZ()));

//                     }
//                 }

//                 //main---------------------------------------------------------------
//                 Atom at;
//                 Molecule m;
//                 //we modify the position of active molecule at random
//                 for (int i = 0; i < nbmol; i++)
//                 {  
//                     if (active[i])
//                     {
//                         Molecule mNew;
//                         //random rotation
//                         float rotx,roty,rotz;
//                         int angle = 15*100;
//                         rotx = glm::radians((float)(std::rand()%angle - (angle/2))/100.0f);
//                         roty = glm::radians((float)(std::rand()%angle - (angle/2))/100.0f);
//                         rotz = glm::radians((float)(std::rand()%angle - (angle/2))/100.0f);

//                         glm::mat4 randmat = glm::eulerAngleYXZ(rotx,roty,rotz);
//                         // glm::mat4 randmat = glm::eulerAngleYXZ(0.1f,0.0f,0.0f);

//                         // glm::mat4 randmat = glm::mat4(1); 

//                         //random translation
//                         float transx,transy,transz;
//                         int limit = 5*100;
//                         transx = (float)(std::rand()%limit - (float)(limit/2.0f))/100.0f;
//                         transy = (float)(std::rand()%limit - (float)(limit/2.0f))/100.0f;
//                         transz = (float)(std::rand()%limit - (float)(limit/2.0f))/100.0f;
//                         glm::vec3 randtrans = glm::vec3(transx,transy,transz);
//                         // glm::vec3 randtrans = glm::vec3(0.0,0.0,0.0);






//                         //physical position
//                         glm::mat4 phymat = getModelMatrix(i);
//                         physrotmat[i] = phymat;//on restock la matrice au cas ou elle as ete changer
//                         btVector3 btmpos=rigidBody[i]->getWorldTransform().getOrigin();

//                         physpos[i] = glm::vec3(glm::vec4(glm::vec3(btmpos.getX(),btmpos.getY(),btmpos.getZ()),1));

                        
//                         //test
//                         glm::mat4 phymodel =  glm::mat4(1);
//                         rigidBody[i]->getWorldTransform().getOpenGLMatrix(glm::value_ptr(phymodel ));
//                         // return phymodel  *matrixToCenter[i];


//                         // m= molecules[i];
//                         // int msize = m.size();
//                         // for (int j = 0; j < msize; j++)
//                         // {   
//                         //     at = m.atoms[j];
//                         //     // at.pos = glm::vec3(glm::mat3(randmat)*glm::vec4(at.pos,1.0f))+randtrans;//+physpos[i];

//                         //     // at.pos = glm::vec3(glm::mat3(matrixToCenter[i]*randmat*phymodel)*glm::vec4(at.pos,1.0f))+randtrans;//+physpos[i]
//                         //     // at.pos = glm::vec3(randmat*phymat*glm::vec4(at.pos,1.0f))+randtrans;//+physpos[i]
//                         //     at.pos = glm::vec3(glm::quat_cast(glm::mat3(randmat*physrotmat[i]))*at.pos)+randtrans;//+physpos[i]


//                         //     mNew.atoms.push_back(at);
//                         // }
//                         randrotmat.push_back(randmat);
//                         randtransvec.push_back(randtrans);
//                         // mNew.calcBarycenter();
//                         // randmol.push_back(mNew);
//                         //test
                        

//                         rigidBody[i]->setActivationState(ACTIVE_TAG);
//                         btDefaultMotionState* myMotionState = (btDefaultMotionState*)rigidBody[i]->getMotionState();

//                         btTransform oldTrans,newTrans;

//                         myMotionState->getWorldTransform(oldTrans);
//                         myMotionState->getWorldTransform(newTrans);
//                         glm::quat rotation = glm::quat_cast(glm::mat3(randrotmat[i]*physrotmat[i]));
//                         newTrans.setRotation(btQuaternion(rotation.x,rotation.y,rotation.z,rotation.w));
//                         glm::vec3 position = physpos[i]+randtransvec[i];
//                         newTrans.setOrigin(btVector3(position.x,position.y,position.z));

//                         myMotionState->setWorldTransform(newTrans);
//                         rigidBody[i]->setWorldTransform(newTrans);
//                         rigidBody[i]->setMotionState(myMotionState);

//                         // proposedTrans.push_back(newTrans);
//                         oldTransform.push_back(oldTrans);
//                         // std::cout << glm::to_string(mNew.barycenter)<<" "<<std::endl;
//                         // setBodyPosition(rigidBody[i], glm::vec3(mpos.getX(),mpos.getY(),mpos.getZ())+randtrans);
//                         // std::cout <<"yo! "<< transx<<" "<<transy<<" "<<transz<<" "<<std::endl;
//                         // setBodyRotation(rigidBody[i], glm::quat_cast(phymat*randmat));
//                         // btmpos=rigidBody[i]->getWorldTransform().getOrigin();
//                         // glm::vec3  mpos=glm::vec3(glm::vec4(glm::vec3(btmpos.getX(),btmpos.getY(),btmpos.getZ()),1));

//                         // std::cout <<"proposed "<<glm::to_string(mpos)<< std::endl;

//                     } 
//                     else
//                     {
//                         randmol.push_back(phymol[i]);
//                         randrotmat.push_back(glm::mat4(1));
//                         randtransvec.push_back(glm::vec3());
//                     }
//                 } 
//                 // we then calculate the energy of the new syteme
//                 energyCalc ();
//                 newEner = energy;

//                 // std::cout <<"ener "<<newEner <<" "<< std::endl;


//                 if (!(newEner < ener)){


//                     //restore energy 
//                     for (int i = 0; i < nbmol; i++)
//                     {
//                         if (active[i])
//                         {
//                             rigidBody[i]->setActivationState(ACTIVE_TAG);
//                             btDefaultMotionState* myMotionState = (btDefaultMotionState*)rigidBody[i]->getMotionState();

//                             myMotionState->setWorldTransform(oldTransform[i]);
//                             rigidBody[i]->setWorldTransform(oldTransform[i]);
//                             rigidBody[i]->setMotionState(myMotionState);


//                             // btVector3 btmpos=rigidBody[i]->getWorldTransform().getOrigin();
//                             // glm::vec3  mpos=glm::vec3(glm::vec4(glm::vec3(btmpos.getX(),btmpos.getY(),btmpos.getZ()),1));

//                             // std::cout <<"final "<<glm::to_string(mpos)<< std::endl;
//                             // std::cout <<"ener "<<newEner<<" "<< energyCalc () <<" "<< std::endl;

//                         }


//                     }

//                 }

//                 else {
//                     ener = newEner;

//                     // std::cout <<"ener "<<newEner<<" "<< energyCalc () <<" "<< std::endl;

//                 }

//             }

//             startTime += elapsed;
//             //progress
//             prog = (startTime/endTime) *100;
//             // std::cout <<prog<<" "<< std::endl;
//             //end-----------------------------------------------------------------------------------
//             if (startTime> endTime)
//             {
//                 optim =false;
//                 startTime = 0.0f;
//                 int nbmol = molecules.size();
//                 //we clear the molecules:
//                 phymol.clear();
//                 physpos.clear();
//                 physrotmat.clear();
//                 //we free the mol to move again
//                 for (int i = 0; i < nbmol; i++)
//                 {  
//                     // setFreeze(rigidBody[i],false);
//                     stopMol(rigidBody[i]);
//                 }

//             }
//         }
//     }


    void limitMolsVelocity(){
        float limit = 500.0f;
        int nbmol = molecules.size();
        for (int i = 0; i < nbmol; i++)
        { 
            glm::vec3 lv = getLinearVelocity(rigidBody[i]);
            glm::vec3 av = getAngularVelocity(rigidBody[i]);

            if (glm::length(lv)>limit){
                lv = glm::normalize(lv) *limit;
                setLinearVelocity(rigidBody[i],lv);
           
           }
            if (glm::length(av)>limit){
                av = glm::normalize(av) *limit;
                setAngularVelocity(rigidBody[i],av);
           }
            
        } 
    }

    //what happens when you drop a new molecule during the damp?
    void dampMols(float elapsed, bool damp, Molecule * ignoreMol = NULL)
    {
        static std::vector<glm::vec3> angDamping,linDamping;
        static float _DureeFade = 1.0f;
        static bool lastDamp = false;
        static float elapsedFade = 0;

        if(!damp)
        {
            lastDamp = false;
            return;
        }



        // on recupere les vitesse angulaire et lineaire  
        std::vector<glm::vec3> angSpeed,linSpeed;

        int nbmol = molecules.size();
        for (int i = 0; i < nbmol; i++)
        { 
           linSpeed.push_back(getLinearVelocity(rigidBody[i]));
           angSpeed.push_back(getAngularVelocity(rigidBody[i]));
        } 

        //initialisation 
        if(lastDamp == false && damp == true)
        {
            for (int i = 0; i < nbmol; i++)
            { 
                angDamping.push_back((angSpeed[i] / _DureeFade));
                linDamping.push_back((linSpeed[i] / _DureeFade));
            } 
            lastDamp = true;
            elapsedFade = 0.0f;
        }

        for (int i = 0; i < nbmol; i++)
        {
            if(glm::abs(angSpeed[i].x) >= glm::abs(angDamping[i].x)*elapsed){
                angSpeed[i] -= angDamping[i]*elapsed;
            }
            else{
                angSpeed[i] = glm::vec3(0.0f,0.0f,0.0f);
            }
            if(glm::abs(linSpeed[i].x) >= glm::abs(linDamping[i].x)*elapsed){
                linSpeed[i] -= linDamping[i]*elapsed;
            }
            else{
                linSpeed[i] = glm::vec3(0.0f,0.0f,0.0f);
            }

        }
        //new velocity
        for (int i = 0; i < nbmol; i++)
        {
            setLinearVelocity(rigidBody[i],linSpeed[i]);
            setAngularVelocity(rigidBody[i],angSpeed[i]);
        }

        //fin
        elapsedFade += elapsed;
        if(elapsedFade > _DureeFade)
        {
            elapsedFade = 0.0f;
            lastDamp = false;
            angDamping.clear();
            linDamping.clear();
        }
    }




};



#endif
