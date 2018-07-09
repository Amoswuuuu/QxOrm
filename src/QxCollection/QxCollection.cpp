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

#include "../../include/QxPrecompiled.h"

#include "../../include/QxCollection/QxCollection.h"
#include "../../include/QxCollection/QxCollectionIterator.h"

#include "../../include/QxMemLeak/mem_leak.h"

namespace qx {

IxCollection::~IxCollection() { ; }

namespace unit_test {

void qx_collection_unit_test()
{
   QxCollection<QString, QString *> obj;
   QxCollectionIterator<QString, QString *> itr(obj);

   obj.sortByKey();
   obj.sortByValue();

   typedef boost::tuple<QString, std::string, int> new_type;
   qx::QxCollection< new_type, boost::shared_ptr<QWidget> > objColTuple;
   objColTuple.begin();
   objColTuple.reserve(10);
   objColTuple.clear();
   objColTuple.count();
   objColTuple.empty();

   new_type t1 = boost::make_tuple("","",1);
   new_type t2 = boost::make_tuple("","",1);
}

} // namespace unit_test
} // namespace qx
