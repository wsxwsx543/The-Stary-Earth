#include "cube.h"

void cube(
  Eigen::MatrixXd & V,
  Eigen::MatrixXi & F,
  Eigen::MatrixXd & UV,
  Eigen::MatrixXi & UF,
  Eigen::MatrixXd & NV,
  Eigen::MatrixXi & NF)
{
  ////Hint:
  V.resize(8,3);
  F.resize(6,4);
  UV.resize(14,2);
  UF.resize(6,4);
  NV.resize(6,3);
  NF.resize(6,4);

  V.row(0) = Eigen::Vector3d(0, 0, 0);
  V.row(1) = Eigen::Vector3d(0, 0, 1);
  V.row(2) = Eigen::Vector3d(0, 1, 0);
  V.row(3) = Eigen::Vector3d(0, 1, 1);
  V.row(4) = Eigen::Vector3d(1, 0, 0);
  V.row(5) = Eigen::Vector3d(1, 0, 1);
  V.row(6) = Eigen::Vector3d(1, 1, 0);
  V.row(7) = Eigen::Vector3d(1, 1, 1);

  F.row(0) = Eigen::Vector4i(0, 4, 5, 1); // bottom
  F.row(1) = Eigen::Vector4i(2, 3, 7, 6); // top
  F.row(2) = Eigen::Vector4i(7, 3, 1, 5); // front
  F.row(3) = Eigen::Vector4i(2, 6, 4, 0); // back
  F.row(4) = Eigen::Vector4i(2, 0, 1, 3); // left
  F.row(5) = Eigen::Vector4i(6 ,7, 5, 4); // right

  UV.row(0) = Eigen::Vector2d(1, 0);
  UV.row(1) = Eigen::Vector2d(2, 0);
  UV.row(2) = Eigen::Vector2d(0, 1);
  UV.row(3) = Eigen::Vector2d(1, 1);
  UV.row(4) = Eigen::Vector2d(2, 1);
  UV.row(5) = Eigen::Vector2d(3, 1);
  UV.row(6) = Eigen::Vector2d(4, 1);
  UV.row(7) = Eigen::Vector2d(0, 2);
  UV.row(8) = Eigen::Vector2d(1, 2);
  UV.row(9) = Eigen::Vector2d(2, 2);
  UV.row(10) = Eigen::Vector2d(3, 2);
  UV.row(11) = Eigen::Vector2d(4, 2);
  UV.row(12) = Eigen::Vector2d(1, 3);
  UV.row(13) = Eigen::Vector2d(2, 3);
  UV = UV * 0.25;

  UF.row(0) = Eigen::Vector4i(1, 4, 3, 0); // bottom white
  UF.row(1) = Eigen::Vector4i(9, 13, 12, 8); // top yellow
  UF.row(2) = Eigen::Vector4i(4, 9, 8, 3); // front green
  UF.row(3) = Eigen::Vector4i(6, 11, 10, 5); // back orange
  UF.row(4) = Eigen::Vector4i(3, 8, 7, 2); // left red
  UF.row(5) = Eigen::Vector4i(5, 10, 9, 4); // right blue

  NV.row(0) = Eigen::Vector3d(1, 0, 0);
  NV.row(1) = Eigen::Vector3d(-1, 0, 0);
  NV.row(2) = Eigen::Vector3d(0, 1, 0);
  NV.row(3) = Eigen::Vector3d(0, -1, 0);
  NV.row(4) = Eigen::Vector3d(0, 0, 1);
  NV.row(5) = Eigen::Vector3d(0, 0, -1);

  NF.row(0) = Eigen::Vector4i(3, 3, 3, 3); // bottom
  NF.row(1) = Eigen::Vector4i(2, 2, 2, 2); // top
  NF.row(2) = Eigen::Vector4i(4, 4, 4, 4); // front
  NF.row(3) = Eigen::Vector4i(5, 5, 5, 5); // back
  NF.row(4) = Eigen::Vector4i(1, 1, 1, 1); // top
  NF.row(5) = Eigen::Vector4i(0, 0, 0, 0); // front

}
