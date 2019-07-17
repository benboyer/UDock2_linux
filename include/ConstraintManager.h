

#ifndef __CONSTRAINT_MANAGER_H
#define __CONSTRAINT_MANAGER_H

#include <iostream>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "render_utils.h"
#include "glm/ext.hpp"
#include "motion.h"
// a constraint is a position atached to a parent
class Constraint 
{	
	public:
	glm::vec3 pos; //the relative position of the constraint to center of the parent molecule
	btRigidBody* parent;// the molecule on wich the constraint is positioned
    float radius = 0.6f; // we assume spherique constraint
    Constraint(){
    	pos =glm::vec3();
		parent = NULL;
    }
	Constraint(glm::vec3 p,btRigidBody* body)
	{
		pos =p;
		parent = body;
	}
};

//a class that manage constraint, pair them and compute their effect(?).
class ConstraintManager
{
	// lineRenderer l;
	public:
	std::vector<Constraint> constraints;
	// std::vector<lineRenderer> links;
     
	ConstraintManager()
	{

		// l =  lineRenderer(glm::vec3(100.0,0.0,0.0),glm::vec3(-100.0,0.0,0.0));
	}

	Constraint getConstraint(int i){
		return constraints[i];
	}

	void addConstraint(glm::vec3 pos,btRigidBody* body){
		int csize =constraints.size();
		//a pair can't have it's two point on the same molecule 
		if ( csize%2 ==0 || (constraints.back().parent != body) )
		{
            std::cout<<pos.x<<" "<<pos.y<<" "<<pos.z<<" "<<std::endl;
			constraints.push_back(Constraint(pos,body));
		}
	}


	unsigned int getConstraintID( glm::vec3 contactPoint){
		Constraint c  = Constraint(glm::vec3(),NULL);
        glm::mat4 phymodel = glm::mat4(1);
		btScalar m[16];

		for (unsigned int i =0; i < constraints.size(); i++)
		{
			c =constraints[i];
			//set the correct postion for the first point
            // c.parent->getWorldTransform().getOpenGLMatrix(glm::value_ptr(phymodel ));


            c.parent->getWorldTransform().getOpenGLMatrix(m );

            phymodel = glm::make_mat4(m);

            glm::vec3 realPos= glm::vec3((phymodel)*glm::vec4(c.pos,1.0f));
			
			std::cout<<glm::length(realPos-contactPoint)<<std::endl;
			if (glm::length(realPos-contactPoint)  < constraints[i].radius )
			{
				return i;
			}
		}
		return -1;

	}


	// void removeConstraint(int ID){

	// }

	void render(lineRenderer& l ,glm::mat4 view, glm::mat4 proj)
	{
		int nbcons = constraints.size();
		if (nbcons >=2 )
		{
		// l.render();
		// lineRenderer l =  lineRenderer(glm::vec3(100.0,0.0,0.0),glm::vec3(-100.0,0.0,0.0));

			l.setView(view);
			l.setProjection(proj);
			l.setColor(glm::vec4(0.0f,0.0f,0.0f,1.0f));
	        Constraint c  = Constraint(glm::vec3(),NULL);
	        glm::mat4 phymodel = glm::mat4(1);
	        btScalar m[16];
   

	        glm::vec3 p1,p2;
			// std::cout << "yeah"<<std::endl;
			for (int i =0; i+1<nbcons; i +=2)
			{

				c =constraints[i];
				//set the correct postion for the first point
	            // c.parent->getWorldTransform().getOpenGLMatrix(glm::value_ptr(phymodel ));
	            c.parent->getWorldTransform().getOpenGLMatrix(m );
				phymodel = glm::make_mat4(m);
	            p1= glm::vec3((phymodel)*glm::vec4(c.pos,1.0f));

	            //set the second point
	            c =constraints[i+1];
	            // c.parent->getWorldTransform().getOpenGLMatrix(glm::value_ptr(phymodel ));
	           	c.parent->getWorldTransform().getOpenGLMatrix(m );
				phymodel = glm::make_mat4(m);
	            p2= glm::vec3((phymodel)*glm::vec4(c.pos,1.0f));
	            //set the line renderer
	            // std::cout << glm::to_string(p1)<<" "<<glm::to_string(p2) <<std::endl;
	            l.setPoints(p1,p2);
	            // render the line
	            l.render();
	        }
	    }
	   
	}



	void applyDockingForces(glm::vec3 center)
	{
		Constraint c1,c2  ;//= Constraint(glm::vec3(),NULL)
		glm::mat4 phymodel = glm::mat4(1);
		btScalar m[16];
		glm::vec3 p1,p2, f, t1,t2,sumForce;

		int nbcons = constraints.size();
		// std::cout << "je dock!"<<std::endl;
		std::vector<glm::vec3> forces;
		std::vector<glm::vec3> torques;
		
		if (nbcons >=2 )
		{
			for (int i =0; i+1<nbcons; i +=2)
			{
				c1 =constraints[i];
				//on stop les parents
				stopMol(c1.parent);// non optimal A changer!

				c2 =constraints[i+1];
				stopMol(c2.parent);
				//on calcul la bonne position des constrainte
			}
			for (int i =0; i+1<nbcons; i +=2)
			{

				c1 =constraints[i];
				//on calcul la bonne position des constrainte
				// c1.parent->getWorldTransform().getOpenGLMatrix(glm::value_ptr(phymodel ));
				c1.parent->getWorldTransform().getOpenGLMatrix(m );
				phymodel = glm::make_mat4(m);
				p1= glm::vec3((phymodel)*glm::vec4(c1.pos,1.0f));


				c2 =constraints[i+1];
				//on calcul la bonne position des constrainte
				// c2.parent->getWorldTransform().getOpenGLMatrix(glm::value_ptr(phymodel ));
				c2.parent->getWorldTransform().getOpenGLMatrix(m );
				phymodel = glm::make_mat4(m);
				p2= glm::vec3((phymodel)*glm::vec4(c2.pos,1.0f));

				//on calcul les forces
				f=p2-p1;
				forces.push_back(f);
				sumForce+= f;
				//si elles tirent dans une direction particuliere on les renforcent
				//on calcul les torques

				btVector3 v = c1.parent->getWorldTransform().getOrigin();
				glm::vec3 m1 = glm::vec3(v.getX(),v.getY(),v.getZ());
				glm::vec3 d1=p1-m1;

				t1 = (cross(d1,f));

				v = c2.parent->getWorldTransform().getOrigin();
				glm::vec3 m2 = glm::vec3(v.getX(),v.getY(),v.getZ());
				glm::vec3 d2=p2-m2;
				t2 = (cross(d2,-f));
				// t1 = (cross(c1.pos,f));
				// t2 = (cross(c2.pos,-f));
				// t1 = cross(p1,f);
				// t2 = cross(p2,-f);
				// std::cout <<"valforces "<< glm::length(f)<<std::endl;
	        	// std::cout << valforces<<std::endl;
	        	
	        	// float valtorque= glm::length(t1);//est ce que ca as du sens?
	        	// std::cout <<"valtorque "<< glm::length(t1)<<" "<< glm::length(t2) <<" total: "<< (glm::length(t1)+glm::length(t2))<<std::endl;
				//si les torques sont sufisament important on les appliques
				if ((glm::length(t1)+glm::length(t2))>1000)//magic number
				{
					// std::cout <<"Torque"<<std::endl;
					c1.parent->setActivationState(ACTIVE_TAG);
		        	c1.parent->applyTorque(btVector3(t1.x,t1.y,t1.z)*7.0f);//magic number
		        	c2.parent->setActivationState(ACTIVE_TAG);
		        	c2.parent->applyTorque(btVector3(t2.x,t2.y,t2.z)*7.0f);
	        	}
	        	else
	        	{
	        		//sinon on applique les forces plus une forces suplementaire vers le centre

	        		// std::cout <<"force"<<std::endl;
	        		float valforces= glm::length(f);
	        		//si les molecules ne sont pas trop proches

	        		if (valforces>0.5f){//magic number
	        			f =f *15.0f; //magic number
	        		}

	        		c1.parent->setActivationState(ACTIVE_TAG);
					c1.parent->applyForce(btVector3(f.x,f.y,f.z),btVector3(d1.x,d1.y,d1.z));
	        		c2.parent->setActivationState(ACTIVE_TAG);
	        		c2.parent->applyForce(btVector3(f.x,f.y,f.z)*-1,btVector3(d2.x,d2.y,d2.z));	  
	        		//force to center
	        		glm::vec3 f2c1 = center-m1;
	        		glm::vec3 f2c2 = center-m2;

	        		c1.parent->setActivationState(ACTIVE_TAG);
					c1.parent->applyForce(btVector3(f2c1.x,f2c1.y,f2c1.z),btVector3(d1.x,d1.y,d1.z));
	        		c2.parent->setActivationState(ACTIVE_TAG);
	        		c2.parent->applyForce(btVector3(f2c2.x,f2c2.y,f2c2.z)*-1,btVector3(d2.x,d2.y,d2.z));	      		
	        	}

	        }

	    }

	}

};

#endif
