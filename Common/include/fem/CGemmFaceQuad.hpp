/*!
 * \file CGemmFaceQuad.hpp
 * \brief Class for carrying out a GEMM multiplication for a face
 *        when the adjacent element is a quadrilateral. In this
 *        case a tensor product is used.
 *        The functions are in the <i>CGemmFaceQuad.cpp</i> file.
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

#pragma once

#include "CGemmBase.hpp"
#include "../CConfig.hpp"
#include "../tensor_products/TensorProductSurfaceIntPoints2D.hpp"

using namespace std;

/*!
 * \class CGemmFaceQuad
 * \brief Class to carry out a GEMM multiplication for a face when the adjacent
 *        element is a quadrilateral. In this case a tensor product is used.
 * \author E. van der Weide
 * \version 7.0.8 "Blackbird"
 */
class CGemmFaceQuad final : public CGemmBase {

public:
  /*-----------------------------------------------------------------------------------*/
  /*---                     Constructors and destructors.                           ---*/
  /*-----------------------------------------------------------------------------------*/

  /*!
   * \brief Default constructor of the class, deleted to make sure the
   *        overloaded constructor is always used.
   */
  CGemmFaceQuad() = delete;

  /*!
   * \overload
   * \param[in] val_M    - First tensor dimensions of A and C in the gemm call.
   * \param[in] val_Type - The type of tensor product to be carried out.
   * \param[in] val_K    - First tensor dimensions of B and last tensor dimensions
   *                       of A in the gemm call.
   */
  CGemmFaceQuad(const int val_M, const int val_Type, const int val_K);

  /*!
   * \brief Destructor, nothing to be done.
   */
  ~CGemmFaceQuad() = default;

private:

  int M;   /*!< \brief First tensor dimensions of A and C in the gemm call. */
  int K;   /*!< \brief First tensor dimensions of B and last tensor dimensions
                       of A in the gemm call. */

  int TypeTensorProduct; /*!< \brief Indicates the type of tensor product to be
                                     carried out, DOFS_TO_INT or INT_TO_DOFS. */

  TPIS2D TensorProductDataSurfIntPoints = nullptr; /*!< \brief Function pointer to carry out the tensor product
                                                               to compute the data in the surface integration points. */
};