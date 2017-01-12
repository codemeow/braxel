/* «Braxel» - Braille-based console drawer
 *
 * Copyrigth (c) Alexey Shishkin 2016-2017
 *
 *  This file is part of Project Braxel.
 *
 *  Project Braxel is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Project Braxel is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with Project Braxel. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef BRXXERRORS_H
#define BRXXERRORS_H

#include "../definitions/brxexport.h"

EXPORT_FROM
#define BRX_X_ERRORS_LIST \
    /*        Name                Code              Description              */ \
    /* No error                                                              */ \
    BRX(SUCCESS             , (0x00),        "Successfull                    ") \
    /* System general errors                                                 */ \
    BRX(SYSTEM_GENERALERROR , (0x01),        "Generic error                  ") \
    BRX(SYSTEM_MISUSEOFBLTNS, (0x02),        "Misuse of shell built-ins      ") \
    /* Memory errors                                                         */ \
    BRX(USER_BADALLOC       , (0x10 + 0x01), "Memory allocation failed       ") \
    /* Pointer errors                                                        */ \
    BRX(USER_BADPOINTER     , (0x20 + 0x01), "Bad pointer provided           ") \
    /* Geometry errors                                                       */ \
    BRX(USER_BADWIDTH       , (0x30 + 0x01), "Bad width provided             ") \
    BRX(USER_BADHEIGHT      , (0x30 + 0x02), "Bad height provided            ") \
    BRX(USER_BADX           , (0x30 + 0x03), "Bad x provided                 ") \
    BRX(USER_BADY           , (0x30 + 0x04), "Bad y provided                 ") \
    /* Filesystem errors                                                     */ \
    BRX(USER_BADFILE        , (0x40 + 0x01), "Bad file provided              ") \
    BRX(USER_BADPNGFILE     , (0x40 + 0x02), "Bad PNG file provided          ") \
    /* System errors                                                         */ \
    BRX(SYSTEM_CANNOTEXECUTE, (0x7E),        "Command invoked cannot execute ") \
    BRX(SYSTEM_NOTFOUND     , (0x7F),        "Command not found              ") \
    BRX(SYSTEM_INVALIDEXIT  , (0x80),        "Invalid argument to exit       ") \
    /* System signals                                                        */ \
    BRX(SYSTEM_SIGHUP       , (0x80 + 0x01), "Hangup                         ") \
    BRX(SYSTEM_SIGINT       , (0x80 + 0x02), "Interrupt                      ") \
    BRX(SYSTEM_SIGQUIT      , (0x80 + 0x03), "Quit                           ") \
    BRX(SYSTEM_SIGILL       , (0x80 + 0x04), "Illegal Instruction            ") \
    BRX(SYSTEM_SIGTRAP      , (0x80 + 0x05), "Trace/Breakpoint Trap          ") \
    BRX(SYSTEM_SIGABRT      , (0x80 + 0x06), "Abort                          ") \
    BRX(SYSTEM_SIGEMT       , (0x80 + 0x07), "Emulation Trap                 ") \
    BRX(SYSTEM_SIGFPE       , (0x80 + 0x08), "Arithmetic Exception           ") \
    BRX(SYSTEM_SIGKILL      , (0x80 + 0x09), "Killed                         ") \
    BRX(SYSTEM_SIGBUS       , (0x80 + 0x0A), "Bus Error                      ") \
    BRX(SYSTEM_SIGSEGV      , (0x80 + 0x0B), "Segmentation Fault             ") \
    BRX(SYSTEM_SIGSYS       , (0x80 + 0x0C), "Bad System Call                ") \
    BRX(SYSTEM_SIGPIPE      , (0x80 + 0x0D), "Broken Pipe                    ") \
    BRX(SYSTEM_SIGALRM      , (0x80 + 0x0E), "Alarm Clock                    ") \
    BRX(SYSTEM_SIGTERM      , (0x80 + 0x0F), "Terminated                     ") \
    BRX(SYSTEM_SIGUSR1      , (0x80 + 0x10), "User Signal 1                  ") \
    BRX(SYSTEM_SIGUSR2      , (0x80 + 0x11), "User Signal 2                  ") \
    BRX(SYSTEM_SIGCHLD      , (0x80 + 0x12), "Child Status                   ") \
    BRX(SYSTEM_SIGPWR       , (0x80 + 0x13), "Power Fail/Restart             ") \
    BRX(SYSTEM_SIGWINCH     , (0x80 + 0x14), "Window Size Change             ") \
    BRX(SYSTEM_SIGURG       , (0x80 + 0x15), "Urgent Socket Condition        ") \
    BRX(SYSTEM_SIGPOLL      , (0x80 + 0x16), "Socket I/O Possible            ") \
    BRX(SYSTEM_SIGSTOP      , (0x80 + 0x17), "Stopped (signal)               ") \
    BRX(SYSTEM_SIGTSTP      , (0x80 + 0x18), "Stopped (user)                 ") \
    BRX(SYSTEM_SIGCONT      , (0x80 + 0x19), "Continued                      ") \
    BRX(SYSTEM_SIGTTIN      , (0x80 + 0x1A), "Stopped (tty input)            ") \
    BRX(SYSTEM_SIGTTOU      , (0x80 + 0x1B), "Stopped (tty output)           ") \
    BRX(SYSTEM_SIGVTALRM    , (0x80 + 0x1C), "Virtual Timer Expired          ") \
    BRX(SYSTEM_SIGPROF      , (0x80 + 0x1D), "Profiling Timer Expired        ") \
    BRX(SYSTEM_SIGXCPU      , (0x80 + 0x1E), "CPU time limit exceeded        ") \
    BRX(SYSTEM_SIGXFSZ      , (0x80 + 0x1F), "File size limit exceeded       ") \
    BRX(SYSTEM_SIGWAITING   , (0x80 + 0x20), "All LWPs blocked               ") \
    BRX(SYSTEM_SIGLWP       , (0x80 + 0x21), "VI Interrupt for T. Library    ") \
    BRX(SYSTEM_SIGAIO       , (0x80 + 0x22), "Asynchronous I/O               ") \
                                                                                \
    BRX(SYSTEM_OUTOFRANGE   , (0xFF),        "Exit status unknown            ") \
    /* Maximum is 255, as this index will be sent to exit() function         */
EXPORT_TO

#endif /* BRXXERRORS_H */

