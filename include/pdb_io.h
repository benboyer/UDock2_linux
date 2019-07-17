///il faudra utiliser boost system pour l'interoperabilite
#ifndef PDB_IO_H
#define PDB_IO_H

#include <iostream>
#include <fstream>
#include <cctype>
#include <algorithm>

#include "Atom.h"
#include "Molecule.h"

bool isAtom(const std::string & line)
{
    if (line.size() < 10) return false;
    if (line.substr(0, 6) == (std::string) "ATOM  ") return true;
    return false;
}

std::string readatomtype(const std::string &ligne)
{
    std::string type = "";
    int i = 12;
    while (ligne[i] == ' ')
    {
        i++;
        if (i > 15) return type;
    }

    int j = i;
    while (ligne[j] != ' ') j++;

    type = ligne.substr(i, j - i);
    std::transform(type.begin(), type.end(),
                   type.begin(), //destination
                   (int(*)(int)) toupper //to upper: convert to upper case
                   );


    return type;
}

std::string readresidtype(const std::string &line)
{
    std::string type = "";
    int i = 17;
    while (line[i] == ' ')
    {
        i++;
        if (i > 19) return type;
    }

    int j = i;
    while (line[j] != ' ') j++;

    type = line.substr(i, j - i);
    std::transform(type.begin(), type.end(),
                   type.begin(), //destination
                   (int(*)(int)) toupper //to upper: convert to upper case
                   );

    return type;
}

Molecule readPDB(std::ifstream& file)
{
    Molecule mol;

    std::string line;
    while (std::getline(file, line))
    {
        if (isAtom(line))
        {

            Atom at;

            at.pos.x = atof(line.substr(30, 8).c_str());
            at.pos.y = atof(line.substr(38, 8).c_str());
            at.pos.z = atof(line.substr(46, 8).c_str());

            at.atomType = readatomtype(line);
            at.residueType = readresidtype(line);
            at.chain = line.substr(21, 1);
            at.residueId = atoi(line.substr(22, 4).c_str());
            at.atomId = atoi(line.substr(6, 5).c_str());


            mol.atoms.push_back(at);
        }


    }
    mol.calcBarycenter();
    return mol;
}

Molecule readPDB(const std::string fileName)
{
    std::ifstream file(fileName.c_str());
    if (!file)
    {
        throw std::invalid_argument("readPDB:Could not open file \"" + fileName + "\" #####");
    }
    Molecule mol = readPDB(file);
    file.close();
    return mol;
}

void writePDB(Molecule mol, const std::string filename, char mode)
{

    FILE* file = fopen(filename.c_str(), "w");

    for (glm::uint i = 0; i < mol.size(); i++)
    {

        const char * chainID = "A";

        Atom at = mol.atoms[i];
        const char* atomname = at.atomType.c_str();
        const char* residName = at.residueType.c_str();
        int residnumber = at.residueId;
        chainID = at.chain.c_str();

        int atomnumber = at.atomId;

        glm::vec3 coord = at.pos;
        double x = coord.x;
        double y = coord.y;
        double z = coord.z;



        fprintf(file, "ATOM  %5d %-4s %3s %1s%4d    %8.3f%8.3f%8.3f", atomnumber, atomname, residName, chainID, residnumber, x, y, z);
        fprintf(file, "\n");
    }

    fclose(file);
}


#endif

