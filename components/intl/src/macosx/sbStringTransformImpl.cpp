/*
//
// BEGIN SONGBIRD GPL
//
// This file is part of the Songbird web player.
//
// Copyright(c) 2005-2008 POTI, Inc.
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

#include "sbStringTransformImpl.h"

#include <nsMemory.h>
#include <nsStringGlue.h>

#include <prmem.h>

#include <CoreFoundation/CoreFoundation.h>

sbStringTransformImpl::sbStringTransformImpl()
{
}

sbStringTransformImpl::~sbStringTransformImpl()
{
}

nsresult 
sbStringTransformImpl::Init() {
  return NS_OK;
}

NS_IMETHODIMP 
sbStringTransformImpl::NormalizeString(const nsAString & aCharset, 
                                       PRUint32 aTransformFlags, 
                                       const nsAString & aInput, 
                                       nsAString & _retval)
{
	CFMutableStringRef str = CFStringCreateMutable(kCFAllocatorDefault, 0);
	NS_ENSURE_TRUE(str, NS_ERROR_OUT_OF_MEMORY);
	
	CFStringAppendCharacters(str, aInput.BeginReading(), aInput.Length());
	CFShow(str);
		
  if(aTransformFlags & sbIStringTransform::TRANSFORM_LOWERCASE) {
		CFStringLowercase(str, NULL);
  }
	
  if(aTransformFlags & sbIStringTransform::TRANSFORM_UPPERCASE) {
		CFStringUppercase(str, NULL);
  }
	
  if(aTransformFlags & sbIStringTransform::TRANSFORM_IGNORE_NONSPACE) {
		CFStringTransform(str, NULL, kCFStringTransformStripCombiningMarks, false);
  }
	
  if(aTransformFlags & sbIStringTransform::TRANSFORM_IGNORE_SYMBOLS) {
    //Not supported at this time. Does nothing.
  }
	
	CFShow(str);
	const UniChar *transformedStr = CFStringGetCharactersPtr(str);
	NS_ENSURE_TRUE(transformedStr, NS_ERROR_UNEXPECTED);
	
	_retval.Assign(transformedStr);
  
	return NS_OK;
}

NS_IMETHODIMP 
sbStringTransformImpl::ConvertToCharset(const nsAString & aDestCharset, 
                                        const nsAString & aInput, 
                                        nsAString & _retval)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP 
sbStringTransformImpl::GuessCharset(const nsAString & aInput, 
                                    nsAString & _retval)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}
