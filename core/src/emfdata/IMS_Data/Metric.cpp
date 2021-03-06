// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * IMS_Data/Metric.cpp
 * Copyright (C) Cátedra SAES-UMU 2010 <andres.senac@um.es>
 *
 * EMF4CPP is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EMF4CPP is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Metric.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "IMS_Data/IMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::IMS_Data;

// Default constructor
Metric::Metric() :
    m_type(0), m_value(0), m_time(0)
{

    /*PROTECTED REGION ID(MetricImpl__MetricImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

Metric::~Metric()
{
}

/*PROTECTED REGION ID(Metric.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::IMS_Data::MetricType Metric::getType() const
{
    return m_type;
}

void Metric::setType(::IMS_Data::MetricType _type)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::IMS_Data::MetricType _old_type = m_type;
#endif
    m_type = _type;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::IMS_Data::IMS_DataPackage::_instance()->getMetric__type(),
                _old_type,
                m_type
        );
        eNotify(&notification);
    }
#endif
}

::ecore::ELong Metric::getValue() const
{
    return m_value;
}

void Metric::setValue(::ecore::ELong _value)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::ELong _old_value = m_value;
#endif
    m_value = _value;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::IMS_Data::IMS_DataPackage::_instance()->getMetric__value(),
                _old_value,
                m_value
        );
        eNotify(&notification);
    }
#endif
}

::ecore::ELong Metric::getTime() const
{
    return m_time;
}

void Metric::setTime(::ecore::ELong _time)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::ELong _old_time = m_time;
#endif
    m_time = _time;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::IMS_Data::IMS_DataPackage::_instance()->getMetric__time(),
                _old_time,
                m_time
        );
        eNotify(&notification);
    }
#endif
}

// References

