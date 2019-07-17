#ifndef __HASHER_3D_H__
#define __HASHER_3D_H__

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <cstring> //memset
/// Nettoyage d'un pointeur (cree avec new)
#define SAFEDELETE(a) { if (a) {delete a; a = NULL; }}


/**
  * Tout objet hashable dans le hasher3d doit hériter de Bucketable
  */
class Bucketable
{
	public:
		Bucketable * _NextInBucket;
		glm::vec3 _Pos;

		Bucketable()
		{
			_NextInBucket = NULL;
		}
};

/**
  * Pour effectuer une requete avec les buckets voisins du bucket principal
  */
class BucketRequest
{
	public:
		Bucketable * _Buckets[27];

};

/**
  * Hasher sans allocation de mémoire inutile et surtout au runtime.
  * Chaque bucket pointe sur le premier objet qu'il contient, et ensuite chaque objet dans le bucket pointe vers son suivant.
  * Les buckets sont cubiques et l'espace de hashage aussi
  */
class Hasher3D
{
	public:
		Bucketable ** _Buckets; ///< Tous les buckets. Chacun pointe sur le premier objet d'une liste d'objets
		float _BucketSize; ///< Largeur d'un bucket en unité monde (bucket sont cubiques)
		int _NbBucketsInRow; ///< Nombre de buckets sur la largeur de l'espace de hashage (espace cubique)
		int _NbBucketsTotal; ///< Nombre total de buckets dans l'espace de hashage (_NbBucketsInRow ^3)
		glm::vec3 _Origin; ///< Point d'ou démarre l'espace de hashage

		Hasher3D()
		{
			_Buckets = NULL;
		}

		void createBuckets(glm::vec3  origin, float totalSize, float bucketSize)
		{
			_Origin = origin;
			_BucketSize = bucketSize;
			_NbBucketsInRow = (int)(totalSize / _BucketSize) + 1;
			int nbBucketsTotal = _NbBucketsInRow * _NbBucketsInRow * _NbBucketsInRow;

			if(nbBucketsTotal < 0){
				// Log::log(Log::ENGINE_ERROR,"Error on number of _Buckets for Hasher3D");
				fprintf(stderr, "Error on number of _Buckets for Hasher3D\n");
				return ;
			}

			//Si on a un nouveau nombre de bickets
			if(_NbBucketsTotal != nbBucketsTotal)
			{
				_NbBucketsTotal = nbBucketsTotal;
				SAFEDELETE(_Buckets);	
				_Buckets = new Bucketable*[_NbBucketsTotal];
				if(_Buckets == NULL){
					// Log::log(Log::ENGINE_ERROR,"Memory allocation failed for _Buckets of Hasher3D");
					fprintf(stderr, "Memory allocation failed for _Buckets of Hasher3D\n");
					return;
				}
			}
			resetBucketContent();
		}

		void resetBucketContent(void)
		{
			memset(_Buckets,0x00,_NbBucketsTotal*sizeof(Bucketable*));
		}

		void putInBucket(glm::vec3 position, Bucketable * objectToStore)
		{
			position -= _Origin;

			int x = (int)(position.x / _BucketSize);
			int y = (int)(position.y / _BucketSize);
			int z = (int)(position.z / _BucketSize);

			if(x < 0 || x >= _NbBucketsInRow ||
			   y < 0 || y >= _NbBucketsInRow ||
			   z < 0 || z >= _NbBucketsInRow)
			{
				fprintf(stderr,"putInBucket() : You try to hash a position that is NOT in the hash space cake\n");
			    return;
			}

			Bucketable * object = _Buckets[(x * _NbBucketsInRow * _NbBucketsInRow) + (y * _NbBucketsInRow) + z];
			if(object == NULL)
				_Buckets[(x * _NbBucketsInRow * _NbBucketsInRow) + (y * _NbBucketsInRow) + z] = objectToStore;
			else
			{
				while (object->_NextInBucket != NULL)
					object = object->_NextInBucket;
				object->_NextInBucket = objectToStore;
			}

			objectToStore->_NextInBucket = NULL;
		}

		int calcBucketSize(Bucketable * objects)
		{
			int nbObjects = 0;
			while(objects != NULL)
			{
				nbObjects++;
				objects = objects->_NextInBucket;
			}
			return nbObjects;
		}

		int calcBiggestBucketSize(void)
		{
			int nbObjectsMax = 0;
			for(int i=0;i<_NbBucketsTotal;i++)
			{
				int size = calcBucketSize(_Buckets[i]);
				if(size > nbObjectsMax)
					nbObjectsMax = size;
			}
			return nbObjectsMax;
		}

		int calcNbObjInBuckets(void)
		{
			int nbObjects = 0;
			for(int i=0;i<_NbBucketsTotal;i++)
			{
				nbObjects += calcBucketSize(_Buckets[i]);
			}
			return nbObjects;
		}

		Bucketable * getBucketContent(glm::vec3 position)
		{
			position -= _Origin;

			int x = (int)(position.x / _BucketSize);
			int y = (int)(position.y / _BucketSize);
			int z = (int)(position.z / _BucketSize);

			if(x < 0 || x >= _NbBucketsInRow ||
			   y < 0 || y >= _NbBucketsInRow ||
			   z < 0 || z >= _NbBucketsInRow)
			{
				fprintf(stderr,"putInBucket() : You try to hash a position that is NOT in the hash space cake\n");
			    return NULL;
			}

			return _Buckets[(x * _NbBucketsInRow * _NbBucketsInRow) + (y * _NbBucketsInRow) + z];
		}

		/**
		  * Voir dans _BucketRequest pour le résultat
		  */
		void getBucketContentWithNeighbours(glm::vec3 position,BucketRequest * request)
		{
			position -= _Origin;

			int x = (int)(position.x / _BucketSize);
			int y = (int)(position.y / _BucketSize);
			int z = (int)(position.z / _BucketSize);

			x--;
			y--;
			z--;

			int num = 0;
			int tmpX,tmpY,tmpZ;
			int offset[3] = {1,0,2};//En utilisant ces offset, on place en premier le bucket du milieu, qui contient les elements les plus proches (permet de speeder les algos suivants)
			for(int i=0;i<3;i++)
			{
				for(int j=0;j<3;j++)
				{
					for(int k=0;k<3;k++)
					{
						tmpX = x+offset[i];
						tmpY = y+offset[j];
						tmpZ = z+offset[k];

						if(tmpX < 0 || tmpX >= _NbBucketsInRow ||
						   tmpY < 0 || tmpY >= _NbBucketsInRow ||
						   tmpZ < 0 || tmpZ >= _NbBucketsInRow)
						{
							request->_Buckets[num] = NULL;
						}
						else
						{
							request->_Buckets[num] = _Buckets[(tmpX * _NbBucketsInRow * _NbBucketsInRow) + (tmpY * _NbBucketsInRow) + tmpZ];
						}
						num++;
					}
				}
			}
		}

		Bucketable * getNearest(glm::vec3 point)
		{
			BucketRequest request;
			getBucketContentWithNeighbours(point,&request);
			float minDist = -1;
			Bucketable * nearest = NULL;
			for(int i=0;i<27;i++)
			{
				Bucketable * bucki = request._Buckets[i];
				while (bucki != NULL)
				{
					//On vérifie
					glm::vec3 diff = bucki->_Pos - point;
					float dist = glm::length2 	( diff) ;// beware of length
					if(minDist < 0 || dist < minDist)
					{
						minDist = dist;
						nearest = bucki;
					}

					//On passe au suivant du bucket
					bucki =  bucki->_NextInBucket;
				}
			}

			return nearest;
		}
};

#endif
