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

namespace qx {
namespace dao {
namespace detail {

template <class T>
struct QxSqlQueryHelper_Insert
{

   static void sql(QString & sql, qx::IxSqlQueryBuilder & builder)
   {
      BOOST_STATIC_ASSERT(qx::trait::is_qx_registered<T>::value);
      long l1(0), l2(0);
      qx::IxDataMember * p = NULL;
      qx::IxDataMember * pId = builder.getDataId();
      qx::IxSqlRelation * pRelation = NULL;
      qx::QxSqlRelationParams params(0, 0, (& sql), (& builder), NULL, NULL);
      QString table = builder.table();
      sql = "INSERT INTO " + table + " (";
      if (pId && ! pId->getAutoIncrement()) { sql += pId->getName() + ", "; }
      while ((p = builder.nextData(l1))) { sql += p->getName() + ", "; }
      while ((pRelation = builder.nextRelation(l2))) { params.setIndex(l2); pRelation->lazyInsert(params); }
      sql = sql.left(sql.count() - 2); // Remove last ", "
      sql += ") VALUES (";
      l1 = 0; l2 = 0; p = NULL; pRelation = NULL;
      if (pId && ! pId->getAutoIncrement()) { sql += pId->getSqlPlaceHolder() + ", "; }
      while ((p = builder.nextData(l1))) { sql += p->getSqlPlaceHolder() + ", "; }
      while ((pRelation = builder.nextRelation(l2))) { params.setIndex(l2); pRelation->lazyInsert_Values(params); }
      sql = sql.left(sql.count() - 2); // Remove last ", "
      sql += ")";
   }

   static void resolveInput(T & t, QSqlQuery & query, qx::IxSqlQueryBuilder & builder)
   {
      BOOST_STATIC_ASSERT(qx::trait::is_qx_registered<T>::value);
      long l1(0), l2(0);
      qx::IxDataMember * p = NULL;
      qx::IxDataMember * pId = builder.getDataId();
      qx::IxSqlRelation * pRelation = NULL;
      qx::QxSqlRelationParams params(0, 0, NULL, (& builder), (& query), (& t));
      if (pId && ! pId->getAutoIncrement()) { pId->setSqlPlaceHolder(query, (& t)); }
      while ((p = builder.nextData(l1))) { p->setSqlPlaceHolder(query, (& t)); }
      while ((pRelation = builder.nextRelation(l2))) { params.setIndex(l2); pRelation->lazyInsert_ResolveInput(params); }
   }

   static void resolveOutput(T & t, QSqlQuery & query, qx::IxSqlQueryBuilder & builder)
   { Q_UNUSED(t); Q_UNUSED(query); Q_UNUSED(builder); }

};

} // namespace detail
} // namespace dao
} // namespace qx
