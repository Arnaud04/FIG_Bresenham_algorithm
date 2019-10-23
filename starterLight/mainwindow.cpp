#include "mainwindow.h"
#include "ui_mainwindow.h"

/* **** début de la partie boutons et IHM **** */

/* **** autres fonction **** */

// exemple pour charger un fichier .obj
void MainWindow::on_pushButton_chargement_clicked()
{
    // fenêtre de sélection des fichiers
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Mesh"), "", tr("Mesh Files (*.obj)"));

    // chargement du fichier .obj dans la variable globale "mesh"
    OpenMesh::IO::read_mesh(mesh, fileName.toUtf8().constData());

    mesh.update_normals();

    // initialisation des couleurs et épaisseurs (sommets et arêtes) du mesh
    resetAllColorsAndThickness(&mesh);

    // on affiche le maillage
    displayMesh(&mesh);
}

void MainWindow::draw_first_vertex(MyMesh *_mesh)
{
    int indiceP1 = vertex2_y * nbGridPoints + vertex2_x;
    //int indiceP2 = point_x2 * vertexNumber + point_y2 -1;

    //resetAllColorsAndThickness(_mesh);
    if(indiceP1 >= 0)
    {
        VertexHandle v = _mesh->vertex_handle(indiceP1);

        _mesh->set_color(v, MyMesh::Color(120, 0, 0));
        _mesh->data(v).thickness = 10;

    }
    //displayMesh(_mesh);

}

void MainWindow::draw_last_vertex(MyMesh *_mesh)
{
    //resetAllColorsAndThickness(_mesh);
    int indiceP2 = vertex1_y * nbGridPoints + vertex1_x;

    if(indiceP2 >= 1)
    {
        VertexHandle v = _mesh->vertex_handle(indiceP2);

        _mesh->set_color(v, MyMesh::Color(0, 0, 120));
        _mesh->data(v).thickness = 10;

    }
    //displayMesh(_mesh);
}

void MainWindow::printListPoint(MyMesh *_mesh)
{

    for (MyMesh::VertexIter curVert = _mesh->vertices_begin(); curVert != _mesh->vertices_end(); curVert++)
    {
        _mesh->data(*curVert).thickness = 3;
        _mesh->set_color(*curVert, MyMesh::Color(0, 0, 0));
        qDebug()<<curVert->idx();
    }
}

void MainWindow::drawSegment(MyMesh *_mesh,int x1, int y1, int x2, int y2)
{
    int x,y,dx,dy;
    double e,e_1_0,e_0_1; //valeur d'erreur et increment

    dy = y2-y1;
    dx = x2-x1;
    y = y1;
    e = 0.0;
    e_1_0 = (double)dy/(double)dx;
    e_0_1 = -1.0;

    for(int x = x1; x <= x2; x++)
    {
        qDebug()<<"x :"<<x<<"y :"<<y;
        int indiceP = y * nbGridPoints + x;

        //if((x!=vertex1_x && y!=vertex1_y)||(x!=vertex2_x && y!=vertex2_y))
        //{
            VertexHandle v = _mesh->vertex_handle(indiceP);
            _mesh->set_color(v, MyMesh::Color(0, 120, 0));
            _mesh->data(v).thickness = 10;
        //}
        if(demos)
        {
            sleep(1);
            displayMesh(_mesh);
        }

        if((e=e+e_1_0)> 0.5)
        {
            y = y+1;
            e = e + e_0_1;
        }

    }

}

/* **** fin de la partie boutons et IHM **** */


/* **** fonctions supplémentaires **** */
// permet d'initialiser les couleurs et les épaisseurs des élements du maillage
void MainWindow::resetAllColorsAndThickness(MyMesh* _mesh)
{
    for (MyMesh::VertexIter curVert = _mesh->vertices_begin(); curVert != _mesh->vertices_end(); curVert++)
    {
        _mesh->data(*curVert).thickness = 1;
        _mesh->set_color(*curVert, MyMesh::Color(0, 0, 0));
    }

    for (MyMesh::FaceIter curFace = _mesh->faces_begin(); curFace != _mesh->faces_end(); curFace++)
    {
        _mesh->set_color(*curFace, MyMesh::Color(150, 150, 150));
    }

    for (MyMesh::EdgeIter curEdge = _mesh->edges_begin(); curEdge != _mesh->edges_end(); curEdge++)
    {
        _mesh->data(*curEdge).thickness = 1;
        _mesh->set_color(*curEdge, MyMesh::Color(0, 0, 0));
    }
}

// charge un objet MyMesh dans l'environnement OpenGL
void MainWindow::displayMesh(MyMesh* _mesh, bool isTemperatureMap, float mapRange)
{
    GLuint* triIndiceArray = new GLuint[_mesh->n_faces() * 3];
    GLfloat* triCols = new GLfloat[_mesh->n_faces() * 3 * 3];
    GLfloat* triVerts = new GLfloat[_mesh->n_faces() * 3 * 3];

    int i = 0;

    if(isTemperatureMap)
    {
        QVector<float> values;

        if(mapRange == -1)
        {
            for (MyMesh::VertexIter curVert = _mesh->vertices_begin(); curVert != _mesh->vertices_end(); curVert++)
                values.append(fabs(_mesh->data(*curVert).value));
            qSort(values);
            mapRange = values.at(values.size()*0.8);
            qDebug() << "mapRange" << mapRange;
        }

        float range = mapRange;
        MyMesh::ConstFaceIter fIt(_mesh->faces_begin()), fEnd(_mesh->faces_end());
        MyMesh::ConstFaceVertexIter fvIt;

        for (; fIt!=fEnd; ++fIt)
        {
            fvIt = _mesh->cfv_iter(*fIt);
            if(_mesh->data(*fvIt).value > 0){triCols[3*i+0] = 255; triCols[3*i+1] = 255 - std::min((_mesh->data(*fvIt).value/range) * 255.0, 255.0); triCols[3*i+2] = 255 - std::min((_mesh->data(*fvIt).value/range) * 255.0, 255.0);}
            else{triCols[3*i+2] = 255; triCols[3*i+1] = 255 - std::min((-_mesh->data(*fvIt).value/range) * 255.0, 255.0); triCols[3*i+0] = 255 - std::min((-_mesh->data(*fvIt).value/range) * 255.0, 255.0);}
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++; ++fvIt;
            if(_mesh->data(*fvIt).value > 0){triCols[3*i+0] = 255; triCols[3*i+1] = 255 - std::min((_mesh->data(*fvIt).value/range) * 255.0, 255.0); triCols[3*i+2] = 255 - std::min((_mesh->data(*fvIt).value/range) * 255.0, 255.0);}
            else{triCols[3*i+2] = 255; triCols[3*i+1] = 255 - std::min((-_mesh->data(*fvIt).value/range) * 255.0, 255.0); triCols[3*i+0] = 255 - std::min((-_mesh->data(*fvIt).value/range) * 255.0, 255.0);}
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++; ++fvIt;
            if(_mesh->data(*fvIt).value > 0){triCols[3*i+0] = 255; triCols[3*i+1] = 255 - std::min((_mesh->data(*fvIt).value/range) * 255.0, 255.0); triCols[3*i+2] = 255 - std::min((_mesh->data(*fvIt).value/range) * 255.0, 255.0);}
            else{triCols[3*i+2] = 255; triCols[3*i+1] = 255 - std::min((-_mesh->data(*fvIt).value/range) * 255.0, 255.0); triCols[3*i+0] = 255 - std::min((-_mesh->data(*fvIt).value/range) * 255.0, 255.0);}
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++;
        }
    }
    else
    {
        MyMesh::ConstFaceIter fIt(_mesh->faces_begin()), fEnd(_mesh->faces_end());
        MyMesh::ConstFaceVertexIter fvIt;
        for (; fIt!=fEnd; ++fIt)
        {
            fvIt = _mesh->cfv_iter(*fIt);
            triCols[3*i+0] = _mesh->color(*fIt)[0]; triCols[3*i+1] = _mesh->color(*fIt)[1]; triCols[3*i+2] = _mesh->color(*fIt)[2];
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++; ++fvIt;
            triCols[3*i+0] = _mesh->color(*fIt)[0]; triCols[3*i+1] = _mesh->color(*fIt)[1]; triCols[3*i+2] = _mesh->color(*fIt)[2];
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++; ++fvIt;
            triCols[3*i+0] = _mesh->color(*fIt)[0]; triCols[3*i+1] = _mesh->color(*fIt)[1]; triCols[3*i+2] = _mesh->color(*fIt)[2];
            triVerts[3*i+0] = _mesh->point(*fvIt)[0]; triVerts[3*i+1] = _mesh->point(*fvIt)[1]; triVerts[3*i+2] = _mesh->point(*fvIt)[2];
            triIndiceArray[i] = i;

            i++;
        }
    }


    ui->displayWidget->loadMesh(triVerts, triCols, _mesh->n_faces() * 3 * 3, triIndiceArray, _mesh->n_faces() * 3);

    delete[] triIndiceArray;
    delete[] triCols;
    delete[] triVerts;

    GLuint* linesIndiceArray = new GLuint[_mesh->n_edges() * 2];
    GLfloat* linesCols = new GLfloat[_mesh->n_edges() * 2 * 3];
    GLfloat* linesVerts = new GLfloat[_mesh->n_edges() * 2 * 3];

    i = 0;
    QHash<float, QList<int> > edgesIDbyThickness;
    for (MyMesh::EdgeIter eit = _mesh->edges_begin(); eit != _mesh->edges_end(); ++eit)
    {
        float t = _mesh->data(*eit).thickness;
        if(t > 0)
        {
            if(!edgesIDbyThickness.contains(t))
                edgesIDbyThickness[t] = QList<int>();
            edgesIDbyThickness[t].append((*eit).idx());
        }
    }
    QHashIterator<float, QList<int> > it(edgesIDbyThickness);
    QList<QPair<float, int> > edgeSizes;
    while (it.hasNext())
    {
        it.next();

        for(int e = 0; e < it.value().size(); e++)
        {
            int eidx = it.value().at(e);

            MyMesh::VertexHandle vh1 = _mesh->to_vertex_handle(_mesh->halfedge_handle(_mesh->edge_handle(eidx), 0));
            linesVerts[3*i+0] = _mesh->point(vh1)[0];
            linesVerts[3*i+1] = _mesh->point(vh1)[1];
            linesVerts[3*i+2] = _mesh->point(vh1)[2];
            linesCols[3*i+0] = _mesh->color(_mesh->edge_handle(eidx))[0];
            linesCols[3*i+1] = _mesh->color(_mesh->edge_handle(eidx))[1];
            linesCols[3*i+2] = _mesh->color(_mesh->edge_handle(eidx))[2];
            linesIndiceArray[i] = i;
            i++;

            MyMesh::VertexHandle vh2 = _mesh->from_vertex_handle(_mesh->halfedge_handle(_mesh->edge_handle(eidx), 0));
            linesVerts[3*i+0] = _mesh->point(vh2)[0];
            linesVerts[3*i+1] = _mesh->point(vh2)[1];
            linesVerts[3*i+2] = _mesh->point(vh2)[2];
            linesCols[3*i+0] = _mesh->color(_mesh->edge_handle(eidx))[0];
            linesCols[3*i+1] = _mesh->color(_mesh->edge_handle(eidx))[1];
            linesCols[3*i+2] = _mesh->color(_mesh->edge_handle(eidx))[2];
            linesIndiceArray[i] = i;
            i++;
        }
        edgeSizes.append(qMakePair(it.key(), it.value().size()));
    }

    ui->displayWidget->loadLines(linesVerts, linesCols, i * 3, linesIndiceArray, i, edgeSizes);

    delete[] linesIndiceArray;
    delete[] linesCols;
    delete[] linesVerts;

    GLuint* pointsIndiceArray = new GLuint[_mesh->n_vertices()];
    GLfloat* pointsCols = new GLfloat[_mesh->n_vertices() * 3];
    GLfloat* pointsVerts = new GLfloat[_mesh->n_vertices() * 3];

    i = 0;
    QHash<float, QList<int> > vertsIDbyThickness;
    for (MyMesh::VertexIter vit = _mesh->vertices_begin(); vit != _mesh->vertices_end(); ++vit)
    {
        float t = _mesh->data(*vit).thickness;
        if(t > 0)
        {
            if(!vertsIDbyThickness.contains(t))
                vertsIDbyThickness[t] = QList<int>();
            vertsIDbyThickness[t].append((*vit).idx());
        }
    }
    QHashIterator<float, QList<int> > vitt(vertsIDbyThickness);
    QList<QPair<float, int> > vertsSizes;

    while (vitt.hasNext())
    {
        vitt.next();

        for(int v = 0; v < vitt.value().size(); v++)
        {
            int vidx = vitt.value().at(v);

            pointsVerts[3*i+0] = _mesh->point(_mesh->vertex_handle(vidx))[0];
            pointsVerts[3*i+1] = _mesh->point(_mesh->vertex_handle(vidx))[1];
            pointsVerts[3*i+2] = _mesh->point(_mesh->vertex_handle(vidx))[2];
            pointsCols[3*i+0] = _mesh->color(_mesh->vertex_handle(vidx))[0];
            pointsCols[3*i+1] = _mesh->color(_mesh->vertex_handle(vidx))[1];
            pointsCols[3*i+2] = _mesh->color(_mesh->vertex_handle(vidx))[2];
            pointsIndiceArray[i] = i;
            i++;
        }
        vertsSizes.append(qMakePair(vitt.key(), vitt.value().size()));
    }

    ui->displayWidget->loadPoints(pointsVerts, pointsCols, i * 3, pointsIndiceArray, i, vertsSizes);

    delete[] pointsIndiceArray;
    delete[] pointsCols;
    delete[] pointsVerts;
}


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->demo_start_stop->setStyleSheet("background-color: red;");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_generer_clicked()
{
    MyMesh mesh;
    int maxSizeX = nbGridPoints; //2n+1
    int maxSizeY = nbGridPoints; //

    // on construit une liste de sommets
    MyMesh::VertexHandle sommets[maxSizeX*maxSizeY];

    int precision = 1;
    int numberPoint =0;

    for(int x=0; x<maxSizeX; x++)
    {
        for(int y=0; y<maxSizeY;y++)
        {
            numberPoint++;
            sommets[numberPoint] = mesh.add_vertex(MyMesh::Point(x, y, 0));
        }

    }

    // initialisation des couleurs et épaisseurs (sommets et arêtes) du mesh
    resetAllColorsAndThickness(&mesh);
    printListPoint(&mesh);


    draw_first_vertex(&mesh);
    //displayMesh(&mesh);

    draw_last_vertex(&mesh);

    //faire boucle

    drawSegment(&mesh,vertex1_x, vertex1_y, vertex2_x, vertex2_y);

    displayMesh(&mesh);

}

void MainWindow::on_addGridVertex_clicked()
{
    nbGridPoints  ++;
    on_pushButton_generer_clicked();
}

void MainWindow::on_delateGridVertex_clicked()
{
    if(vertex2_x < (nbGridPoints-1) && vertex2_y < (nbGridPoints-1))
        nbGridPoints  --;
    on_pushButton_generer_clicked();
}

//==============Placement du premier point ==================

//======= x ========
void MainWindow::on_vertex1_x_plus_clicked()
{
    if(vertex1_x < nbGridPoints-1)
    {
        if(!blocked &&(vertex2_x - vertex1_x) > (vertex2_y -vertex1_y))
        {
            vertex1_x ++;
        }
        /*if((vertex2_x - vertex1_x) <= (vertex2_y - vertex1_y))
        {
            blocked = true;
        }

        if(blocked && ((vertex2_x - vertex1_x+1) > (vertex2_y -vertex1_y)))
        {
            blocked = false;
            vertex1_x ++;
        }*/
    }
    on_pushButton_generer_clicked();
}

void MainWindow::on_vertex1_x_moins_clicked()
{
    if(vertex1_x > 0 )
    {
        if(!blocked &&(vertex2_x - vertex1_x) > (vertex2_y -vertex1_y))
        {
            vertex1_x --;
        }
        /*if((vertex2_x - vertex1_x) <= (vertex2_y -vertex1_y))
        {
            blocked = true;
        }

        if(blocked && ((vertex2_x - vertex1_x-1) > (vertex2_y -vertex1_y)))
        {
            blocked = false;
            vertex1_x --;
        }*/
    }
    on_pushButton_generer_clicked();
}
//======= y ========

void MainWindow::on_vertex1_y_plus_clicked()
{
    if(vertex1_y < vertex2_y-1)
    {
        if(!blocked &&(vertex2_x - vertex1_x) > (vertex2_y -vertex1_y))
        {
            vertex1_y ++;
        }
        /*if((vertex2_x - vertex1_x) <= (vertex2_y -vertex1_y))
        {
            blocked = true;
        }

        if(blocked && ((vertex2_x - vertex1_x) > (vertex2_y -vertex1_y+1)))
        {
            blocked = false;
            vertex1_y ++;
        }*/
    }
    on_pushButton_generer_clicked();
}

void MainWindow::on_vertex1_y_moins_clicked()
{
    if(vertex1_y > 0 )
    {
        if(!blocked &&(vertex2_x - vertex1_x) > (vertex2_y -vertex1_y))
        {
            vertex1_y --;
        }
        /*if((vertex2_x - vertex1_x) <= (vertex2_y -vertex1_y))
        {
            blocked = true;
        }

        if(blocked && ((vertex2_x - vertex1_x) > (vertex2_y -vertex1_y-1)))
        {
            blocked = false;
            vertex1_y --;
        }*/
    }

    on_pushButton_generer_clicked();
}

//==============Placement du deuxieme point ==================

//======= x ========
void MainWindow::on_vertex2_x_plus_clicked()
{
    if(vertex2_x < nbGridPoints-1)
    {
        if(!blocked &&(vertex2_x - vertex1_x) > (vertex2_y -vertex1_y))
        {
            vertex2_x ++;
        }
        if((vertex2_x - vertex1_x) <= (vertex2_y -vertex1_y))
        {
            blocked = true;
        }

        if(blocked && ((vertex2_x+1 - vertex1_x) > (vertex2_y -vertex1_y)))
        {
            blocked = false;
            vertex2_x ++;
        }
    }
    on_pushButton_generer_clicked();
}

void MainWindow::on_vertex2_x_moins_clicked()
{
    if(vertex2_x > (vertex1_x+1))
    {
        if(!blocked &&(vertex2_x - vertex1_x) > (vertex2_y -vertex1_y))
        {
            vertex2_x --;
        }
        if((vertex2_x - vertex1_x) <= (vertex2_y -vertex1_y))
        {
            blocked = true;
        }

        if(blocked && ((vertex2_x-1 - vertex1_x) > (vertex2_y -vertex1_y)))
        {
            blocked = false;
            vertex2_x --;
        }
    }
    on_pushButton_generer_clicked();
}

//======= y ========
void MainWindow::on_vertex2_y_plus_clicked()
{
    if(vertex2_y < nbGridPoints-1)
    {
        if(!blocked &&(vertex2_x - vertex1_x) > (vertex2_y -vertex1_y))
        {
            vertex2_y ++;
        }

        if((vertex2_x - vertex1_x) <= (vertex2_y -vertex1_y))
        {
            blocked = true;
        }

        if(blocked && ((vertex2_x - vertex1_x) > (vertex2_y+1 -vertex1_y)))
        {
            blocked = false;
            vertex2_y ++;
        }
    }
    on_pushButton_generer_clicked();
}

void MainWindow::on_vertex2_y_moins_clicked()
{
    if(vertex2_y > (vertex1_y+1))
    {
        if(!blocked &&(vertex2_x - vertex1_x) > (vertex2_y -vertex1_y))
        {
            vertex2_y --;
        }

        if((vertex2_x - vertex1_x) <= (vertex2_y -vertex1_y))
        {
            blocked = true;
        }

        if(blocked && ((vertex2_x - vertex1_x) > (vertex2_y-1 -vertex1_y)))
        {
            blocked = false;
            vertex2_y--;
        }
    }
    on_pushButton_generer_clicked();
}

void MainWindow::on_clearGrid_clicked()
{
    //to do
}

void MainWindow::on_demo_start_stop_clicked()
{
    if(demos == false)
    {
        demos = true;
        //ui->demo_start_stop->setStyleSheet("background-color: blue;");
        ui->demo_start_stop->setStyleSheet("background-color: green;");

    }
    else
    {
        demos = false;
        ui->demo_start_stop->setStyleSheet("background-color: red;");
    }
    on_pushButton_generer_clicked();
}
