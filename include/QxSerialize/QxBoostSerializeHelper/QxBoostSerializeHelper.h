/****************************************************************************
**
** Copyright (C) 2010 QxOrm France and/or its subsidiary(-ies)
** Contact: QxOrm France Information (contact@qxorm.com)
**
** This file is part of the QxOrm library
**
** Commercial Usage
** Licensees holding valid QxOrm Commercial licenses may use this file in
** accordance with the QxOrm Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and QxOrm France
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file 'license.gpl3.txt' included in the
** packaging of this file. Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html
**
** If you are unsure which license is appropriate for your use, please
** contact the support department at support@qxorm.com
**
****************************************************************************/

#ifndef _QX_BOOST_SERIALIZE_HELPER_H_
#define _QX_BOOST_SERIALIZE_HELPER_H_

#ifdef _MSC_VER
#pragma once
#endif

#include <boost/type_traits/is_same.hpp>
#include <boost/serialization/void_cast.hpp>

#include "../../../include/QxTraits/get_base_class.h"

#include "../../../include/QxSerialize/QxBoostSerializeHelper/QxBoostInitGuid.h"

namespace qx {

template <class T>
class QxBoostSerializeHelper
{

private:

   typedef T type_derived;
   typedef typename qx::trait::get_base_class<type_derived>::type type_base;

public:

   template <class Archive>
   static inline void register_type_helper(Archive & ar, const T * t)
   { Q_UNUSED(t); ar.template register_type<type_derived>(); }

   static inline void void_cast_register_helper(const T * t)
   { Q_UNUSED(t); QxBoostVoidCastRegisterHelper<boost::is_same<type_base, qx::trait::no_base_class_defined>::value, 0>::helper(); }

   static inline void init_guid(const T * t)
   { Q_UNUSED(t); QxBoostInitGuid<T>::init(); }

private:

   template <bool bNoBaseClass /* = false */, int dummy>
   struct QxBoostVoidCastRegisterHelper
   { static inline void helper() { boost::serialization::void_cast_register(static_cast<type_derived *>(NULL), static_cast<type_base *>(NULL)); } };

   template <int dummy>
   struct QxBoostVoidCastRegisterHelper<true, dummy>
   { static inline void helper() { /* Nothing */ ; } };

};

} // namespace qx

#endif // _QX_BOOST_SERIALIZE_HELPER_H_
