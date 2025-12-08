/* 
 * YOU MAY USE THIS SOFTWARE AND ANY DERIVATIVES EXCLUSIVELY WITH MICROCHIP
 * PRODUCTS
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, ARE PROVIDED BY MICROCHIP OR APPLY TO THIS 
 * SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE, OR ITS INTERACTION 
 * WITH MICROCHIP PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN 
 * ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP  
 * HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES OR THE DAMAGES ARE 
 * FORESEEABLE.  TO THE FULLEST EXTENT ALLOWED BY LAW, YOU WAIVE ANY 
 * RECOURSE AGAINST MICROCHIP WITH RESPECT TO ANY DAMAGES ARISING FROM 
 * CLAIMS IN ANY WAY RELATED TO THE SOFTWARE AND AGREE THAT ANY SUCH CLAIMS 
 * SHALL NEVER EXCEED THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY 
 * TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/*REVISION HISTORY
 *
 * rev1         12/18/19
 *              - ported code from Darwin rev67
 *              - changed config bits settings
 *              - code builds with no errors  
 * rev2         12/20/19
 *              - comment changes
 *              - peripheral setup verified when checked against PIC16LF18856
 * rev3         1/8/20
 *              - XHAND input moved to RE3
 *              - OPTOPWR moved to RB6
 *              - Motor braking removed
 *              - MOTOR_EN functionality added
 * rev4         1/14/20
 *              - CSWEN enabled
 *              - RC2PPS corrected
 *              - MOTOR_EN made an output in TRISC
 * rev5         1/15/20
 *              - Improved XHAND algorithm working
 * rev6         1/15/20
 *              - Improved cabinet_check( ) routine to allow the unit to sleep
 *                  even when the cabinet is open
 * rev7         1/15/20
 *              - small cleanup of motor control algorithm
 * rev8         1/24/20
 *              - starting PWM increased from 70% to 80%
 * rev9         1/31/20
 *              - voltage check and FVR functionality modified
 * rev10        2/3/20
 *              - created charge_enable( ) and charge_disable( ) functions
 *              - added timeout for charge
 * rev11        2/6/20
 *              - fixed cabinet check where the open cabinet is acknowledged
 *                  but did not hold off hand sensing
 * rev12        2/6/20
 *              - hand_scan( ) and cabinet_check( ) function return values changed 
 *                  to type machineState
 * rev13        2/7/20
 *              - fixed pmux for uart transmit pin
 * rev14        2/13/20
 *              - divide by 4 added to timer 1 and motor pulse capture value modified
 *                  to reflect that change
 * rev15        2/13/20
 *              - run is at 32MHZ
 *              - CPUDOZE cleared (not necessary, but for completeness)
 *              - Encoder input and INT from RFID device made Schmitt triggers
 * rev16        2/26/20
 *              - initial cleanup of MB code for warnings
 *              - motor control function changed to check MIN and MAX PWM values
 *              - SPI changed to 1MHZ
 * rev17        2/26/20
 *              - added in changes to Vpp charge control that Paul and I worked on
 *              - use this revision moving forward.
 * rev18        2/26/20
 *              - moved Vpp check timer to cabinet_check( ) function
 * rev19        3/2/20
 *              - Marks MBrev18 code that writes to tags.
 * rev20        3/6/20
 *              - new demo.c file that has the collision routines intact again (Mark removed his
 *                  inserted code)
 * rev21        3/6/20
 *              - LPBOR enabled, BOR disabled
 * rev22        3/9/20
 *              - ST25 initialization at startup added
 * rev23        3/9/20
 *              - cabinet_check( ) and check_charge( ) separated into two separate files
 * rev23a       3/10/20
 *              - intermediate revision with some code added in cabinet_check.c to read and 
 *                  write tags
 * rev23b       3/12/20
 *              - tag is reading correctly but technology checks seem to not be working since
 *                  it will pass these tests when a tag is not present
 * rev23c       3/12/20
 *              - both tags are read properly and stored in the proper tag arrays
 * rev23d       3/12/20
 *              - comment changes
 * rev24        3/12/20
 *              - tags are read after every cabinet closure.  Single tag reads are still not working but 
 *                  2 tags are read properly.
 * rev25        3/12/20
 *              - single and dual tags are read properly. Changed the device count on the second collision
 *                  poller to >0
 * rev26        3/12/20
 *              - the read routine checks if the Tag Type byte is 1 and saves that data in the Dispenser
 *                  data array
 * rev27        3/12/20
 *              - FVR_disable( ) added in voltage_reads.c after the voltage is checked and if the charge
 *                  is not subsequently enabled
 * rev28        3/13/20
 *              - LED blink turned back on
 *              - switch/case allows progression past cabinet check
 * rev29        3/13/20
 *              - green LED blink fixed
 * rev30        3/13/30
 *              - same as 29 but code optimization is turned off
 * rev31        3/13/20
 *              - first pass at taking ADC readings with the transmitter on and off (working well)
 * rev32        3/13/20
 *              - hand sensor tests first with transmitter off
 * rev33        3/14/20
 *              - hand read routine improved to remove individual counts to determine if the scan passed.
 *                  Instead, a simple moving average plus a percentage is used.
 *              - additional hand scan test with xmtr off is added to eliminate the possibility of light
 *                  affecting the sensor if it comes on right after the xmtr off test
 * rev34        3/16/20
 *              - comment changes only
 * rev35        4/1/20
 *              - new LED routine working with normal operation flash
 * rev35a       4/1/20
 *              - example with three patterns on the normal operation flash
 * rev35b       4/1/20
 *              - led sequences tested with repetitions and works fine
 * rev35c       4/1/20
 *              - all LED values changed from index of 1 - 6 to 0 - 5 and everything is working
 * rev35d       4/1/20
 *              - switch/case statements removed from LED control and replaced with arrays
 *              - LED sequencing working
 * rev35e       4/1/20
 *              - LED algorithm working with new API
 * rev35f       4/1/20
 *              - extra array spaces created for led patterns and timing to avoid overruns when
 *                  next value is checked
 * rev35g       4/2/20
 *              - LED timings increased to 16-bit
 * rev35h       4/2/20
 *              - all 10 array members of LED timing is reloaded in reload_pattern( ) function
 * rev35i       4/2/20
 *              - sleep/wake LED implementation added
 * rev35j       4/2/20
 *              - LED sequence started after waking from load during previous wake cycle
 * rev35k       4/2/20
 *              - multiple LED sequences working
 * rev35l       4/2/20
 *              - all LED sequencing working (tested more in depth)
 * rev35m       4/2/20
 *              - changed open cabinet pattern, no other code changes
 * rev36        4/3/20
 *              - added LED blinks for tag discovery (yellow for 2 tags, blue for 1 tag)
 * rev37        4/8/20
 *              - initial coding for SMART LINK transmitter (not tested)
 * rev37a       4/9/20
 *              - smart link transmitter routine complete but not tested
 * rev37b       4/10/20
 *              - transmit buffers added along with buffer management 
 *              - started initial testing
 * rev37c       4/13/20
 *              - transmit for smart link working for multiple messages
 * rev37d       4/13/20
 *              - separated dispenser event initiator combination
 * rev38        4/13/20
 *              - global interrupts on during sleep
 *              - TMR0 interrupt flag cleared on wakeup in interrupt routine
 * rev39a       4/14/20
 *              - break added to UART transmission
 * rev39b       4/14/20
 *              - uart transmit flags moved to their own structure
 * rev39c       4/16/20
 *              - first pass at adding UART receive -- not tested
 * rev39d       4/16/20
 *              - 32MHZ clock turned on just before tag read and turn back to 4MHZ after
 * rev39e       4/16/20
 *              - led sequence cleared if there are no wakeup interval repetitions specified
 *              - green led blink starts after hand sensor delay
 * rev40        4/17/20
 *              - actual UART receive not tested but code is in place
 *              - LED control from UART receiver buffer command working
 * rev41        4/21/20
 *              - SMART LINK working for the first message but buffer rotation needs to be fixed
 * rev42        4/21/20
 *              - SMART LINK receiver working with circular buffer
 *              - tested with SET LED command
 * rev43        4/21/20
 *              - UART receive working with WUE bit
 * rev44        4/21/20
 *              - TXREG loaded properly per the data sheet when a break character is sent
 *              - SL receive functionality working for messages with and without a break character
 * rev45        4/22/20
 *              - UART receive is working when using the WUE bit and the micro is sleeping or running
 *              - removed the LFINTOSC clock change since TMR0 already has a clock source and the main
 *                  clock does not need to be changed.
 * rev45a       4/22/20
 *              - cleaned up / simplified check of WUE bit in receive interrupt
 * rev45b       4/22/20
 *              - eliminated call to start 4MHZ clock after wakeup since it was already running there
 *                  before going to sleep
 *              - before changing clock speeds, a check is now in place to wait for any ongoing communications
 *                  on the UART receive and transmit
 *              - UART SPBRGL value changed when clock rate changes
 * rev46        4/23/20
 *              - Response to requested packet working (transmit after receiving packet request)
 * rev47        5/11/20
 *              - no changes to rev46.  Just restarting code after 3 weeks.
 * rev48        5/11/20
 *              - hibernate functionality added for ST25 but still locks up at times
 * rev49        5/11/20
 *              - using default values from data sheet in hibernate function
 * rev50        5/11/20
 *              - improved hand scan (raw accums are used instead of shifted values)
 * rev51        5/12/20
 *              - added a failure flash for ST25 initialization
 *              - on a failure, code will still cycle through sleep/wake periods, but the only functionality
 *                  performed is LED flashes
 *              - hand scan delay added after cabinet is closed
 * rev52        5/12/20
 *              - 8 samples reduced from 16 taken for ADC when IR is on.
 * 
 * MPLABX 5.40
 * XC8 v2.20
 * rev53        5/12/20  
 *              - MPLABX update
 * rev54        5/14/20
 *              - changed RA0 and RA1 to analog inputs
 * rev55        5/14/20
 *              - changed hand scan to use hardware sampling and accumulation
 * rev56        5/14/20
 *              - hand scan working with accumulated samples
 *              - IR_Read( ) now just sets the GO bit and returns the accumulated value
 * rev57        5/15/20
 *              - 8 samples are accumulated per scan for each of the three states
 * rev58-59     5/18/20
 *              - both revisions deleted since Gojo was having problems getting the units to run
 * rev60        5/18/20
 *              - running average buffer is 16 readings
 *              - removed duplicate ADC readings
 *              - startup delay is 18 intervals (4.5s)
 * rev61        5/19/20
 *              - LED outputs changed to match T2 board pinout
 * rev62        6/2/20
 *              - dispense message sent properly (test led transmit was used previously)
 * rev63        6/5/20
 *              - adc routine modified to be universal for all calls
 *              - 8 samples taken for all adc reads
 *              - cabinet debounced completely each scan instead of being spread across
 *                  two scans
 * rev63a       6/10/20
 *              - hand scan constants made more sensitive
 * rev63b       6/11/20
 *              - AUX_OUT is input due to bridge on board
 *              - LED test loop added to receive message on hand activation
 * rev63c       6/11/20
 *              - still working on UART receive
 * rev63d       6/11/20
 *              - UART RCV working from sleep
 * rev63e       6/12/20
 *              - comment changes
 * rev64        6/17/20
 *              - added extra IR on hand check if first one passes to eliminate strobe 
 *                  light activations
 *              - margin shifts changed to decrease sensitivity
 *              - minimum hand detect threshold increased
 * rev65        6/17/20
 *              - running average eliminated in hand scan and activate threshold determined
 *                  each pass by the off and on LED values
 * rev65a       6/17/20
 *              - deleted unused variables
 * rev66        6/18/20
 *              - added do...while loop for IR_ON hand scan
 *              - timeout added for SMARTLINK receiver
 *              - hand scan works in direct sunlight also
 *              - SL receiver working fine after timeout addition
 * rev66a       6/18/20
 *              - comment additions and updates
 *              - loop test removed and actual activation transmission replaces it
 * rev66b       6/24/20
 *              - 4 IR ON scans performed
 * rev67        6/24/20
 *              - More SMART LINK packets added
 * rev67a       6/25/20
 *              - added tag write for four locations just to verify functionality
 * rev68        6/25/20
 *              - new hand scan rolling average algorithm
 * rev68a       6/26/20
 *              - AC acquisition time reduced to 40 clocks from 100
 *              - ADC hand sense threshold increased from 30 to 50
 * rev68b       6/26/20
 *              - 16 window encoder wheel
 *              - LED sequence not executed if a hand is present
 * rev68c       6/26/20
 *              - hand presence removed for LED sequence execution
 *              - green LED not turned off in LED interrupt if the motor is activated
 * rev69        6/26/20
 *              - comment changes
 * rev69a       6/26/20
 *              - new hand algorithm where one off/on IR reading pair is taken.  If there is a
 *                  hit, take another off/on reading to verify.  Still 4 samples per ADC sample
 *                  with accumulation
 * rev69b       6/26/20
 *              - one sample per adc conversion
 * rev69c       6/27/20
 *              - threshold increased to 18 from 15
 * rev69d       6/28/20
 *              - do...while loop used for hand ir tests with 4 successes required
 * rev69e       6/29/20
 *              - comment changes
 * rev70        6/30/20
 *              - AUX_OUT made an output and set high at startup
 * rev71        7/2/20
 *              - AUX_OUT made an input
 *              - FVR enabled at wakeup and disabled just before going to sleep
 *              - ADC startup at wakeup with Vss connected to the mux
 *              - ADC only turned on at wakeup and turned off before sleeping
 * rev71a       7/2/20
 *              - AUX_OUT made an output
 * rev72        7/10/20
 *              - AUX_OUT is an input to run on rev1 boards
 *              - refill tag info fixed for GET_PACKET to meet new tag specs document
 * rev73        - AUX_OUT is still an input to run on rev1 boards
 *              - hand scan will not run if the tag type is not a 1 for the dispenser tag
 * rev74        7/13/20
 *              - proper dispenser type check with mask added
 * rev74a       7/13/20
 *              - AUX_OUT made an output for the rev2 boards
 * rev75        7/21/20
 *              - sensitivity increased for hand sense (18 to 12)
 *              - successive hand hits reduced from 4 to 3
 *              - AUX_OUT is an output
 * rev75a       - 7/21/20
 *              - comment cleanup and removal of unused constants
 * rev75b       7/22/20
 *              - 8-second moving average on hand ADC reads
 * rev76        7/22/20
 *              - 10 ohm resistor in emitter circuit and threshold increased from 12 to 40
 * rev77        7/22/20
 *              - voltage checks and charge enable/disable for the supercaps is working
 * rev77a       7/22/20
 *              - voltage checks and charge/discharge working with both EOR and AUX
 *                  batteries
 * rev77b       7/22/20
 *              - the EOR becomes active when the cabinet is closed in case a new refill was
 *                  added
 * rev77c       7/22/20
 *              - added wait period between hand activations
 * rev77d       7/23/20
 *              - unit checks supercap voltage every minute instead of 15 seconds
 * rev77e       7/27/20 
 *              - voltage monitoring consolidated into one routine
 *              - routine is working
 * rev77f       7/27/20
 *              - minimum voltage required to activate motor
 * rev77g       7/28/20
 *              - fixed hand rx averaging of ambient (broke when interval waiting was added
 *                  in rev77c)
 *              - reduced ADACQ from 100 to 50
 * rev77h       8/7/20
 *              - brownout turned on while awake
 *              - wdt enabled with a 4s window
 * rev77i       8/10/20
 *              - tags read after reset
 * rev77j       8/12/20
 *              - RFID reader is initialized after the supercap voltage is at the proper level
 * rev78        8/27/20
 *              - all tag data cleared before reading tags when cabinet is closed
 *              - delay increased from 10ms to 100ms between tag activation, tag reads, and hibernate
 *              - voltage check value count reduced from 550 to 500 at startup
 * rev78a       8/27/20
 *              - voltage check removed at startup
 *              - flags set for vppActive and auxActive at startup
 *              - ST25init removed at startup
 * rev78b       8/27/20
 *              - LED outputs controlled by TRIS register to eliminate flickers at startup
 * rev78c       8/27/20
 *              - voltage check removed at startup and vppActive and auxActive are set at startup (these
 *                  changes were not made in 78a as noted)
 * rev78d       8/27/20
 *              - voltage charge limit lowered and the charge timeout raised
 * rev78e       8/27/20
 *              - increased minimum operating voltage for motor activation
 * rev78f       8/28/20
 *              - line 89 of charge_check.h commented out to never set failure flag
 *              - fixed condition where voltage is checked before the hand scan and if the check failed,
 *                  that state would stay the same
 *              - voltage checked in the motor activate state instead of before the hand scan
 * rev78g       8/28/20
 *              - separate charge timeout times for startup and run
 * rev78h       8/29/20
 *              - Vpp voltage checked before the hand scan
 * rev79        9/1/20
 *              - EEPROM written and read for tag data
 * rev80        9/3/20
 *              - voltage reads for the EOR and AUX batteries added
 *              - Vpp voltage check moved from before the hand sense to just before motor activation
 * rev81        9/9/20
 *              - fixed activations between intervals setting
 * rev82        9/9/20
 *              - eeprom single byte reads and writes added
 *              - eeprom default write and read functions added and used at startup
 * rev83        9/9/20 
 *              - SL Set Configuration message added along with eeprom save
 *              - dispense lockout added
 * rev83a       9/10/20
 *              - voc and refill tag arrays updated with Mark
 *              - voc tag data request added
 *              - transmiter buffer size increased from 20 bytes to 32 bytes
 *              - u8rcvBufferByte array increased from 25 bytes to 32 bytes
 * rev83b       9/11/20
 *              - fixed eeprom addresses for default writes
 * rev83c       9/16/20
 *              - fixed refill tag byte length
 *              - fixed VOC tag transmit
 * rev83d       9/16/20
 *              - added cabinet and dispense cycle counts and EEPROM saves
 * rev83e       9/17/20
 *              - diagnostics structure added 
 *              - voltage BCD format entered
 *              - dispenser diagnostics message added
 * rev83f       9/18/20
 *              - SL messages streamlined
 *              - XHAND Dispense SL message added
 * rev84        9/19/20
 *              - started making changes for Grant's new hardware (still commented out)
 *              - added startup eeprom writes for cabinet lid counter
 *              - cleared all eeprom loacations at initial startup
 *              - fixed crc location in dispense_diagnostics message
 *              - fixed crc on refill message
 * rev85        9/23/20
 *              - not used since it has changed I/O for Grant's new switcher.  It is kept in 
 *                  a separate project folder.
 * rev86        9/23/20
 *              - removed all of the new switcher conditional compiles
 *              - check if EOR voltage is lower than the AA low limit on each charge_disable.
 *                  If it is, the AUX battery is turned on.  There was a problem with switching
 *                  over to the AUX output.
 *              - check the AA voltage when the cabinet is closed to determine if an AA battery
 *                  is installed
 * rev86a       9/23/20
 *              - battery source checked after cabinet closure
 *              - battery source checked on every charge disable
 *              - EOR depletion checked before charge_enable( )
 * rev86b       9/23/20
 *              - Vpp voltage multiplier changed to 30 from 10
 * rev86c       9/24/20
 *              - vpp check voltage flag set after a cabinet lid closure in case the voltage sagged
 *                  with the tag read
 * rev86d       10/2/20
 *              - clear time changed to 1s
 *              - multiplier in BCD calculation fixed
 * rev86e       10/15/20
 *              - removed "restore the battery connection" in voltage_reads.c
 *              - modified check_for_battery_switch( ) to make the proper battery selection no matter
 *                  the states of the battery connections
 * rev86f       10/19/20
 *              - cleared sensorActivated flag if the voltage is too low to operate the motor.  This fixes
 *                  the stuck green LED.
 * rev86g       10/19/20
 *              - added dispenser pumphouse information message
 *              - changed clear time to be reported in 250ms intervals
 *              - added cabinet, refill tag, and voc tag status to pumphouse message
 * rev86h       10/21/20
 *              - voltage check flag set if charge is not active and motor activation voltage is low
 * rev87        10/26/20
 *              - voltage BCD conversions moved to actual smartlink message
 *              - voltage readings moved to charge_check routine
 *              - lowered Vpp voltage limits since charge was staying on since limit was not reached
 *              - charge algorithm and battery switchover working
 * rev88        11/2/20
 *              - pulses read in VOC tag and used for motor spin revs
 * rev89        11/17/20
 *              - break signal always on
 * 
 * 
 * 
 */

