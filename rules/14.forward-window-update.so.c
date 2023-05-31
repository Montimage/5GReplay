
/** 1207
 * This file is generated automatically on 2023-05-31 11:07:49
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "plugin_header.h"
#include "mmt_fsm.h"
#include "mmt_lib.h"
#include "pre_embedded_functions.h"

#ifndef RULE_SUFFIX
#define RULE_SUFFIX
#endif
#define __NAME(x,y)    x ## y
#define  _NAME(x,y)  __NAME(x,y)
#define   NAME(x)     _NAME(x,RULE_SUFFIX)
#define on_load                       NAME(on_load)
#define on_unload                     NAME(on_unload)
#define mmt_sec_get_plugin_info       NAME(mmt_sec_get_plugin_info)
#define mmt_sec_get_rule_version_info NAME(mmt_sec_get_rule_version_info)
/** 1234
 * Embedded functions
 */

#include "../lib/http2.h"
#include <stdlib.h>
#include <unistd.h>

	void on_load(){
		

		//printf("Loaded successfully rule http2 Post  \n" );
		
	}
	
	static void em_modif_then_forward(
	   const rule_info_t *rule, int verdict, uint64_t timestamp, 
	   uint64_t counter, const mmt_array_t * const trace ){	
  	const char* trace_str = mmt_convert_execution_trace_to_json_string( trace, rule );
   	printf( "DETECT Header Window Update %d\n%s\n", rule->id, trace_str );

   	printf( "em_modif_then_forward \n");
		for(int i=0;i<10;i++){

     		set_numeric_value( PROTO_HTTP2, HTTP2_INJECT_WIN_UPDATE, 0 );
     		forward_packet();
     		set_numeric_value( PROTO_HTTP2, HTTP2_RESTORE_PACKET, 0 );
     		printf("Wait 10 ms\n");
 		usleep(10000); // Sleep for 10 milliseconds
		}
	}



	void on_unload(){
		printf("Unloaded successfully rule 1\n");
	}



 //======================================RULE 14======================================
 #define EVENTS_COUNT_14 2

 #define PROTO_ATTS_COUNT_14 2

/** 862
 * Proto_atts for rule 14
 */

 static proto_attribute_t proto_atts_14[ PROTO_ATTS_COUNT_14 ] = {{.proto = "http2", .proto_id = 700, .att = "header_method", .att_id = 2, .data_type = 0, .dpi_type = 1},
 {.proto = "http2", .proto_id = 700, .att = "type", .att_id = 1, .data_type = 0, .dpi_type = 1}};
/** 874
 * Detail of proto_atts for each event
 */

 static mmt_array_t proto_atts_events_14[ 3 ] = { {.elements_count = 0, .data = NULL}, 
	 {//event_1
		 .elements_count = 1,
		 .data = (void* []) { &proto_atts_14[ 1 ] }
	 },
	 {//event_2
		 .elements_count = 1,
		 .data = (void* []) { &proto_atts_14[ 0 ] }
	 } 
 };//end proto_atts_events_

 static mmt_array_t excluded_filter_14[ 3 ] = { {.elements_count = 0, .data = NULL}, 
	 {//event_1
		 .elements_count = 0,
		 .data = NULL
	 },
	 {//event_2
		 .elements_count = 0,
		 .data = NULL
	 } 
 };//end excluded_filter_

/** 517
 * Structure to represent event data
 */
typedef struct _msg_struct_14{
	 uint16_t _http2_header_method;
	 uint16_t _http2_type;
 }_msg_t_14;
/** 551
 * Create an instance of _msg_t_14
 */
static _msg_t_14 _m14;
 static void _allocate_msg_t_14( const char* proto, const char* att, uint16_t index ){
	 if( strcmp( proto, "http2" ) == 0 && strcmp( att, "header_method" ) == 0 ){ _m14._http2_header_method = index; return; }
	 if( strcmp( proto, "http2" ) == 0 && strcmp( att, "type" ) == 0 ){ _m14._http2_type = index; return; }
 }
/** 92
 * Rule 14, event 1
  * Forward 
 */
static inline int g_14_1( const message_t *msg, const fsm_t *fsm ){
	 if( unlikely( msg == NULL || fsm == NULL )) return 0;
	 const message_t *his_msg;
	 const void *data;/* 57 */

	 data = get_element_data_message_t( msg, _m14._http2_type );
	 double _http2_type = 0;
	 if (data != NULL)  _http2_type = *(double*) data;

	 return (_http2_type == 8);
 }
 
/** 92
 * Rule 14, event 2
  * Nothing
 */
static inline int g_14_2( const message_t *msg, const fsm_t *fsm ){
	 if( unlikely( msg == NULL || fsm == NULL )) return 0;
	 const message_t *his_msg;
	 const void *data;/* 57 */

	 data = get_element_data_message_t( msg, _m14._http2_header_method );
	 double _http2_header_method = 0;
	 if (data != NULL)  _http2_header_method = *(double*) data;

	 return (_http2_header_method != 0);
 }
 
/** 410
 * States of FSM for rule 14
 */

/** 411
 * Predefine list of states: init, fail, pass, ...
 */
static fsm_state_t s_14_0, s_14_1, s_14_2, s_14_3, s_14_4;
/** 424
 * Initialize states: init, error, final, ...
 */
static fsm_state_t
/** 430
 * initial state
 */
 s_14_0 = {
	 .delay        = {.time_min = 0, .time_max = 0, .counter_min = 0, .counter_max = 0},
	 .is_temporary = 0,
	 .description  = "Modify stream id",
	 .entry_action = 0, //FSM_ACTION_DO_NOTHING
	 .exit_action  = 1, //FSM_ACTION_CREATE_INSTANCE
	 .transitions  = (fsm_transition_t[]){
		 /** 458 Forward  */
		 /** 460 A real event */
		 { .event_type = 1, .guard = &g_14_1, .action = 1, .target_state = &s_14_4}  //FSM_ACTION_CREATE_INSTANCE
	 },
	 .transitions_count = 1
 },
/** 430
 * timeout/error state
 */
 s_14_1 = {
	 .delay        = {.time_min = 0, .time_max = 0, .counter_min = 0, .counter_max = 0},
	 .is_temporary = 0,
	 .description  =  NULL ,
	 .entry_action = 0, //FSM_ACTION_DO_NOTHING
	 .exit_action  = 0, //FSM_ACTION_DO_NOTHING
	 .transitions  = NULL,
	 .transitions_count = 0
 },
/** 430
 * pass state
 */
 s_14_2 = {
	 .delay        = {.time_min = 0, .time_max = 0, .counter_min = 0, .counter_max = 0},
	 .is_temporary = 0,
	 .description  =  NULL ,
	 .entry_action = 0, //FSM_ACTION_DO_NOTHING
	 .exit_action  = 0, //FSM_ACTION_DO_NOTHING
	 .transitions  = NULL,
	 .transitions_count = 0
 },
/** 430
 * inconclusive state
 */
 s_14_3 = {
	 .delay        = {.time_min = 0, .time_max = 0, .counter_min = 0, .counter_max = 0},
	 .is_temporary = 0,
	 .description  =  NULL ,
	 .entry_action = 0, //FSM_ACTION_DO_NOTHING
	 .exit_action  = 0, //FSM_ACTION_DO_NOTHING
	 .transitions  = NULL,
	 .transitions_count = 0
 },
/** 430
 * root node
 */
 s_14_4 = {
	 .delay        = {.time_min = 0LL, .time_max = 0LL, .counter_min = 0LL, .counter_max = 0LL},
	 .is_temporary = 1,
	 .description  = "Modify stream id",
	 .entry_action = 0, //FSM_ACTION_DO_NOTHING
	 .exit_action  = 0, //FSM_ACTION_DO_NOTHING
	 .transitions  = (fsm_transition_t[]){
		 /** 460 Timeout event will fire this transition */
		 { .event_type = 0, .guard = NULL  , .action = 0, .target_state = &s_14_1}, //FSM_ACTION_DO_NOTHING
		 /** 458 Nothing */
		 /** 460 A real event */
		 { .event_type = 2, .guard = &g_14_2, .action = 2, .target_state = &s_14_2}  //FSM_ACTION_RESET_TIMER
	 },
	 .transitions_count = 2
 };
/** 487
 * Create a new FSM for this rule
 */
static void *create_new_fsm_14(){
		 return fsm_init( &s_14_0, &s_14_1, &s_14_2, &s_14_3, EVENTS_COUNT_14, sizeof( _msg_t_14 ) );//init, error, final, inconclusive, events_count
 }//end function
/** 609
 * Moment the rules being encoded
  * PUBLIC API
 */

static const rule_version_info_t version = {.created_date=1685524069, .hash = "dcce935", .number="0.0.6", .index=600, .dpi="1.7.8 (68bd7d93)"};
const rule_version_info_t * mmt_sec_get_rule_version_info(){ return &version;};

 //======================================GENERAL======================================
/** 623
 * Information of 1 rules
  * PUBLIC API
 */
size_t mmt_sec_get_plugin_info( const rule_info_t **rules_arr ){
	  static const rule_info_t rules[] = (rule_info_t[]){
		 {
			 .id               = 14,
			 .type_id          = 4,
			 .type_string      = "FORWARD",
			 .events_count     = EVENTS_COUNT_14,
			 .description      = "Modify stream id",
			 .if_satisfied     = em_modif_then_forward,
			 .if_not_satisfied = NULL,
			 .proto_atts_count = PROTO_ATTS_COUNT_14,
			 .proto_atts       = proto_atts_14,
			 .proto_atts_events= proto_atts_events_14,
			 .excluded_filter  = excluded_filter_14,
			 .create_instance  = &create_new_fsm_14,
			 .hash_message     = &_allocate_msg_t_14,
			 .version          = &version,
		 }
	 };
	 *rules_arr = rules;
	 return 1;
 }