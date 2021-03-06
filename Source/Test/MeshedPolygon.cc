/******************************************************************************

  This source file is part of the Delaunay project.

  Copyright T.J. Corona

  This source code is released under the New BSD License, (the "License").

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

******************************************************************************/

#include "Shape/LineSegment.hh"
#include "Shape/Polygon.hh"
#include "Shape/Point.hh"

#include "Mesh/Mesh.hh"

#include "Discretization/ConstrainedDelaunayMesh.hh"
#include "Discretization/DiscretizePolygon.hh"
// #include "Discretization/RuppertsAlgorithm.hh"

#include "Misc/Random.hh"

#include "Validation/IsValidPolygon.hh"

#include "Visualization/Color.hh"
#include "Visualization/CVCanvas.hh"
// #include "Visualization/VTKCanvas.hh"

#include <algorithm>
#include <iostream>

#include "PolygonGenerator.cc"

using namespace Delaunay;
using namespace Delaunay::Visualization;

int main(int argc,char** argv)
{
  if (argc > 1)
  {
    int seed = atoi(argv[1]);
    std::cout<<"Random seed: "<<seed<<std::endl;
    Misc::Random::GetInstance().Seed(seed);
  }

  PolygonType testType = Random;

  // if (argc > 1)
  //   testType = static_cast<PolygonType>(atoi(argv[1]));

  if (testType < 0 || testType > 3)
  {
    std::cout<<"MeshedPolygon: possible options:"<<std::endl;
    std::cout<<"               0: Regular"<<std::endl;
    std::cout<<"               1: Star Convex"<<std::endl;
    std::cout<<"               2: Random Evolve"<<std::endl;
    std::cout<<"               3: Random (default)"<<std::endl;
    return 1;
  }

  if (testType == Random)
    testType = static_cast<PolygonType>(Misc::Random::GetInstance().Uniform(3));

  std::cout<<"Test type: ";
  switch (testType)
  {
    case Regular:
      std::cout<<"Regular"<<std::endl;
      break;
    case StarConvex:
      std::cout<<"Star Convex"<<std::endl;
      break;
    case RandomEvolve:
      std::cout<<"RandomEvolve"<<std::endl;
      break;
    default:
      assert(false);
  }

  // create a canvas with x span from 0 to 10, and y span from 0 to 10
  double bounds[4] = {0.,10.,0.,10.};
  Visualization::CVCanvas canvas(bounds[0],bounds[1],bounds[2],bounds[3]);
  // Visualization::VTKCanvas canvas(bounds[0],bounds[1],bounds[2],bounds[3]);

  const unsigned nPoints = 3 + Misc::Random::GetInstance().Uniform(100);

  std::vector<Shape::Point> vertices(std::move(GeneratePolygonPoints(testType,
								     nPoints,
								     bounds)));

  // create a polygon from the point vector
  Shape::Polygon polygon(vertices);

  Validation::IsValidPolygon isValidPolygon;
  if (!isValidPolygon(polygon))
  {
    std::cout<<"Invalid polygon was formed"<<std::endl;
    return 0;
  }

  Mesh::Mesh mesh;
  // Discretization::DiscretizePolygon discretize;
  Discretization::ConstrainedDelaunayMesh discretize;
  // Discretization::RuppertsAlgorithm discretize;
  discretize(polygon, mesh);

  Color faintRed(255,0,0,128);

  canvas.Draw(polygon,Red,faintRed);
  // canvas.Draw(mesh.GetPerimeter(),Red,faintRed);

  for (auto triangle : mesh.GetTriangles())
    canvas.Draw(triangle, Visualization::Black);

  for (auto edge : mesh.GetEdges())
    if (edge.boundary)
      canvas.Draw(edge, Delaunay::Visualization::Green);

  // for (unsigned i=0;i<mesh.GetPerimeter().GetPoints().size();i++)
  //   canvas.Draw(mesh.GetPerimeter().GetPoints()[i],
  // 		Visualization::Color(i/(mesh.GetPerimeter()
  //       				.GetPoints().size()-1.),
  // 				     Visualization::Color::BlueToRed));

  canvas.SetTimeDelay(1);
  canvas.Update();

  return 0;
}
