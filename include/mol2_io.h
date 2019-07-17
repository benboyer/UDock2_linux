#ifndef MOL2_IO_H
#define MOL2_IO_H

#include <iostream>
#include <fstream>
#include <cctype>
#include <algorithm>
#include <vector>
#include <string>
#include <iterator>
#include <sstream>






#include "Atom.h"
#include "Molecule.h"









// from epock code: https://bitbucket.org/epock/epock
// Description:
// Return a vector of the words in the string s using the 
// space as delimiter.
inline std::vector<std::string> split(const std::string &s)

{
    std::vector<std::string> tokens;
    std::istringstream iss(s);
    copy(std::istream_iterator<std::string>(iss),
         std::istream_iterator<std::string>(),
         std::back_inserter<std::vector<std::string>>(tokens));
    return tokens;
}



bool isStartOfAtom(const std::string & line)
{
    if (line.substr(0, 13) == (std::string) "@<TRIPOS>ATOM") return true;
    return false;
}


bool isEndOfAtom(const std::string & line)
{
    if (line.substr(0, 13) == (std::string) "@<TRIPOS>BOND") return true;
    return false;
}

Molecule readMOL2(std::ifstream& file)
{
    Molecule mol;
    std::string line;
    std::vector<std::string> splitedline;
    bool readingAtom = false;
    while (std::getline(file, line))
    {
    	// stop reading atom when it's the line @<TRIPOS>BOND
        if (isEndOfAtom(line))
        {
			readingAtom = false;
        }

        
        if (readingAtom){
        	splitedline = split(line);
            Atom at;

            at.pos.x = stof(splitedline[2]);
            at.pos.y = stof(splitedline[3]);
            at.pos.z = stof(splitedline[4]);

            at.atomType = splitedline[5];
            at.residueType = splitedline[7];
            at.charge = stof(splitedline[8]);
            at.residueId = stoi(splitedline[6]);
            at.atomId = stoi(splitedline[0]);
            at.UdockId = at.findAtomUdockId();


            if (at.atomType =="C_3"){
                   at.radius = 1.908f;
            }
            else if (at.atomType =="C_2"){
                   at.radius = 1.908f;
            }
            else if (at.atomType =="C_AR"){
                   at.radius = 1.908f;
            }
            else if (at.atomType =="C_CAT"){
                   at.radius = 1.908f;
            }
            else if (at.atomType =="N_2"){
                   at.radius = 1.824f;
            }
            else if (at.atomType =="N_3"){
                   at.radius = 1.875f;;
            }
            else if (at.atomType =="N_4"){
                   at.radius = 1.824f;
            }
            else if (at.atomType =="N_AR"){
                   at.radius = 1.824f;
            }
            else if (at.atomType =="N_AM"){
                   at.radius = 1.824f;
            }
            else if (at.atomType =="N_PL3"){
                   at.radius = 1.824f;
            }
            else if (at.atomType =="O_2"){
                   at.radius = 1.6612f;
            }
            else if (at.atomType =="O_3"){
                   at.radius = 1.721f;
            }
            else if (at.atomType =="O_CO2"){
                   at.radius = 1.6612f;
            }
            else if (at.atomType =="S_3"){
                   at.radius = 2.0f;
            }
            else if (at.atomType =="P_3"){
                   at.radius = 2.1f;
            }
            else if (at.atomType =="F"){
                   at.radius = 1.75f;
            }
            else if (at.atomType =="H"){
                   at.radius = 1.4870f;
            }
            else if (at.atomType =="LI"){
                   at.radius = 1.137f;
            }


            mol.atoms.push_back(at);
        }

        //when we encounter the line @<TRIPOS>ATOM start reading atom
        if (isStartOfAtom(line))
        {
			readingAtom = true;
        }
// 


    }
    mol.CenterToOrigin();
    mol.calcRadius();
    std::cout <<"radius "<< mol.radius<< std::endl;
    
    return mol;
}

Molecule readMOL2(const std::string fileName)
{
    std::ifstream file(fileName.c_str());
    if (!file)
    {
        throw std::invalid_argument("readPDB:Could not open file \"" + fileName + "\" #####");
    }
    Molecule mol = readMOL2(file);
    file.close();
    return mol;
}



#endif