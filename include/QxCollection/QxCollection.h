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

#ifndef _QX_COLLECTION_H_
#define _QX_COLLECTION_H_

#ifdef _MSC_VER
#pragma once
#endif

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4996)
#pragma warning(disable:4503)
#endif // _MSC_VER

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/type_traits/is_pointer.hpp>

#include "../../include/QxCollection/IxCollection.h"
#include "../../include/QxCollection/QxForeach.h"

#include "../../include/QxCommon/QxHashValue.h"

#include "../../include/QxTraits/get_class_name.h"
#include "../../include/QxTraits/is_smart_ptr.h"

namespace qx {

template <typename Key, typename Value>
class QxCollection : public IxCollection
{

public:

   typedef typename std::pair<Key, Value> qx_coll_pair_key_value;

protected:

   typedef boost::multi_index::member<qx_coll_pair_key_value, Key, & qx_coll_pair_key_value::first> qx_coll_hash_key;
   typedef boost::multi_index::random_access<> qx_coll_first_index;
   typedef boost::multi_index::hashed_unique<qx_coll_hash_key> qx_coll_second_index;
   typedef boost::multi_index::indexed_by<qx_coll_first_index, qx_coll_second_index> qx_coll_indexed_by;
   typedef boost::multi_index::multi_index_container<qx_coll_pair_key_value, qx_coll_indexed_by> qx_coll_type_container;

   typedef typename qx_coll_type_container::template nth_index<0>::type qx_coll_type_rand;
   typedef typename qx_coll_type_container::template nth_index<1>::type qx_coll_type_hash;

public:

   typedef typename qx_coll_type_rand::iterator iterator;
   typedef typename qx_coll_type_rand::const_iterator const_iterator;
   typedef typename qx_coll_type_rand::reverse_iterator reverse_iterator;
   typedef typename qx_coll_type_rand::const_reverse_iterator const_reverse_iterator;

   typedef const Key & const_reference_key;
   typedef const Value & const_reference_value;

protected:

   qx_coll_type_container m_qxCollection; // Collection with random access (like std::vector) and fast lookup by key (like boost::unordered_map)

public:

   QxCollection();
   QxCollection(const QxCollection<Key, Value> & other);
   virtual ~QxCollection();

   QxCollection<Key, Value> & operator= (const QxCollection<Key, Value> & other);
   bool operator== (const QxCollection<Key, Value> & other) const;
   bool operator!= (const QxCollection<Key, Value> & other) const;

   inline iterator begin();
   inline iterator end();
   inline const_iterator begin() const;
   inline const_iterator end() const;

   inline reverse_iterator rbegin();
   inline reverse_iterator rend();
   inline const_reverse_iterator rbegin() const;
   inline const_reverse_iterator rend() const;

   inline long capacity() const;
   inline void reserve(long size);
   inline void reverse();
   inline void clear();
   inline long count() const;
   inline long size() const;
   inline bool contains(const Key & key) const;
   inline bool exist(const Key & key) const;
   inline bool empty() const;

   inline bool push_back(const Key & key, const Value & value);
   inline bool push_front(const Key & key, const Value & value);
   inline bool insert(const Key & key, const Value & value);
   inline bool insert(long index, const Key & key, const Value & value);
   inline bool insert(const QxCollection<Key, Value> & other);
   inline bool insert(long index, const QxCollection<Key, Value> & other);
   inline bool replace(long index, const Key & key, const Value & value);
   inline bool swap(long index1, long index2);
   inline bool move(long indexFrom, long indexTo);

   inline bool removeByKey(const Key & key);
   inline bool removeByIndex(long index);
   inline bool removeByIndex(long first, long last);
   inline bool removeFirst();
   inline bool removeLast();

   inline const_reference_value getByKey(const Key & key) const;
   inline const_reference_value getByIndex(long index) const;
   inline const_reference_value getFirst() const;
   inline const_reference_value getLast() const;
   inline const_reference_key getKeyByIndex(long index) const;

   inline void sortByKey(bool bAscending = true);
   inline void sortByValue(bool bAscending = true);

   template <typename Compare>
   inline void sort(Compare comp) { m_qxCollection.template get<0>().sort(comp); }

private:

   void cloneCollection(QxCollection<Key, Value> * pClone, const QxCollection<Key, Value> & pRef);
   bool isSameCollection(const QxCollection<Key, Value> * p1, const QxCollection<Key, Value> & p2) const;

   template <bool bIsPointer /* = false */, int dummy>
   struct compareKeyValue
   {
      static inline bool compareByKeyAscending(const qx_coll_pair_key_value & v1, const qx_coll_pair_key_value & v2)     { return (v1.first < v2.first); }
      static inline bool compareByKeyDescending(const qx_coll_pair_key_value & v1, const qx_coll_pair_key_value & v2)    { return (v1.first > v2.first); }
      static inline bool compareByValueAscending(const qx_coll_pair_key_value & v1, const qx_coll_pair_key_value & v2)   { return (v1.second < v2.second); }
      static inline bool compareByValueDescending(const qx_coll_pair_key_value & v1, const qx_coll_pair_key_value & v2)  { return (v1.second > v2.second); }
   };

   template <int dummy>
   struct compareKeyValue<true, dummy>
   {
      static inline bool compareByKeyAscending(const qx_coll_pair_key_value & v1, const qx_coll_pair_key_value & v2)     { return ((v1.first && v2.first) ? ((* v1.first) < (* v2.first)) : false); }
      static inline bool compareByKeyDescending(const qx_coll_pair_key_value & v1, const qx_coll_pair_key_value & v2)    { return ((v1.first && v2.first) ? ((* v1.first) > (* v2.first)) : true); }
      static inline bool compareByValueAscending(const qx_coll_pair_key_value & v1, const qx_coll_pair_key_value & v2)   { return ((v1.second && v2.second) ? ((* v1.second) < (* v2.second)) : false); }
      static inline bool compareByValueDescending(const qx_coll_pair_key_value & v1, const qx_coll_pair_key_value & v2)  { return ((v1.second && v2.second) ? ((* v1.second) > (* v2.second)) : true); }
   };

};

} // namespace qx

#include "../../inl/QxCollection/QxCollection.inl"

QX_REGISTER_CLASS_NAME_TEMPLATE_2(qx::QxCollection)

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER

#endif // _QX_COLLECTION_H_
