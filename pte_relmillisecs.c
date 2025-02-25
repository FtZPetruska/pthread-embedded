/*
 * pte_relmillisecs.c
 *
 * Description:
 * This translation unit implements miscellaneous thread functions.
 *
 * --------------------------------------------------------------------------
 *
 *      Pthreads-embedded (PTE) - POSIX Threads Library for embedded systems
 *      Copyright(C) 2008 Jason Schmidlapp
 *
 *      Contact Email: jschmidlapp@users.sourceforge.net
 *
 *
 *      Based upon Pthreads-win32 - POSIX Threads Library for Win32
 *      Copyright(C) 1998 John E. Bossom
 *      Copyright(C) 1999,2005 Pthreads-win32 contributors
 *
 *      Contact Email: rpj@callisto.canberra.edu.au
 *
 *      The original list of contributors to the Pthreads-win32 project
 *      is contained in the file CONTRIBUTORS.ptw32 included with the
 *      source code distribution. The list can also be seen at the
 *      following World Wide Web location:
 *      http://sources.redhat.com/pthreads-win32/contributors.html
 *
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
 *
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 *
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#include <pte_osal.h>

#include "pthread.h"
#include "implement.h"

typedef long long int64_t;

unsigned int
pte_relmillisecs (const struct timespec * abstime)
{
  const long long NANOSEC_PER_MILLISEC = 1000000;
  const long long MILLISEC_PER_SEC = 1000;
  unsigned int milliseconds;
  long long tmpAbsMilliseconds;
  long long tmpCurrMilliseconds;
  struct timeb currSysTime;

  /*
   * Calculate timeout as milliseconds from current system time.
   */

  /*
   * subtract current system time from abstime in a way that checks
   * that abstime is never in the past, or is never equivalent to the
   * defined INFINITE value (0xFFFFFFFF).
   *
   * Assume all integers are unsigned, i.e. cannot test if less than 0.
   */
  tmpAbsMilliseconds =  (int64_t)abstime->tv_sec * MILLISEC_PER_SEC;
  tmpAbsMilliseconds += ((int64_t)abstime->tv_nsec + (NANOSEC_PER_MILLISEC/2)) / NANOSEC_PER_MILLISEC;

  /* get current system time */

  _ftime(&currSysTime);

  tmpCurrMilliseconds = (int64_t) currSysTime.time * MILLISEC_PER_SEC;
  tmpCurrMilliseconds += (int64_t) currSysTime.millitm;

  if (tmpAbsMilliseconds > tmpCurrMilliseconds)
    {
      milliseconds = (unsigned int) (tmpAbsMilliseconds - tmpCurrMilliseconds);
      if (milliseconds == 0xFFFFFFFF)
        {
          /* Timeouts must be finite */
          milliseconds--;
        }
    }
  else
    {
      /* The abstime given is in the past */
      milliseconds = 0;
    }

  return milliseconds;
}
