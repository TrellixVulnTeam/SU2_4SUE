/*!
 * \file CFEMStandardQuadAdjacentHexSol.cpp
 * \brief Functions for the class CFEMStandardQuadAdjacentHexSol.
 * \author E. van der Weide
 * \version 7.0.8 "Blackbird"
 *
 * SU2 Project Website: https://su2code.github.io
 *
 * The SU2 Project is maintained by the SU2 Foundation
 * (http://su2foundation.org)
 *
 * Copyright 2012-2020, SU2 Contributors (cf. AUTHORS.md)
 *
 * SU2 is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * SU2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with SU2. If not, see <http://www.gnu.org/licenses/>.
 */

#include "../../include/fem/CFEMStandardQuadAdjacentHexSol.hpp"

/*----------------------------------------------------------------------------------*/
/*            Public member functions of CFEMStandardQuadAdjacentHexSol.            */
/*----------------------------------------------------------------------------------*/

CFEMStandardQuadAdjacentHexSol::CFEMStandardQuadAdjacentHexSol(const unsigned short val_nPoly,
                                                               const unsigned short val_orderExact,
                                                               const unsigned short val_faceID_Elem,
                                                               const unsigned short val_orientation,
                                                               CGemmBase           *val_gemm_1,
                                                               CGemmBase           *val_gemm_2)
  : CFEMStandardHexBase(),
    CFEMStandardQuadBase(val_nPoly, val_orderExact) {

  /*--- Store the faceID of the element, the orientation and the
        pointers for the gemm functionalities. ---*/
  faceID_Elem  = val_faceID_Elem;
  orientation  = val_orientation;
  gemmDOFs2Int = val_gemm_1;
  gemmInt2DOFs = val_gemm_2;

  /*--- Convert the 2D coordinates of the standard quadrilateral to the parametric 
        coordinates in normal and two tangential directions of the adjacent
        hexahedron. ---*/
  vector<passivedouble> rNorm, rTang0, rTang1;
  ConvertCoor2DQuadFaceTo3DHex(rLineInt, faceID_Elem, orientation, swapTangInTensor,
                               rNorm, rTang0, rTang1);

  /*--- Create the 1D Legendre basis functions for the parametric
        coordinates in normal direction. ---*/
  ColMajorMatrix<passivedouble> legN(1, nPoly+1), derLegN(1, nPoly+1);

  Vandermonde1D(nPoly, rNorm, legN);
  GradVandermonde1D(nPoly, rNorm, derLegN);

  /*--- Create the 1D Legendre basis functions for the parametric
        coordinates in the first tangential direction. ---*/
  ColMajorMatrix<passivedouble> legT0, derLegT0;
  legT0.resize(nIntegrationPad, nPoly+1);    legT0.setConstant(0.0);
  derLegT0.resize(nIntegrationPad, nPoly+1); derLegT0.setConstant(0.0);

  Vandermonde1D(nPoly, rTang0, legT0);
  GradVandermonde1D(nPoly, rTang0, derLegT0);

  /*--- Create the 1D Legendre basis functions for the parametric
        coordinates in the second tangential direction. ---*/
  ColMajorMatrix<passivedouble> legT1, derLegT1;
  legT1.resize(nIntegrationPad, nPoly+1);    legT1.setConstant(0.0);
  derLegT1.resize(nIntegrationPad, nPoly+1); derLegT1.setConstant(0.0);

  Vandermonde1D(nPoly, rTang1, legT1);
  GradVandermonde1D(nPoly, rTang1, derLegT1);

  /*--- Allocate the memory for the first index of tensorSol, etc. As this
        function is only called for 3D simulations, there are three
        contributions to the tensor products. ---*/
  tensorSol.resize(3);
  tensorDSolDr.resize(3);
  tensorDSolDs.resize(3);
  tensorDSolDt.resize(3);

  /*--- Set the components of the tensors. For the derivatives this
        depends on the faceID in the element. ---*/
  tensorSol[0] = legN;
  tensorSol[1] = legT0;
  tensorSol[2] = legT1;

  if((faceID_Elem == 0) || (faceID_Elem == 1)) {
    tensorDSolDr[0] = legN;    tensorDSolDr[1] = derLegT0; tensorDSolDr[2] = legT1;
    tensorDSolDs[0] = legN;    tensorDSolDs[1] = legT0;    tensorDSolDs[2] = derLegT1;
    tensorDSolDt[0] = derLegN; tensorDSolDt[1] = legT0;    tensorDSolDt[2] = legT1;
  }
  else if((faceID_Elem == 2) || (faceID_Elem == 3)) {
    tensorDSolDr[0] = legN;    tensorDSolDr[1] = derLegT0; tensorDSolDr[2] = legT1;
    tensorDSolDs[0] = derLegN; tensorDSolDs[1] = legT0;    tensorDSolDs[2] = legT1;
    tensorDSolDt[0] = legN;    tensorDSolDt[1] = legT0;    tensorDSolDt[2] = derLegT1;
  }
  else {
    tensorDSolDr[0] = derLegN; tensorDSolDr[1] = legT0;    tensorDSolDr[2] = legT1;
    tensorDSolDs[0] = legN;    tensorDSolDs[1] = derLegT0; tensorDSolDs[2] = legT1;
    tensorDSolDt[0] = legN;    tensorDSolDt[1] = legT0;    tensorDSolDt[2] = derLegT1;
  }
}