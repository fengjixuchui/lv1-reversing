/**
 * (c) 2016 The LV1RE Project.
 * Released under MIT license. Read LICENSE for more details.
 */

#include "debugger.h"

S32 rsx_utils_debugger_ctor(void) {
    U64 value = 0;
    rsx_dbg_obj_t* dbg_obj = NULL;
    
    
    // get RSX debug level
    get_param_value("sys.lv1.rsxdebug", &value);
    
    // allocate RSX debug object
    dbg_obj = lv1_kmalloc(sizeof(rsx_dbg_obj_t));
    if (dbg_obj == NULL)
      return -2;  // alloc fail
    
    // init members
    dbg_obj->unk_00 = 0;
    dbg_obj->unk_08 = 0;
    dbg_obj->unk_10 = 0xF420;
    //dbg_obj->unk_18 = 0xF420 + get_tb();
    dbg_obj->unk_20 = 0;
    
    
    // set global RSX debug object
    g_rsx_dbg_obj = (void*)dbg_obj;
    
    return 0;  // OK
}
