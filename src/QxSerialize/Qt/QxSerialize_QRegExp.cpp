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

#include "../../../include/QxPrecompiled.h"

#include "../../../include/QxSerialize/Qt/QxSerialize_QRegExp.h"

#include "../../../include/QxMemLeak.h"

namespace boost {
namespace serialization {

template <class Archive>
inline void qx_save(Archive & ar, const QRegExp & t, const unsigned int file_version)
{
   Q_UNUSED(file_version);
   QString sPattern = t.pattern();
   int iCaseSensitivity = static_cast<int>(t.caseSensitivity());
   int iPatternSyntax = static_cast<int>(t.patternSyntax());
   bool bMinimal = t.isMinimal();

   ar << boost::serialization::make_nvp("pattern", sPattern);
   ar << boost::serialization::make_nvp("caseSensitivity", iCaseSensitivity);
   ar << boost::serialization::make_nvp("patternSyntax", iPatternSyntax);
   ar << boost::serialization::make_nvp("minimal", bMinimal);
}

template <class Archive>
inline void qx_load(Archive & ar, QRegExp & t, const unsigned int file_version)
{
   Q_UNUSED(file_version);
   QString sPattern; bool bMinimal(false);
   int iCaseSensitivity(0), iPatternSyntax(0);

   ar >> boost::serialization::make_nvp("pattern", sPattern);
   ar >> boost::serialization::make_nvp("caseSensitivity", iCaseSensitivity);
   ar >> boost::serialization::make_nvp("patternSyntax", iPatternSyntax);
   ar >> boost::serialization::make_nvp("minimal", bMinimal);

   t.setPattern(sPattern);
   t.setCaseSensitivity(static_cast<Qt::CaseSensitivity>(iCaseSensitivity));
   t.setPatternSyntax(static_cast<QRegExp::PatternSyntax>(iPatternSyntax));
   t.setMinimal(bMinimal);
}

} // namespace boost
} // namespace serialization

QX_SERIALIZE_FAST_COMPIL_SAVE_LOAD_CPP(QRegExp)
