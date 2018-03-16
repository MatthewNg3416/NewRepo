#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------
*	True and False
*---------------------------------------------------------*/
#define		TRUE				1
#define		FALSE				0

/*-----------------------------------------------------------
*	High and Low
*---------------------------------------------------------*/
#define		HIGH				1
#define		LOW  				0

/*-----------------------------------------------------------
*	Circle Move Direction
*---------------------------------------------------------*/
#define		CIR_CCW 			1
#define		CIR_CW  			0

/*-----------------------------------------------------------
*	Coordinate  Direction
*---------------------------------------------------------*/
#define		CORD_CCW 			1
#define		CORD_CW  			0

/*-----------------------------------------------------------
*	Encoder  Direction
*---------------------------------------------------------*/
#define		ENCODER_CCW 		1
#define		ENCODER_CW  		0

/*-----------------------------------------------------------
*	Event Source Status defines
*---------------------------------------------------------*/
#define		ST_NONE					0x0000
#define		ST_HOME_SWITCH			0x0001
#define		ST_POS_LIMIT			0x0002
#define		ST_NEG_LIMIT   			0x0004
#define		ST_AMP_FAULT			0x0008
#define		ST_A_LIMIT    			0x0010
#define		ST_V_LIMIT  			0x0020
#define		ST_X_NEG_LIMIT 			0x0040
#define		ST_X_POS_LIMIT			0x0080
#define		ST_ERROR_LIMIT			0x0100
#define		ST_PC_COMMAND  			0x0200
#define		ST_OUT_OF_FRAMES    	0x0400
#define		ST_AMP_POWER_ONOFF  	0x0800
#define		ST_ABS_COMM_ERROR   	0x1000
#define		ST_INPOSITION_STATUS	0x2000
#define		ST_RUN_STOP_COMMAND 	0x4000 		
#define		ST_COLLISION_STATE  	0x8000 		
#define		ST_PAUSTATE_STATE  		0x10000 		

/*-----------------------------------------------------------
*	Event defines
*---------------------------------------------------------*/
#define		NO_EVENT			0 	/* ignore a condition */
#define		STOP_EVENT			1	/* generate a stop event */
#define 	E_STOP_EVENT		2 	/* generate an e_stop event */
#define		ABORT_EVENT			3 	/* disable PID control, and disable the amplifier */

/*-----------------------------------------------------------
*	Digital Filter Defines
*---------------------------------------------------------*/
#define		GAIN_NUMBER			5	/* elements expected get/set_filter(...) */
#define		GA_P				0	/* proportional gain */
#define		GA_I				1	/* integral gain */
#define		GA_D				2	/* derivative gain-damping term */
#define		GA_F       			3	/* velocity feed forward */
#define		GA_ILIMIT 			4	/* integration summing limit */

/*-----------------------------------------------------------
*	Error Defines
*---------------------------------------------------------*/
#define		MAX_ERROR_LEN							80	/* maximum length for error massage string */
#define		MMC_OK									0	/* no problems */
#define		MMC_NOT_INITIALIZED						1	/* be sure to call mmc_init(...) */
#define		MMC_TIMEOUT_ERR							2	/* DPRAM Communication Error */
#define		MMC_INVALID_AXIS						3	/* axis out of range or not configured */
#define		MMC_ILLEGAL_ANALOG						4	/* analog channel < 0 or > 4 */
#define		MMC_ILLEGAL_IO							5	/* illegal I/O port */
#define		MMC_ILLEGAL_PARAMETER					6	/* move with zero accel or velocity */
#define		MMC_NO_MAP								7 	/* The map_axes(...) funcation has not been called */
#define		MMC_AMP_FAULT							8 	/* amp fault occured */
#define		MMC_ON_MOTION							9 	/* Motion is not completed */
#define		MMC_NON_EXIST							10	/* MMC Board is not exist */
#define		MMC_BOOT_OPEN_ERROR						11	/* MMC Boot File Read/Write Error*/
#define		MMC_CHKSUM_OPEN_ERROR					12	/* MMC Checksum File Read/Write Error*/
#define		MMC_WINNT_DRIVER_OPEN_ERROR				13	/* MMC Windows NT Driver Open Error*/
#define		MMC_EVENT_OCCUR_ERROR					14
#define		MMC_AMP_POWER_OFF     					15
#define		MMC_DATA_DIRECTORY_OPEN_ERROR			16
#define		MMC_INVALID_CPMOTION_GROUP				17
#define		MMC_VELOCITY_ILLEGAL_PARAMETER			18	/* move with zero accel or velocity */
#define		MMC_ACCEL_ILLEGAL_PARAMETER				19	/* move with zero accel or velocity */
#define		FUNC_ERR								-1	/* Function Error				*/


/*-----------------------------------------------------------
*	General Defines
*---------------------------------------------------------*/
#define		POSITIVE		1
#define		NEGATIVE		0

/*-----------------------------------------------------------
*	Motor Type
*---------------------------------------------------------*/
#define		SERVO_MOTOR		0
#define		STEPPER			1
#define		MICRO_STEPPER	2

/*-----------------------------------------------------------
*	Feedback Configuration
*---------------------------------------------------------*/
#define		FB_ENCODER		0
#define		FB_UNIPOLAR		1
#define		FB_BIPOLAR		2

/*-----------------------------------------------------------
*	Control_Loop Method
*---------------------------------------------------------*/
#define		OPEN_LOOP		0
#define		CLOSED_LOOP		1
#define		SEMI_LOOP		2

/*-----------------------------------------------------------
*	Control Method
*---------------------------------------------------------*/
#define		V_CONTROL		0
#define		T_CONTROL		1

#define		IN_STANDING		0
#define		IN_ALWAYS		1

#define		TWO_PULSE		0
#define		SIGN_PULSE		1
/*-----------------------------------------------------------
*	Limit Vlaue
*---------------------------------------------------------*/
#define		MMC_ACCEL_LIMIT		25000
#define		MMC_VEL_LIMIT		5000000
#define		MMC_POS_SW_LIMIT	2147483640
#define		MMC_NEG_SW_LIMIT	-2147483640
#define		MMC_ERROR_LIMIT		35000
#define		MMC_PULSE_RATIO		255
/*-----------------------------------------------------------
*	Type Define
*---------------------------------------------------------*/
#define		CHAR				char
#define		INT					short
#define		LONG				long
#define		FLOAT				float
#define		VOID				void

#ifdef WIN32
/* WIN32 programs which do not use the WINAPI doesn't need to include
*  the "windows.h", so the WIN32 sections are moved here.*/
#define		API			_stdcall
#define		pCHAR		CHAR *
#define     pINT		INT *
#define		pDOUBLE		double *
#define		pLONG		LONG *
#define		pFLOAT		FLOAT *
#else
/* Static library can use any reference according to it's compiling model.
*	In this code, but, large model assumed and not tested building any others.*/
#define 	API
#define 	pCHAR 		CHAR *
#define 	pINT		INT *
#define 	pDOUBLE		double *
#define 	pLONG		LONG *
#define		pFLOAT		FLOAT *
#endif

#ifdef	WIN32
/* In windows programs, there is no easy way to export its global variable,
* other than functions to other module referencing this DLL.*/
INT API get_mmc_error( VOID);
INT	API get_version( VOID);
INT	API get_axis_num( VOID);
INT	API get_bd_num( VOID);
INT	API get_velocity( INT);
#else
extern INT mmc_error;
#endif

INT API mmc_initx(INT, pLONG);
INT	API version_chk( INT, pINT);
INT API set_dpram_addr(INT, LONG);
INT API get_dpram_addr(INT, pLONG);
INT API error_message(INT, pCHAR);
pCHAR	API _error_message(INT);
INT API save_boot_frame(VOID);

INT API mmc_axes(INT, pINT);
INT API mmc_all_axes(VOID);
INT API get_stepper(INT);
INT API set_stepper(INT);
INT API set_servo(INT);
INT API set_feedback(INT, INT);
INT API get_feedback(INT, pINT);
INT API set_closed_loop(INT, INT);
INT API get_closed_loop(INT, pINT);
INT API set_unipolar(INT, INT);
INT API get_unipolar(INT);

INT API set_abs_encoder(INT);
INT API get_micro_stepper(INT);
INT API set_micro_stepper(INT);

INT API mmcDelay(LONG);
INT API mmc_dwell(INT, LONG);
INT API mmc_io_trigger(INT,INT,INT);

INT API get_counter(INT, pDOUBLE);
INT API get_sync_position(pDOUBLE, pDOUBLE);

INT API set_position(INT, double);
INT API get_position(INT, pDOUBLE);
INT API set_command(INT, double);
INT API get_command(INT, pDOUBLE);
INT API get_error(INT, pDOUBLE);
INT API get_com_velocity(INT ax);
INT API get_act_velocity(INT ax);

INT API in_sequence(INT);
INT API in_motion(INT);
INT API in_position(INT);
INT API frames_left(INT);
INT API motion_done(INT);
INT API axis_done(INT);
INT API axis_state(INT);
INT API axis_source(INT);
LONG API axis_sourcex(INT);
INT API clear_status(INT);
INT API frames_clear(INT);

INT API map_axes( INT , pINT);
INT API set_move_speed(double);
INT API set_move_accel(INT);
INT API set_arc_division(double);
INT API all_done(VOID);
INT API move_2(double, double);
INT API move_3(double, double, double);
INT API move_4(double, double, double, double);
INT API move_n(pDOUBLE);
INT API move_2ax(INT,INT,double, double, double, INT);
INT API move_3ax(INT,INT,INT,double, double,double, double, INT);
INT API move_4ax(INT,INT,INT,INT,double, double, double,double, double, INT);
INT API move_nax(INT, pINT, pDOUBLE, double, INT);
INT API move_2axgr(INT,INT,INT,double, double, double, INT);
INT API move_3axgr(INT,INT,INT,INT,double, double,double, double, INT);
INT API move_4axgr(INT,INT,INT,INT,INT,double, double, double,double, double, INT);

INT API smove_2(double, double);
INT API smove_3(double, double, double);
INT API smove_4(double, double, double, double);
INT API smove_n(pDOUBLE);
INT API smove_2ax(INT,INT,double, double, double, INT);
INT API smove_3ax(INT,INT,INT,double, double, double, double, INT);
INT API smove_4ax(INT,INT,INT,INT,double, double, double, double, double, INT);
INT API smove_nax(INT, pINT, pDOUBLE, double, INT);
INT API smove_2axgr(INT,INT,INT,double, double, double, INT);
INT API smove_3axgr(INT,INT,INT,INT,double, double, double, double, INT);
INT API smove_4axgr(INT,INT,INT,INT,INT,double, double, double, double, double, INT);

INT API arc_2(double, double, double);
INT API arc_2ax(INT, INT, double, double , double, double, INT);
INT API spl_line_move2(pDOUBLE, double, INT);
INT API spl_line_move3(pDOUBLE, double, INT);
INT API spl_line_move2ax(INT,INT,pDOUBLE, double, INT);
INT API spl_line_move3ax(INT,INT,INT,pDOUBLE, double, INT);

INT API spl_arc_move2(double , double, pDOUBLE, double, INT, INT);
INT API spl_arc_move3(double , double, pDOUBLE, double, INT, INT);
INT API spl_arc_move2ax(INT,INT, double, double, pDOUBLE, double, INT, INT);
INT API spl_arc_move3ax(INT,INT,INT, double, double, pDOUBLE, double, INT, INT);

INT API spl_move(INT, INT, INT, INT, pDOUBLE, pDOUBLE, pDOUBLE, double, INT);
INT API rect_move(INT, INT, pDOUBLE, double, INT);
INT API set_spl_auto_off(INT,INT);


INT API start_move( INT, double, double, INT);
INT API move( INT, double, double, INT);
INT API start_r_move( INT, double, double, INT);
INT API r_move( INT, double, double, INT);
INT API start_s_move( INT, double, double, INT);
INT API s_move( INT, double,double, INT);
INT API start_rs_move( INT, double, double, INT);
INT API rs_move( INT, double,double, INT);
INT API start_t_move( INT, double, double, INT, INT);
INT API t_move( INT, double,double, INT, INT);
INT API start_ts_move( INT, double, double, INT, INT);
INT API ts_move( INT, double,double, INT, INT);
INT API start_tr_move( INT, double, double, INT, INT);
INT API tr_move( INT, double,double, INT, INT);
INT API start_trs_move( INT, double, double, INT, INT);
INT API trs_move( INT, double,double, INT, INT);
INT API start_t_move_all( INT , pINT, pDOUBLE, pDOUBLE, pINT,pINT);
INT API t_move_all( INT, pINT, pDOUBLE,pDOUBLE, pINT, pINT);
INT API start_ts_move_all( INT , pINT, pDOUBLE, pDOUBLE, pINT,pINT);
INT API ts_move_all( INT, pINT, pDOUBLE,pDOUBLE, pINT, pINT);


INT API start_move_all( INT, pINT, pDOUBLE, pDOUBLE, pINT);
INT API move_all( INT, pINT, pDOUBLE, pDOUBLE, pINT);
INT API start_s_move_all( INT, pINT, pDOUBLE, pDOUBLE, pINT);
INT API s_move_all( INT, pINT, pDOUBLE, pDOUBLE, pINT);
INT API wait_for_done( INT);
INT API wait_for_all( INT, pINT);
INT API v_move( INT, double, INT);

INT API set_positive_sw_limit(INT, double, INT );
INT API get_positive_sw_limit(INT, pDOUBLE, pINT);
INT API set_negative_sw_limit(INT, double, INT);
INT API get_negative_sw_limit(INT, pDOUBLE, pINT);

INT API get_accel_limit(INT, pINT);
INT API set_accel_limit(INT, INT );
INT API fset_accel_limit(INT, INT );
INT API get_vel_limit(INT, pDOUBLE);
INT API set_vel_limit(INT, double );

INT API set_positive_limit(INT, INT );
INT API get_positive_limit(INT, pINT);
INT API set_negative_limit(INT, INT );
INT API get_negative_limit(INT, pINT);
INT API set_in_position(INT, double);
INT API get_in_position(INT, pDOUBLE);
INT API set_error_limit(INT, double, INT);
INT API get_error_limit(INT, pDOUBLE, pINT);
INT API set_positive_level(INT, INT );
INT API get_positive_level(INT, pINT);
INT API set_negative_level(INT, INT );
INT API get_negative_level(INT, pINT);

INT API home_switch(INT);
INT API pos_switch(INT);
INT API neg_switch(INT);
INT API amp_fault_switch(INT);
INT API set_io(INT, LONG);
INT API get_io(INT, pLONG);
INT API get_out_io(INT, pLONG);
INT API set_bit(INT);
INT API reset_bit(INT);


INT API get_gain(INT, pLONG);
INT API fget_gain(INT, pLONG);
INT API set_gain(INT, pLONG);
INT API fset_gain(INT, pLONG);
INT API get_v_gain(INT, pLONG);
INT API fget_v_gain(INT, pLONG);
INT API set_v_gain(INT, pLONG);
INT API fset_v_gain(INT, pLONG);

INT API fset_p_integration(INT ax, INT mode);
INT API fset_v_integration(INT ax, INT mode);
INT API fget_p_integration(INT, pINT);
INT API fget_v_integration(INT, pINT);
INT API set_p_integration(INT, INT);
INT API get_p_integration(INT, pINT);
INT API set_v_integration(INT, INT);
INT API get_v_integration(INT, pINT);

INT API set_amp_enable(INT,INT);
INT API get_amp_enable(INT,pINT);
INT API amp_fault_reset(INT);
INT API amp_fault_set(INT);
INT API set_amp_enable_level(INT,INT);
INT API get_amp_enable_level(INT,pINT);
INT API fget_amp_enable_level(INT,pINT);
INT API set_control(INT,INT);
INT API get_control(INT,pINT);
INT API set_electric_gear(INT, double);
INT API get_electric_gear(INT, pDOUBLE);
INT API fget_electric_gear(INT, pDOUBLE);
INT API set_step_mode(INT, INT);
INT API get_step_mode(INT, pINT);
INT API set_sync_map_axes(INT, INT);
INT API set_sync_control(INT);
INT API get_sync_control(pINT);
INT API set_sync_gain(LONG);
INT API fset_sync_gain(LONG syncgain);
INT API get_sync_gain(pLONG);
INT API fget_sync_gain(pLONG);
INT API compensation_pos(INT, pINT, pDOUBLE, pINT);

INT API set_pulse_ratio(INT,INT);
INT API fset_pulse_ratio(INT,INT);
INT API get_pulse_ratio(INT,pINT);

INT API set_stop(INT);
INT API set_stop_rate(INT, INT);
INT API get_stop_rate(INT, pINT);
INT API set_e_stop(INT);
INT API set_e_stop_rate(INT, INT);
INT API get_e_stop_rate(INT, pINT);

INT API set_home(INT, INT );
INT API get_home(INT, pINT);
INT API set_home_level(INT, INT );
INT API get_home_level(INT, pINT);
INT API set_index_required(INT, INT );
INT API get_index_required(INT, pINT);

INT API io_interrupt_enable(INT, INT);
INT API io_interrupt_on_stop(INT, INT);
INT API io_interrupt_on_e_stop(INT, INT);
INT API io_interrupt_pcirq(INT, INT);
INT API io_interrupt_pcirq_eoi(INT);

INT API set_amp_fault(INT, INT );
INT API get_amp_fault(INT, pINT);
INT API set_amp_fault_level(INT, INT );
INT API get_amp_fault_level(INT, pINT);
INT API fget_amp_fault_level(INT, pINT);
INT API set_amp_reset_level(INT, INT );
INT API get_amp_reset_level(INT, pINT);
INT API fget_amp_reset_level(INT, pINT);

INT API get_analog(INT, pINT);
INT API set_dac_output(INT, INT);
INT API get_dac_output(INT, pINT);

INT API fset_stepper(INT);
INT API fset_servo(INT);
INT API fset_feedback(INT, INT);
INT API fset_closed_loop(INT, INT );
INT API fset_unipolar(INT, INT);

INT API fset_micro_stepper(INT);
INT API fget_micro_stepper(INT);

INT API fset_amp_fault(INT, INT );

INT API fset_control(INT,INT);
INT API fset_electric_gear(INT, double);
INT API fset_step_mode(INT, INT);

INT API fset_home(INT, INT );
INT API fset_index_required(INT, INT );

INT API fio_interrupt_enable(INT, INT);
INT API fio_interrupt_on_stop(INT, INT);
INT API fio_interrupt_on_e_stop(INT, INT);
INT API fio_interrupt_pcirq(INT, INT);

INT API fset_positive_sw_limit(INT, double, INT);
INT API fset_negative_sw_limit(INT, double, INT);
INT API fset_positive_limit(INT, INT );
INT API fset_negative_limit(INT, INT );
INT API fset_in_position(INT, double);
INT API fset_error_limit(INT, double, INT);

INT API fset_stop_rate(INT, INT);
INT API fset_e_stop_rate(INT, INT);

INT API fget_stepper(INT);
INT API fget_feedback(INT, pINT);
INT API fget_closed_loop(INT, pINT);
INT API fget_unipolar(INT);

INT API fget_amp_fault(INT, pINT);

INT API fget_control(INT,pINT);
INT API fget_step_mode(INT, pINT);

INT API fget_home(INT, pINT);
INT API fget_index_required(INT, pINT);

INT API fget_positive_sw_limit(INT, pDOUBLE, pINT);
INT API fget_negative_sw_limit(INT, pDOUBLE, pINT);
INT API fget_positive_limit(INT, pINT);
INT API fget_negative_limit(INT, pINT);
INT API fget_in_position(INT, pDOUBLE);
INT API fget_error_limit(INT, pDOUBLE, pINT);

INT API fget_stop_rate(INT, pINT);
INT API fget_e_stop_rate(INT, pINT);

INT API set_interpolation(INT, pINT,pLONG,INT);
INT API frames_interpolation(INT);

INT API v_move_stop(INT);
INT API set_inposition_level(INT, INT);
INT API fset_inposition_level(INT, INT);
INT API get_inposition_level(INT, pINT);
INT API fget_inposition_level(INT, pINT);

INT API controller_idle(INT);
INT API controller_run(INT);

INT API arm_latch(INT, INT);
INT API latch_status(INT);
INT API get_latched_position(INT, pDOUBLE);
INT API latch(INT);

INT API set_timer(INT, LONG);
INT API get_timer(INT, pLONG);

INT API fget_home_level(INT, pINT);
INT API fget_positive_level(INT, pINT);
INT API fget_negative_level(INT, pINT);

INT API fset_amp_fault_level(INT, INT);
INT API fset_amp_reset_level(INT, INT);
INT API fset_amp_enable_level(INT,INT);
INT API fget_pulse_ratio(INT,pINT);
INT API fset_home_level(INT, INT);
INT API fget_accel_limit(INT, pINT);
INT API fget_vel_limit(INT, pDOUBLE);
INT API fset_vel_limit(INT, double);
INT API fset_positive_level(INT, INT);
INT API fset_negative_level(INT, INT);

INT API set_io_mode(INT, INT);
INT API fset_io_mode(INT, INT);
INT API get_io_mode(INT, pINT);
INT API fget_io_mode(INT, pINT);
INT API get_io_num(INT, pINT);

INT API set_analog_offset(INT, INT);
INT API fset_analog_offset(INT, INT);
INT API get_analog_offset(INT, pINT);
INT API fget_analog_offset(INT, pINT);

INT API set_inposition_required(INT, INT);
INT API fset_inposition_required(INT, INT);
INT API get_inposition_required(INT, pINT);
INT API fget_inposition_required(INT, pINT);

INT API set_coordinate_direction(INT, INT);
INT API fset_coordinate_direction(INT, INT);
INT API get_coordinate_direction(INT, pINT);
INT API fget_coordinate_direction(INT, pINT);

INT API set_encoder_direction(INT, INT);
INT API fset_encoder_direction(INT, INT);
INT API get_encoder_direction(INT, pINT);
INT API fget_encoder_direction(INT, pINT);

INT API set_axis_runstop(INT,INT);
INT API get_axis_runstop(INT,pINT);

INT API set_endless_rotationax(INT, INT, INT);
INT API fset_endless_rotationax(INT, INT, INT);
INT API get_endless_rotationax(INT, pINT);
INT API fget_endless_rotationax(INT, pINT);

INT API set_endless_linearax(INT, INT, INT);
INT API fset_endless_linearax(INT, INT, INT);
INT API get_endless_linearax(INT, pINT);
INT API fget_endless_linearax(INT, pINT);
INT API set_endless_range(INT, double);
INT API fset_endless_range(INT, double);
INT API get_endless_range(INT, pDOUBLE);
INT API fget_endless_range(INT, pDOUBLE);

INT API set_linear_all_stop_flag(INT, INT);
INT API get_linear_all_stop_flag(INT, pINT);

INT API get_command_rpm(INT, pINT);
INT API get_encoder_rpm(INT, pINT);

INT API set_amp_resolution(INT, INT);
INT API fset_amp_resolution(INT, INT);
INT API get_amp_resolution(INT, pINT);
INT API fget_amp_resolution(INT, pINT);

INT API set_collision_prevent_flag(INT, INT);
INT API get_collision_prevent_flag(INT, pINT);
INT API set_collision_prevent(INT, INT, INT, INT, double);

INT API set_abs_encoder_type(INT, INT);
INT API get_abs_encoder_type(INT, pINT);

INT API set_fast_read_encoder(INT, INT);
INT API get_fast_read_encoder(INT, pINT);

INT API set_control_timer(INT, INT);
INT API fset_control_timer(INT , INT);
INT API get_control_timer(INT, pINT);
INT API fget_control_timer(INT , pINT);
INT API spl_move_data(INT, INT, INT, INT, INT, pDOUBLE, pDOUBLE, pDOUBLE, double,INT);
INT API spl_movex(INT, INT, INT, INT);
INT API spl_arc_movenax(INT, pINT, double, double, pDOUBLE, double, INT, INT);
INT API spl_line_movenax(INT, pINT, pDOUBLE, double, INT);

INT API set_mmc_led_num(INT);
INT API get_mmc_led_num(INT, pINT);
INT API get_fast_position(INT, pDOUBLE);

INT API set_encoder_ratioa(INT, INT);
INT API fset_encoder_ratioa(INT, INT);
INT API get_encoder_ratioa(INT, pINT);
INT API fget_encoder_ratioa(INT, pINT);

INT API set_encoder_ratiob(INT, INT);
INT API fset_encoder_ratiob(INT, INT);
INT API get_encoder_ratiob(INT, pINT);
INT API fget_encoder_ratiob(INT, pINT);

INT API set_analog_limit(INT, LONG);
INT API fset_analog_limit(INT, LONG);
INT API get_analog_limit(INT, pLONG);
INT API fget_analog_limit(INT, pLONG);

INT API set_position_lowpass_filter(INT, double);
INT API fset_position_lowpass_filter(INT, double);
INT API get_position_lowpass_filter(INT, double *);
INT API fget_position_lowpass_filter(INT, double *);

INT API set_velocity_lowpass_filter(INT, double);
INT API fset_velocity_lowpass_filter(INT, double);
INT API get_velocity_lowpass_filter(INT, double *);
INT API fget_velocity_lowpass_filter(INT, double *);

INT API set_position_notch_filter(INT, double);
INT API fset_position_notch_filter(INT, double);
INT API get_position_notch_filter(INT, pDOUBLE);
INT API fget_position_notch_filter(INT, pDOUBLE);

INT API set_velocity_notch_filter(INT, double);
INT API fset_velocity_notch_filter(INT, double);
INT API get_velocity_notch_filter(INT, pDOUBLE);
INT API fget_velocity_notch_filter(INT, pDOUBLE);

INT API set_mmc_parameter_init(INT);
INT API axis_all_status(INT, pINT, pLONG, pDOUBLE);

INT API set_sensor_auto_off(INT, INT);
INT API fset_sensor_auto_off(INT, INT);
INT API get_sensor_auto_off(INT, pINT);
INT API fget_sensor_auto_off(INT, pINT);
INT API get_spline_move_num(INT, pINT);

INT API set_servo_linear_flag(INT, INT);
INT API fset_servo_linear_flag(INT, INT);
INT API get_servo_linear_flag(INT, pINT);
INT API fget_servo_linear_flag(INT, pINT);

INT API spl_auto_line_move2ax(INT,INT,pDOUBLE, double, INT, INT);
INT API spl_auto_line_move3ax(INT,INT,INT,pDOUBLE, double, INT, INT);
INT API spl_auto_arc_move2ax(INT,INT, double, double, pDOUBLE, double, INT, INT, INT);
INT API spl_auto_arc_move3ax(INT,INT,INT, double, double, pDOUBLE, double, INT, INT, INT);
INT API spl_auto_arc_movenax(INT, pINT, double, double, pDOUBLE, double, INT, INT, INT);
INT API spl_auto_line_movenax(INT, pINT, pDOUBLE, double, INT, INT);

INT API set_analog_direction(INT, INT);
INT API fset_analog_direction(INT, INT);
INT API get_analog_direction(INT, pINT);
INT API fget_analog_direction(INT, pINT);

INT API position_compare(INT, INT, INT, INT, INT, INT, INT, INT, double, LONG);
INT API position_compare_enable(INT, INT);
INT API position_compare_index_clear(INT, INT);
INT API position_compare_init(INT, INT, INT);
INT API position_compare_read(INT, INT, pDOUBLE);

INT API spl_arc_deg_move2(double , double, pDOUBLE, double, INT, INT);
INT API spl_arc_deg_move3(double , double, pDOUBLE, double, INT, INT);

INT API spl_arc_deg_move2ax(INT, INT, double, double, pDOUBLE, double, INT, INT);
INT API spl_arc_deg_move3ax(INT, INT, INT, double, double, pDOUBLE, double, INT, INT);
INT API spl_arc_deg_movenax(INT, pINT, double, double, pDOUBLE, double, INT, INT);

INT API spl_auto_arc_deg_move2ax(INT, INT, double, double, pDOUBLE, double, INT, INT, INT);
INT API spl_auto_arc_deg_move3ax(INT, INT, INT, double, double, pDOUBLE, double, INT, INT, INT);
INT API spl_auto_arc_deg_movenax(INT, pINT, double, double, pDOUBLE, double, INT, INT, INT);

INT API arc_3(double , double , double , pDOUBLE);
INT API arc_3ax(INT , INT, INT, double, double, double, pDOUBLE, double, INT);

INT API set_encoder_filter_num(INT, INT);
INT API fset_encoder_filter_num(INT, INT);
INT API get_encoder_filter_num(INT, pINT);
INT API fget_encoder_filter_num(INT, pINT);
INT	API	get_mmc_init_chkx();
INT	API	set_mmc_init_chkx(INT, INT);

INT API spl_line_move1(pDOUBLE, double, INT);
INT API spl_line_move1ax(INT, pDOUBLE, double, INT);
INT API spl_auto_line_move1ax(INT, pDOUBLE, double, INT, INT);

INT API position_compare_interval(INT dir, INT ax, INT bitNo, double startpos,double limitpos, LONG interval,LONG time);
INT API set_control_timer_ax(INT ax, double time);
INT API fset_control_timer_ax(INT ax, double time);
INT API get_control_timer_ax(INT ax, pDOUBLE time);
INT API fget_control_timer_ax(INT ax, pDOUBLE time);

INT API position_io_onoff(INT pos_num, INT bitNo, INT ax, double pos, INT encflag);
INT API position_io_allclear(INT ax);
INT API position_io_clear(INT ax, INT pos_num);

INT API set_friction_gain(INT axis, LONG gain);
INT API fset_friction_gain(INT axis, LONG gain);
INT API get_friction_gain(INT axis, pLONG gain);
INT API fget_friction_gain(INT axis, pLONG gain);
INT API set_friction_range(INT axno, double range);
INT API fset_friction_range(INT axno, double range);
INT API get_friction_range(INT axno, pDOUBLE range);
INT API fget_friction_range(INT axno, pDOUBLE range);

INT API set_system_io(INT ax, INT onoff);
INT API get_system_io(INT ax, pINT onoff);
INT API set_amp_resolution32(INT ax, LONG resolution);
INT API fset_amp_resolution32(INT ax, LONG resolution);
INT API get_amp_resolution32(INT ax, pLONG resolution);
INT API fget_amp_resolution32(INT ax, pLONG resolution);

INT API set_collision_prevent_ax(INT ax, INT enable, INT slave_ax, INT add_sub, INT non_equal, double c_pos, INT type);
INT API get_collision_prevent_ax(INT ax, pINT enable);

INT API set_sync_control_ax(INT ax, INT enable, INT master_ax, LONG gain);
INT API get_sync_control_ax(INT ax, pINT enable, pINT master_ax, pLONG gain);

INT API set_pause_control(INT bn, INT enable, long io_bit);
INT API get_collision_position(INT ax, pDOUBLE position);
INT API get_teachposition(INT ax, pDOUBLE position);

#ifdef __cplusplus
}
#endif

