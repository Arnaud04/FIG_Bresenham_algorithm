#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QMainWindow>
#include <math.h>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <unistd.h>
#include "matrix.h"

namespace Ui {
class MainWindow;
}

using namespace OpenMesh;
using namespace OpenMesh::Attributes;

struct MyTraits : public OpenMesh::DefaultTraits
{
    // use vertex normals and vertex colors
    VertexAttributes( OpenMesh::Attributes::Normal | OpenMesh::Attributes::Color | OpenMesh::Attributes::Status);
    // store the previous halfedge
    HalfedgeAttributes( OpenMesh::Attributes::PrevHalfedge );
    // use face normals face colors
    FaceAttributes( OpenMesh::Attributes::Normal | OpenMesh::Attributes::Color | OpenMesh::Attributes::Status);
    EdgeAttributes( OpenMesh::Attributes::Color | OpenMesh::Attributes::Status );
    // vertex thickness
    VertexTraits{float thickness; float value;};
    // edge thickness
    EdgeTraits{float thickness;};
};
typedef OpenMesh::TriMesh_ArrayKernelT<MyTraits> MyMesh;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void displayMesh(MyMesh *_mesh, bool isTemperatureMap = false, float mapRange = -1);
    void resetAllColorsAndThickness(MyMesh* _mesh);
    void printListPoint(MyMesh *_mesh);
    void draw_last_vertex(MyMesh* _mesh);
    void draw_first_vertex(MyMesh *_mesh);
    void drawSegment(MyMesh *_mesh, int x1, int y1, int x2, int y2, int reverseX, int reverseY);

private slots:

    void on_pushButton_generer_clicked();

    void on_addGridVertex_clicked();

    void on_delateGridVertex_clicked();

    void on_vertex1_x_plus_clicked();

    void on_vertex1_x_moins_clicked();

    void on_vertex1_y_plus_clicked();

    void on_vertex1_y_moins_clicked();

    void on_vertex2_x_plus_clicked();

    void on_vertex2_x_moins_clicked();

    void on_vertex2_y_plus_clicked();

    void on_vertex2_y_moins_clicked();

    void on_clearGrid_clicked();

    void on_demo_start_stop_clicked();

    void on_pushButton_clear_clicked();

private:

    MyMesh mesh;
    int nbGridPoints = 11;
    int vertex1_x=1;
    int vertex1_y=1;
    int vertex2_x=9;
    int vertex2_y=3;
    bool blocked = false;
    bool demos = false;

    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
