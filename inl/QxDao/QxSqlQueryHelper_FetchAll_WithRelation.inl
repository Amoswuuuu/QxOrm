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
struct QxSqlQueryHelper_FetchAll_WithRelation
{

   static void sql(qx::IxSqlRelationX * pRelationX, QString & sql, qx::IxSqlQueryBuilder & builder)
   {
      BOOST_STATIC_ASSERT(qx::trait::is_qx_registered<T>::value);
      if (! pRelationX || (pRelationX->count() <= 0)) { qAssert(false); QxSqlQueryHelper_FetchAll<T>::sql(sql, builder); return; }
      long l1(0), l2(0), l3(0), l4(0);
      qx::IxDataMember * p = NULL;
      qx::IxDataMember * pId = builder.getDataId();
      qx::IxSqlRelation * pRelation = NULL;
      qx::QxSqlRelationParams params(0, 0, (& sql), (& builder), NULL, NULL);
      QString table = builder.table();
      sql = "SELECT ";
      if (pId) { sql += (table + "." + pId->getName() + " AS " + pId->getSqlAlias(& table) + ", "); }
      while ((p = builder.nextData(l1))) { sql += (table + "." + p->getName() + " AS " + p->getSqlAlias(& table) + ", "); }
      while ((pRelation = builder.nextRelation(l2)))
      { params.setIndex(l2); if (pRelationX->exist(pRelation->getKey())) { pRelation->eagerSelect(params); } else { pRelation->lazySelect(params); } }
      sql = sql.left(sql.count() - 2); // Remove last ", "
      sql += " FROM " + table + ", ";
      while ((pRelation = builder.nextRelation(l3)))
      { params.setIndex(l3); if (pRelationX->exist(pRelation->getKey())) { pRelation->eagerFrom(params); } else { pRelation->lazyFrom(params); } }
      sql = sql.left(sql.count() - 2); // Remove last ", "
      while ((pRelation = builder.nextRelation(l4)))
      { params.setIndex(l4); if (pRelationX->exist(pRelation->getKey())) { pRelation->eagerJoin(params); } else { pRelation->lazyJoin(params); } }
   }

   static void resolveInput(qx::IxSqlRelationX * pRelationX, T & t, QSqlQuery & query, qx::IxSqlQueryBuilder & builder)
   { Q_UNUSED(pRelationX); Q_UNUSED(t); Q_UNUSED(query); Q_UNUSED(builder); }

   static void resolveOutput(qx::IxSqlRelationX * pRelationX, T & t, QSqlQuery & query, qx::IxSqlQueryBuilder & builder)
   {
      BOOST_STATIC_ASSERT(qx::trait::is_qx_registered<T>::value);
      if (! pRelationX || (pRelationX->count() <= 0)) { qAssert(false); QxSqlQueryHelper_FetchAll<T>::resolveOutput(t, query, builder); return; }
      long l1(0), l2(0);
      qx::IxDataMember * p = NULL;
      qx::IxDataMember * pId = builder.getDataId();
      qx::IxSqlRelation * pRelation = NULL;
      short iOffsetId = (pId ? 1 : 0);
      QVariant vId = query.value(0); QVariant vIdRelation;
      bool bComplex = builder.getCartesianProduct();
      bool bByPass = (bComplex && builder.existIdX(0, vId, vId));
      qx::QxSqlRelationParams params(0, (builder.getDataCount() + iOffsetId), NULL, (& builder), (& query), (& t));

      if (! bByPass)
      {
         if (pId) { pId->fromVariant((& t), vId); }
         while ((p = builder.nextData(l1))) { p->fromVariant((& t), query.value(l1 + iOffsetId - 1)); }
         if (bComplex) { builder.insertIdX(0, vId, vId, (& t)); }
      }

      while ((pRelation = builder.nextRelation(l2)))
      {
         params.setIndex(l2);
         bool bEager = pRelationX->exist(pRelation->getKey());
         if (bComplex) { vIdRelation = pRelation->getIdFromQuery(bEager, params); }
         bool bValidId = (bComplex && qx::trait::is_valid_primary_key(vIdRelation));
         bByPass = (bValidId && builder.existIdX(l2, vId, vIdRelation));
         if (bByPass) { pRelation->updateOffset(bEager, params); continue; }
         if (bEager) { pRelation->eagerFetch_ResolveOutput(params); }
         else { pRelation->lazyFetch_ResolveOutput(params); }
         if (bValidId) { builder.insertIdX(l2, vId, vIdRelation, (& t)); }
      }
   }

};

} // namespace detail
} // namespace dao
} // namespace qx
