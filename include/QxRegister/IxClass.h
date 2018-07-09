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

#ifndef _IX_CLASS_H_
#define _IX_CLASS_H_

#ifdef _MSC_VER
#pragma once
#endif

#include "../../include/QxDataMember/IxDataMemberX.h"

#include "../../include/QxFunction/IxFunction.h"

namespace qx {

class QX_DLL_EXPORT IxClass
{

protected:

   IxDataMemberX * m_pDataMemberX;  // List of data member
   IxFunctionX_ptr m_pFctMemberX;   // List of function member

   QString m_sKey;                  // 'IxClass' key <=> class name
   QString m_sKeyBaseClass;         // 'IxClass' base class key <=> base class name
   QString m_sName;                 // 'IxClass' name <=> database table name (if empty => class name)
   QString m_sDescription;          // 'IxClass' description
   long m_lVersion;                 // 'IxClass' version
   long m_lDaoStrategy;             // Dao class strategy to access data member
   bool m_bFinalClass;              // Class cannot have base class : 'qx::trait::no_base_class_defined' and 'QObject'

   QByteArray m_byteName;           // Optimization to retrieve name under "const char *" format
   const char * m_pName;            // Optimization to retrieve name under "const char *" format

protected:

   IxClass() : m_pDataMemberX(NULL), m_lVersion(-1), m_lDaoStrategy(QX_TABLE_PER_CLASS), m_bFinalClass(false), m_pName(NULL) { ; }
   virtual ~IxClass() = 0;

   void updateClassX();

public:

   inline QString getKey() const                   { return m_sKey; }
   inline QString getKeyBaseClass() const          { return m_sKeyBaseClass; }
   inline QString getName() const                  { return m_sName; }
   inline const char * getNamePtr() const          { return m_pName; }
   inline QString getDescription() const           { return m_sDescription; }
   inline long getVersion() const                  { return m_lVersion; }
   inline long getDaoStrategy() const              { return m_lDaoStrategy; }
   inline bool getFinalClass() const               { return m_bFinalClass; }
   inline IxDataMemberX * getDataMemberX() const   { return m_pDataMemberX; }
   inline IxFunctionX * getFctMemberX() const      { return m_pFctMemberX.get(); }
   inline IxDataMember * getId() const             { return (m_pDataMemberX ? m_pDataMemberX->getId() : NULL); }

   inline void setName(const QString & sName)         { m_sName = sName; updateNamePtr(); }
   inline void setDescription(const QString & sDesc)  { m_sDescription = sDesc; }
   inline void setDaoStrategy(long lDaoStrategy)      { m_lDaoStrategy = lDaoStrategy; }

private:

   inline void updateNamePtr()   { m_byteName = m_sName.toAscii(); m_pName = m_byteName.constData(); }

};

typedef boost::shared_ptr<IxClass> IxClass_ptr;

} // namespace qx

#endif // _IX_CLASS_H_
