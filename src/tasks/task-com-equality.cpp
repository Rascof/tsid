//
// Copyright (c) 2017 CNRS
//
// This file is part of PinInvDyn
// PinInvDyn is free software: you can redistribute it
// and/or modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation, either version
// 3 of the License, or (at your option) any later version.
// PinInvDyn is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// General Lesser Public License for more details. You should have
// received a copy of the GNU Lesser General Public License along with
// PinInvDyn If not, see
// <http://www.gnu.org/licenses/>.
//

#include <pininvdyn/tasks/task-com-equality.hpp>

namespace pininvdyn
{
  namespace tasks
  {
    using namespace pininvdyn::math;
    using namespace se3;

    TaskComEquality::TaskComEquality(const std::string & name,
                                     RobotWrapper & robot):
      TaskMotion(name, robot),
      m_constraint(name, 3, robot.nv())
    {
      m_Kp.setZero(3);
      m_Kd.setZero(3);
    }

    int TaskComEquality::dim() const
    {
      //return self._mask.sum ()
      return 3;
    }

    const Vector3 & TaskComEquality::Kp(){ return m_Kp; }

    const Vector3 & TaskComEquality::Kd(){ return m_Kd; }

    void TaskComEquality::Kp(ConstRefVector Kp)
    {
      assert(Kp.size()==3);
      m_Kp = Kp;
    }

    void TaskComEquality::Kd(ConstRefVector Kd)
    {
      assert(Kd.size()==3);
      m_Kd = Kd;
    }

    void TaskComEquality::setReference(const TrajectorySample & ref)
    {
      m_ref = ref;
    }

    const Vector & TaskComEquality::position_error() const
    {
      return m_p_error_vec;
    }

    const Vector & TaskComEquality::velocity_error() const
    {
      return m_v_error_vec;
    }

    const Vector & TaskComEquality::position() const
    {
      return m_p_error_vec;
    }

    const Vector & TaskComEquality::velocity() const
    {
      return m_p_error_vec;
    }

    const Vector & TaskComEquality::position_ref() const
    {
      return m_p_error_vec;
    }

    const Vector & TaskComEquality::velocity_ref() const
    {
      return m_p_error_vec;
    }

    const ConstraintBase & TaskComEquality::getConstraint() const
    {
      return m_constraint;
    }

    const ConstraintBase & TaskComEquality::compute(const double t,
                                                    ConstRefVector q,
                                                    ConstRefVector v,
                                                    Data & data)
    {
      Vector3 p_com, v_com, drift;
      m_robot.com(data, p_com, v_com, drift);

      // Compute errors
      m_p_error = p_com - m_ref.pos;
      m_v_error = v_com - m_ref.vel;
      Vector3 m_a_des = - m_Kp.cwiseProduct(m_p_error)
                        - m_Kd.cwiseProduct(m_v_error)
                        + m_ref.acc;

      m_p_error_vec = m_p_error;
      m_v_error_vec = m_v_error;
#ifndef NDEBUG
//      std::cout<<m_name<<" errors: "<<m_p_error.norm()<<" "
//        <<m_v_error.norm()<<std::endl;
#endif

      // Get CoM jacobian
      const Matrix3x & Jcom = m_robot.Jcom(data);

      m_constraint.setMatrix(Jcom);
      m_constraint.setVector(m_a_des - drift);
      return m_constraint;
    }
    
  }
}
