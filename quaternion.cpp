#include "quaternion.h"
#include <QDebug>

Quaternion::Quaternion()
{
    //on défini la matrice des quaternions
    modelQuat_matrix[0]=un;modelQuat_matrix[1]=i;modelQuat_matrix[2]=j;modelQuat_matrix[3] = k;
    modelQuat_matrix[4]=i;modelQuat_matrix[5]=moins_un;modelQuat_matrix[6]=k;modelQuat_matrix[7] = moins_j;
    modelQuat_matrix[8]=j;modelQuat_matrix[9]=moins_k;modelQuat_matrix[10]=moins_un;modelQuat_matrix[11] = i;
    modelQuat_matrix[12]=k;modelQuat_matrix[13]=j;modelQuat_matrix[14]=moins_i;modelQuat_matrix[15] = moins_un;
}

float Quaternion::mult(Quaternion::Quat elem1, Quaternion::Quat elem2)
{
    if(elem1 == Quaternion::Quat::j && elem1 == Quaternion::Quat::j)
    {
        return -1;
    }
    else if(elem1 == Quaternion::Quat::i && elem1 == Quaternion::Quat::i)
    {
        return -1;
    }
    else if(elem1 == Quaternion::Quat::k && elem1 == Quaternion::Quat::k)
    {
        return -1;
    }
    else
    {
        qDebug()<<"MAUVAISE MULTIPLICATION";
        return 0;
    }
}

float Quaternion::mult3(Quaternion::Quat elem1, Quaternion::Quat elem2, Quaternion::Quat elem3)
{

    if(elem1 == Quaternion::Quat::i && elem2 == Quaternion::Quat::j && elem3 == Quaternion::Quat::k)
    {
        return -1;
    }
    else
    {
        qDebug()<<"MAUVAISE MULTIPLICATION";
        return 0;
    }
}

QVector3D Quaternion::scalaire(QVector3D u, QVector3D v)
{
    return QVector3D(u.y()*v.z() - u.z()*v.y(),u.z()*v.x() - u.x()*v.z(),u.x()*v.y() - u.y()*v.x());
}

QVector3D Quaternion::rotate(float theta, QVector3D vectS, QVector3D vectV)
{
    return pow(sin(theta),2)*(vectS*vectV)*vectS + pow(cos(theta),2)*vectV + sin(theta)*cos(theta)*scalaire(vectS,vectV) - pow(sin(theta),2)*scalaire(scalaire(vectS,vectV),vectS);
}

float Quaternion::getNorme(QVector3D vect)
{
    return sqrt(pow(vect.x(),2)+pow(vect.y(),2)+pow(vect.z(),2));
}

QVector3D Quaternion::vector_unitaire(QVector3D vect)
{
    float norme = getNorme(vect);
    if(norme == 1)
    {
        qDebug()<<"etait deja unitaire";
        return vect;
    }
    else
    {
        return QVector3D(vect.x()/norme,vect.y()/norme,vect.z()/norme);
    }

}
