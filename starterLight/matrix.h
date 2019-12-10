#ifndef MATRIX_H
#define MATRIX_H
#include <QVector>
#include <QDebug>

class matrix
{
public:
    matrix(unsigned int dim_x, unsigned int dim_y, QVector<float> arg);
    unsigned int getSize_x() const;
    void setSize_x(unsigned int value);

    inline void affiche_dim(){qDebug()<<"size is : "<<getSize_x()<<"X"<<getSize_y();};

    unsigned int getSize_y() const;
    void setSize_y(unsigned int value);
    matrix multMat(matrix m2);
    void afficher();
    float at(unsigned int i, unsigned int j);
private :
    unsigned int size_x = 0;
    unsigned int size_y = 0;
    float ** mat;

};

#endif // MATRIX_H
