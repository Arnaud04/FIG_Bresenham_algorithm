#ifndef QUATERNION_H
#define QUATERNION_H

#include <QVector3D>
class Quaternion
{
public:
    Quaternion();
    enum Quat{i,moins_i,j,moins_j,k,moins_k,un,moins_un};
    Quat modelQuat_matrix[16];
    float mult(Quat elem1,Quat elem2);
    float mult3(Quaternion::Quat elem1, Quaternion::Quat elem2, Quaternion::Quat elem3);
    float scalaire(QVector3D v1, QVector3D v2);
    //pow(sin(theta),2)*(vectS*vectV)*vectS + pow(cos(theta),2)*vectV + (sin(theta)*cos(theta)*scalaire(vectS,vectV) - pow(sin(theta),2)*scalaire(scalaire(vectS,vectV),VectS)
};

#endif // QUATERNION_H
