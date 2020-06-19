#include "per_vertex_normals.h"
#include "triangle_area_normal.h"

void per_vertex_normals(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  Eigen::MatrixXd & N)
{
  N = Eigen::MatrixXd::Zero(V.rows(),3);

  const int numv = V.rows();
  const int numf = F.rows();

  for (int i = 0; i < numv; i++) {
    Eigen::RowVector3d normal(0.0, 0.0, 0.0);
    for (int j = 0; j < numf; j++) {
      if (F.row(j).x() == i || F.row(j).y() == i || F.row(j).z() == i) { // adjacent to the vertex
        // the norm of triangle area normal is the area of the triangle.
        // then we will caculate the area-weighted per-vertex normals
        normal += triangle_area_normal(V.row(F.row(j).x()), V.row(F.row(j).y()), V.row(F.row(j).z()));
      }
    }
    N.row(i) = normal.normalized();
  }
}