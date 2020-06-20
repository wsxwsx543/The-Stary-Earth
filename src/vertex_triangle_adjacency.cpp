#include "vertex_triangle_adjacency.h"

void vertex_triangle_adjacency(
  const Eigen::MatrixXi & F,
  const int num_vertices,
  std::vector<std::vector<int> > & VF)
{
  VF.resize(num_vertices);

  const int numf = F.rows();
  for (int i = 0; i < num_vertices; i++) {
    // the ith pt
    for (int j = 0; j < numf; j++) {
      if (F.row(j).x() == i || F.row(j).y() == i || F.row(j).z() == i) { // adjacent to the ith vertex
        VF[i].push_back(j);
      }
    }
  }
}

