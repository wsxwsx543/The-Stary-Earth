#include "per_face_normals.h"
#include "triangle_area_normal.h"

void per_face_normals(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  Eigen::MatrixXd & N)
{
  N = Eigen::MatrixXd::Zero(F.rows(),3);
  int rows = F.rows();

  for (int r = 0; r < rows; r++) {
    Eigen::RowVector3d a(V.row(F(r, 0)));
    Eigen::RowVector3d b(V.row(F(r, 1)));
    Eigen::RowVector3d c(V.row(F(r, 2)));
    
    N.row(r) = triangle_area_normal(a, b, c);
  }
}
