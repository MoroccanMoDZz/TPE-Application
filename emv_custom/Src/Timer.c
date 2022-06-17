//****************************************************************************
//       INGENICO                                INGEDEV 7
//============================================================================
//       FILE  TIMER.C                            (Copyright INGENICO 2012)
//============================================================================
//  Created :       13-July-2012     Kassovic
//  Last modified : 13-July-2012     Kassovic
//  Module : TRAINING
//
//  Purpose :
//                          *** Timer management ***
//  How to start, get and stop a timer.
//
//  List of routines in file :
//      TimerStart : Start a timer number.
//      TimerGet : Return the state of a timer number.
//      TimerStop : Stop a timer number.
//
//  File history :
//  071312-BK : File created
//
//****************************************************************************

//****************************************************************************
//      INCLUDES
//****************************************************************************
#include "sdk.h" // IngeDev automatic refactoring - 2014/04/29 14:29:58 - Replace '#include "SDK30.H"' to '#include "sdk.h"'

#include "Training.h"

//****************************************************************************
//      EXTERN
//****************************************************************************
	/* */

//****************************************************************************
//      PRIVATE CONSTANTS
//****************************************************************************
#define TMR_NBR 4

//****************************************************************************
//      PRIVATE TYPES
//****************************************************************************
    /* */

//****************************************************************************
//      PRIVATE DATA
//****************************************************************************
static T_OSL_TIMESTP tllTimer[TMR_NBR];

//****************************************************************************
//             long TimerStart (byte ucTimerNbr, long lDelay)
//  This function starts a timer number for lDelay/1000 seconds.
//  After starting a timer a function TimerGet() should be called to know
//  whether it is over or not.
//  The timer should be over after iDelay/1000 seconds.
//   - OSL_TimeStp_Now() : Return number of elapsed ticks since startup(1tick=1ns).
//  This function has parameters.
//     ucTimerNbr (I-) : Timer number 0..3
//     lDelay (I-) : Initial timer value in thousandth of second (ms)
//  This function has return value.
//     >=0 : TimerStart done
//     <0  : TimerStart failed
//****************************************************************************

long TimerStart(byte ucTimerNbr, long lDelay)
{
	// Local variables
    // ***************
	T_OSL_TIMESTP llTimer;

    // Start timer in progress
    // ***********************
    CHECK(ucTimerNbr<TMR_NBR, lblKO);

    // Get the current timer value
    // ===========================
    llTimer = OSL_TimeStp_Now();    // In 1ns ticks.

    // Set the timer to a 'period' into the future
    // ===========================================
    llTimer += (T_OSL_TIMESTP) lDelay*1000*1000;

    // Assign it to the proper timer
    // =============================
    tllTimer[ucTimerNbr]= llTimer;

    goto lblEnd;

	// Errors treatment
    // ****************
lblKO:
    lDelay=-1;
lblEnd:
    return lDelay;
}

//****************************************************************************
//                 int TimerGet (byte ucTimerNbr)
//  This function returns the state of the timer number.
//   - OSL_TimeStp_Now() : Return number of elapsed ticks since startup(1tick=1ns).
//   - OSL_TimeStp_ElapsedMs() : Get elapsed time between 2 time stamp (end-begin in ms)
//  This function has parameters.
//     ucTimerNbr (I-) : Timer number 0..3
//  This function has return value.
//     >=0 : The number of miliseconds rest
//     <0  : TimerGet failed
//****************************************************************************

long TimerGet(byte ucTimerNbr)
{
	// Local variables
    // ***************
	T_OSL_TIMESTP llTimerEnd, llTimer;
    long lRet;

    // Get timer in progress
    // *********************
    CHECK(ucTimerNbr<TMR_NBR, lblKO);

    // Get the remaining timer value before expiration
    // ===============================================
    llTimerEnd = tllTimer[ucTimerNbr];                      // Retrieve the timer value to reach (ns)

    llTimer = OSL_TimeStp_Now();                            // Get the current timer value (ns).

    if(llTimer < llTimerEnd)
    	lRet = OSL_TimeStp_ElapsedMs(llTimer, llTimerEnd);  // Return the remaining value (ms)
    else
    	lRet = 0;                                           // Timer expired

    goto lblEnd;

	// Errors treatment
    // ****************
lblKO:
    lRet=-1;
lblEnd:
    return lRet;
}

//****************************************************************************
//                  int TimerStop (byte ucTimerNbr)
//  This function should be called when the timer number is no more needed.
//  This function has parameters.
//     ucTimerNbr (I-) : Timer number 0..3
//  This function has return value.
//     >=0 : TimerStop done
//     <0  : TimerStop failed
//****************************************************************************

int TimerStop(byte ucTimerNbr)
{
	// Local variables
    // ***************
    int iRet;

	// Stop timer in progress
	// **********************
    CHECK(ucTimerNbr<TMR_NBR, lblKO);

    tllTimer[ucTimerNbr] = 0;

    iRet=0;
    goto lblEnd;

	// Errors treatment
    // ****************
lblKO:
    iRet=-1;
lblEnd:
    return iRet;
}

//****************************************************************************
//                            void Timer(void)
//  This function manages a timer. A timer counts thousandth of second.
//  When the timer expires after 15s, the terminal displays "Timer expired !!!".
//  This function has no parameters.
//  This function has no return value.
//****************************************************************************

void Timer(void)
{
	// Local variables
    // ***************
	char tcDisplay[16+1];
	long lRet0;

    // Timer demo in progress
    // **********************
    lRet0 = TimerStart(0, 15*1000);                       // Timer0 starts to 15s
    CHECK(lRet0>=0, lblKO);
    while ((lRet0=TimerGet(0)) > 0)                       // Check Timer0
    {
    	CHECK(lRet0>=0, lblKO);

        // Display Remaining Time
    	// ======================
    	_clrscr();                                        // Clear screen
        sprintf(tcDisplay, "%2ld.%03ld Sec", lRet0/1000, lRet0%1000);
        DisplayMedium(0, 0, tcDisplay);
     }

    _clrscr();                                            // Clear screen
    DisplayMedium(0, 0, "Timer expired !!");              // Timer expired
    buzzer(10);
    Telium_Ttestall(0, 3*100);
    goto lblEnd;

	// Errors treatment
    // ****************
lblKO:
    _clrscr();                                            // Clear screen
    DisplayMedium(0, 0, "Processing Error");              // Oops! error
    Telium_Ttestall(0, 5*100);                            // Delay 5s
    goto lblEnd;
lblEnd:
    TimerStop(0);
}

