#include "cube.h"
#include "sphere.h"
#include "write_obj.h"
#include "QuadViewer.h"
#include "set_texture_from_png.h"
#include <igl/readOBJ.h>
#include <Eigen/Core>
#include <vector>
#include <iostream>
#include <memory>
#include <limits>
#include <functional>


int main(int argc, char * argv[])
{
  {
    // Create mesh of a sphere
    Eigen::MatrixXd V,UV,NV;
    Eigen::MatrixXi F,UF,NF;
    sphere(60,40,V,F,UV,UF,NV,NF);
    write_obj("sphere.obj",V,F,UV,UF,NV,NF);
    igl::readOBJ("sphere.obj",V,UV,NV,F,UF,NF);
    {
      QuadViewer v;
      v.set_mesh(V,F,UV,UF,NV,NF);
      set_texture_from_png("../../style-transfer/output.png", v.viewer.data());
      v.launch();
    }
  }
}
