#include "write_obj.h"
#include <fstream>
#include <cassert>
#include <iostream>

bool write_obj(
  const std::string & filename,
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  const Eigen::MatrixXd & UV,
  const Eigen::MatrixXi & UF,
  const Eigen::MatrixXd & NV,
  const Eigen::MatrixXi & NF)
{
  assert((F.size() == 0 || F.cols() == 3 || F.cols() == 4) && "F must have 3 or 4 columns");

  // Reference: https://en.wikipedia.org/wiki/Wavefront_.obj_file
  std::ofstream(file);
  if (!file) {
    std::cout << "Cannot open the file: " << filename << "." << std::endl;
    return false;
  }
  
  // the order based on wikipedia.
  // wirte V with identifier v
  for (int i = 0; i < V.rows(); i++) {
    file << "v";
    for (int j  = 0; j < V.cols(); j++) {
      file << " " << V(i, j);
    }
    file << std::endl;
  }

  // write texture coordinates (UV with identifier vt)
  for (int i = 0; i < UV.rows(); i++) {
    file << "vt";
    for (int j = 0; j < UV.rows(); j++) {
      file << " " << UV(i, j);
    }
    file << std::endl;
  }

  // write vertex normals (NV with identifier vn)
  // Note: vn should not have to be normal.
  for (int i = 0; i < NV.rows(); i++) {
    file << "vn";
    for (int j = 0; j < NV.cols(); j++) {
      file << " " << NV(i, j);
    }
    file << std::endl;
  }

  assert(F.size() == UF.size() && F.size() == NF.size() && "size of F, UF and NF should be same.");
  // write faces with identifier f
  // f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 ...
  for (int i = 0; i < F.rows(); i++) {
    file << "f";
    for (int j = 0; j < F.cols(); j++) {
      file << " " << F(i, j) << "/" << UF(i, j) << "/" << NF(i, j);
    }
    file << std::endl;
  }
  return true;
}
