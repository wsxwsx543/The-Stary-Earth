#include "triangle_area_normal.h"
#include <Eigen/Geometry>

Eigen::RowVector3d triangle_area_normal(
  const Eigen::RowVector3d & a, 
  const Eigen::RowVector3d & b, 
  const Eigen::RowVector3d & c)
{
  // Since the norm of vector which is produced by the cross product is the 
  // area of the parallelogram. Therefore we can simply by making it divided by 2.0
  return ((a - c).cross(b - c) / 2.0);
}
