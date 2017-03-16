//
// Copyright (c) 2017 CNRS
//
// This file is part of PinInvDyn
// PinInvDyn is free software: you can redistribute it
// and/or modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation, either version
// 3 of the License, or (at your option) any later version.
// pinocchio is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// General Lesser Public License for more details. You should have
// received a copy of the GNU Lesser General Public License along with
// PinInvDyn. If not, see
// <http://www.gnu.org/licenses/>.
//

#ifndef __invdyn_math_constraint_inequality_hpp__
#define __invdyn_math_constraint_inequality_hpp__

#include <Eigen/Core>
#include <pininvdyn/math/constraint-base.hpp>
#include <exception>

namespace pininvdyn
{
  namespace math
  {

    class ConstraintInequality
        :public pininvdyn::math::ConstraintBase
    {
    public:

      ConstraintInequality(const std::string & name);

      ConstraintInequality(const std::string & name,
                           const unsigned int rows,
                           const unsigned int cols);

      ConstraintInequality(const std::string & name,
                           ConstRefMatrix A,
                           ConstRefVector lb,
                           ConstRefVector ub);

      unsigned int rows() const;
      unsigned int cols() const;
      void resize(const unsigned int r, const unsigned int c);

      bool isEquality() const;
      bool isInequality() const;
      bool isBound() const;

      const Vector & vector()     const;
      const Vector & lowerBound() const;
      const Vector & upperBound() const;

      Vector & vector();
      Vector & lowerBound();
      Vector & upperBound();

      bool setVector(ConstRefVector b);
      bool setLowerBound(ConstRefVector lb);
      bool setUpperBound(ConstRefVector ub);

      bool checkConstraint(ConstRefVector x, double tol=1e-6) const;

    protected:
      Vector m_lb;
      Vector m_ub;
    };

  }
}

#endif // ifndef __invdyn_math_constraint_equality_hpp__
