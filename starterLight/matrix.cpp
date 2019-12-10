#include "matrix.h"
#include <iostream>


matrix::matrix(unsigned int dim_x, unsigned int dim_y, QVector<float> arg)
{
    if(dim_x*dim_y > arg.size() || dim_x*dim_y < arg.size())
    {
        qDebug()<<"size error";
    }
    if(dim_x*dim_y <= arg.size() )
    {
        this->size_x = dim_x;
        this->size_y = dim_y;
        this->mat = new float * [size_x];
        for(unsigned int k = 0; k<size_x;k++)
        {
            mat[k]=new float[size_y];
        }
        //float temp[size_x][size_y];
        int idx = 0;
        for(unsigned int i = 0;i<size_x;i++)
        {
            for(unsigned int j = 0; j<size_y;j++)
            {
                //qDebug()<<arg.at(idx);
                mat[i][j] = arg.at(idx);
                idx++;
            }
        }
        //this->mat = temp;
    }
    else if(dim_x*dim_y > arg.size())
    {
        qDebug()<<"0 for lost data";
        this->size_x = dim_x;
        this->size_y = dim_y;
        this->mat = new float * [size_x];
        for(unsigned int k = 0; k<size_x;k++)
        {
            mat[k]=new float[size_y];
        }
        int idx = 0;
        for(unsigned int i = 0;i<size_y;i++)
        {
            for(unsigned int j = 0; j<size_x;j++)
            {
                if(idx>arg.size()-1)
                {
                    mat[i][j] = 0;
                }
                else
                {
                    mat[i][j] = arg.at(idx);
                }
                idx++;
            }
        }
    }
}

unsigned int matrix::getSize_x() const
{
    return size_x;
}

void matrix::setSize_x(unsigned int value)
{
    size_x = value;
}

unsigned int matrix::getSize_y() const
{
    return size_y;
}

void matrix::setSize_y(unsigned int value)
{
    size_y = value;
}

matrix matrix::multMat(matrix m2)
{
    QVector<float> arg;
    if(m2.getSize_y() != this->getSize_x())
    {
        qDebug()<<"ERROR MULT DIM";
        return matrix(0,0,arg);
    }
    else
    {
        float C[this->size_x][m2.size_y];

        for (unsigned int i = 0; i < this->size_x; i++)
        {
            for (unsigned int j = 0; j < m2.size_y; j++)
            {
                C[i][j] = 0;

                for (unsigned int k = 0; k < m2.size_x; k++)
                {
                    C[i][j] += this->mat[i][k] * m2.mat[k][j];
                }
            }
        }

        for (unsigned int i = 0; i < this->size_x; i++)
        {
            for (unsigned int j = 0; j < m2.size_y; j++)
            {
                arg.push_back(C[i][j]);
            }
        }
        return matrix(this->size_x,m2.size_y,arg);

    }
}

void matrix::afficher()
{
    std::cout<<"[ ";
    for(unsigned int i = 0; i<this->size_x; i++)
    {
        std::cout<<"[ ";
        for(unsigned int j = 0; j<this->size_y; j++)
        {
            std::cout<<this->mat[i][j]<<" ";
        }
        std::cout<<"] "<<std::endl;
    }
    std::cout<<"] "<<std::endl;
}

float matrix::at(unsigned int i, unsigned int j)
{
    if(i<this->size_x && j<this->size_y)
    {
        return this->mat[i][j];
    }
    else
    {
        qDebug()<<"out of range";
        return 0;
    }
}
