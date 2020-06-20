#include "per_corner_normals.h"
#include "triangle_area_normal.h"
// Hint:
#include "vertex_triangle_adjacency.h"
#include <iostream>

void per_corner_normals(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  const double corner_threshold,
  Eigen::MatrixXd & N)
{
  N = Eigen::MatrixXd::Zero(F.rows()*3,3);

  std::vector<std::vector<int> > VF;
  const int num_vertices = V.rows();
  vertex_triangle_adjacency(F, num_vertices, VF);

  const int numf = F.rows();
  const int num_vertices_per_triangle = 3;
  const double pi = 2*acos(0.0);
  const double epsilon = cos(2 * pi * corner_threshold / 360);
  for (int i = 0; i < numf; i++) {
    Eigen::RowVector3i curr_f = F.row(i);
    // curr_face_normal is area normal
    Eigen::RowVector3d curr_face_normal = triangle_area_normal(V.row(curr_f.x()), V.row(curr_f.y()), V.row(curr_f.z()));
    for (int j = 0; j < num_vertices_per_triangle; j++) {
      // normal is the final normal based on the curr_f and current vertex
      Eigen::RowVector3d normal(0.0, 0.0, 0.0);
      // adjacent_faces contains the faces adjacent to the vertex
      std::vector<int> adjacent_faces = VF[curr_f(j)];
      for (int k = 0; k < adjacent_faces.size(); k++) {
        Eigen::RowVector3i adjacent_face = F.row(adjacent_faces[k]);
        Eigen::RowVector3d adjacent_face_normal = triangle_area_normal(V.row(adjacent_face.x()), V.row(adjacent_face.y()), V.row(adjacent_face.z()));
        if (abs(adjacent_face_normal.normalized().dot(curr_face_normal.normalized())) > epsilon)
          normal += adjacent_face_normal;
      }
      N.row(i * num_vertices_per_triangle + j) = normal.normalized();
    }    
  }
}
