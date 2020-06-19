#include "sphere.h"
#include <iostream>

void sphere(
  const int num_faces_u,
  const int num_faces_v,
  Eigen::MatrixXd & V,
  Eigen::MatrixXi & F,
  Eigen::MatrixXd & UV,
  Eigen::MatrixXi & UF,
  Eigen::MatrixXd & NV,
  Eigen::MatrixXi & NF)
{
  // value of pi. Reference: https://www.geeksforgeeks.org/pi-in-c-with-examples/
  const double pi = 2*acos(0.0);
  const int total_f = num_faces_u * num_faces_v;
  const int total_v = (num_faces_u + 1) * (num_faces_v + 1);

  // init size of matrices.
  V.resize(total_v, 3);
  UV.resize(total_v, 2);
  NV.resize(total_v, 3);

  F.resize(total_f, 4);
  UF.resize(total_f, 4);
  NF.resize(total_f, 4);
  
  // I will use polar coordiantes
  const double dphi = pi / num_faces_v;
  const double dtheta = 2 * pi / num_faces_u;

  // init v matrices
  for (int i = 0; i < num_faces_u + 1; i++) {
    for (int j = 0; j < num_faces_v + 1; j++) {
      // since r = 1.0
      // we need to take negative since the origin of the texture is at the left bottom
      double z = -sin(j * dphi) * cos(i * dtheta);
      double x = -sin(j * dphi) * sin(i * dtheta);
      double y = -cos(j * dphi);

      int row_idx = i * (num_faces_v + 1) + j;
      V.row(row_idx) = Eigen::Vector3d(x, y, z);
      UV.row(row_idx) = Eigen::Vector2d((float)i / num_faces_u, (float)j / num_faces_v);
      NV.row(row_idx) = Eigen::Vector3d(x, y, z);
    }
  }

  for (int i = 0; i < num_faces_u; i++) {
    for (int j = 0; j < num_faces_v; j++) {
      int row_idx = i * num_faces_v + j;
      
      // counter-clock wise
      int first = i * (num_faces_v + 1) + j;
      int second = (i + 1) * (num_faces_v + 1) + j;
      int third = second + 1;
      int fourth = first + 1;
      F.row(row_idx) = Eigen::RowVector4i(first, second, third, fourth);
      UF.row(row_idx) = Eigen::RowVector4i(first, second, third, fourth);
      NF.row(row_idx) = Eigen::RowVector4i(first, second, third, fourth);
    }
  }
}
