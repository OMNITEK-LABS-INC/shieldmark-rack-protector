#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/Pallarum_Tester_V1.0.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/Pallarum_Tester_V1.0.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=clock_config.c configuration.c global_variables.c hand_rx.c initialize.c interrupt_manager.c main.c read_IR.c smartlink_messages.c voltage_reads.c SPI.c rfid_control.c sleep.c adc.c eeprom_read_write.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/clock_config.p1 ${OBJECTDIR}/configuration.p1 ${OBJECTDIR}/global_variables.p1 ${OBJECTDIR}/hand_rx.p1 ${OBJECTDIR}/initialize.p1 ${OBJECTDIR}/interrupt_manager.p1 ${OBJECTDIR}/main.p1 ${OBJECTDIR}/read_IR.p1 ${OBJECTDIR}/smartlink_messages.p1 ${OBJECTDIR}/voltage_reads.p1 ${OBJECTDIR}/SPI.p1 ${OBJECTDIR}/rfid_control.p1 ${OBJECTDIR}/sleep.p1 ${OBJECTDIR}/adc.p1 ${OBJECTDIR}/eeprom_read_write.p1
POSSIBLE_DEPFILES=${OBJECTDIR}/clock_config.p1.d ${OBJECTDIR}/configuration.p1.d ${OBJECTDIR}/global_variables.p1.d ${OBJECTDIR}/hand_rx.p1.d ${OBJECTDIR}/initialize.p1.d ${OBJECTDIR}/interrupt_manager.p1.d ${OBJECTDIR}/main.p1.d ${OBJECTDIR}/read_IR.p1.d ${OBJECTDIR}/smartlink_messages.p1.d ${OBJECTDIR}/voltage_reads.p1.d ${OBJECTDIR}/SPI.p1.d ${OBJECTDIR}/rfid_control.p1.d ${OBJECTDIR}/sleep.p1.d ${OBJECTDIR}/adc.p1.d ${OBJECTDIR}/eeprom_read_write.p1.d

# Object Files
OBJECTFILES=${OBJECTDIR}/clock_config.p1 ${OBJECTDIR}/configuration.p1 ${OBJECTDIR}/global_variables.p1 ${OBJECTDIR}/hand_rx.p1 ${OBJECTDIR}/initialize.p1 ${OBJECTDIR}/interrupt_manager.p1 ${OBJECTDIR}/main.p1 ${OBJECTDIR}/read_IR.p1 ${OBJECTDIR}/smartlink_messages.p1 ${OBJECTDIR}/voltage_reads.p1 ${OBJECTDIR}/SPI.p1 ${OBJECTDIR}/rfid_control.p1 ${OBJECTDIR}/sleep.p1 ${OBJECTDIR}/adc.p1 ${OBJECTDIR}/eeprom_read_write.p1

# Source Files
SOURCEFILES=clock_config.c configuration.c global_variables.c hand_rx.c initialize.c interrupt_manager.c main.c read_IR.c smartlink_messages.c voltage_reads.c SPI.c rfid_control.c sleep.c adc.c eeprom_read_write.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/Pallarum_Tester_V1.0.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F26Q10
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/clock_config.p1: clock_config.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/clock_config.p1.d 
	@${RM} ${OBJECTDIR}/clock_config.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/clock_config.p1 clock_config.c 
	@-${MV} ${OBJECTDIR}/clock_config.d ${OBJECTDIR}/clock_config.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/clock_config.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/configuration.p1: configuration.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/configuration.p1.d 
	@${RM} ${OBJECTDIR}/configuration.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/configuration.p1 configuration.c 
	@-${MV} ${OBJECTDIR}/configuration.d ${OBJECTDIR}/configuration.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/configuration.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/global_variables.p1: global_variables.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/global_variables.p1.d 
	@${RM} ${OBJECTDIR}/global_variables.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/global_variables.p1 global_variables.c 
	@-${MV} ${OBJECTDIR}/global_variables.d ${OBJECTDIR}/global_variables.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/global_variables.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hand_rx.p1: hand_rx.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/hand_rx.p1.d 
	@${RM} ${OBJECTDIR}/hand_rx.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/hand_rx.p1 hand_rx.c 
	@-${MV} ${OBJECTDIR}/hand_rx.d ${OBJECTDIR}/hand_rx.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/hand_rx.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/initialize.p1: initialize.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/initialize.p1.d 
	@${RM} ${OBJECTDIR}/initialize.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/initialize.p1 initialize.c 
	@-${MV} ${OBJECTDIR}/initialize.d ${OBJECTDIR}/initialize.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/initialize.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/interrupt_manager.p1: interrupt_manager.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/interrupt_manager.p1.d 
	@${RM} ${OBJECTDIR}/interrupt_manager.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/interrupt_manager.p1 interrupt_manager.c 
	@-${MV} ${OBJECTDIR}/interrupt_manager.d ${OBJECTDIR}/interrupt_manager.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/interrupt_manager.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/main.p1: main.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.p1.d 
	@${RM} ${OBJECTDIR}/main.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/main.p1 main.c 
	@-${MV} ${OBJECTDIR}/main.d ${OBJECTDIR}/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/read_IR.p1: read_IR.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/read_IR.p1.d 
	@${RM} ${OBJECTDIR}/read_IR.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/read_IR.p1 read_IR.c 
	@-${MV} ${OBJECTDIR}/read_IR.d ${OBJECTDIR}/read_IR.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/read_IR.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/smartlink_messages.p1: smartlink_messages.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/smartlink_messages.p1.d 
	@${RM} ${OBJECTDIR}/smartlink_messages.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/smartlink_messages.p1 smartlink_messages.c 
	@-${MV} ${OBJECTDIR}/smartlink_messages.d ${OBJECTDIR}/smartlink_messages.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/smartlink_messages.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/voltage_reads.p1: voltage_reads.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/voltage_reads.p1.d 
	@${RM} ${OBJECTDIR}/voltage_reads.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/voltage_reads.p1 voltage_reads.c 
	@-${MV} ${OBJECTDIR}/voltage_reads.d ${OBJECTDIR}/voltage_reads.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/voltage_reads.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/SPI.p1: SPI.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SPI.p1.d 
	@${RM} ${OBJECTDIR}/SPI.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/SPI.p1 SPI.c 
	@-${MV} ${OBJECTDIR}/SPI.d ${OBJECTDIR}/SPI.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/SPI.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/rfid_control.p1: rfid_control.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/rfid_control.p1.d 
	@${RM} ${OBJECTDIR}/rfid_control.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/rfid_control.p1 rfid_control.c 
	@-${MV} ${OBJECTDIR}/rfid_control.d ${OBJECTDIR}/rfid_control.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/rfid_control.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/sleep.p1: sleep.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/sleep.p1.d 
	@${RM} ${OBJECTDIR}/sleep.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/sleep.p1 sleep.c 
	@-${MV} ${OBJECTDIR}/sleep.d ${OBJECTDIR}/sleep.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/sleep.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/adc.p1: adc.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/adc.p1.d 
	@${RM} ${OBJECTDIR}/adc.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/adc.p1 adc.c 
	@-${MV} ${OBJECTDIR}/adc.d ${OBJECTDIR}/adc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/adc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/eeprom_read_write.p1: eeprom_read_write.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/eeprom_read_write.p1.d 
	@${RM} ${OBJECTDIR}/eeprom_read_write.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/eeprom_read_write.p1 eeprom_read_write.c 
	@-${MV} ${OBJECTDIR}/eeprom_read_write.d ${OBJECTDIR}/eeprom_read_write.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/eeprom_read_write.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/clock_config.p1: clock_config.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/clock_config.p1.d 
	@${RM} ${OBJECTDIR}/clock_config.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/clock_config.p1 clock_config.c 
	@-${MV} ${OBJECTDIR}/clock_config.d ${OBJECTDIR}/clock_config.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/clock_config.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/configuration.p1: configuration.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/configuration.p1.d 
	@${RM} ${OBJECTDIR}/configuration.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/configuration.p1 configuration.c 
	@-${MV} ${OBJECTDIR}/configuration.d ${OBJECTDIR}/configuration.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/configuration.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/global_variables.p1: global_variables.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/global_variables.p1.d 
	@${RM} ${OBJECTDIR}/global_variables.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/global_variables.p1 global_variables.c 
	@-${MV} ${OBJECTDIR}/global_variables.d ${OBJECTDIR}/global_variables.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/global_variables.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/hand_rx.p1: hand_rx.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/hand_rx.p1.d 
	@${RM} ${OBJECTDIR}/hand_rx.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/hand_rx.p1 hand_rx.c 
	@-${MV} ${OBJECTDIR}/hand_rx.d ${OBJECTDIR}/hand_rx.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/hand_rx.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/initialize.p1: initialize.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/initialize.p1.d 
	@${RM} ${OBJECTDIR}/initialize.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/initialize.p1 initialize.c 
	@-${MV} ${OBJECTDIR}/initialize.d ${OBJECTDIR}/initialize.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/initialize.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/interrupt_manager.p1: interrupt_manager.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/interrupt_manager.p1.d 
	@${RM} ${OBJECTDIR}/interrupt_manager.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/interrupt_manager.p1 interrupt_manager.c 
	@-${MV} ${OBJECTDIR}/interrupt_manager.d ${OBJECTDIR}/interrupt_manager.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/interrupt_manager.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/main.p1: main.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.p1.d 
	@${RM} ${OBJECTDIR}/main.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/main.p1 main.c 
	@-${MV} ${OBJECTDIR}/main.d ${OBJECTDIR}/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/read_IR.p1: read_IR.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/read_IR.p1.d 
	@${RM} ${OBJECTDIR}/read_IR.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/read_IR.p1 read_IR.c 
	@-${MV} ${OBJECTDIR}/read_IR.d ${OBJECTDIR}/read_IR.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/read_IR.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/smartlink_messages.p1: smartlink_messages.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/smartlink_messages.p1.d 
	@${RM} ${OBJECTDIR}/smartlink_messages.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/smartlink_messages.p1 smartlink_messages.c 
	@-${MV} ${OBJECTDIR}/smartlink_messages.d ${OBJECTDIR}/smartlink_messages.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/smartlink_messages.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/voltage_reads.p1: voltage_reads.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/voltage_reads.p1.d 
	@${RM} ${OBJECTDIR}/voltage_reads.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/voltage_reads.p1 voltage_reads.c 
	@-${MV} ${OBJECTDIR}/voltage_reads.d ${OBJECTDIR}/voltage_reads.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/voltage_reads.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/SPI.p1: SPI.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SPI.p1.d 
	@${RM} ${OBJECTDIR}/SPI.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/SPI.p1 SPI.c 
	@-${MV} ${OBJECTDIR}/SPI.d ${OBJECTDIR}/SPI.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/SPI.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/rfid_control.p1: rfid_control.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/rfid_control.p1.d 
	@${RM} ${OBJECTDIR}/rfid_control.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/rfid_control.p1 rfid_control.c 
	@-${MV} ${OBJECTDIR}/rfid_control.d ${OBJECTDIR}/rfid_control.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/rfid_control.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/sleep.p1: sleep.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/sleep.p1.d 
	@${RM} ${OBJECTDIR}/sleep.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/sleep.p1 sleep.c 
	@-${MV} ${OBJECTDIR}/sleep.d ${OBJECTDIR}/sleep.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/sleep.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/adc.p1: adc.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/adc.p1.d 
	@${RM} ${OBJECTDIR}/adc.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/adc.p1 adc.c 
	@-${MV} ${OBJECTDIR}/adc.d ${OBJECTDIR}/adc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/adc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/eeprom_read_write.p1: eeprom_read_write.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/eeprom_read_write.p1.d 
	@${RM} ${OBJECTDIR}/eeprom_read_write.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/eeprom_read_write.p1 eeprom_read_write.c 
	@-${MV} ${OBJECTDIR}/eeprom_read_write.d ${OBJECTDIR}/eeprom_read_write.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/eeprom_read_write.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/Pallarum_Tester_V1.0.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/Pallarum_Tester_V1.0.${IMAGE_TYPE}.map  -D__DEBUG=1  -mdebugger=none  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto        $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/Pallarum_Tester_V1.0.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	@${RM} ${DISTDIR}/Pallarum_Tester_V1.0.${IMAGE_TYPE}.hex 
	
	
else
${DISTDIR}/Pallarum_Tester_V1.0.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/Pallarum_Tester_V1.0.${IMAGE_TYPE}.map  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -memi=wordwrite -O0 -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/Pallarum_Tester_V1.0.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
