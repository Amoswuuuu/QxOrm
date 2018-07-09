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

#ifndef _IX_SQL_QUERY_BUILDER_H_
#define _IX_SQL_QUERY_BUILDER_H_

#ifdef _MSC_VER
#pragma once
#endif

#include "../../include/QxDataMember/IxDataMemberX.h"

#include "../../include/QxDao/IxSqlRelation.h"

namespace qx {

class QX_DLL_EXPORT IxSqlQueryBuilder
{

public:

   typedef QPair<QString, QString> type_id;
   typedef QHash<type_id, void *> type_ptr_by_id;
   typedef boost::shared_ptr<type_ptr_by_id> type_ptr_by_id_ptr;
   typedef QList<type_ptr_by_id_ptr> type_lst_ptr_by_id;
   typedef boost::shared_ptr<type_lst_ptr_by_id> type_lst_ptr_by_id_ptr;

protected:

   QxCollection<QString, IxDataMember *> * m_lstDataMemberPtr;    // Collection of 'IxDataMember' to build sql query
   IxSqlRelationX * m_lstSqlRelationPtr;                          // Collection of 'IxSqlRelation' to build sql query
   IxDataMember * m_pDataMemberId;                                // Data member id for sql query
   QString m_sSqlQuery;                                           // Current sql query
   QString m_sTableName;                                          // Sql table name of current object
   QString m_sHashRelation;                                       // Optimization : hash to retrieve sql query with relation
   bool m_bCartesianProduct;                                      // Recordset can return cartesian product => same id in multiple records
   type_lst_ptr_by_id_ptr m_pIdX;                                 // Collection of id (and pointer associated) to avoid multiple fetch on same id (cartesian product)

public:

   IxSqlQueryBuilder() : m_lstDataMemberPtr(NULL), m_lstSqlRelationPtr(NULL), m_pDataMemberId(NULL), m_bCartesianProduct(false) { ; }
   IxSqlQueryBuilder(const QString & sql) : m_lstDataMemberPtr(NULL), m_lstSqlRelationPtr(NULL), m_pDataMemberId(NULL), m_sSqlQuery(sql), m_bCartesianProduct(false) { ; }
   virtual ~IxSqlQueryBuilder() = 0;

   inline QxCollection<QString, IxDataMember *> * getLstDataMember() const    { return m_lstDataMemberPtr; }
   inline IxSqlRelationX * getLstRelation() const                             { return m_lstSqlRelationPtr; }

   inline void setSqlQuery(const QString & sql)          { m_sSqlQuery = sql; }
   inline void setHashRelation(const QString & s)        { m_sHashRelation = s; }
   inline void setCartesianProduct(bool b)               { m_bCartesianProduct = b; }
   inline QString getSqlQuery() const                    { return m_sSqlQuery; }
   inline QString getHashRelation() const                { return m_sHashRelation; }
   inline QString table() const                          { return m_sTableName; }
   inline bool getCartesianProduct() const               { return m_bCartesianProduct; }
   inline long getDataCount() const                      { return (m_lstDataMemberPtr ? m_lstDataMemberPtr->count() : 0); }
   inline long getRelationCount() const                  { return (m_lstSqlRelationPtr ? m_lstSqlRelationPtr->count() : 0); }
   inline IxDataMember * getDataId() const               { return m_pDataMemberId; }
   inline IxDataMember * nextData(long & l) const        { if ((! m_lstDataMemberPtr) || (l < 0) || (l >= m_lstDataMemberPtr->count())) { return NULL; }; ++l; return m_lstDataMemberPtr->getByIndex(l - 1); }
   inline IxSqlRelation * nextRelation(long & l) const   { if ((! m_lstSqlRelationPtr) || (l < 0) || (l >= m_lstSqlRelationPtr->count())) { return NULL; }; ++l; return m_lstSqlRelationPtr->getByIndex(l - 1); }

   void displaySqlQuery(int time_ms = -1) const;
   bool insertIdX(long lIndex, const QVariant & idOwner, const QVariant & idData, void * ptr);
   void * existIdX(long lIndex, const QVariant & idOwner, const QVariant & idData);

   virtual void init() = 0;
   virtual IxDataMemberX * getDataMemberX() const = 0;

   virtual IxSqlQueryBuilder & count() = 0;
   virtual IxSqlQueryBuilder & exist() = 0;
   virtual IxSqlQueryBuilder & fetchAll() = 0;
   virtual IxSqlQueryBuilder & fetchById() = 0;
   virtual IxSqlQueryBuilder & insert() = 0;
   virtual IxSqlQueryBuilder & update() = 0;
   virtual IxSqlQueryBuilder & deleteAll() = 0;
   virtual IxSqlQueryBuilder & deleteById() = 0;
   virtual IxSqlQueryBuilder & createTable() = 0;

   virtual IxSqlQueryBuilder & fetchAll_WithRelation(IxSqlRelationX * pRelationX) = 0;
   virtual IxSqlQueryBuilder & fetchById_WithRelation(IxSqlRelationX * pRelationX) = 0;

private:

   void initIdX();

};

typedef boost::shared_ptr<IxSqlQueryBuilder> IxSqlQueryBuilder_ptr;

} // namespace qx

#endif // _IX_SQL_QUERY_BUILDER_H_
