#include "catmull_clark.h"
#include <vector>
#include <map>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <functional>

// find all adjacent faces for a vertex
void findVF(
  const Eigen::MatrixXd & V, 
  const Eigen::MatrixXi & F, 
  std::vector<std::vector<int> > & VF)
{
  int vrow = V.rows();
  int frow = F.rows();
  VF.resize(vrow);
  for (int i = 0; i < vrow; i++) {
    for (int j = 0; j < frow; j++) {
      if (F(j, 0) == i || F(j, 1) == i || F(j, 2) == i || F(j, 3) == i) 
        VF[i].push_back(j);
    }
  }
}

// calculate all face points for all faces
void calculate_face_points(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  Eigen::MatrixXd & face_points 
)
{
  int frow = F.rows();
  face_points.resize(frow, 3);
  for (int i = 0; i < frow; i++) {
    Eigen::RowVector3d face_point = (V.row(F(i, 0)) + V.row(F(i, 1)) + V.row(F(i, 2)) + V.row(F(i, 3))) / 4.0;
    face_points.row(i) = face_point;
  }
}

// make an edge by two endpoints of the edge. endpoint with smaller
// vertex index should be the first.
std::pair<int, int> make_edge(
  const int & id1,
  const int & id2
)
{
  assert(id1 != id2 && "this is a point not an edge!!!");
  if (id1 < id2) return std::make_pair(id1, id2);
  else return std::make_pair(id2, id1);
}

// find midpoints for all edges.
void init_edges_mid_point(
  const Eigen::MatrixXd & V, 
  const Eigen::MatrixXi & F, 
  std::map<std::pair<int, int>, Eigen::RowVector3d> & edge_mid_map
)
{
  int frow = F.rows();
  for (int i = 0; i < frow; i++) {
    std::pair<int, int> e1 = make_edge(F(i, 0), F(i, 1));
    std::pair<int, int> e2 = make_edge(F(i, 1), F(i, 2));
    std::pair<int, int> e3 = make_edge(F(i, 2), F(i, 3));
    std::pair<int, int> e4 = make_edge(F(i, 3), F(i, 0));
    edge_mid_map[e1] = (Eigen::RowVector3d)((V.row(F(i, 0)) + V.row(F(i, 1))) / 2.0);
    edge_mid_map[e2] = (Eigen::RowVector3d)((V.row(F(i, 1)) + V.row(F(i, 2))) / 2.0);
    edge_mid_map[e3] = (Eigen::RowVector3d)((V.row(F(i, 2)) + V.row(F(i, 3))) / 2.0);
    edge_mid_map[e4] = (Eigen::RowVector3d)((V.row(F(i, 3)) + V.row(F(i, 0))) / 2.0);
    
  }
}

// find all adjacent faces for all edges.
void findEF(
  const Eigen::MatrixXi & F, 
  std::map<std::pair<int, int>, std::vector<int> > & EF
)
{
  int frow = F.rows();
  for (int i = 0; i < frow; i++) {
    std::pair<int, int> e1 = make_edge(F(i, 0), F(i, 1));
    std::pair<int, int> e2 = make_edge(F(i, 1), F(i, 2));
    std::pair<int, int> e3 = make_edge(F(i, 2), F(i, 3));
    std::pair<int, int> e4 = make_edge(F(i, 3), F(i, 0));
    EF[e1].push_back(i);
    EF[e2].push_back(i);
    EF[e3].push_back(i);
    EF[e4].push_back(i);
  }
}

// find all adjacent edges for each vertex
void findVE(
  const Eigen::MatrixXi & F, 
  std::map<int, std::vector<std::pair<int, int> > > & VE
)
{
  int frow = F.rows();
  for (int i = 0; i < frow; i++) {
    std::pair<int, int> e1 = make_edge(F(i, 0), F(i, 1));
    std::pair<int, int> e2 = make_edge(F(i, 1), F(i, 2));
    std::pair<int, int> e3 = make_edge(F(i, 2), F(i, 3));
    std::pair<int, int> e4 = make_edge(F(i, 3), F(i, 0));
    VE[F(i, 0)].push_back(e1);
    VE[F(i, 0)].push_back(e4);
    VE[F(i, 1)].push_back(e1);
    VE[F(i, 1)].push_back(e2);
    VE[F(i, 2)].push_back(e2);
    VE[F(i, 2)].push_back(e3);
    VE[F(i, 3)].push_back(e3);
    VE[F(i, 3)].push_back(e4);
  }
}

// calculate new point for each original vertex
void calculate_newP(
  const Eigen::MatrixXd & V, 
  const Eigen::MatrixXi & F, 
  std::map<int, std::vector<std::pair<int, int> > > & VE, 
  std::map<std::pair<int, int>, Eigen::RowVector3d> & edge_mid_map, 
  std::vector<std::vector<int> > & VF,
  Eigen::MatrixXd & face_points, 

  Eigen::MatrixXd & newP
)
{
  int vrow = V.rows();
  int frow = F.rows();
  newP.resize(vrow, 3);
  for (int i = 0; i < vrow; i++) {
    Eigen::RowVector3d F(0, 0, 0);
    for (int j = 0; j < VF[i].size(); j++) {
      F += face_points.row(VF[i][j]);
    }
    F /= VF[i].size();
    // std::cout<<F<<std::endl;
    Eigen::RowVector3d R(0, 0, 0);
    for (int j = 0; j < VE[i].size(); j++) R += edge_mid_map[VE[i][j]];
    R /= VE[i].size();
    newP.row(i) = (F + 2 * R + (VF[i].size() - 3) * V.row(i)) / VF[i].size();
  }

}

// calculate the edge_points for each edge
void calculate_edge_points(
  const Eigen::MatrixXd & V, 
  const Eigen::MatrixXi & F, 
  std::map<std::pair<int, int>, std::vector<int> > & EF, 
  Eigen::MatrixXd & face_points,
   
  std::map<std::pair<int, int>, Eigen::RowVector3d> & edge_points
)
{
  std::map<std::pair<int, int>, std::vector<int> >::iterator e;
  for (e = EF.begin(); e != EF.end(); e++) {
    int cnt = 0;
    Eigen::RowVector3d edge_point(0, 0, 0);
    for (int i = 0; i < (e->second).size(); i++) {
      edge_point += face_points.row(e->second[i]);
    }
    edge_point += V.row(e->first.first) + V.row(e->first.second);
    edge_point /= 2 + e->second.size();
    edge_points[e->first] = edge_point;
  }
}

// since Eigen is not hashable, then I will use tuple to save in the hashmap as key.
std::tuple<double, double, double> vec_to_pair(
  Eigen::RowVector3d vec
)
{
  return std::make_tuple(vec.x(), vec.y(), vec.z());
}


void catmull_clark(
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  const int num_iters,
  Eigen::MatrixXd & SV,
  Eigen::MatrixXi & SF)
{
  if (num_iters == 0) {
    SV = V;
    SF = F;
    return;    
  } // base case
  std::map<std::tuple<double, double, double>, int> points_info;
  std::vector<std::vector<int> > VF;
  findVF(V, F, VF);

  Eigen::MatrixXd face_points;
  calculate_face_points(V, F, face_points);

  std::map<std::pair<int, int>, Eigen::RowVector3d> edge_mid_map;
  init_edges_mid_point(V, F, edge_mid_map);

  std::map<std::pair<int, int>, std::vector<int> > EF;
  findEF(F, EF);

  std::map<int, std::vector<std::pair<int, int> > > VE;
  findVE(F, VE);

  Eigen::MatrixXd newP;
  calculate_newP(V, F, VE, edge_mid_map, VF, face_points, newP);

  std::map<std::pair<int, int>, Eigen::RowVector3d> edge_points;
  calculate_edge_points(V, F, EF, face_points, edge_points);
  
  int rowf = F.rows();
  int rowv = V.rows();
  Eigen::MatrixXd TSV;
  Eigen::MatrixXi TSF;
  // Idea: we need to find all new vertex and all new faces. Consider that for each original faces will be seperated into four parts.
  // Then I will focus on those four parts and save it. However the problem is there will be dupulicate points. Then I will use hashmap
  // to solve this problem. For each point it will have an index. And then I will save it in the points_info hashmap. In the end, I will
  // save all points to SV based on the index of each point.
  TSF.resize(4*rowf, 4);
  TSV.resize(rowv+rowf+edge_points.size(), 3);
  int pt_count = 0;
  int f_count = 0;
  for (int i = 0; i < rowf; i++) {
    std::pair<int, int> e1 = make_edge(F(i, 0), F(i, 1));
    std::pair<int, int> e2 = make_edge(F(i, 1), F(i, 2));
    std::pair<int, int> e3 = make_edge(F(i, 2), F(i, 3));
    std::pair<int, int> e4 = make_edge(F(i, 3), F(i, 0));
    // std::cout << "a\n";
    if (points_info.count(vec_to_pair(edge_points[e1])) == 0) 
      points_info[vec_to_pair(edge_points[e1])] = pt_count++;
    if (points_info.count(vec_to_pair(edge_points[e2])) == 0) 
      points_info[vec_to_pair(edge_points[e2])] = pt_count++;
    if (points_info.count(vec_to_pair(edge_points[e3])) == 0) points_info[vec_to_pair(edge_points[e3])] = pt_count++;
    if (points_info.count(vec_to_pair(edge_points[e4])) == 0) points_info[vec_to_pair(edge_points[e4])] = pt_count++;
    if (points_info.count(vec_to_pair(newP.row(F(i, 0)))) == 0) points_info[vec_to_pair(newP.row(F(i, 0)))] = pt_count++;
    if (points_info.count(vec_to_pair(newP.row(F(i, 1)))) == 0) points_info[vec_to_pair(newP.row(F(i, 1)))] = pt_count++;
    if (points_info.count(vec_to_pair(newP.row(F(i, 2)))) == 0) points_info[vec_to_pair(newP.row(F(i, 2)))] = pt_count++;
    if (points_info.count(vec_to_pair(newP.row(F(i, 3)))) == 0) points_info[vec_to_pair(newP.row(F(i, 3)))] = pt_count++;
    if (points_info.count(vec_to_pair(face_points.row(i))) == 0) points_info[vec_to_pair(face_points.row(i))] = pt_count++;
    // std::cout<<"b\n";
    TSF.row(f_count++) = Eigen::RowVector4i(points_info[vec_to_pair(edge_points[e1])], points_info[vec_to_pair(newP.row(F(i, 1)))], points_info[vec_to_pair(edge_points[e2])], points_info[vec_to_pair(face_points.row(i))]);
    TSF.row(f_count++) = Eigen::RowVector4i(points_info[vec_to_pair(face_points.row(i))], points_info[vec_to_pair(edge_points[e2])], points_info[vec_to_pair(newP.row(F(i, 2)))], points_info[vec_to_pair(edge_points[e3])]);
    TSF.row(f_count++) = Eigen::RowVector4i(points_info[vec_to_pair(edge_points[e4])], points_info[vec_to_pair(face_points.row(i))], points_info[vec_to_pair(edge_points[e3])], points_info[vec_to_pair(newP.row(F(i, 3)))]);
    TSF.row(f_count++) = Eigen::RowVector4i(points_info[vec_to_pair(newP.row(F(i, 0)))], points_info[vec_to_pair(edge_points[e1])], points_info[vec_to_pair(face_points.row(i))], points_info[vec_to_pair(edge_points[e4])]);
    // std::cout << "c\n";
  }
  // std::cout << 9 <<std::endl;

  std::map<std::tuple<double, double, double>, int>::iterator it;
  for (it = points_info.begin(); it != points_info.end(); it++) {
    Eigen::RowVector3d curr_v(std::get<0>(it->first), std::get<1>(it->first), std::get<2>(it->first));
    TSV.row(it->second) = curr_v;
  }
  
  catmull_clark(TSV, TSF, num_iters - 1, SV, SF);
}
