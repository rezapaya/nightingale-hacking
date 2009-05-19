/*
//
// BEGIN SONGBIRD GPL
//
// This file is part of the Songbird web player.
//
// Copyright(c) 2005-2009 POTI, Inc.
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

#import "SBNSWorkspace+Utils.h"


@implementation NSWorkspace (SongbirdProcessUtils)

+ (BOOL)isProcessAlreadyRunning:(NSString *)aBundleIdentifier
{
  // Need to find out information about the current process ID.
  ProcessSerialNumber runningProcess;
  GetCurrentProcess(&runningProcess);
    UInt32 flags = kProcessDictionaryIncludeAllInformationMask;

  NSDictionary *processDict = 
    (NSDictionary *)ProcessInformationCopyDictionary(&runningProcess, flags);

  NSNumber *runningProcessPID = [processDict objectForKey:@"pid"];

  // Since the regular API's on this interface doesn't look at background
  // processes that are being run by the user, we need to use the HIServices
  // API for this (it's bundled under ApplicationServices since 10.2).
  BOOL found = NO;
  ProcessSerialNumber curProcessSerialNum = { 0, kNoProcess };

  // Loop through all the users processes to see if a match is found to the
  // passed in bundle identifer.
  while (GetNextProcess(&curProcessSerialNum) == noErr) {
    NSDictionary *curProcessDict = 
      (NSDictionary *)ProcessInformationCopyDictionary(&curProcessSerialNum,
                                                       flags);
    // Don't attempt to get any keys out of a nil dictionary. This could 
    // happen if the user gets a process reference for something they can't
    // read.
    if (!curProcessDict) {
      continue;
    }

    NSString *curProcessBundleId = 
      [curProcessDict objectForKey:(NSString *)kCFBundleIdentifierKey];
    if ([curProcessBundleId isEqualToString:aBundleIdentifier]) {
      // Since we have a matching bundle ID - make sure this isn't the current
      // process by matching up the PIDs.
      NSNumber *curProcessPID = [curProcessDict objectForKey:@"pid"];
      if (![runningProcessPID isEqualToNumber:curProcessPID]) {
        found = YES;
        break;
      }
    }
  }

  return found;
}

@end

