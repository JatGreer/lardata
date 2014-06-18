//
// File: SurfaceTest.cxx
//
// Purpose: Single source executable with tests for surface classes 
//          Surface and SurfYZPlane and SurfXYZPlane.
//

#include <iostream>
#include <cassert>
#include <cmath>
#include "RecoObjects/SurfYZPlane.h"
#include "RecoObjects/SurfXYZPlane.h"
#include "RecoObjects/KalmanLinearAlgebra.h"
#include "cetlib/exception.h"

int main()
{
  // Make sure assert is enabled.

  bool assert_flag = false;
  assert((assert_flag = true, assert_flag));
  if ( ! assert_flag ) {
    std::cerr << "Assert is disabled" << std::endl;
    return 1;
  }

  // Make some YZ surfaces.

  trkf::SurfYZPlane surf1;              // Default constructed.
  trkf::SurfYZPlane surf2(0., 0., 0.);  // Same as surf1.
  trkf::SurfYZPlane surf3(1., 1., 0.);  // Different origin, parallel to surf1 and surf2.
  trkf::SurfYZPlane surf4(2., 2., 1.);  // Not parallel.

  // Test binary equality comparisions.

  assert(surf1.isEqual(surf2));
  assert(!surf1.isEqual(surf3));
  assert(!surf1.isEqual(surf4));
  assert(!surf2.isEqual(surf3));
  assert(!surf2.isEqual(surf4));
  assert(!surf3.isEqual(surf4));
  std::cout << "SurfYZPlane Equality OK." << std::endl;

  // Test all binary parallel comparisions.

  assert(surf1.isParallel(surf2));
  assert(surf1.isParallel(surf3));
  assert(!surf1.isParallel(surf4));
  assert(surf2.isParallel(surf3));
  assert(!surf2.isParallel(surf4));
  assert(!surf3.isParallel(surf4));
  std::cout << "SurfYZPlane Parallel OK." << std::endl;

  // Test coordinate transformations.

  double xyz1[3] = {1., 2., 3.};
  double xyz2[3];
  double uvw[3];
  surf4.toLocal(xyz1, uvw);
  surf4.toGlobal(uvw, xyz2);
  for(int i=0; i<3; ++i)
    assert(std::abs(xyz1[i] - xyz2[i]) < 1.e-6);  
  std::cout << "SurfYZPlane Coordinate transformation OK." << std::endl;

  // Test separation.

  assert(surf1.distanceTo(surf2) == 0.);
  assert(surf1.distanceTo(surf3) == 1.);
  assert(surf3.distanceTo(surf1) == -1.);

  // Should throw exception (not parallel).

  bool caught = false;
  try {
    surf1.distanceTo(surf4);
  }
  catch (cet::exception& x) {
    caught = true;
  }
  assert(caught);
  std::cout << "SurfYZPlane Separation OK." << std::endl;

  // Test track parameters.

  trkf::TrackVector v(5);
  v(0) = 0.1;   // u.
  v(1) = 0.2;   // v.
  v(2) = 2.;    // du/dw.
  v(3) = 3.;    // dv/dw.
  v(4) = 0.5;   // p = 2 GeV.

  // For this vector, the direction cosines are.
  // du/ds = 2./sqrt(14.);
  // dv/ds = 3./sqrt(14.);
  // dw/ds = 1./sqrt(14.);

  double xyz[3];
  double mom[3];
  surf1.getPosition(v, xyz);
  assert(std::abs(xyz[0] - 0.1) < 1.e-6);
  assert(std::abs(xyz[1] - 0.2) < 1.e-6);
  assert(std::abs(xyz[2]) < 1.e-6);
  surf3.getPosition(v, xyz);
  assert(std::abs(xyz[0] - 0.1) < 1.e-6);
  assert(std::abs(xyz[1] - 1.2) < 1.e-6);
  assert(std::abs(xyz[2] - 1.0) < 1.e-6);
  surf1.getMomentum(v, mom, trkf::Surface::FORWARD);
  assert(std::abs(mom[0] - 4./std::sqrt(14.)) < 1.e-6);
  assert(std::abs(mom[1] - 6./std::sqrt(14.)) < 1.e-6);
  assert(std::abs(mom[2] - 2./std::sqrt(14.)) < 1.e-6);
  surf1.getMomentum(v, mom, trkf::Surface::BACKWARD);
  assert(std::abs(mom[0] + 4./std::sqrt(14.)) < 1.e-6);
  assert(std::abs(mom[1] + 6./std::sqrt(14.)) < 1.e-6);
  assert(std::abs(mom[2] + 2./std::sqrt(14.)) < 1.e-6);
  surf4.getMomentum(v, mom, trkf::Surface::FORWARD);
  assert(std::abs(mom[0] - 4./std::sqrt(14.)) < 1.e-6);
  assert(std::abs(mom[1] - (6.*std::cos(1.) - 2.*std::sin(1.))/std::sqrt(14.)) < 1.e-6);
  assert(std::abs(mom[2] - (6.*std::sin(1.) + 2.*std::cos(1.))/std::sqrt(14.)) < 1.e-6);

  // Should throw exception (no direction).

  caught = false;
  try {
    surf1.getMomentum(v, mom);
  }
  catch(cet::exception& x) {
    caught = true;
  }
  assert(caught);
  std::cout << "SurfYZPlane Position/momentum OK." << std::endl;

  // Make some XYZ surfaces.

  trkf::SurfXYZPlane surf1x;                      // Default constructed.
  trkf::SurfXYZPlane surf2x(0., 0., 0., 0., 0.);  // Same as surf1x.
  trkf::SurfXYZPlane surf3x(1., 1., 1., 0., 0.);  // Different origin, parallel.
  trkf::SurfXYZPlane surf4x(1., 2., 2., 1., 0.1); // Not parallel.

  // Test binary equality comparisions.

  assert(surf1x.isEqual(surf2x));
  assert(!surf1x.isEqual(surf3x));
  assert(!surf1x.isEqual(surf4x));
  assert(!surf2x.isEqual(surf3x));
  assert(!surf2x.isEqual(surf4x));
  assert(!surf3x.isEqual(surf4x));
  std::cout << "SurfXYZPlane Equality OK." << std::endl;

  // Test all binary parallel comparisions.

  assert(surf1x.isParallel(surf2x));
  assert(surf1x.isParallel(surf3x));
  assert(!surf1x.isParallel(surf4x));
  assert(surf2x.isParallel(surf3x));
  assert(!surf2x.isParallel(surf4x));
  assert(!surf3x.isParallel(surf4x));
  std::cout << "SurfXYZPlane Parallel OK." << std::endl;

  // Test coordinate transformations.

  xyz1[0] = 1.;
  xyz1[1] = 2.;
  xyz2[2] = 3.;
  surf4x.toLocal(xyz1, uvw);
  surf4x.toGlobal(uvw, xyz2);
  for(int i=0; i<3; ++i)
    assert(std::abs(xyz1[i] - xyz2[i]) < 1.e-6);  
  std::cout << "SurfXYZPlane Coordinate transformation OK." << std::endl;

  // Test separation.

  assert(surf1x.distanceTo(surf2x) == 0.);
  assert(surf1x.distanceTo(surf3x) == 1.);
  assert(surf3x.distanceTo(surf1x) == -1.);

  // Should throw exception (not parallel).

  caught = false;
  try {
    surf1x.distanceTo(surf4x);
  }
  catch (cet::exception& x) {
    caught = true;
  }
  assert(caught);
  std::cout << "SurfXYZPlane Separation OK." << std::endl;

  // Done (success).

  std::cout << "SurfaceTest: All tests passed." << std::endl;

  return 0;
}
