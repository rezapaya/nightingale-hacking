/*
//
// BEGIN SONGBIRD GPL
//
// This file is part of the Songbird web player.
//
// Copyright(c) 2005-2007 POTI, Inc.
// http://songbirdnest.com
//
// This file may be licensed under the terms of of the
// GNU General Public License Version 2 (the "GPL").
//
// Software distributed under the License is distributed
// on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either
// express or implied. See the GPL for the specific language
// governing rights and limitations.
//
// You should have received a copy of the GPL along with this
// program. If not, go to http://www.gnu.org/licenses/gpl.html
// or write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
// END SONGBIRD GPL
//
*/

#ifndef __SBPROGRESSPROPERTYINFO_H__
#define __SBPROGRESSPROPERTYINFO_H__

#include "sbTextPropertyInfo.h"
#include <sbIPropertyManager.h>

#include <nsStringGlue.h>
#include <prlock.h>

#define NS_FORWARD_SBIPROPERTYINFO_OVERRIDE(_to) \
  NS_IMETHOD GetOPERATOR_EQUALS(nsAString & aOPERATOR_EQUALS) { return _to GetOPERATOR_EQUALS(aOPERATOR_EQUALS); } \
  NS_IMETHOD GetOPERATOR_NOTEQUALS(nsAString & aOPERATOR_NOTEQUALS) { return _to GetOPERATOR_NOTEQUALS(aOPERATOR_NOTEQUALS); } \
  NS_IMETHOD GetOPERATOR_GREATER(nsAString & aOPERATOR_GREATER) { return _to GetOPERATOR_GREATER(aOPERATOR_GREATER); } \
  NS_IMETHOD GetOPERATOR_GREATEREQUAL(nsAString & aOPERATOR_GREATEREQUAL) { return _to GetOPERATOR_GREATEREQUAL(aOPERATOR_GREATEREQUAL); } \
  NS_IMETHOD GetOPERATOR_LESS(nsAString & aOPERATOR_LESS) { return _to GetOPERATOR_LESS(aOPERATOR_LESS); } \
  NS_IMETHOD GetOPERATOR_LESSEQUAL(nsAString & aOPERATOR_LESSEQUAL) { return _to GetOPERATOR_LESSEQUAL(aOPERATOR_LESSEQUAL); } \
  NS_IMETHOD GetOPERATOR_CONTAINS(nsAString & aOPERATOR_CONTAINS) { return _to GetOPERATOR_CONTAINS(aOPERATOR_CONTAINS); } \
  NS_IMETHOD GetOPERATOR_BEGINSWITH(nsAString & aOPERATOR_BEGINSWITH) { return _to GetOPERATOR_BEGINSWITH(aOPERATOR_BEGINSWITH); } \
  NS_IMETHOD GetOPERATOR_ENDSWITH(nsAString & aOPERATOR_ENDSWITH) { return _to GetOPERATOR_ENDSWITH(aOPERATOR_ENDSWITH); } \
  NS_IMETHOD GetOPERATOR_BETWEEN(nsAString & aOPERATOR_BETWEEN) { return _to GetOPERATOR_BETWEEN(aOPERATOR_BETWEEN); } \
  NS_IMETHOD GetNullSort(PRUint32 *aNullSort) { return _to GetNullSort(aNullSort); } \
  NS_IMETHOD SetNullSort(PRUint32 aNullSort) { return _to SetNullSort(aNullSort); } \
  NS_IMETHOD GetSortProfile(sbIPropertyArray * *aSortProfile) { return _to GetSortProfile(aSortProfile); } \
  NS_IMETHOD SetSortProfile(sbIPropertyArray * aSortProfile) { return _to SetSortProfile(aSortProfile); } \
  NS_IMETHOD GetName(nsAString & aName) { return _to GetName(aName); } \
  NS_IMETHOD SetName(const nsAString & aName) { return _to SetName(aName); } \
  NS_IMETHOD GetType(nsAString & aType) { return _to GetType(aType); } \
  NS_IMETHOD SetType(const nsAString & aType) { return _to SetType(aType); } \
  NS_IMETHOD GetDisplayName(nsAString & aDisplayName) { return _to GetDisplayName(aDisplayName); } \
  NS_IMETHOD SetDisplayName(const nsAString & aDisplayName) { return _to SetDisplayName(aDisplayName); } \
  NS_IMETHOD GetUserViewable(PRBool *aUserViewable) { return _to GetUserViewable(aUserViewable); } \
  NS_IMETHOD SetUserViewable(PRBool aUserViewable) { return _to SetUserViewable(aUserViewable); } \
  NS_IMETHOD GetUserEditable(PRBool *aUserEditable) { return _to GetUserEditable(aUserEditable); } \
  NS_IMETHOD SetUserEditable(PRBool aUserEditable) { return _to SetUserEditable(aUserEditable); } \
  NS_IMETHOD GetDisplayUsingSimpleType(nsAString & aDisplayUsingSimpleType) { return _to GetDisplayUsingSimpleType(aDisplayUsingSimpleType); } \
  NS_IMETHOD SetDisplayUsingSimpleType(const nsAString & aDisplayUsingSimpleType) { return _to SetDisplayUsingSimpleType(aDisplayUsingSimpleType); } \
  NS_IMETHOD GetDisplayUsingXBLWidget(nsIURI * *aDisplayUsingXBLWidget) { return _to GetDisplayUsingXBLWidget(aDisplayUsingXBLWidget); } \
  NS_IMETHOD SetDisplayUsingXBLWidget(nsIURI * aDisplayUsingXBLWidget) { return _to SetDisplayUsingXBLWidget(aDisplayUsingXBLWidget); } \
  NS_IMETHOD GetUnits(nsAString & aUnits) { return _to GetUnits(aUnits); } \
  NS_IMETHOD SetUnits(const nsAString & aUnits) { return _to SetUnits(aUnits); } \
  NS_IMETHOD GetOperators(nsISimpleEnumerator * *aOperators) { return _to GetOperators(aOperators); } \
  NS_IMETHOD SetOperators(nsISimpleEnumerator * aOperators) { return _to SetOperators(aOperators); } \
  NS_IMETHOD GetOperator(const nsAString & aOperator, sbIPropertyOperator **_retval) { return _to GetOperator(aOperator, _retval); } \
  NS_IMETHOD Validate(const nsAString & aValue, PRBool *_retval) { return _to Validate(aValue, _retval); } \
  NS_IMETHOD Sanitize(const nsAString & aValue, nsAString & _retval) { return _to Sanitize(aValue, _retval); } \
  NS_IMETHOD MakeSortable(const nsAString & aValue, nsAString & _retval) { return _to MakeSortable(aValue, _retval); }

class sbProgressPropertyInfo : public sbTextPropertyInfo,
                               public sbIProgressPropertyInfo
{
public:
  NS_DECL_ISUPPORTS_INHERITED
  NS_FORWARD_SBIPROPERTYINFO_OVERRIDE(sbTextPropertyInfo::);
  NS_FORWARD_SBITEXTPROPERTYINFO(sbTextPropertyInfo::);
  NS_FORWARD_SBIREMOTEPROPERTYINFO(sbTextPropertyInfo::);
  NS_DECL_SBIPROGRESSPROPERTYINFO

  sbProgressPropertyInfo();

  ~sbProgressPropertyInfo();

  NS_IMETHOD GetDisplayPropertiesForValue(const nsAString& aValue,
                                          nsAString& _retval);

  NS_IMETHOD Format(const nsAString& aValue,
                    nsAString& _retval);

private:
  PRLock* mLock;

  nsString mModePropertyName;
};

#endif /* __SBPROGRESSPROPERTYINFO_H__ */
