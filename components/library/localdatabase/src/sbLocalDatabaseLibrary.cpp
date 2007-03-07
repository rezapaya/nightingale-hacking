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

#include "sbLocalDatabaseLibrary.h"
#include "sbLocalDatabaseMediaItem.h"
#include "sbLocalDatabaseCID.h"

#include <sbIMediaListFactory.h>
#include <sbIMediaItem.h>
#include <sbIMediaList.h>
#include <sbISQLBuilder.h>
#include <sbSQLBuilderCID.h>
#include <sbIDatabaseQuery.h>
#include <DatabaseQuery.h>
#include <sbIDatabaseResult.h>

#include <nsCOMPtr.h>
#include <nsComponentManagerUtils.h>
#include <nsServiceManagerUtils.h>

#include <stdio.h>

NS_IMPL_ISUPPORTS1(sbLocalDatabaseLibrary, sbILibrary)

sbLocalDatabaseLibrary::sbLocalDatabaseLibrary(const nsAString& aDatabaseGuid) :
  mDatabaseGuid(aDatabaseGuid)
{
}

sbLocalDatabaseLibrary::~sbLocalDatabaseLibrary()
{
}

nsresult
sbLocalDatabaseLibrary::Init()
{
  nsresult rv;
  // Maybe check to this that this db is valid, etc?
  // Check version and migrate if needed?

  mPropertyCache =
    do_CreateInstance(SB_LOCALDATABASE_PROPERTYCACHE_CONTRACTID, &rv);
  NS_ENSURE_SUCCESS(rv, rv);

  rv = mPropertyCache->SetDatabaseGUID(mDatabaseGuid);
  NS_ENSURE_SUCCESS(rv, rv);

  /*
   * Build some queries
   */
  nsCOMPtr<sbISQLSelectBuilder> builder =
    do_CreateInstance(SB_SQLBUILDER_SELECT_CONTRACTID, &rv);
  NS_ENSURE_SUCCESS(rv, rv);

  rv = builder->AddColumn(NS_LITERAL_STRING("_mlt"),
                          NS_LITERAL_STRING("factory_contractid"));
  NS_ENSURE_SUCCESS(rv, rv);

  rv = builder->SetBaseTableName(NS_LITERAL_STRING("media_items"));
  NS_ENSURE_SUCCESS(rv, rv);

  rv = builder->SetBaseTableAlias(NS_LITERAL_STRING("_mi"));
  NS_ENSURE_SUCCESS(rv, rv);

  rv = builder->AddJoin(sbISQLSelectBuilder::JOIN_LEFT,
                        NS_LITERAL_STRING("media_list_types"),
                        NS_LITERAL_STRING("_mlt"),
                        NS_LITERAL_STRING("media_list_type_id"),
                        NS_LITERAL_STRING("_mi"),
                        NS_LITERAL_STRING("media_list_type_id"));
  NS_ENSURE_SUCCESS(rv, rv);

  nsCOMPtr<sbISQLBuilderCriterion> criterion;
  rv = builder->CreateMatchCriterionParameter(NS_LITERAL_STRING("_mi"),
                                              NS_LITERAL_STRING("guid"),
                                              sbISQLSelectBuilder::MATCH_EQUALS,
                                              getter_AddRefs(criterion));
  NS_ENSURE_SUCCESS(rv, rv);

  rv = builder->AddCriterion(criterion);
  NS_ENSURE_SUCCESS(rv, rv);

  rv = builder->ToString(mGetContractIdForGuidQuery);
  NS_ENSURE_SUCCESS(rv, rv);

  rv = builder->Reset();
  NS_ENSURE_SUCCESS(rv, rv);

  rv = builder->AddColumn(EmptyString(), NS_LITERAL_STRING("media_item_id"));
  NS_ENSURE_SUCCESS(rv, rv);

  rv = builder->SetBaseTableName(NS_LITERAL_STRING("media_items"));
  NS_ENSURE_SUCCESS(rv, rv);

  rv = builder->CreateMatchCriterionParameter(EmptyString(),
                                              NS_LITERAL_STRING("guid"),
                                              sbISQLSelectBuilder::MATCH_EQUALS,
                                              getter_AddRefs(criterion));
  NS_ENSURE_SUCCESS(rv, rv);

  rv = builder->AddCriterion(criterion);
  NS_ENSURE_SUCCESS(rv, rv);

  rv = builder->ToString(mGetMediaItemIdForGuidQuery);
  NS_ENSURE_SUCCESS(rv, rv);

  return NS_OK;
}

nsresult
sbLocalDatabaseLibrary::GetContractIdForGuid(const nsAString& aGuid,
                                             nsACString &aContractId)
{
  nsresult rv;
  PRInt32 dbOk;

  /*
   * We could probably cache the result of this method
   */
  nsCOMPtr<sbIDatabaseQuery> query =
    do_CreateInstance(SONGBIRD_DATABASEQUERY_CONTRACTID, &rv);
  NS_ENSURE_SUCCESS(rv, rv);

  rv = query->SetDatabaseGUID(mDatabaseGuid);
  NS_ENSURE_SUCCESS(rv, rv);

  rv = query->SetAsyncQuery(PR_FALSE);
  NS_ENSURE_SUCCESS(rv, rv);

  rv = query->AddQuery(mGetContractIdForGuidQuery);
  NS_ENSURE_SUCCESS(rv, rv);

  rv = query->BindStringParameter(0, aGuid);
  NS_ENSURE_SUCCESS(rv, rv);

  rv = query->Execute(&dbOk);
  NS_ENSURE_SUCCESS(rv, rv);
  NS_ENSURE_SUCCESS(dbOk, dbOk);

  rv = query->WaitForCompletion(&dbOk);
  NS_ENSURE_SUCCESS(rv, rv);
  NS_ENSURE_SUCCESS(dbOk, dbOk);

  nsCOMPtr<sbIDatabaseResult> result;
  rv = query->GetResultObject(getter_AddRefs(result));
  NS_ENSURE_SUCCESS(rv, rv);

  PRUint32 rowCount;
  rv = result->GetRowCount(&rowCount);
  NS_ENSURE_SUCCESS(rv, rv);

  if (rowCount == 1) {
    nsAutoString contractId;
    rv = result->GetRowCell(0, 0, contractId);
    NS_ENSURE_SUCCESS(rv, rv);

    aContractId = NS_ConvertUTF16toUTF8(contractId);
  }
  else {
    return NS_ERROR_NOT_AVAILABLE;
  }

  return NS_OK;
}

nsresult
sbLocalDatabaseLibrary::GetMediaItemIdForGuid(const nsAString& aGuid,
                                              PRUint32* aMediaItemId)
{
  nsresult rv;
  PRInt32 dbOk;

  nsCOMPtr<sbIDatabaseQuery> query =
    do_CreateInstance(SONGBIRD_DATABASEQUERY_CONTRACTID, &rv);
  NS_ENSURE_SUCCESS(rv, rv);

  rv = query->SetDatabaseGUID(mDatabaseGuid);
  NS_ENSURE_SUCCESS(rv, rv);

  rv = query->SetAsyncQuery(PR_FALSE);
  NS_ENSURE_SUCCESS(rv, rv);

  rv = query->AddQuery(mGetMediaItemIdForGuidQuery);
  NS_ENSURE_SUCCESS(rv, rv);

  rv = query->BindStringParameter(0, aGuid);
  NS_ENSURE_SUCCESS(rv, rv);

  rv = query->Execute(&dbOk);
  NS_ENSURE_SUCCESS(rv, rv);
  NS_ENSURE_SUCCESS(dbOk, dbOk);

  rv = query->WaitForCompletion(&dbOk);
  NS_ENSURE_SUCCESS(rv, rv);
  NS_ENSURE_SUCCESS(dbOk, dbOk);

  nsCOMPtr<sbIDatabaseResult> result;
  rv = query->GetResultObject(getter_AddRefs(result));
  NS_ENSURE_SUCCESS(rv, rv);

  PRUint32 rowCount;
  rv = result->GetRowCount(&rowCount);
  NS_ENSURE_SUCCESS(rv, rv);

  if (rowCount == 1) {
    nsAutoString mediaItemIdStr;
    rv = result->GetRowCell(0, 0, mediaItemIdStr);
    NS_ENSURE_SUCCESS(rv, rv);

    *aMediaItemId = mediaItemIdStr.ToInteger(&rv);
    NS_ENSURE_SUCCESS(rv, rv);
  }
  else {
    return NS_ERROR_NOT_AVAILABLE;
  }

  return NS_OK;
}

void
sbLocalDatabaseLibrary::GetDatabaseGuid(nsAString& aDatabaseGuid)
{
  aDatabaseGuid = mDatabaseGuid;
}

NS_IMETHODIMP
sbLocalDatabaseLibrary::GetDevice(sbIDevice** aDevice)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
sbLocalDatabaseLibrary::GetSupportsForeignMediaItems(PRBool *aSupportsForeignMediaItems)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
sbLocalDatabaseLibrary::Resolve(nsIURI *aUri, nsIChannel **_retval)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
sbLocalDatabaseLibrary::CreateMediaItem(sbIMediaItem **_retval)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
sbLocalDatabaseLibrary::GetMediaItem(const nsAString& aGuid, 
                                     sbIMediaItem **_retval)
{
  nsresult rv;

  /*
   * Get the contract id of the guid.  If the contract id is blank, we know
   * it is just a media item and we can simply create it.  Otherwise get the
   * guid's factory and create an instace
   */
  nsCAutoString contractId;
  rv = GetContractIdForGuid(aGuid, contractId);
  NS_ENSURE_SUCCESS(rv, rv);

  nsCOMPtr<sbIMediaItem> item;
  if (contractId.IsEmpty()) {
    item = new sbLocalDatabaseMediaItem(this, aGuid);
    NS_ENSURE_TRUE(item, NS_ERROR_OUT_OF_MEMORY);
  }
  else {
    nsCOMPtr<sbIMediaListFactory> factory = do_GetService(contractId.get(), &rv);
    NS_ENSURE_SUCCESS(rv, rv);

    rv = factory->SetLibrary(this);
    NS_ENSURE_SUCCESS(rv, rv);

    nsCOMPtr<sbIMediaList> list;
    rv = factory->InstantiateMediaList(aGuid, getter_AddRefs(list));
    NS_ENSURE_SUCCESS(rv, rv);

    item = do_QueryInterface(list, &rv);
    NS_ENSURE_SUCCESS(rv, rv);
  }

  NS_ADDREF(*_retval = item);
  return NS_OK;
}

NS_IMETHODIMP
sbLocalDatabaseLibrary::GetMediaListFactories(nsISimpleEnumerator** aMediaListFactories)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
sbLocalDatabaseLibrary::GetMediaListFactory(const nsAString& aType,
                                            sbIMediaListFactory** _retval)
{
  nsresult rv;
  // TODO: This is totally fake, needs to look up name in the list of
  // registered factories, etc.
  const char* contractid;

  if (aType.EqualsLiteral("view")) {
    contractid = SB_LOCALDATABASE_VIEWMEDIALISTFACTORY_CONTRACTID;
  }
  else {
    if (aType.EqualsLiteral("simple")) {
      contractid = SB_LOCALDATABASE_SIMPLEMEDIALISTFACTORY_CONTRACTID;
    }
    else {
      return NS_ERROR_INVALID_ARG;
    }
  }

  nsCOMPtr<sbIMediaListFactory> factory = do_GetService(contractid, &rv);
  NS_ENSURE_SUCCESS(rv, rv);
  rv = factory->SetLibrary(this);
  NS_ENSURE_SUCCESS(rv, rv);

  NS_ADDREF(*_retval = factory);
  return NS_OK;
}

NS_IMETHODIMP
sbLocalDatabaseLibrary::AddMediaListFactory(const nsAString& aType,
                                            const nsAString& aContractId)
{
  // insert new thing into db
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
sbLocalDatabaseLibrary::ImportMediaList(sbIMediaList* aMediaList,
                                        sbIMediaList** _retval)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
sbLocalDatabaseLibrary::BeginBatch(PRBool aIsAsync)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
sbLocalDatabaseLibrary::CancelBatch()
{
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
sbLocalDatabaseLibrary::EndBatch()
{
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
sbLocalDatabaseLibrary::TidyUp()
{
  return NS_ERROR_NOT_IMPLEMENTED;
}

// sbILibraryResource
// XXX - Can this be agg'd?
NS_IMETHODIMP
sbLocalDatabaseLibrary::GetUri(nsIURI** aUri)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
sbLocalDatabaseLibrary::GetCreated(PRInt32* aCreated)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
sbLocalDatabaseLibrary::GetUpdated(PRInt32* aUpdated)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
sbLocalDatabaseLibrary::GetProperty(const nsAString& aName,
                                    nsAString& _retval)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
sbLocalDatabaseLibrary::SetProperty(const nsAString& aName,
                                    const nsAString& aValue)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}

